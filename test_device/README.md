# Overview

This linux kernel module is a simple character device driver. It functions as a FIFO queue for bytes. When the device is opened and written to, the bytes written are stored in a queue of size 1024 bytes. When bytes are read from the device, the bytes in the queue are removed in FIFO fashion and returned to the reader. If too many bytes are written, some bytes will be truncated if there is no room in the queue to store them. If too many bytes are requested to be read, only the bytes in the queue remaining are read and returned to the reader.

# Usage

First, use make to run Makefile. Then, use insmod to install the resulting fifo_device.ko file. At this point, you should see that the module was successfully handled if you run dmesg. Now you can open, read from, write to, and release the device. It should be located at /dev/fifodev in the filesystem. Once you have finished, you can remove the module with rmmod. Below is an example usage for Ubuntu 14.04:

'''

cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ make
make -C /lib/modules/4.4.0-109-generic/build M=/home/cop4600/Documents/pa2/test_device modules
make[1]: Entering directory `/usr/src/linux-headers-4.4.0-109-generic'
  CC [M]  /home/cop4600/Documents/pa2/test_device/fifo_device.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/cop4600/Documents/pa2/test_device/fifo_device.mod.o
  LD [M]  /home/cop4600/Documents/pa2/test_device/fifo_device.ko
make[1]: Leaving directory `/usr/src/linux-headers-4.4.0-109-generic'
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ sudo insmod fifo_device.ko
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ dmesg | tail -4
[20357.507951] Initializing the FIFO device...
[20357.507955] Registered FIFO device with major number 246.
[20357.507967] Created FIFO device class.
[20357.511433] Successfully created FIFO device!
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ sudo -i
root@cop4600-VirtualBox:~# echo "Here are some bytes!" > /dev/fifodev
root@cop4600-VirtualBox:~# echo "Even more bytes!" > /dev/fifodev
root@cop4600-VirtualBox:~# cat /dev/fifodev
Here are some bytes!
Even more bytes!
root@cop4600-VirtualBox:~# exit
logout
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ sudo rmmod fifo_device.ko
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ dmesg | tail -12
[20524.582219] FIFO device opened.
[20524.582228] 21 bytes sent to FIFO device, 21 bytes written.
[20524.582230] FIFO device closed.
[20536.986134] FIFO device opened.
[20536.986143] 17 bytes sent to FIFO device, 17 bytes written.
[20536.986145] FIFO device closed.
[20542.023931] FIFO device opened.
[20542.023942] 38 bytes read from FIFO device.
[20542.023947] 0 bytes read from FIFO device.
[20542.023949] FIFO device closed.
[20585.037183] Cleaning up FIFO device!
[20585.040739] FIFO device cleaned up!
cop4600@cop4600-VirtualBox:~/Documents/pa2/test_device$ 


'''

# Authors

This module was written by Philip Rodriguez, Steven Chen, and Ryan Beck.
