/*---------------------------------------------------------
* Kasumi.h
*---------------------------------------------------------*/
#ifndef KASUMI_KASUMI_H
#define KASUMI_KASUMI_H

//typedef unsigned char u8;
typedef unsigned short u16;
//typedef unsigned long u32;
typedef unsigned int u32;

void inline KeySchedule( u16 *key , int branch );
void Kasumi( u32 *data, int branch );
void Kasumid( u32 *data, int branch );
static inline u32 FO( u32 in, int index , int branch );
static inline u32 FL( u32 in, int index , int branch );
u16 KLi1[8][4], KLi2[8][4];
u16 KOi1[8][4], KOi2[8][4], KOi3[8][4];
u16 KIi1[8][4], KIi2[8][4], KIi3[8][4];
u32 pa[2], pb[2], pc[2];
u32 pd[2], cb[2], ca[2];
u32 cc[2], cd[2];

#include <stdio.h>

#endif //KASUMI_KASUMI_H