from PyQt5 import QtGui, uic
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel, QLineEdit, QSpinBox
import sys
import cv2
import numpy as np



class lhk(QMainWindow):
    def __init__(self):
        super(lhk, self).__init__()
        uic.loadUi("untitled.ui", self)

        self.loadBtn = self.findChild(QPushButton, 'loadBtn')
        self.orImage = self.findChild(QLabel, 'orImage')
        self.loadBtn.clicked.connect(self.load)

        self.addBtn = self.findChild(QPushButton, 'addBtn')
        self.addImage = self.findChild(QLabel, 'addImage')
        self.addValue = self.findChild(QSpinBox, 'addValue')
        self.addBtn.clicked.connect(self.add)

        self.subBtn = self.findChild(QPushButton, 'subBtn')
        self.subImage = self.findChild(QLabel, 'subImage')
        self.subValue = self.findChild(QSpinBox, 'subValue')
        self.subBtn.clicked.connect(self.sub)

        self.mulBtn = self.findChild(QPushButton, 'mulBtn')
        self.mulImage = self.findChild(QLabel, 'mulImage')
        self.mulValue = self.findChild(QSpinBox,'mulValue')
        self.mulBtn.clicked.connect(self.mul)

        self.liBtn = self.findChild(QPushButton,'liBtn')
        self.liImage = self.findChild(QLabel,'liImage')
        self.liValue = self.findChild(QSpinBox,'liValue')
        self.liBtn.clicked.connect(self.linear)

        self.show()

    def load(self):
        self.pixmap = QPixmap('Anh-phong-canh.jpg')
        self.image = cv2.imread('Anh-phong-canh.jpg')
        self.orImage.setPixmap(self.pixmap)

    def add(self):
        addImage = self.image + self.addValue.value()
        height, width, channel = addImage.shape
        bytesPerLine = 3 * width
        qImg = QImage(addImage.data, width, height, bytesPerLine, QImage.Format_RGB888)
        self.addImage.setPixmap(QPixmap(qImg))

    def sub(self):
        subImage = self.image - self.subValue.value()
        height, width, channel = subImage.shape
        bytesPerLine = 3 * width
        qImg = QImage(subImage.data, width, height, bytesPerLine, QImage.Format_RGB888)
        self.subImage.setPixmap(QPixmap(qImg))

    def mul(self):
        mulImage = self.image * self.mulValue.value()
        height, width, channel = mulImage.shape
        bytesPerLine = 3 * width
        qImg = QImage(mulImage.data, width, height, bytesPerLine, QImage.Format_RGB888)
        self.mulImage.setPixmap(QPixmap(qImg))
    def linear(self):
        imgMean = np.mean(self.image)
        imgStd = np.std(self.image)

        outMean = self.liValue.value()
        outStd = 20

        scale = outStd / imgStd
        shift = outMean - scale *imgMean
        imgLinear = shift +scale*self.image
        try:
            height, width, channel = imgLinear.shape
            bytesPerLine = 3 * width
            qImg = QImage(imgLinear.data, width, height, bytesPerLine, QImage.Format_RGB888)
            self.liImage.setPixmap(QPixmap(qImg))
        except:
            pass

        # cv2.imshow('li',imgLinear)

    # def add(self, n):
    #     addImage = self.image + n
    #     return addImage


app = QApplication(sys.argv)
win = lhk()
app.exec_()
# image = cv2.imread('L.A.M.png')
# plt.hist(image.ravel(),256,[0,256]);
# plt.imshow(image)
# plt.show()


# c = 255 / np.log(1 + np.max(image))
# log_image = c * (np.log(image + 1))
#
# log_image = np.array(log_image, dtype = np.uint8)
# # cv2.imshow('test',im)
# # cv2.waitKey(0)
#
# plt.imshow(log_image)
# plt.show()
