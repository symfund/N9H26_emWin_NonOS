#ifndef _PORT_H_
#define _PORT_H_

#if 0
#define boolean int
#define int8_t   char
#define uint8_t  unsigned char
#define int16_t  short
#define uint16_t unsigned short
#define int32_t  int
#define uint32_t unsigned int
#define int64_t  __int64
#define uint64_t unsigned __int64
#else
#include <stdint.h>
#endif
#define bool int

typedef unsigned char	Uint8;
typedef unsigned int	Uint32;
typedef unsigned short	Uint16;

#define CACHE_LINE  32
#define ptr_t uint32_t
#define		CACHE_BIT31 0x80000000

#ifndef NULL
#define	NULL	0
#endif
#define mdelay	sysDelay


#define	M_DEBUG	sysprintf
#define printk sysprintf

#if defined(__GNUC__)
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#elif defined(__CC_ARM)
#define PACK(__Declaration__) __Declaration__ __attribute__((packed))
#elif defined(_MSC_VER)
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#elif defined(__GNUC__) || defined(__CC_ARM)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif

#define ALIGNED_TYPE_(t,x) typedef t ALIGNED_(x)

#if defined (_MSC_VER)
#define __weak
#endif

#endif

