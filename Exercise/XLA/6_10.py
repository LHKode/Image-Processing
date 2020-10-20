import numpy as np
import cv2
import sys
import matplotlib.pyplot as plt
from PyQt5 import uic
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QMainWindow, QLabel, QPushButton, QApplication
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

class Go(QMainWindow):
    def __init__(self):
        super(Go,self).__init__()
        uic.loadUi('0610.ui',self)

        self.image = cv2.imread('a.png',0)

        self.loadBtn = self.findChild(QPushButton,'loadBtn')
        self.idealBtn = self.findChild(QPushButton,'idealBtn')
        self.imageLbl = self.findChild(QLabel,'orgImage')

        image = cv2.imread('a.png')
        self.imageLbl.setPixmap(QPixmap(self.cv_to_pixmap(image)))

        self.show()
    def plotting_3D(self,code):

        fig = plt.figure()
        ax = fig.gca(projection='3d')

        if code == 0:
            H = self.ideal(self.image,5)



        surf = ax.plot_surface(np.abs(x), np.abs(y), np.abs(z), cmap=cm.coolwarm,
                               linewidth=0, antialiased=False)

        ax.set_zlim(-1.01, 1.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()
    def cv_to_pixmap(self,cvImg):
        height, width , chanel= cvImg.shape
        bytesPerLine = 3 * width
        qImg = QImage(cvImg.data, width, height, bytesPerLine, QImage.Format_RGB888)
        return qImg
    def butterworthip(self, x, y, d0, n):
        hr = x/2
        hc = y/2
        x = np.arange(-hc,hc)
        y = np.arange(-hr,hr)
        
    def Gausi(self,image,x,y):
        return cv2.GaussianBlur(image, (x, y), 0)


app = QApplication(sys.argv)
a=Go()
app.exec_()


