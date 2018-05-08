# pre

## 1. Setting  user environment variables
a. Copy the 'opencl-intel-fpga-env.sh' to '/etc/profile.d'
```bash
sudo cp opencl-intel-fpga-env.sh /etc/profile.d 
``` 
b. Open the file, modify the variable 'ALTERAOCLSDKROOT' to the path where your Intel OpenCL FPGA SDK installed,
and modify the variable 'QUARTUS_ROOTDIR_OVERRIDE' to the path where your Quartus installed.

c. Restart your system or run follow command to activate these variables:
```bash
source /etc/profile
```

## 2. Link essential dynamic libraries to '/usr/lib'
Run follow commands:
```bash
sudo ln -s $INTELFPGAOCLSDKROOT/host/linux64/lib/* /usr/lib
sudo ln -s $AOCL_BOARD_PACKAGE_ROOT/linux64/lib/* /usr/lib
sudo ldconfig
```

# run

1. get platform

```c++
platform = findPlatform("Intel");
```

2. query available divece
```c++
device.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
```
