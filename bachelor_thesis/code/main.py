# Bakalářská práce
# Identifikace zvěře na základě biometrických informací
# Autor: Maxim Plička <xplick04@fit.vutbr.cz>

import cv2 as cv
import math
import numpy as np
import argparse
import os

# argument parser function
def argParse():
    parser = argparse.ArgumentParser(prog='main.py')
    parser.add_argument('-i','--input', type=str, required=True, nargs=1, help="Input photo")
    parser.add_argument('-c', '--compareDir', type=str, default='', nargs=1, help="Comparation directory")
    args = parser.parse_args()
    if args.compareDir == '':
        return args.input[0], ''
    else:
        return args.input[0], args.compareDir[0]

# function for rescaling image based on width
def rescaleImage(img, width):
    scale = width / img.shape[1]    
    width = int(img.shape[1] * scale)
    height = int(img.shape[0] * scale)
    dimensions = (width, height)
    return cv.resize(img, dimensions, interpolation=cv.INTER_AREA)

# callback function for drawing circle on input image
def circle(event, x, y, p, img):
    global mouse_x, mouse_y
    if event == cv.EVENT_LBUTTONDBLCLK:
        cv.circle(img, (x, y), 10, (0, 0, 255), 2)
        mouse_x, mouse_y = x, y

# function for image annotations
def makeAnnotations(img, fileName):
    img_copy = img.copy()
    points = []
    cv.namedWindow(fileName)
    cv.setMouseCallback(fileName, circle, img)

    while(1):
        cv.imshow(fileName, img)
        k = cv.waitKey(20) & 0xFF
        if k == ord('a'): # add point
            points.append([mouse_x, mouse_y])
        elif k == ord('d'): # next
            if(len(points) >= 6):
                cv.destroyAllWindows()
                break
            else:
                print("You need to specify all 6 points")
        if k == ord('r'): # restart
            points = []
            img = img_copy.copy()
            cv.destroyAllWindows()
            cv.namedWindow(fileName)
            cv.setMouseCallback(fileName, circle, param=img)       
        elif k == 27: # ESC, end
            cv.destroyAllWindows()
            exit()

    return (points[0], points[1], points[2]), (points[3], points[4], points[5])

# function for getting two nearest point from two sets of anotated points
def getTwoNearest(left, right):
    left_f = left[0]    # default
    right_f = right[0]  # default
    distance = math.sqrt((left_f[0] - right_f[0])**2 + (left_f[1] - right_f[1])**2) # default

    for pointA in left:
        for pointB in right:
            cmp_distance = math.sqrt((pointA[0] - pointB[0])**2 + (pointA[1] - pointB[1])**2)
            if cmp_distance < distance:
                distance = cmp_distance
                left_f = pointA
                right_f = pointB      
    return left_f, right_f

# function for rotation of image
def rotateImg(img, pointA, pointB):
    mid_point = ( int((pointB[0] + pointA[0]) * 0.5), int((pointB[1] + pointA[1]) * 0.5))
    A = abs(pointB[0] - pointA[0])    # distance of points on X axis
    B = abs(pointB[1] - pointA[1])    # distance of points on Y axis

    if(pointA[0] < pointB[0]):
        if(pointA[1] > pointB[1]):
            angle = math.degrees(math.atan2(B, A)) * -1
        else:
            angle = math.degrees(math.atan2(B, A))
    else:
        if(pointA[1] > pointB[1]):
            angle = math.degrees((math.atan2(B, A))) + 180
        else:
            angle = math.degrees(math.atan2(B, A)) * -1 + 180
    
    rotation_matrix = cv.getRotationMatrix2D(mid_point, angle, scale=1)
    rotated_img = cv.warpAffine(img, rotation_matrix, (img.shape[1], img.shape[0]))
    return rotated_img, rotation_matrix

# function for refreshing old coordinates of anotated points
def getNewCoordinates(points, matrix):
    array = np.array([ [[points[0][0], points[0][1]]], [[points[1][0], points[1][1]]], [[points[2][0], points[2][1]]] ])
    new_coordinates = cv.transform(array, matrix) 
    points_new = (
                (new_coordinates[0][0][0], new_coordinates[0][0][1]), 
                (new_coordinates[1][0][0], new_coordinates[1][0][1]), 
                (new_coordinates[2][0][0], new_coordinates[2][0][1])
                )
    return points_new

# function for cropping region od interest from image
def cropRectangle(img, left, right):
    pointA, pointB = getTwoNearest(left, right)
    distance = math.sqrt((pointA[0] - pointB[0])**2 + (pointA[1] - pointB[1])**2)
    return img[ pointA[1] - int(distance * 0.25): pointA[1] + int(distance * 0.75), pointA[0] : pointB[0] ]

# function for applying CLAHE
def make_CLAHE(img, clip, size):
    clahe = cv.createCLAHE(clipLimit=clip, tileGridSize=size)
    return clahe.apply(img)

