from PyQt5 import QtGui, uic
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel, QComboBox
import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt

class Conv(QMainWindow):
    def __init__(self):
        super(Conv,self).__init__()
        uic.loadUi('conv.ui',self)

        self.cvImage = cv2.imread('sudoku.PNG')
        self.resize_img = cv2.resize(self.cvImage, (600,600))
        self.qImage = QPixmap('Anh-phong-canh.jpg')
        self.cvImage1 = cv2.imread('Anh-phong-canh.jpg')

        self.orImage = self.findChild(QLabel,'orImage')
        self.orImage.setPixmap(self.qImage)

        self.sizeCbb = self.findChild(QComboBox,'sizeCbb')
        self.typeCbb = self.findChild(QComboBox,'typeCbb')
        self.fiImage = self.findChild(QLabel,'fiImage')
        self.fiBtn = self.findChild(QPushButton,'fiBtn')
        self.vhBtn = self.findChild(QPushButton,'V_H')
        self.sharpBtn = self.findChild(QPushButton,'sharp')

        self.fiBtn.clicked.connect(self.click)
        self.vhBtn.clicked.connect(self.Ver_Ho)
        self.sharpBtn.clicked.connect(self.sharpen)

        self.show()
    def cv_to_pixmap(self,cvImg):
        height, width, channel = cvImg.shape
        bytesPerLine = 3 * width
        qImg = QImage(cvImg.data, width, height, bytesPerLine, QImage.Format_RGB888)
        return qImg
    def click(self):
        reImage = self.cvImage
        value = self.sizeCbb.currentIndex()*2+1
        if(self.typeCbb.currentIndex()==3):
            reImage = cv2.medianBlur(self.cvImage,value)
        elif(self.typeCbb.currentIndex()==1):
            reImage = cv2.blur(self.cvImage,(value,value))
        elif(self.typeCbb.currentIndex()==2):
            reImage = cv2.GaussianBlur(self.cvImage,(value,value),0)


        qImage = self.cv_to_pixmap(reImage)
        self.fiImage.setPixmap(QPixmap(qImage))
    def Ver_Ho(self):
        matrix_X = np.array([[0,0,0],
                          [-1,2,-1],
                          [0,0,0]])
        matrix_Y = np.array([[0,-1,0],
                             [0,2,0],
                             [0,-1,0]])
        Gx_image = cv2.filter2D(self.resize_img,-1,matrix_X)
        GY_image = cv2.filter2D(self.resize_img,-1,matrix_Y)
        XY_image = np.arctan(GY_image+Gx_image)
        mul_image = np.concatenate((Gx_image,self.resize_img,XY_image), axis = 1)

        cv2.imshow("Vertical_Original_Horizontal",mul_image)
        cv2.imshow("Arctan",XY_image)
    def sharpen(self):
        matrix_Y = np.array([[-1, -1, -1],
                             [-1, 9, -1],
                             [-1, -1, -1]])
        sharp = cv2.filter2D(cv2.resize(self.cvImage1,(600,600)),-1,matrix_Y)
        mul = np.concatenate((cv2.resize(self.cvImage1,(600,600)),sharp),axis=1)
        cv2.imshow("Original_Sharpen",mul)




app = QApplication(sys.argv)
go = Conv()
app.exec_()