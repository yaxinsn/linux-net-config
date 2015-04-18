
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <net/if.h>
#include <net/if_arp.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/rtnetlink.h>
#include <linux/sockios.h>
#include <linux/ip.h>
#include <linux/if_link.h>
#include <linux/if_vlan.h>
#include <linux/if_bridge.h>
#include <linux/if_tunnel.h>
#include <linux/ip6_tunnel.h>
#include <linux/ethtool.h>
#include <linux/fib_rules.h>
#include <linux/version.h>

/* netlink *.h */
#include <netlink/socket.h>
#include <netlink.h>
#include <netlink/msg.h>
#include <netlink/handlers.h>



#ifndef IFF_LOWER_UP
#define IFF_LOWER_UP	0x10000
#endif

#define true 1
#define false 0


typedef  int bool;

static struct nl_sock *sock_rtnl = NULL;



static struct nl_sock *
create_socket(int protocol, int groups)
{
	struct nl_sock *sock;

	sock = nl_socket_alloc();
	if (!sock)
		return NULL;

	if (groups)
		nl_join_groups(sock, groups);

	if (nl_connect(sock, protocol))
		return NULL;

	return sock;
}
static int cb_rtnl_event(struct nl_msg *msg, void *arg)
{
	struct nlmsghdr *nh = nlmsg_hdr(msg);
	struct ifinfomsg *ifi = NLMSG_DATA(nh);
	struct nlattr *nla[__IFLA_MAX];

	if (nh->nlmsg_type != RTM_NEWLINK)
		goto out;

	nlmsg_parse(nh, sizeof(*ifi), nla, __IFLA_MAX - 1, NULL);
	if (!nla[IFLA_IFNAME])
		goto out;
	printf("pid %d::%s is newlink ,link is 0x%x,up is 1,down is 0.\n",getpid(),(char*)nla_data(nla[IFLA_IFNAME]),ifi->ifi_flags & IFF_LOWER_UP);
	//printf("%s is newlink \n",nla_data(nla[IFLA_IFNAME]));
out:
	return 0;
}
static bool
create_event_socket(struct nl_sock *sock, 
		    int (*cb)(struct nl_msg *msg, void *arg))
{		    
	// Install the valid custom callback handler
	nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, cb, NULL);

	// Disable sequence number checking on event sockets
	nl_socket_disable_seq_check(sock);

	// Increase rx buffer size to 65K on event sockets

	//if (nl_socket_set_buffer_size(sock,65535, 0))
	//	return false;

	return true;
	
}

int main(void)
{
	sock_rtnl = create_socket(NETLINK_ROUTE,1);
	if(sock_rtnl == NULL){
		perror("create_socket");
		return -1;
	}
	create_event_socket(sock_rtnl,cb_rtnl_event);
	
		while (1) {
		fd_set rfds;
		int fd, retval;

		fd = nl_socket_get_fd(sock_rtnl);

		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		/* wait for an incoming message on the netlink socket */
		retval = select(fd+1, &rfds, NULL, NULL, NULL);

		if (retval) {
			/* FD_ISSET(fd, &rfds) will be true */
			nl_recvmsgs_default(sock_rtnl);
		}
	}
	return 0;
}
