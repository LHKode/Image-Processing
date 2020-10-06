def salt_pepper(self):
    row, col, ch = self.resized_img1.shape
    s_vs_p = 0.5
    amount = 0.004
    out = np.copy(self.resized_img1)
    # Salt mode
    num_salt = np.ceil(amount * self.resized_img1.size * s_vs_p)
    coords = [np.random.randint(0, i - 1, int(num_salt))
              for i in self.resized_img1.shape]
    out[coords] = 1
    # Pepper mode
    num_pepper = np.ceil(amount * self.resized_img1.size * (1. - s_vs_p))
    coords = [np.random.randint(0, i - 1, int(num_pepper))
              for i in self.resized_img1.shape]
    out[coords] = 0
    return out


def threshold(self):
    img = cv2.medianBlur(self.resized_img1, 5)
    img1 = self.resized_img1.salt_pepper()
    ret, th1 = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)
    # th2 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_MEAN_C,\
    #                             cv2.THRESH_BINARY, 11, 2)
    # th3 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
    #                             cv2.THRESH_BINARY, 11, 2)
    # titles = ['Original Image', 'Global Thresholding (v = 127)',
    #           'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
    # images = [img, th1, th2, th3]
    # for i in range(4):
    # plt.subplot(2, 2), plt.imshow(th1, 'gray')
    # plt.title(th1)
    # plt.xticks([]), plt.yticks([])image
    # plt.show()
    mul_image = np.concatenate((img, th1), axis=1)
    cv2.imshow('th', img1)