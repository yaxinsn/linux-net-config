
#ifndef SETOPT_H__
#define SETOPT_H__

#define NS_BASE_CTL 1024


#define NS_SO_SET_COUNT			(NS_BASE_CTL)
#define NS_SO_SET_COUNTL		(NS_BASE_CTL + 1)


#define NS_SO_GET_COUNT			(NS_BASE_CTL)
#define NS_SO_GET_COUNTL		(NS_BASE_CTL + 1)


typedef struct kernel_info_st{
	int count;
	long long countl;
}kinfo_t;

#endif  
