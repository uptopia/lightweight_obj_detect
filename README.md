# lightweight_obj_detect
## Plugins for (https://github.com/uptopia/MOST2021_assembly)
## Dependencies
* Install Intel OpenVino(2021.4.2)
https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/download.html

## Demo from usb_cam

1. Prepare Environment
```
mkdir -p MOST2021_assembly/src
cd MOST2021_assembly/src
git clone https://github.com/ycxxn/lightweight_obj_detect.git
git clone https://github.com/ros-drivers/usb_cam.git
cd ..
catkin_make
```
2. Open usb_cam
```
roslaunch usb_cam usb_cam-test.launch
```
3. Run object detect
```
roscd lightweight_obj_detect
rosrun lightweight_obj_detect NanoDet_Node
```

## Demo
```
mkdir -p MOST2021_assembly/src
cd MOST2021_assembly/src
git https://github.com/ycxxn/lightweight_obj_detect.git
cd ..
catkin_make
roscd lightweight_obj_detect
rosrun lightweight_obj_detect NanoDet_Node
```
