

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/capability.h>
#include <linux/fcntl.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/if_packet.h>
#include <linux/wireless.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <net/net_namespace.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/ioctls.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mutex.h>

#include <linux/crypto.h>
#if 0
#define NS_BASE_CTL 1024


#define NS_SO_SET_COUNT			(NS_BASE_CTL)
#define NS_SO_SET_COUNTL		(NS_BASE_CTL + 1)


#define NS_SO_GET_COUNT			(NS_BASE_CTL)
#define NS_SO_GET_COUNTL		(NS_BASE_CTL + 1)


typedef struct kernel_info_st{
	int count;
	long long countl;
}kinfo_t;
#else

#include "setopt.h"	
#endif

kinfo_t g_info;

/******************************************************************************************/

static int
do_ns_set_ctl(struct sock *sk, int cmd, void __user *user, unsigned int len)
{
	char buf[100];
	int ret;
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (copy_from_user(buf, user, len) != 0) {
		ret = -EFAULT;
	}
	memcpy(&g_info,buf,sizeof(kinfo_t));
	printk( "%s: g_info count %d countl %lld \n", __func__, g_info.count, g_info.countl);
	printk( "%s: g_info count %d  \n", __func__, g_info.count);
	//simple_strtol(buf, NULL, 0);//lib vsprintf.c

}
static int do_ns_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;
	if (!capable(CAP_NET_ADMIN))
	return -EPERM;
	
	switch (cmd) {
		case NS_SO_GET_COUNT:
			if (copy_to_user(user, &g_info.count, sizeof(g_info.count)) != 0)
				ret = -EFAULT;
			else
				ret = 0;
			break;
		case NS_SO_GET_COUNTL:
			if (copy_to_user(user, &g_info.countl, sizeof(g_info.countl)) != 0)
				ret = -EFAULT;
			else
				ret = 0;
			break;
		default:
			printk("err cmd %d. \n",cmd);
	}
	return ret;
		
}

static struct nf_sockopt_ops ns_ops = {
	.pf		= PF_INET,
	.set_optmin	= NS_BASE_CTL,
	.set_optmax	= NS_BASE_CTL+1,
	.set		= do_ns_set_ctl,
//#ifdef CONFIG_COMPAT
	//.compat_set	= compat_do_ns_set_ctl,
//#endif
	.get_optmin	= NS_BASE_CTL,
	.get_optmax	= NS_BASE_CTL+1,
	.get		= do_ns_get_ctl,
//#ifdef CONFIG_COMPAT
//	.compat_get	= compat_do_ip6t_get_ctl,
//#endif
	.owner		= THIS_MODULE,
};


/********************** init proc ************/


/****************init and cleanup ************/
static int __init _ns_module_init(void)
{
	nf_register_sockopt(&ns_ops);
	printk("--ns module init! \n");
	return 0;
}

static void __exit _ns_module_cleanup(void)
{

	nf_unregister_sockopt(&ns_ops);	
	printk("--ns module clearup! \n");
	
}


module_init(_ns_module_init);
module_exit(_ns_module_cleanup);

MODULE_LICENSE("GPL");

