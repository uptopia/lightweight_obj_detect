# lightweight_obj_detect
## Plugins for (https://github.com/uptopia/MOST2021_assembly)
## Dependencies
* Install Intel OpenVino(2021.4.2)
https://www.intel.com/content/www/us/en/developer/tools/openvino-toolkit/download.html
Environment: Runtime
Operating System: Linux
OpenVINO Toolkit Version: 2021.4.2 LTS
Language: C++
Distribution: Offline Installer

```
The following steps apply to Ubuntu*, CentOS*, Red Hat* Enterprise Linux*, and Yocto* versions of Linux.

Step 1: Open a command prompt terminal window.

Step 2: Unpack the .tgz file.

		tar -xvzf l_openvino_toolkit_p_<version>.tgz
		tar -xvzf l_openvino_toolkit_p_2021.4.752.tgz		

Step 3: Go to the l_openvino_toolkit_p_<version> directory:

		cd l_openvino_toolkit_p_<version>
		cd l_openvino_toolkit_p_2021.4.752	

Step 4: Install dependencies.

		cd /opt/intel/openvino_2021.4.752/install_dependencies/
		sudo -E ./install_openvino_dependencies.sh

Step 4: Choose your installation option.

    GUI Installation Wizard: sudo ./install_GUI.sh
    Command-Line Instructions: sudo ./install.sh

Step 5: Follow the instructions in the installer.
		
		gedit ~/.bashrc
		source /opt/intel/openvino_2021.4.752/bin/setupvars.sh

Step 6: The core components are installed. To complete the configuration, follow the instructions in the Complete Installation Guide.
```

## Demo
```
mkdir -p MOST2021_assembly/src
cd MOST2021_assembly/src
git https://github.com/uptopia/lightweight_obj_detect.git
```

change main.cpp
const char *nanodet_xml = "/home/dual-arm/MOST_lightweight/src/lightweight_obj_detect/nanodet.xml";
to
const char *nanodet_xml = "<change_to_actual_dir>/MOST2021_assembly/src/lightweight_obj_detect/nanodet.xml";

```
cd ..
catkin_make
. devel/setup.bash
rosrun lightweight_obj_detect NanoDet_Node
```
