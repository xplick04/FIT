# Bakalářská práce
# Identifikace zvěře na základě biometrických informací
# modul pro extrakci oblasti zájmu
# Autor: Maxim Plička <xplick04@fit.vutbr.cz>

import cv2 as cv
import math
import numpy as np

def rescaleFrame(img, width):
    scale = width / img.shape[1]
    width = int(img.shape[1] * scale)
    height = int(img.shape[0] * scale)
    dimensions = (width, height)
    return cv.resize(img, dimensions, interpolation=cv.INTER_AREA)


def rotateImg(img, pointA, pointB):
    midPoint = ( int((pointB[0] + pointA[0]) * 0.5), int((pointB[1] + pointA[1]) * 0.5))
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
    
    rotation_matrix = cv.getRotationMatrix2D(midPoint, angle, scale=1)
    rotated_img = cv.warpAffine(img, rotation_matrix, (img.shape[1], img.shape[0]))
    return rotated_img, rotation_matrix
    

def getNewCoordinates(points, matrix):
    array = np.array([ [[points[0][0], points[0][1]]], [[points[1][0], points[1][1]]], [[points[2][0], points[2][1]]] ])
    new_coordinates = cv.transform(array, matrix) 
    points_new = (
                (new_coordinates[0][0][0], new_coordinates[0][0][1]), 
                (new_coordinates[1][0][0], new_coordinates[1][0][1]), 
                (new_coordinates[2][0][0], new_coordinates[2][0][1])
                )
    return points_new


def getTwoNearest(LEFT, RIGHT):
    LEFT_f = LEFT[0]
    RIGHT_f = RIGHT[0]
    distance = math.sqrt((LEFT_f[0] - RIGHT_f[0])**2 + (LEFT_f[1] - RIGHT_f[1])**2)
    for pointA in LEFT:
        for pointB in RIGHT:
            cmp_distance = math.sqrt((pointA[0] - pointB[0])**2 + (pointA[1] - pointB[1])**2)
            if cmp_distance < distance:
                distance = cmp_distance
                LEFT_f = pointA
                RIGHT_f = pointB      
    return LEFT_f, RIGHT_f


def cropRectangle(img, left, right):
    pointA, pointB = getTwoNearest(left, right)
    distance = math.sqrt((pointA[0] - pointB[0])**2 + (pointA[1] - pointB[1])**2)
    return img[ pointA[1] - int(distance * 0.25): pointA[1] + int(distance * 0.75), pointA[0] : pointB[0] ]


if __name__ == "__main__":
    f = open("../img/others/full_annotations.txt", 'r') # file with annotated points
    array = f.readline().split(';')


    while array[0] != '':
        img = cv.imread("../img/others/full/" + array[0], 1) # file with original photos
        img = rescaleFrame(img, 600)

        LEFT = (int(array[1]), int(array[2])), (int(array[3]), int(array[4])), (int(array[5]), int(array[6]))
        RIGHT = (int(array[7]), int(array[8])), (int(array[9]), int(array[10])), (int(array[11]), int(array[12]))

        pointA, pointB = getTwoNearest(LEFT, RIGHT)
        img, rotation_matrix = rotateImg(img, pointA, pointB)

        # refreshing old coordinates
        LEFT = getNewCoordinates(LEFT, rotation_matrix)
        RIGHT = getNewCoordinates(RIGHT, rotation_matrix)
        crop_img = cropRectangle(LEFT, RIGHT)
        crop_img = rescaleFrame(crop_img, 300)

        #cv.imshow("cropped", crop_img)
        cv.imwrite("../img/cropped1.png", crop_img)

        k = cv.waitKey(0) # wait before closing
        cv.destroyAllWindows()
        if k == 27: # ESC
            exit()
        array = f.readline().split(';')

    f.close()