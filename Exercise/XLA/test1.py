import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('hw4_radiograph_2.jpg',0)
img_resize = cv2.resize(img, (600,600))
# gauss = np.random.normal(0,1,img.size)
# gauss = gauss.reshape(img.shape[0], img.shape[1], img.shape[2]).astype('uint8')
# img_gauss = cv2.add(img, gauss)
ret,img_binary = cv2.threshold(img_resize,127,255,cv2.THRESH_BINARY)
ret,img_trunc = cv2.threshold(img_resize,127,255,cv2.THRESH_TRUNC)
ret,img_tozero = cv2.threshold(img_resize,127,255,cv2.THRESH_TOZERO)
ret, img_otsu = cv2.threshold(img_resize, 0,255, cv2.THRESH_OTSU)
img_threshold = cv2.adaptiveThreshold(img_resize, 255, cv2.ADAPTIVE_THRESH_MEAN_C,\
                                      cv2.THRESH_BINARY,11,2)
img_gaussian = cv2.adaptiveThreshold(img_resize, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
                                    cv2.THRESH_BINARY,11,2)

titles = ['Mean', 'Gaussian', 'Tozero', 'Binary', 'Trunc','Otsu']
images = [img_threshold, img_gaussian, img_tozero, img_binary, img_trunc, img_otsu]
cv2.imshow('Original', img)
for i in range(6):
    plt.subplot(3,3,i+1)
    plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
plt.show()

cv2.waitKey(0)