# function for filling holes in contours
def getFilled(img):
    contours, hierarchy = cv.findContours(img, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    mask = np.zeros(img.shape[:2], dtype=np.uint8)
 
    for i, c in enumerate(contours):
        if hierarchy[0][i][3] == -1:    #internal contour
            cv.fillPoly(mask, [c], (255))       
    return cv.bitwise_or(img, mask)

# function for filtering undesired contours from image
def getFiltered(img):
    contours, _ = cv.findContours(img, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
    img_height = img.shape[0]
    img_width = img.shape[1]

    for _, c in enumerate(contours):
        hull = cv.convexHull(c)

        if cv.contourArea(c) < cv.contourArea(hull) * 0.6:  # convex filtering
            cv.fillPoly(img, [c], (0))
            continue

        if not (100 < cv.contourArea(c) < 10000):  # size filtering
            cv.fillPoly(img, [c], (0))
            continue

        x, y, w, h = cv.boundingRect(c)
        if x <= 0 or y <= 0 or x+w >= img_width or y+h >= img_height:   # edge filtering
            cv.fillPoly(img, [c], (0))
    return img

# function for morphological opening
def getErodedDilated(img, kernel_size, iter):
    kernel = cv.getStructuringElement(cv.MORPH_ELLIPSE,(kernel_size, kernel_size))
    erosion = cv.erode(img, kernel, iterations=iter)
    dilation = cv.dilate(erosion, kernel, iterations=iter)
    return dilation

# function for extracting features from contours
def getCentroids(img, original):
    contours, _ = cv.findContours(img, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
    sorted_contours = sorted(contours, key= lambda c : (cv.contourArea(c)), reverse=True)
    centroid_array = []

    for c in sorted_contours:
        # get center of the contour
        M = cv.moments(c)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        # draw contour and center of the contour on the image
        cv.drawContours(original, [c], -1, (0, 0, 255), 1)
        cv.circle(original, (cX, cY), 3, (0, 255, 255), -1)
        centroid_array.append([cX, cY])
    return original, centroid_array

# function for reformating array with extracted features
def getPositionsArray(centroids):
    img_size = 300
    rows, cols = (30, 30)
    array = [[0 for i in range(cols)] for j in range(rows)]

    for c in centroids:
        x = int(c[0] / (img_size / cols))
        y = int(c[1] / (img_size / rows))
        array[y][x] += 1
    return array

# function for comparing two arrays with features
def compareArray(array, array2):
    total = 0
    matched = 0

    for i1 in range(len(array)):
        for i2 in range(len(array[i1])):
                total += max(array[i1][i2], array2[i1][i2])
                matched += min(array[i1][i2], array2[i1][i2])
    return matched, total

# function for finding best matching photo from database
def findBestMatch(directory, centroid_array):
    array = []
    best_percentage = 0

    for filename in os.scandir(directory):
        if filename.is_file() and filename.path.endswith(".txt"):
            
            f = open(filename.path, "r")
            lines = f.readlines()

            for line in lines:
                line_array = [int(x) for x in line.split(",")]
                array.append(line_array)

            matched, total = compareArray(centroid_array, array)
            percentage = matched / total * 100
            if percentage > best_percentage:
                best_percentage = percentage
                best_file = os.path.splitext(filename.path)[0]
            array = []
            f.close()       
    return best_file, best_percentage
            
   
if __name__ == "__main__":
    in_file, cmp_dir = argParse() 
    
    try:
        img_in = cv.imread(in_file, 1)
    except:
        print("Wrong input path")
        exit()

    #----------------------------------------------------NORMALIZATION START
    img_rescaled = rescaleImage(img_in, 600)
    img_rescaled_copy = img_rescaled.copy() # copying image for annotations, so anotated circles will not be visible
    left, right = makeAnnotations(img_rescaled_copy, in_file)
    pointA, pointB = getTwoNearest(left, right)
    img_rotated, rotation_matrix = rotateImg(img_rescaled, pointA, pointB)
    
    # refreshing old coordinates
    left = getNewCoordinates(left, rotation_matrix)
    right = getNewCoordinates(right, rotation_matrix)

    img_cropped = cropRectangle(img_rotated, left, right)
    img_cropped_rescaled = rescaleImage(img_cropped, 300)
    #cv.imshow("cropped", img_cropped_rescaled)
    #----------------------------------------------------NORMALIZATION END


    #----------------------------------------------------PREPROCCESSING START
    img_gray = cv.cvtColor(img_cropped_rescaled, cv.COLOR_BGR2GRAY)
    #cv.imshow("gray", img_gray)

    img_bilateral = cv.bilateralFilter(img_gray, 11, 75, 75)
    #cv.imshow("bilateral", img_bilateral)

    img_CLAHE = make_CLAHE(img_bilateral, 3.0, (10,10))
    #cv.imshow("clahe", img_CLAHE)

    img_thresh = cv.adaptiveThreshold(img_CLAHE, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 15, 2)
    #cv.imshow("thresh", img_thresh)

    img_filled = getFilled(img_thresh)
    #cv.imshow("filled", img_filled)

    img_erdi = getErodedDilated(img_filled, kernel_size=2, iter=1)
    #cv.imshow("erdi", img_erdi)

    img_filtered = getFiltered(img_erdi)
    #cv.imshow("filtered", img_filtered)
    #----------------------------------------------------PREPROCCESSING END


    #----------------------------------------------------EXTRACTION START
    img_centers, centroid_array = getCentroids(img_filtered, img_cropped_rescaled)
    positions_array = getPositionsArray(centroid_array)
    #cv.imshow("centers", img_centers)
    #----------------------------------------------------EXTRACTION END


    #----------------------------------------------------PRINT/COMPARE START
    if(cmp_dir == ''):  # if compare directory is not set
        print(centroid_array)
    else:
        best_file, percentage = findBestMatch(cmp_dir, positions_array)
        img_best_match = cv.imread(best_file + ".jpg", 1)
        cv.imshow(str(os.path.basename(best_file)) + ", " + "{:.1f}".format(percentage) + '%', img_best_match)
        cv.imshow("original image", img_rescaled)
    #----------------------------------------------------PRINT/COMPARE END
    
    cv.waitKey(0) # wait before closing
    cv.destroyAllWindows() # destroy widnows
