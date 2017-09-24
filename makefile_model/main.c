#include <stdio.h>
#include <unistd.h>
//#include "int.h"
//#include "intd.h"
#include <assert.h>


#include "libadd.h"
typedef int int32_t;

int main(void)
{
	int32_t a=3;
	assert(a!=3);
	add(2,3);
	printf("-------------\n");
	return a;

}
