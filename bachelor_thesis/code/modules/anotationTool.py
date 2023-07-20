# Bakalářská práce
# Identifikace zvěře na základě biometrických informací
# modul pro anotaci fotografií
# Autor: Maxim Plička <xplick04@fit.vutbr.cz>

import cv2 as cv
import os


def rescaleFrame(img):
    scale = 600 / img.shape[1]
    width = int(img.shape[1] * scale)
    height = int(img.shape[0] * scale)
    dimensions = (width, height)
    return cv.resize(img, dimensions, interpolation=cv.INTER_AREA)


def draw_circle(event, x, y, p):
    global mouseX, mouseY
    if event == cv.EVENT_LBUTTONDBLCLK:
        cv.circle(img, (x, y), 10, (0, 0, 255), 2)
        mouseX, mouseY = x, y


if __name__ == "__main__":
    directory = 'img/full_db'  # directory to annotate
    # format: Image name; left; left; left; right; right; right; 15591_2.jpg
    fAnnotations = open("img/annotations.txt", "a")    # text file for annotations

    for filename in os.listdir(directory):
        imgPath = os.path.join(directory, filename)
        if os.path.isfile(imgPath):
            img = cv.imread(imgPath, 1)
            img = rescaleFrame(img)
            cv.namedWindow(filename)
            cv.setMouseCallback(filename, draw_circle)
            fAnnotations.write(filename + ';')
            while(1):
                cv.imshow(filename, img)
                k = cv.waitKey(20) & 0xFF
                if k == 27: # ESC
                    cv.destroyAllWindows()
                    fAnnotations.close()
                    exit()
                elif k == ord('a'): # printKey
                    xStr = repr(mouseX)
                    yStr = repr(mouseY)
                    fAnnotations.write(xStr + ';' + yStr + ';')
                elif k == ord('d'): # next
                    cv.destroyAllWindows()
                    fAnnotations.write('\n')
                    break

    fAnnotations.close()