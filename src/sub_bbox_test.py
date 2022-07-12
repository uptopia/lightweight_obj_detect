import rospy
import cv2

from openvino_test.msg import bbox, bboxes

from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image as msg_Image

class Sub_Node:
    def __init__(self):
        rospy.init_node("Sub_Node")
        self.bridge = CvBridge()
        rospy.Subscriber("/usb_cam/image_raw", msg_Image, self.imageCallback)
        rospy.Subscriber("/Det_other_objs_bboxes", bboxes, self.bboxes_Callback)
        rospy.spin()

    def imageCallback(self, img_msg):
        self.cv_image = self.bridge.imgmsg_to_cv2(img_msg, "bgr8")

    def bboxes_Callback(self, bbox_msg):
        print(bbox_msg)

        for bb in bbox_msg.bboxes:
            cv2.rectangle(self.cv_image, (bb.xmin, bb.ymin), (bb.xmax, bb.ymax), (0, 255, 0), 2)

        cv2.imshow("img", self.cv_image)
        cv2.waitKey(1)


if __name__ == "__main__":
    Sub_Node()
    # cv2.waitKey(1)