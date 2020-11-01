import os
import sys
import cv2
import math
import numpy as np
from PyQt5 import uic, QtGui
from matplotlib import pyplot as plt
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

class MyWindow(QMainWindow):
    def __init__(self):
        super(MyWindow,self).__init__()
        uic.loadUi('mid.ui',self)
        # self.imgPath = cv2.imread('Anh-phong-canh.jpg')
        # Open file from folder
        layout = QVBoxLayout()
        self.browseImg = self.findChild(QPushButton, 'browseBtn')
        self.browseImg.clicked.connect(self.getfile)
        layout.addWidget(self.browseImg)

        self.imgOriginal = self.findChild(QLabel, 'imageOriginal')
        self.imgRes = self.findChild(QLabel,'imageResult')

        #Light
        self.addBtn = self.findChild(QPushButton,'addBtn')
        self.addValue = self.findChild(QSpinBox,'addValue')
        self.addBtn.clicked.connect(self.light)
        #Sub
        self.subBtn = self.findChild(QPushButton, 'subBtn')
        self.subValue = self.findChild(QSpinBox, 'subValue')
        self.subBtn.clicked.connect(self.dark)
        #Mul
        self.mulBtn = self.findChild(QPushButton, 'mulBtn')
        self.mulValue = self.findChild(QSpinBox, 'mulValue')
        self.mulBtn.clicked.connect(self.multiple)
        #Log
        self.logBtn = self.findChild(QPushButton,'logBtn')
        self.logBtn.clicked.connect(self.log)
        #Gamma
        self.gammaBtn = self.findChild(QPushButton,'gammaBtn')
        self.gammaValue = self.findChild(QSpinBox,'gammaValue')
        self.gammaBtn.clicked.connect(self.gamma)
        #Power
        self.powerBtn = self.findChild(QPushButton,'powerBtn')
        self.gammaValueP = self.findChild(QSpinBox,'gammaValueP')
        self.cValueP = self.findChild(QSpinBox,'cValueP')
        self.powerBtn.clicked.connect(self.power)
        #Plotting Hist
        self.plotHistBtn = self.findChild(QPushButton,'plotHistBtn')
        self.plotHistBtn.clicked.connect(self.plotting_histogram)
        #Equalize Histogram
        self.equalHistBtn = self.findChild(QPushButton,'equalHistBtn')
        self.equalHistBtn.clicked.connect(self.equalize_histogram)
        # Blur
        self.blurBtn = self.findChild(QPushButton,'blurBtn')
        self.ksizeValue = self.findChild(QComboBox, 'valueBlur')
        self.blurBtn.clicked.connect(self.blur)
        # Gaussian Blur
        self.gaussblurBtn = self.findChild(QPushButton, 'gaussBlurBtn')
        self.gaussValue = self.findChild(QComboBox, 'gBlurValue')
        self.gaussblurBtn.clicked.connect(self.gaussian_blur)
        # Median Blur
        self.medianBlurBtn = self.findChild(QPushButton, 'medianBtn')
        self.medianValue = self.findChild(QComboBox, 'mBlurValue')
        self.medianBlurBtn.clicked.connect(self.median)
        # Sharpen
        self.sharpBtn = self.findChild(QPushButton,'sharpBtn')
        self.sharpBtn.clicked.connect(self.sharpen)
        # Ideal Low_pass
        # Error
        self.idealLow = self.findChild(QPushButton, 'iLowBtn')
        self.idealLow.clicked.connect(self.ideal_lp)
        # Butterworth Low_pass
        # Error
        self.buttLow = self.findChild(QPushButton,'buttLowBtn')
        self.buttLow.clicked.connect(self.butterworth_lp)

        # Gaussian Low_pass
        # Error
        self.gaussLow = self.findChild(QPushButton,'gLowBtn')
        self.gaussLow.clicked.connect(self.gaussian_lp)
        # Ideal High_pass
        # Error
        self.idealHigh = self.findChild(QPushButton, 'iHighBtn')
        self.idealHigh.clicked.connect(self.ideal_hp)
        # Butterworth High_pass
        # Error
        self.buttHigh = self.findChild(QPushButton, 'buttHighBtn')
        self.buttHigh.clicked.connect(self.butterworth_hp)
        # Gaussian High_pass
        # Error
        self.gaussHigh = self.findChild(QPushButton,'gHighBtn')
        self.gaussHigh.clicked.connect(self.gaussian_hp)
        # Binary Threshold
        self.binaryThreshold = self.findChild(QPushButton, 'binaryBtn')
        self.binaryValue = self.findChild(QSpinBox, 'binValue')
        self.binaryThreshold.clicked.connect(self.threshold_binary)
        # Trunc Threshold
        self.truncThreshold = self.findChild(QPushButton,'truncBtn')
        self.truncValue = self.findChild(QSpinBox, 'truncValue')
        self.truncThreshold.clicked.connect(self.threshold_trunc)
        # Tozero Threshold
        self.tozeroThreshold = self.findChild(QPushButton, 'tozeroBtn')
        self.tozeroValue = self.findChild(QSpinBox,'tozeroValue')
        self.tozeroThreshold.clicked.connect(self.threshold_tozero)
        # Otsu Threshold
     
        self.otsuThreshold = self.findChild(QPushButton, 'otsuBtn')
        self.otsuValue = self.findChild(QSpinBox, 'otsuValue')
        self.otsuThreshold.clicked.connect(self.threshold_otsu)
        # Adap Mean Threshold
        # Error
        self.adapMeanBtn = self.findChild(QPushButton, 'adapMean')
        self.adapMeanBtn.clicked.connect(self.threshold_mean_C)
        # Adap Gauss Threshold
        # Error
        self.adapGauBtn = self.findChild(QPushButton, 'adapGauss')
        self.adapGauBtn.clicked.connect(self.threshold_gaussian_C)
        # Sobel
        # Error
        self.sobelBtn = self.findChild(QPushButton, 'sobelBtn')
        self.sobelBtn.clicked.connect(self.sobel)
        # Prewitt
        # Error
        self.prewittButton = self.findChild(QPushButton,'prewittBtn')
        self.prewittButton.clicked.connect(self.prewitt)



        self.show()
    def cv2_to_pixmap(self, img):
        height, width, channel = img.shape
        bytesPerLine = 3 * width
        qImg = QImage(img.data, width, height, bytesPerLine, QImage.Format_RGB888)
        return qImg
    def getfile(self):
        fname = QFileDialog.getOpenFileName(self, 'Open file', 'c\\','Image files(*.jpg *.gif)')
        self.imgPath = fname[0]
        self.ffImage = self.imgPath
        pixmap = QPixmap(self.imgPath)
        self.img = cv2.imread(self.imgPath)
        self.imgOriginal.setPixmap(QPixmap(pixmap))
    # def type(self):
    #     image = self.ffImage
    #     value = self.sizeImg.currentIndex()*2+1
    #     if(self.typeImg.currentIndex() == 1):
    #         image = cv2.blur(self.ffImage, (value,value))
    #     elif(self.typeImg.currentIndex() == 2):
    #         image = cv2.GaussianBlur(self.ffImage, (value,value),0)
    #     elif(self.typeImg.currentIndex() == 3):
    #         image = cv2.medianBlur(self.ffImage,value)
    #     # cv2.imshow('test',image)
    #     self.labelImg.setPixmap(QPixmap(image))

    def light(self):
        Img = cv2.imread(self.imgPath)
        value = self.addValue.value()
        resImg = Img+value
        qImg = self.cv2_to_pixmap(resImg)
        self.imgRes.setPixmap(QPixmap(qImg))
    def dark(self):
        Img = cv2.imread(self.imgPath)
        value = self.subValue.value()
        resImg = Img - value
        qImg = self.cv2_to_pixmap(resImg)
        self.imgRes.setPixmap(QPixmap(qImg))
    def multiple(self):
        Img = cv2.imread(self.imgPath)
        value = self.mulValue.value()
        resImg = Img * value
        qImg = self.cv2_to_pixmap(resImg)
        self.imgRes.setPixmap(QPixmap(qImg))
    def log(self):
        Img = cv2.imread(self.imgPath)
        c=255/np.log(1+np.max(Img))
        log_Img = c*(np.log(Img+1))
        log_Img = np.array(log_Img,dtype=np.uint8)
        qImg = self.cv2_to_pixmap(log_Img)
        self.imgRes.setPixmap(QPixmap(qImg))

    def gamma(self):
        Img = cv2.imread(self.imgPath)
        gamma_vl = self.gammaValue.value()
        gamma_img = np.array(255*(Img/255)**gamma_vl,dtype='uint8')
        qImg = self.cv2_to_pixmap(gamma_img)
        self.imgRes.setPixmap(QPixmap(qImg))
    def power(self):
        Img = cv2.imread(self.imgPath)
        gamma_vl = self.gammaValueP.value()
        c = self.cValueP.value()
        gamma_img = np.array(c * (Img / 255) ** gamma_vl, dtype='uint8')
        qImg = self.cv2_to_pixmap(gamma_img)
        self.imgRes.setPixmap(QPixmap(qImg))

    def plotting_histogram(self):
        Img = cv2.imread(self.imgPath)
        plt.hist(Img.ravel(),256,[0,256])
        plt.show()
    def equalize_histogram(self):
        Img = cv2.imread(self.imgPath)
        equal_hist_img = cv2.equalizeHist(Img)
        res = np.hstack((Img,equal_hist_img))
        plt.hist(equal_hist_img.ravel(),256,[0,256])
        plt.show()

    def filter2D(self, Matrix):
        Img = cv2.imread(self.imgPath)
        ma = np.array([[0, 0, 0],
                       [1, 1, 1],
                       [0, 0, 0]])
        if Matrix == 5:
            ma = np.array([[0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0],
                           [1, 1, 1, 1, 1],
                           [0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0]])
        elif Matrix == 7:
            ma = np.array([[0, 0, 0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0, 0, 0],
                           [1, 1, 1, 1, 1, 1, 1],
                           [0, 0, 0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0, 0, 0],
                           [0, 0, 0, 0, 0, 0, 0]])
        reImg = cv2.filter2D(Img, -1, ma / Matrix)
        return reImg

    def gaussian_blur(self):
        Img = cv2.imread(self.imgPath)
        value = int(self.gaussValue.currentText())
        gau_blur_img = cv2.GaussianBlur(Img,(value,value),0)
        qImg = self.cv2_to_pixmap(gau_blur_img)
        self.imgRes.setPixmap(QPixmap(qImg))
    def blur(self):
        Img = cv2.imread(self.imgPath)
        value = int(self.ksizeValue.currentText())
        blur_img = cv2.blur(Img,(value,value))
        qImg = self.cv2_to_pixmap(blur_img)
        self.imgRes.setPixmap(QPixmap(qImg))
    def median(self):
        Img = cv2.imread(self.imgPath)
        value = int(self.medianValue.currentText())
        median_img = cv2.medianBlur(Img, value, 0)
        qImg = self.cv2_to_pixmap(median_img)
        self.imgRes.setPixmap(QPixmap(qImg))
    def sharpen(self):
        Img = cv2.imread(self.imgPath)
        filter = np.array([[-1, -1, -1],
                           [-1, 9, -1],
                           [-1, -1, -1]])
        sharpen_img = cv2.filter2D(Img,-1,filter)
        qImg = self.cv2_to_pixmap(sharpen_img)
        self.imgRes.setPixmap(QPixmap(qImg))

    def ideal_lp(self):
        Img = cv2.imread(self.imgPath)
        sx, sy = Img.shape[:2]
        x = np.arange(-(sx) / 2, (sx) / 2)  # tâm là (0,0)
        y = np.arange(-(sy) / 2, (sy) / 2)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = d.copy()
        for i in range(0, d.shape[0]):
            for j in range(0, d.shape[1]):
                if d[i, j] <= 40:  # d0 = 100
                    H[i, j] = 1
                else:
                    H[i, j] = 0

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_ideal_lp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        img_ideal = np.uint8(img_ideal_lp)
        qImg = self.cv2_to_pixmap(img_ideal)
        self.imgRes.setPixmap(QPixmap(qImg))
    def butterworth_lp(self):
        d0 = 50
        n = 2
        Img = cv2.imread(self.imgPath)
        sx, sy = Img.shape[:2]
        x = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)
        y = np.arange(-sy / 2, sy / 2)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = d.copy()
        for i in range(0, d.shape[0]):
            for j in range(0, d.shape[1]):
                H[i, j] = 1 / (1 + (d[i, j] / d0) ** (2 * n))  # cho Do = 5, n =2

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_butterworth_lp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        img_butterworth_lp = np.uint8(img_butterworth_lp)
        qImg = self.cv2_to_pixmap(img_butterworth_lp)
        self.imgRes.setPixmap(QPixmap(qImg))
    def gaussian_lp(self):
        Img = cv2.imread(self.imgPath)
        sigma = 50
        sx, sy = Img.shape[:2]
        x = np.arange(-sy / 2, sy / 2)
        y = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = pow(math.e, (-d ** 2 / (2 * (sigma ** 2))))  # cho sigma=50

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_gaussian_lp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        img_gaussian_lp = np.uint8(img_gaussian_lp)
        qImg = self.cv2_to_pixmap(img_gaussian_lp)
        self.imgRes.setPixmap(QPixmap(qImg))
    def ideal_hp(self):
        Img = cv2.imread(self.imgPath)
        sx, sy = Img.shape[:2]
        x = np.arange(-(sx) / 2, (sx) / 2)  # tâm là (0,0)
        y = np.arange(-(sy) / 2, (sy) / 2)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = d.copy()
        for i in range(0, d.shape[0]):
            for j in range(0, d.shape[1]):
                if d[i, j] <= 40:  # d0 = 100
                    H[i, j] = 0
                else:
                    H[i, j] = 1

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_ideal_hp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        img_ideal_hp = np.uint8(img_ideal_hp)
        qImg = self.cv2_to_pixmap(img_ideal_hp)
        self.imgRes.setPixmap(QPixmap(qImg))
    def butterworth_hp(self):
        Img = cv2.imread(self.imgPath)
        d0 =50
        n = 2
        sx, sy = Img.shape[:2]
        x = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)
        y = np.arange(-sy / 2, sy / 2)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = d.copy()
        for i in range(0, d.shape[0]):
            for j in range(0, d.shape[1]):
                H[i, j] = 1 / (1 + (d0 / d[i, j]) ** (2 * n))  # cho Do = 5, n =2

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_butterworth_hp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        # img_butterworth_hp = np.uint8(img_butterworth_hp)
        qpImg = self.cv2_to_pixmap(img_butterworth_hp)
        self.imgRes.setPixmap(QPixmap(qpImg))
    def gaussian_hp(self):
        Img = cv2.imread(self.imgPath)
        sigma = 50
        d0 = 50
        sx, sy = Img.shape[:2]
        x = np.arange(-sx / 2, sx / 2)  # tâm là (0,0)
        y = np.arange(-sy / 2, sy / 2)

        x, y = np.meshgrid(x, y)
        d = np.sqrt(x ** 2 + y ** 2)

        H = 1 - pow(math.e, (-d ** 2 / (2 * (50 ** 2))))  # cho sigma=50

        g = np.fft.fftshift(np.fft.fft2(Img))  # fft and shift to center
        img_apply = g * H  # apply filter
        img_gaussian_hp = abs(np.fft.ifft2(np.fft.ifftshift(img_apply)))
        img_gaussian_hp = np.uint8(img_gaussian_hp <= d0)
        qImg = self.cv2_to_pixmap(img_gaussian_hp)
        self.imgRes.setPixmap(QPixmap(qImg))

    def threshold_binary(self):
        Img = cv2.imread(self.imgPath)
        value = self.binaryValue.value()
        ret, img_binary = cv2.threshold(Img, value, 255, cv2.THRESH_BINARY)
        qImg = self.cv2_to_pixmap(img_binary)
        self.imgRes.setPixmap(QPixmap(qImg))
    def threshold_tozero(self):
        Img = cv2.imread(self.imgPath)
        value = self.tozeroValue.value()
        ret, img_tozero = cv2.threshold(Img, value, 255, cv2.THRESH_TOZERO)
        qImg = self.cv2_to_pixmap(img_tozero)
        self.imgRes.setPixmap(QPixmap(qImg))
    def threshold_trunc(self):
        Img = cv2.imread(self.imgPath)
        value = self.truncValue.value()
        ret, img_trunc = cv2.threshold(Img, value, 255, cv2.THRESH_TRUNC)
        qImg = self.cv2_to_pixmap(img_trunc)
        self.imgRes.setPixmap(QPixmap(qImg))
    def threshold_otsu(self):
        Img = cv2.imread(self.imgPath)
        value = self.otsuValue.value()
        ret, img_otsu = cv2.threshold(Img, value, 255, cv2.THRESH_OTSU)
        qImg = self.cv2_to_pixmap(img_otsu)
        self.imgRes.setPixmap(QPixmap(qImg))
    def threshold_mean_C(self):
        Img = cv2.imread(self.imgPath)
        img_threshold = cv2.adaptiveThreshold(Img, 255, cv2.ADAPTIVE_THRESH_MEAN_C, \
                                              cv2.THRESH_BINARY,11, 2)
        cv2.imread()
        qImg = self.cv2_to_pixmap(img_threshold)
        self.imgRes.setPixmap(QPixmap(qImg))
    def threshold_gaussian_C(self):
        Img = cv2.imread(self.imgPath,0)
        img_gaussian = cv2.adaptiveThreshold(Img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, \
                                             cv2.THRESH_BINARY,11, 2)
        cv2.imwrite('gauc.png',img_gaussian)
        img_gaussian = cv2.imread('gauc.png')
        qImg = self.cv2_to_pixmap(img_gaussian)
        self.imgRes.setPixmap(QPixmap(qImg))
    def sobel(self):
        Img = cv2.imread(self.imgPath)
        img_sobelx = cv2.Sobel(Img, cv2.CV_8U, 1, 0, ksize=5)
        img_sobely = cv2.Sobel(Img, cv2.CV_8U, 0, 1, ksize=5)
        img_sobel = img_sobelx + img_sobely
        qImg = self.cv2_to_pixmap(img_sobel)
        self.imgRes.setPixmap(QPixmap(qImg))
    def prewitt(self):
        Img = cv2.imread(self.imgPath)
        hx = np.array([[1,1,1],
                       [0,0,0],
                       [-1,-1,-1]])
        hy = np.array([[-1,0,1],
                       [-1,0,1],
                       [-1,0,1]])
        prewittX = cv2.filter2D(Img, -1, hx)
        prewittY = cv2.filter2D(Img, -1, hy)
        sum = np.abs(prewittX) + np.abs(prewittY)
        qImg = self.cv2_to_pixmap(sum)
        self.imgRes.setPixmap(QPixmap(qImg))

app = QApplication(sys.argv)
go = MyWindow()
app.exec_()







