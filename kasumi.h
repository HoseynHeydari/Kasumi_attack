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
void Kasumi( u32 *data );
void Kasumid( u32 *data );
static inline u32 FO( u32 in, int index );
static inline u32 FL( u32 in, int index );
u16 KLi1[8][4], KLi2[8][4];
u16 KOi1[8][4], KOi2[8][4], KOi3[8][4];
u16 KIi1[8][4], KIi2[8][4], KIi3[8][4];

#include <stdio.h>

#endif //KASUMI_KASUMI_H