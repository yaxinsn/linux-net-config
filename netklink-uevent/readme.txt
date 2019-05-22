
2019/1/13 15:18:23
	netlink-uevent是使用netlink的一个子项(NETLINK_KOBJECT_UEVENT)，专门用于侦听是否有外设的。应用层的udev服务使用NETLINK_KOBJECT_UEVENT,支持了hotplug.

从内核发上来的uevent的消息结构如下：
add@/devices/pci0000:00/0000:00:11.0/0000:02:03.0/usb1/1-1/1-1:1.0/host13/target13:0:0/13:0:0:0/bsg/13:0:0:0
ACTION=add
DEVPATH=/devices/pci0000:00/0000:00:11.0/0000:02:03.0/usb1/1-1/1-1:1.0/host13/target13:0:0/13:0:0:0/bsg/13:0:0:0
SUBSYSTEM=bsg
MAJOR=248
MINOR=3
DEVNAME=bsg/13:0:0:0
SEQNUM=5123
---------------
前三行是必要子项。
SUBSYSTEM以下是附加的环境变量。
	
	
	
	
	对于冷插拔的设备：
发mouse为你例：	
echo add > /sys/module/psmouse/uevent //当你写入add时，内核重新发一个uevent消息。

add@/module/psmouse
ACTION=add
DEVPATH=/module/psmouse
SUBSYSTEM=module
SEQNUM=5132
