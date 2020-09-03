import cv2
import numpy as np

img_url = '/tmp/mozilla_tuanna800/113.879997_3.500010_2.000010_46.500000_1418_1595991199104.jpg'
# img_url = '/home/tuanna80/Pictures/vacham_issue/data_train/jhxmxmuitjzyfvsaxrcl_1595990304635_ViolationRawFrame.jpg'
list_polygon = [1447, 589, 1066, 626, 265, 632, 270, 717, 554, 797, 1150, 813, 1791, 650]
list_point = []
for i in range(0, len(list_polygon), 2):
    point2i = (int(list_polygon[i]), int(list_polygon[i+1]))
    list_point.append(point2i)

list_point = np.asarray(list_point, dtype=np.int32)

img = cv2.imread(img_url)
cv2.polylines(img, [list_point], True, (0,0,255), 2)

cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()