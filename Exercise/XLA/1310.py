import numpy as np
import cv2
cv2.ad
# image = cv2.imread('d.png',0)
# resized_img = cv2.resize(image, (600,600))
# hx = np.array([[1,0,-1],
#               [2,0,-2],
#               [1,0,-1]])
# hy = np.array([[1,2,1],
#               [0,0,0],
#               [-1,-2,-1]])
# sobelX = cv2.Sobel(resized_img,cv2.CV_8U,1,0,hx)
# sobelY = cv2.Sobel(resized_img,cv2.CV_8U,0,1,hy)
# sumS = np.abs(sobelX) + np.abs(sobelY)
# multi_img = np.concatenate((resized_img, sumS), axis= 1)
# sum = multi_img.astype(np.uint8)
# cv2.imshow('Sobel', sum)
# cv2.waitKey(0)

image = cv2.imread('d.png',0)
resized_img = cv2.resize(image, (600,600))
hx = np.array([[-1,0,1],
              [-1,0,1],
              [-1,0,1]])
hy = np.array([[-1,-1,-1],
              [0,0,0],
              [1,1,1]])
filterX = cv2.filter2D(resized_img,-1,hx)
filterY = cv2.filter2D(resized_img,-1,hy)
sumF = np.abs(filterX) + np.abs(filterY)
multi_img = np.concatenate((resized_img, sumF), axis= 1)
sum = multi_img.astype(np.uint8)
cv2.imshow('Prewitt', sum)
cv2.waitKey(0)





