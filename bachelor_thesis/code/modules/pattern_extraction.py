# Bakalářská práce
# Identifikace zvěře na základě biometrických informací
# modul pro extrakci oblasti zájmu
# Autor: Maxim Plička <xplick04@fit.vutbr.cz>

import cv2 as cv
import os


def make_CLAHE(img, limit, size):
    clahe = cv.createCLAHE(clipLimit=limit, tileGridSize=size)
    return clahe.apply(img)


def getFilled(img):
    contours, hierarchy = cv.findContours(img, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    
    for i, c in enumerate(contours):
        if hierarchy[0][i][3] == -1:    # contour is inside another
            cv.fillPoly(img, [c], (255))
    return img


def getFiltered(img):
    contours, _ = cv.findContours(img, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
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
    

def getCentroids(img, original):
    contours, _ = cv.findContours(img, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
    centroid_array = []

    for c in contours:
        M = cv.moments(c)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        # draw the contour and center of the shape on the image
        #cv.drawContours(original, [c], -1, (0, 0, 255), 1)
        cv.circle(original, (cX, cY), 3, (255, 255, 0), -1)
        centroid_array.append([cX, cY])
    return original, centroid_array


def getErodedDilated(img, kernel_size, iter):
    kernel = cv.getStructuringElement(cv.MORPH_ELLIPSE,(kernel_size, kernel_size))
    erosion = cv.erode(img, kernel, iterations=iter)
    dilation = cv.dilate(erosion, kernel, iterations=iter)
    return dilation


def getPositionsArray(centroids):
    img_size = 300
    rows, cols = (30, 30)
    array = [[0 for i in range(cols)] for j in range(rows)]

    for c in centroids:
        x = int(c[0] / (img_size / cols))
        y = int(c[1] / (img_size / rows))
        array[y][x] += 1

    return array

if __name__ == "__main__":
    directory = "img/full_db_ROI_bez_spatnych" # path to directory with ROIs
    hist_array = []

    for filename in os.scandir(directory):
        if filename.is_file() and filename.path.endswith(".jpg"):
            
            img_o = cv.imread(filename.path, 1)

            gray = cv.cvtColor(img_o, cv.COLOR_BGR2GRAY)

            img_b = cv.bilateralFilter(gray, 11, 75, 75)

            CLAHE_img = make_CLAHE(img_b, 3.0, (10,10))

            thresh = cv.adaptiveThreshold(CLAHE_img, 255, cv.ADAPTIVE_THRESH_MEAN_C, cv.THRESH_BINARY, 15, 2)

            filled = getFilled(thresh)

            img_erdi = getErodedDilated(filled, kernel_size=2, iter=1)

            filtered = getFiltered(filled)
            
            centers, centroid_array = getCentroids(filtered, img_o)
                 
            positions_array = getPositionsArray(centroid_array)

            #cv.imwrite("img/full_db_ROI_bs_result2/" + filename.name , centers)   

            # generating feature templates
            """
            filename_edited = os.path.splitext(filename.path)[0]
            f = open(filename_edited + ".txt", "w")
            for line in positions_array:
                for i, num in enumerate(line):
                    if i != len(line) - 1:
                        f.write(str(num) + ',')
                    else:
                        f.write(str(num))
                f.write('\n')

            f.close()
            """
            

            k = cv.waitKey(0) # wait before closing
            cv.destroyAllWindows()
            if k == 27: # ESC
                exit()
        


        
        