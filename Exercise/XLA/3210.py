import cv2
import numpy as np
import math
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
def gaussianlp(img):
    sx, sy = img.shape[:2]
    x = np.arange(-sy / 2, sy / 2)
    y = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)

    x, y = np.meshgrid(x, y)
    d = np.sqrt(x ** 2 + y ** 2)
    H = pow(math.e, (-d ** 2 / (2 * (50 ** 2))))  # cho sigma=50
    return H
sx, sy = img.shape[:2]
x = np.arange(-sy / 2, sy / 2)
y = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)

x, y = np.meshgrid(x, y)
d = np.sqrt(x ** 2 + y ** 2)

H = gaussianlp(img)
H = pow(math.e, (-d ** 2 / (2 * (50 ** 2))))  # cho sigma=50

g = np.fft.fftshift(np.fft.fft2(img))  # fft and shift to center
img_apply = g * H  # apply filter
img_gaussian_lp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
img_gaussian_lp = np.uint8(img_gaussian_lp)
# cv2.imshow('Orginal',img)
# cv2.imshow('Filter',img_butterworth_lp)

# ret,img_binary = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_BINARY)
# ret,img_trunc = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_TRUNC)
# ret,img_tozero = cv2.threshold(img_butterworth_lp,127,255,cv2.THRESH_TOZERO)
# ret, img_otsu = cv2.threshold(img_butterworth_lp, 0,255, cv2.THRESH_OTSU)
img_threshold = cv2.adaptiveThreshold(img_gaussian_lp, 255, cv2.ADAPTIVE_THRESH_MEAN_C,\
                                      cv2.THRESH_BINARY,11,2)
# img_gaussian = cv2.adaptiveThreshold(img_butterworth_lp, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
#                                     cv2.THRESH_BINARY,11,2)
Z = img_gaussian_lp.reshape((-1,3))
Z = np.float32(Z)
iter = (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER,10,1.0)
# criteria = (iter,10,1.0)
K=8
ret,label,center = cv2.kmeans(Z,K,None,iter,10,cv2.KMEANS_RANDOM_CENTERS)
center=np.uint8(center)
res = center[label.flatten()]
res2 = res.reshape((img_gaussian_lp.shape))
cv2.imshow('KMeans',res2)
cv2.imshow('Mean_C',img_gaussian_lp)
cv2.waitKey()

# titles = ['Original', 'butterworth_lp','binary_threshold', 'Kmean (K='+str(K)+', inter ='+str(iter)+')']
# images = [img, img_butterworth_lp, img_binary,res2]
# for i in range(4):
#     plt.subplot(2,3,i+1)
#     plt.imshow(images[i],'gray')
#     plt.title(titles[i])
#     plt.xticks([]),plt.yticks([])
# plt.show()
# cv2.waitKey()