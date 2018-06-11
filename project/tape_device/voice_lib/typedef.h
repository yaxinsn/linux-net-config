/*
 *  typedef.h
 *
 *  Created on: 2012-5-10
 *      Author: jimway
 */

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

//typedef char int8_t;
//typedef unsigned char uint8_t;
//typedef short int16_t;
//typedef unsigned short uint16_t;
//typedef	long int32_t;				// 经测试, 在windows系统上, int/long长度一样, 均为4. 测试代码见后面的例程
//typedef unsigned long uint32_t;

#if 0	// 测试变量长度的例程
void testlxcd1() {
	printf("'char' len: %d\n", sizeof(char));
	printf("'unsigned char' len: %d\n", sizeof(unsigned char));
	printf("'unsigned short' len: %d\n", sizeof(unsigned short));
	printf("'short' len: %d\n", sizeof(short));
	printf("'unsigned int' len: %d\n", sizeof(unsigned int));
	printf("'int' len: %d\n", sizeof(int));
	printf("'unsigned long' len: %d\n", sizeof(unsigned long));
	printf("'long' len: %d\n", sizeof(long));

	puts("End!!!");
}
#endif	// 0

#endif	// __TYPEDEF_H__
