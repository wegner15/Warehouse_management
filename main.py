from PyQt5 import QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QPixmap
import sys
import cv2
from PyQt5.QtCore import pyqtSignal, pyqtSlot, Qt, QThread
import numpy as np
from Video_Processor import *
from itertools import product
from stream import *




class VideoThread(QThread):
    change_pixmap_signal = pyqtSignal(np.ndarray)

    def __init__(self):
        super().__init__()
        self._run_flag = True

    def run(self):
        # capture from web cam
        URL = "http://192.168.217.35"
        set_resolution(url=URL, index=10)
        # set_quality(url=URL, value=62)
        # set_awb(url=URL, awb=1)
        cap = VideoCapture(URL + ":81/stream")
        # cap = cv2.VideoCapture(0)
        # ret, frame = cap.read()
        while self._run_flag:
            cv_img = cap.read()
            decoder(cv_img)
            # if ret:
            self.change_pixmap_signal.emit(cv_img)
        # shut down capture system
        cap.release()

    def stop(self):
        """Sets run flag to False and waits for thread to finish"""
        self._run_flag = False
        self.wait()


class App(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Warehouse Management")
        self.disply_width = 640
        self.display_height = 480
        # create the label that holds the image
        self.image_label = QLabel(self)
        self.image_label.resize(self.disply_width, self.display_height)
        # create a text label
        self.textLabel = QLabel('Robot Movement')
        self.load_button = QPushButton(self, text="Load Product")
        self.fetch_button = QPushButton(self, text="Fetch Product")
        self.fetch_button.pressed.connect(self.fetch_product)
        self.items_selection = QComboBox(self)

        # wordlist for testing
        wordlist = ["Product A", "Product B", "Product C", "Product D", "Product E"]
        self.items_selection.addItems(wordlist)
        # completers only work for editable combo boxes. QComboBox.NoInsert prevents insertion of the search text
        self.items_selection.setEditable(True)
        self.items_selection.setInsertPolicy(QComboBox.NoInsert)

        # change completion mode of the default completer from InlineCompletion to PopupCompletion
        self.items_selection.completer().setCompletionMode(QCompleter.PopupCompletion)

        # create a vertical box layout and add the two labels
        vbox = QVBoxLayout()
        vbox.addWidget(self.image_label)
        vbox.addWidget(self.textLabel)
        vbox.addWidget(self.items_selection)
        vbox.addWidget(self.load_button)
        vbox.addWidget(self.fetch_button)
        # set the vbox layout as the widgets layout
        self.setLayout(vbox)

        # create the video capture thread
        self.thread = VideoThread()
        # connect its signal to the update_image slot
        self.thread.change_pixmap_signal.connect(self.update_image)
        # start the thread
        self.thread.start()

    def fetch_product(self):
        target_product = self.items_selection.currentText()
        if not target_product:
            pass
        target_products["target"] = target_product
        print(target_products)

    def closeEvent(self, event):
        self.thread.stop()
        event.accept()

    @pyqtSlot(np.ndarray)
    def update_image(self, cv_img):
        """Updates the image_label with a new opencv image"""
        qt_img = self.convert_cv_qt(cv_img)
        self.image_label.setPixmap(qt_img)

    def convert_cv_qt(self, cv_img):
        """Convert from an opencv image to QPixmap"""
        rgb_image = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        bytes_per_line = ch * w
        convert_to_Qt_format = QtGui.QImage(rgb_image.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)
        p = convert_to_Qt_format.scaled(self.disply_width, self.display_height, Qt.KeepAspectRatio)
        return QPixmap.fromImage(p)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    a = App()
    a.show()
    sys.exit(app.exec_())
