import numpy as np
import cv2
import sys
import matplotlib.pyplot as plt
from PyQt5 import uic
import matplotlib as ml
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QMainWindow, QLabel, QPushButton, QApplication
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter


class Go(QMainWindow):
    def __init__(self):
        super(Go, self).__init__()
        uic.loadUi('0610.ui', self)

        self.image = cv2.imread('a.png', 0)

        self.loadBtn = self.findChild(QPushButton, 'loadBtn')
        self.idealBtn = self.findChild(QPushButton, 'idealBtn')
        self.imageLbl = self.findChild(QLabel, 'orgImage')

        image = cv2.imread('a.png')
        self.imageLbl.setPixmap(QPixmap(self.cv_to_pixmap(image)))
        height, width = self.image.shape
        self.idealBtn.clicked.connect(self.butterworthlp)
        self.show()

    def plotting_3D(self, code):
        fig = plt.figure()
        ax = fig.gca(projection='3d')

        if code == 0:
            H = self.ideal(self.image, 5)

        surf = ax.plot_surface(np.abs(x), np.abs(y), np.abs(z), cmap=cm.coolwarm,
                               linewidth=0, antialiased=False)

        ax.set_zlim(-1.01, 1.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()

    def cv_to_pixmap(self, cvImg):
        height, width, chanel = cvImg.shape
        bytesPerLine = 3 * width
        qImg = QImage(cvImg.data, width, height, bytesPerLine, QImage.Format_RGB888)
        return qImg

    def butterworthlp(self, x=225, y=225, d0=5, n=1):
        # fig = plt.figure()
        # ax = fig.gca(projection='3d')
        hr = x / 2
        hc = y / 2
        x = np.arange(-hc, hc)
        y = np.arange(-hr, hr)

        [x, y] = np.meshgrid(x, y)
        mg = np.sqrt(x ** 2 + y ** 2)
        H = 1 / (1 + (mg / d0) ** (2 * n))
        # H = np.array([[1, 2, 3, 4],
        #               [5, 6, 7, 8],
        #               [9, 10, 11, 12],
        #               [13, 14, 15, 16]])
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        x = H[0:, :]
        y = H[:, 0:]
        x, y = np.meshgrid(x, y)
        R = np.sqrt(x ** 2 + y ** 2)
        z = np.sin(R)
        # Plot the surface.
        surf = ax.plot_surface(x, y, z, cmap=cm.coolwarm,
                               linewidth=0, antialiased=False)

        # Customize the z axis.
        ax.set_zlim(-1.01, 1.01)
        ax.zaxis.set_major_locator(LinearLocator(10))
        ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

        # Add a color bar which maps values to colors.
        fig.colorbar(surf, shrink=0.5, aspect=5)

        plt.show()

    def butterworthhp(self, x, y, d0, n):
        return 1 - self.butterworthip(x, y, d0, n)

    def Gausi(self, image, x, y):
        return cv2.GaussianBlur(image, (x, y), 0)


app = QApplication(sys.argv)
a = Go()
app.exec_()
