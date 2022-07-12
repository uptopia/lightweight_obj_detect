# lightweight_obj_detect
## Plugins for (https://github.com/uptopia/MOST2021_assembly)
## Dependencies
* Install Intel OpenVino

# Demo from usb_cam
```
1. Prepare Environment
mkdir MOST2021_assembly
cd MOST2021_assembly
git clone git@github.com:ycxxn/lightweight_obj_detect.git src
git https://github.com/ros-drivers/usb_cam.git src
catkin_make


roscd lightweight_obj_detect
rosrun lightweight_obj_detect NanoDet_Node
```

# Demo
```
mkdir MOST2021_assembly
cd MOST2021_assembly
git git@github.com:ycxxn/lightweight_obj_detect.git src
catkin_make
roscd lightweight_obj_detect
rosrun lightweight_obj_detect NanoDet_Node
```
