import cv2
import numpy as np

image = cv2.imread('Anh-phong-canh.jpg')
imgMean = np.mean(image)
imgStd = np.std(image)

outMean = 1
outStd = 0.5

scale = outStd / imgStd
shift = outMean - scale *imgMean
imgLinear = shift +scale*image
cv2.imshow('original',image)
cv2.imshow('linear',imgLinear)
cv2.waitKey(0)
