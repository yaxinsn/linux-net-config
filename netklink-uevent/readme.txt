
2019/1/13 15:18:23
	netlink-uevent��ʹ��netlink��һ������(NETLINK_KOBJECT_UEVENT)��ר�����������Ƿ�������ġ�Ӧ�ò��udev����ʹ��NETLINK_KOBJECT_UEVENT,֧����hotplug.

���ں˷�������uevent����Ϣ�ṹ���£�
add@/devices/pci0000:00/0000:00:11.0/0000:02:03.0/usb1/1-1/1-1:1.0/host13/target13:0:0/13:0:0:0/bsg/13:0:0:0
ACTION=add
DEVPATH=/devices/pci0000:00/0000:00:11.0/0000:02:03.0/usb1/1-1/1-1:1.0/host13/target13:0:0/13:0:0:0/bsg/13:0:0:0
SUBSYSTEM=bsg
MAJOR=248
MINOR=3
DEVNAME=bsg/13:0:0:0
SEQNUM=5123
---------------
ǰ�����Ǳ�Ҫ���
SUBSYSTEM�����Ǹ��ӵĻ���������
	
	
	
	
	�������ε��豸��
��mouseΪ������	
echo add > /sys/module/psmouse/uevent //����д��addʱ���ں����·�һ��uevent��Ϣ��

add@/module/psmouse
ACTION=add
DEVPATH=/module/psmouse
SUBSYSTEM=module
SEQNUM=5132
