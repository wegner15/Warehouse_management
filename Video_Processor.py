import cv2
import numpy as np
import requests
from pyzbar.pyzbar import decode

target_products = {
    "target": None
}

bot_url = "http://192.168.217.67"


def decoder(image):
    gray_img = cv2.cvtColor(image, 0)
    barcode = decode(gray_img)

    for obj in barcode:
        points = obj.polygon
        (x, y, w, h) = obj.rect
        pts = np.array(points, np.int32)
        pts = pts.reshape((-1, 1, 2))
        cv2.polylines(image, [pts], True, (0, 255, 0), 3)

        barcodeData = obj.data.decode("utf-8")
        barcodeType = obj.type

        string = "Product: " + str(barcodeData)
        if target_products["target"]:
            string = string + " |Target: " + target_products["target"]
        if str(barcodeData) == target_products["target"]:
            speed_request = requests.get(url=bot_url + "/?pick=" + str(1))
            if speed_request.text == "OK":
                print("Request accepted")

            cv2.putText(image, string, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
        else:
            cv2.putText(image, string, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 2)
        print("Barcode: " + barcodeData + " | Type: " + barcodeType)
