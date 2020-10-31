import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('hw4_radiograph_2.jpg',0)
img = cv2.medianBlur(img,5)

ret,th1 = cv2.threshold(img,150,255,cv2.THRESH_BINARY)
th2 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_MEAN_C,\
            cv2.THRESH_BINARY,11,2)
th3 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv2.THRESH_BINARY,11,2)
1


titles = ['Original Image', 'Global Thresholding (v = 150)',
            'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding','OTSU']
images = [img, th1, th2, th3, th4]

for i in range(5):
    plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
plt.show()