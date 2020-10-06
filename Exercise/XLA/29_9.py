from PyQt5 import uic
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QMainWindow, QLabel, QPushButton, QApplication
import numpy as np
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import cv2
import sys
import random

class Go(QMainWindow):
    def __init__(self):
        super(Go,self).__init__()
        uic.loadUi('299.ui',self)

        self.cvImage = cv2.imread('Anh-phong-canh.jpg')
        self.reImage= cv2.resize(self.cvImage,(600,600))
        self.Image = self.findChild(QLabel,'image')
        self.fiImage = self.findChild(QLabel,'filImage')
        self.noiseBtn = self.findChild(QPushButton,'noiseBtn')
        self.filBtn = self.findChild(QPushButton,'filBtn')
        self.img3 = self.findChild(QLabel,'img_3')
        self.img5 = self.findChild(QLabel,'img_5')
        self.img7 = self.findChild(QLabel,'img_7')

        self.Image.setPixmap(QPixmap(self.cv_to_pixmap(self.reImage)))

        self.noiseBtn.clicked.connect(self.add_noise)
        self.filBtn.clicked.connect(self.log)

        self.show()

    def cv_to_pixmap(self,cvImg):
        height, width , chanel= cvImg.shape
        bytesPerLine = 3 * width
        qImg = QImage(cvImg.data, width, height, bytesPerLine, QImage.Format_RGB888)
        return qImg
    def noisy(self,image,prob):
        output = np.zeros(image.shape, np.uint8)
        thres = 1 - prob
        for i in range(image.shape[0]):
            for j in range(image.shape[1]):
                rdn = random.random()
                if rdn < prob:
                    output[i][j] = 0
                elif rdn > thres:
                    output[i][j] = 255
                else:
                    output[i][j] = image[i][j]
        return output
    def add_noise(self):
        img1 =self.reImage
        imgs = []
        for _ in range(20):
            img1 = self.reImage.copy()
            cv2.randn(img1, (0, 0, 0), (50, 50, 50))
            imgs.append(self.reImage+img1)
        # For averaging create an empty array, then add images to this array.
        img_avg = np.zeros((self.reImage.shape[0], self.reImage.shape[1], self.reImage.shape[2]), np.float32)
        for im in imgs:
            img_avg = img_avg + im / 40
        # Round the float values. Always specify the dtype
        img_avg = np.array(np.round(img_avg), dtype=np.uint8)
        self.reImage = imgs[1]
        self.Image.setPixmap(QPixmap(self.cv_to_pixmap(self.noisy(self.reImage,0.00000001))))
    def fil (self):
        arr_3 = np.array([[0,0,0],
                        [1,1,1],
                        [0,0,0]])
        arr_5 = np.array([[0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0],
                          [1, 1, 1, 1, 1],
                          [0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0]])
        arr_7 = np.array([[0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0],
                          [1, 1, 1, 1, 1, 1, 1],
                          [0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0]])
        reImage_3 = cv2.filter2D(self.reImage,-1,arr_3/3)
        reImage_5 = cv2.filter2D(self.reImage,-1,arr_5/5)
        reImage_7 = cv2.filter2D(self.reImage,-1,arr_7/7)
        cv2.imshow('orginal',self.reImage)
        self.img3.setPixmap(QPixmap(self.cv_to_pixmap(reImage_3)))
        self.img5.setPixmap(QPixmap(self.cv_to_pixmap(reImage_5)))
        self.img7.setPixmap(QPixmap(self.cv_to_pixmap(reImage_7)))

    def salt_pepper(self,image,prob):
        output = np.zeros(image.shape, np.uint8)
        thres = 1 - prob
        for i in range(image.shape[0]):
            for j in range(image.shape[1]):
                rdn = random.random()
                if rdn < prob:
                    output[i][j] = 0
                elif rdn > thres:
                    output[i][j] = 255
                else:
                    output[i][j] = image[i][j]
        return output

    def threshold(self):
        img = cv2.medianBlur(self.reImage, 5)
        img1 = self.salt_pepper(img,0.005)
        img2 = img1.copy()
        ret, th1 = cv2.threshold(img1, 127, 255, cv2.THRESH_BINARY)
        mul_image = np.concatenate((img2, th1), axis=1)
        cv2.imshow('salt&peper_Img - threshold_Img', mul_image)
    def log(self):
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        # x = np.arange(-3, 3,1)
        # y = np.arange(-3, 3,1)
        # x,y = np.meshgrid(x,y)
        # z = np.sin(x+y)
        img = cv2.imread('Anh-phong-canh.jpg',0)

        f = np.fft.fft2(img)
        fshift = np.fft.fftshift(f)
        magnitude_spectrum = 20 * np.log(np.abs(fshift))

        surf = ax.plot_surface(np.abs(f), np.abs(fshift), np.abs(magnitude_spectrum), cmap=cm.coolwarm,
                               linewidth=0, antialiased=False)

        ax.set_zlim(-1.01, 1.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()


app = QApplication(sys.argv)
a=Go()
app.exec_()