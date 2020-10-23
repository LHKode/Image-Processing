import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('hw4_radiograph_1.jpg',0)
# sx,sy = image.shape
# hr = np.arange(-(sx)/2,(sx)/2)
# hc = np.arange(-(sy)/2,(sy)/2)
#
# [x,y] = np.meshgrid(hc,hr,sparse=True)
# mg = np.sqrt(x**2+y**2)
# H = 1/(1+(mg/20)**(2*1))
# G = np.fft.fftshift(np.fft.fft2(image))
# Ip = G*H
# Im = np.abs(np.fft.ifft2(np.fft.ifftshift(Ip)))
# cv2.imshow('go',Im)
# cv2.imshow('o',image)
# cv2.waitKey()



sy, sx = img.shape[:2]
x = np.arange(-sx/2, sx/2) #tâm là (0,0)
y =np.arange(-sy/2, sy/2)

x, y = np.meshgrid(x, y)
d = np.sqrt(x**2 + y**2)

H = d.copy()
for i in range(0, d.shape[0]):
    for j in range(0, d.shape[1]):
        H[i, j] = 1/(1+(d[i, j]/40)**(2*1)) #cho Do = 5, n =2


g = np.fft.fftshift(np.fft.fft2(img))  # fft and shift to center
img_apply = g * H  # apply filter
img_butterworth_lp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
img_butterworth_lp = np.uint8(img_butterworth_lp)
cv2.imshow('Orginal',img)
cv2.imshow('Filter',img_butterworth_lp)

img_butterworth_lp = cv2.resize(img, (600,600))
ret,img_binary = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_BINARY)
ret,img_trunc = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_TRUNC)
ret,img_tozero = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_TOZERO)
ret, img_otsu = cv2.threshold(img_butterworth_lp, 0,255, cv2.THRESH_OTSU)
img_threshold = cv2.adaptiveThreshold(img_butterworth_lp, 255, cv2.ADAPTIVE_THRESH_MEAN_C,\
                                      cv2.THRESH_BINARY,11,2)
img_gaussian = cv2.adaptiveThreshold(img_butterworth_lp, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
                                    cv2.THRESH_BINARY,11,2)

titles = ['Mean', 'Gaussian', 'Tozero', 'Binary', 'Trunc','Otsu']
images = [img_threshold, img_gaussian, img_tozero, img_binary, img_trunc, img_otsu]
for i in range(6):
    plt.subplot(3,3,i+1)
    plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
plt.show()
cv2.waitKey()