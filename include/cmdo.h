#ifndef __CMDO_H__
#define __CMDO_H__
/*
 * Copyright (c) 2015 Jim Wood
 * All rights reserved.
 *
 *  cmdo.h - main include file
 */


///////////////////////////////////////////////////////////////////////////////
//             Includes
///////////////////////////////////////////////////////////////////////////////
#include "config.h"

///////////////////////////////////////////////////////////////////////////////
//             Common Types
///////////////////////////////////////////////////////////////////////////////
typedef unsigned long long	u64;
typedef signed long long	s64;
typedef unsigned long		u32;
typedef signed long		s32;
typedef unsigned short		u16;
typedef signed short		s16;
typedef unsigned char		u8;
typedef signed char		s8;

#ifndef NULL
#define NULL ((void *)0)
#endif

///////////////////////////////////////////////////////////////////////////////
//             Definitions
///////////////////////////////////////////////////////////////////////////////
// macros based on clock rate for accurate utimer
#define BIT(x)    			(1L << (x))
#define MHZ(x)    			((x)*(1000000))
#define KHZ(x)    			((x)*(1000))
#define MBYTE(x)    			((x)*(1024*1024))
#define KBYTE(x)    			((x)*(1024))


#define USEC_TICKS  			((CPU_CLK/MHZ(1)) + 1)  // round up
#define TICKSPERUS  			((CPU_CLK/MHZ(1)) + 1)  // MAC clock ticks
#define SEC_TICKS   			(1000 * (CPU_CLK/MHZ(1)) + 1)
#define USEC(x)				((x)*USEC_TICKS)
#define SEC(x)				((x)*SEC_TICKS)

#define REG_WRITE(x,y)  		(*(_Uncached u32 *)(x) = (u32)(y))
#define REG_WRITE8(x,y)  		(*(_Uncached u8 *)(x) = (u8)(y))
#define REG_WRITE16(x,y)  		(*(_Uncached u16 *)(x) = (u16)(y))
#define REG_READ(x)     		(*(_Uncached u32 *)(x))
#define REG_READ8(x)     		(*(_Uncached u8 *)(x))
#define REG_READ16(x)     		(*(_Uncached u16 *)(x))
#define REG_OR(x,y) 			(REG_WRITE((x),REG_READ(x) | (u32)(y)))
#define REG_AND(x,y) 			(REG_WRITE((x),REG_READ(x) & (u32)(y)))
#define REG_CLEAR_BITS(x,y) 		(REG_WRITE((x),REG_READ(x) & (u32)~(y)))
#define SWAP32(x)			((((x)&0xff000000) >> 24) | \
					(((x)&0x00ff0000) >> 8) | \
					(((x)&0x0000ff00) << 8) | \
					(((x)&0x000000ff) << 24))
#define SWAP16(x)			((((x)&0xff00) >> 8) | (((x)&0x00ff) <<8) )
#define SWAP_MID(x)			((((x) >> 16) & 0xffff) | (((x) << 16) & 0xffff0000))

#endif  // __CMDO_H__
