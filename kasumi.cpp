#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include "kasumi.h"
using namespace std;
/*--------- 16 bit rotate left ------------------------------------------*/
#define ROL16(a,b) (u16)((a<<b)|(a>>(16-b)))
/*-------- globals: The subkey arrays -----------------------------------*/
/*static u16 KLi1[8], KLi2[8];
static u16 KOi1[8], KOi2[8], KOi3[8];
static u16 KIi1[8], KIi2[8], KIi3[8];*/
extern u16 KLi1[8][4], KLi2[8][4];
extern u16 KOi1[8][4], KOi2[8][4], KOi3[8][4];
extern u16 KIi1[8][4], KIi2[8][4], KIi3[8][4];
extern u32 pa[2], pb[2], pc[2];
extern u32 pd[2], cb[2], ca[2];
extern u32 cc[2], cd[2];
/*---------------------------------------------------------------------
 * FI()
 * The FI function (fig 3). It includes the S7 and S9 tables.
 * Transforms a 16-bit value.
 *---------------------------------------------------------------------*/
static u16 FI( u16 in, u16 subkey )
{
    u16 nine, seven;
    static u16 S7[] = {
            54, 50, 62, 56, 22, 34, 94, 96, 38, 6, 63, 93, 2, 18,123, 33,
            55,113, 39,114, 21, 67, 65, 12, 47, 73, 46, 27, 25,111,124, 81,
            53, 9,121, 79, 52, 60, 58, 48,101,127, 40,120,104, 70, 71, 43,
            20,122, 72, 61, 23,109, 13,100, 77, 1, 16, 7, 82, 10,105, 98,
            117,116, 76, 11, 89,106, 0,125,118, 99, 86, 69, 30, 57,126, 87,
            112, 51, 17, 5, 95, 14, 90, 84, 91, 8, 35,103, 32, 97, 28, 66,
            102, 31, 26, 45, 75, 4, 85, 92, 37, 74, 80, 49, 68, 29,115, 44,
            64,107,108, 24,110, 83, 36, 78, 42, 19, 15, 41, 88,119, 59, 3};
    static u16 S9[] = {
            167,239,161,379,391,334, 9,338, 38,226, 48,358,452,385, 90,397,
            183,253,147,331,415,340, 51,362,306,500,262, 82,216,159,356,177,
            175,241,489, 37,206, 17, 0,333, 44,254,378, 58,143,220, 81,400,
            95, 3,315,245, 54,235,218,405,472,264,172,494,371,290,399, 76,
            165,197,395,121,257,480,423,212,240, 28,462,176,406,507,288,223,
            501,407,249,265, 89,186,221,428,164, 74,440,196,458,421,350,163,
            232,158,134,354, 13,250,491,142,191, 69,193,425,152,227,366,135,
            344,300,276,242,437,320,113,278, 11,243, 87,317, 36, 93,496, 27,
            487,446,482, 41, 68,156,457,131,326,403,339, 20, 39,115,442,124,
            475,384,508, 53,112,170,479,151,126,169, 73,268,279,321,168,364,
            363,292, 46,499,393,327,324, 24,456,267,157,460,488,426,309,229,
            439,506,208,271,349,401,434,236, 16,209,359, 52, 56,120,199,277,
            465,416,252,287,246, 6, 83,305,420,345,153,502, 65, 61,244,282,
            173,222,418, 67,386,368,261,101,476,291,195,430, 49, 79,166,330,
            280,383,373,128,382,408,155,495,367,388,274,107,459,417, 62,454,
            132,225,203,316,234, 14,301, 91,503,286,424,211,347,307,140,374,
            35,103,125,427, 19,214,453,146,498,314,444,230,256,329,198,285,
            50,116, 78,410, 10,205,510,171,231, 45,139,467, 29, 86,505, 32,
            72, 26,342,150,313,490,431,238,411,325,149,473, 40,119,174,355,
            185,233,389, 71,448,273,372, 55,110,178,322, 12,469,392,369,190,
            1,109,375,137,181, 88, 75,308,260,484, 98,272,370,275,412,111,
            336,318, 4,504,492,259,304, 77,337,435, 21,357,303,332,483, 18,
            47, 85, 25,497,474,289,100,269,296,478,270,106, 31,104,433, 84,
            414,486,394, 96, 99,154,511,148,413,361,409,255,162,215,302,201,
            266,351,343,144,441,365,108,298,251, 34,182,509,138,210,335,133,
            311,352,328,141,396,346,123,319,450,281,429,228,443,481, 92,404,
            485,422,248,297, 23,213,130,466, 22,217,283, 70,294,360,419,127,
            312,377, 7,468,194, 2,117,295,463,258,224,447,247,187, 80,398,
            284,353,105,390,299,471,470,184, 57,200,348, 63,204,188, 33,451,
            97, 30,310,219, 94,160,129,493, 64,179,263,102,189,207,114,402,
            438,477,387,122,192, 42,381, 5,145,118,180,449,293,323,136,380,
            43, 66, 60,455,341,445,202,432, 8,237, 15,376,436,464, 59,461};
    /* The sixteen bit input is split into two unequal halves, *
     * nine bits and seven bits - as is the subkey */
    nine = (u16)(in>>7);
    seven = (u16)(in&0x7F);
    /* Now run the various operations */
    nine = (u16)(S9[nine] ^ seven);
    seven = (u16)(S7[seven] ^ (nine & 0x7F));
    seven ^= (subkey>>9);
    nine ^= (subkey&0x1FF);
    nine = (u16)(S9[nine] ^ seven);
    seven = (u16)(S7[seven] ^ (nine & 0x7F));
    in = (u16)((seven<<9) + nine);

    // printf("FI %x\n", in);

    return( in );
}
/*---------------------------------------------------------------------
 * FO()
 * The FO() function.
 * Transforms a 32-bit value. Uses <index> to identify the
 * appropriate subkeys to use.
 *---------------------------------------------------------------------*/
static u32 FO( u32 in, int index, int branch )
{
    u16 left, right;
    /* Split the input into two 16-bit words */
    left = (u16)(in>>16);
    right = (u16) in;
    /* Now apply the same basic transformation three times */
    left ^= KOi1[index][branch];
    left = FI( left, KIi1[index][branch] );
    left ^= right;
    right ^= KOi2[index][branch];
    right = FI( right, KIi2[index][branch] );
    right ^= left;
    left ^= KOi3[index][branch];
    left = FI( left, KIi3[index][branch] );
    left ^= right;
    in = (((u32)right)<<16)+left;

    // printf("FO %x\n", in);

    return( in );
}
/*---------------------------------------------------------------------
 * FL()
 * The FL() function.
 * Transforms a 32-bit value. Uses <index> to identify the
 * appropriate subkeys to use.
 *---------------------------------------------------------------------*/
static u32 FL( u32 in, int index, int branch )
{
    u16 l, r, a, b;
    /* split out the left and right halves */
    l = (u16)(in>>16);
    r = (u16)(in);
    /* do the FL() operations */
    a = (u16) (l & KLi1[index][branch]);
    r ^= ROL16(a,1);
    b = (u16)(r | KLi2[index][branch]);
    l ^= ROL16(b,1);
    /* put the two halves back together */
    in = (((u32)l)<<16) + r;

    // printf("FL %x\n", in);

    return( in );
}
/*---------------------------------------------------------------------
 * Kasumi()
 * the Main algorithm (fig 1). Apply the same pair of operations
 * four times. Transforms the 64-bit input.
 *---------------------------------------------------------------------*/
void Kasumi( u32 *data , int branch )
{
    u32 left, right, temp;
    //DWORD *d;
    int n;
    /* Start by getting the data into two 32-bit words (endian corect) */
    //d = (DWORD*)data;
    left = data[0];
    //(((u32)d[0].b8[0])<<24)+(((u32)d[0].b8[1])<<16)+(d[0].b8[2]<<8)+(d[0].b8[3]);
    right = data[1];
    //(((u32)d[1].b8[0])<<24)+(((u32)d[1].b8[1])<<16)+(d[1].b8[2]<<8)+(d[1].b8[3]);
    n = 0;
    do{ temp = FL( left, n, branch );
        temp = FO( temp, n++, branch );

        // printf("right f \t%x %x\n", right, temp);

        right ^= temp;

        // printf("left right round\t%x %x %i\n", right, left, n);

        temp = FO( right, n, branch);
        temp = FL( temp, n++, branch );

        // printf("right f \t%x %x\n", left, temp);

        left ^= temp;

        // printf("left right round\t%x %x %i\n", left, right, n);
    }while( n<=7 );
    data[0]=left;
    data[1]=right;
}
/*---------------------------------------------------------------------
 * Kasumid()
 * Decryption algorithm . Apply the same pair of operations
 * four times. Transforms the 64-bit input.
 *---------------------------------------------------------------------*/
void Kasumid( u32 *data, int branch )
{
    u32 left, right, temp;
    //DWORD *d;
    int n;
    /* Start by getting the data into two 32-bit words (endian corect) */
    //d = (DWORD*)data;
    left = data[0];
    //(((u32)d[0].b8[0])<<24)+(((u32)d[0].b8[1])<<16)+(d[0].b8[2]<<8)+(d[0].b8[3]);
    right = data[1];
    //(((u32)d[1].b8[0])<<24)+(((u32)d[1].b8[1])<<16)+(d[1].b8[2]<<8)+(d[1].b8[3]);
    n = 7;
    do{ temp = FO( right, n, branch );
        temp = FL( temp, n--, branch );

        // printf("right f \t%x %x\n", right, temp);

        left ^= temp;

        // printf("left right round\t%x %x %i\n", right, left, n);

        temp = FL( left, n, branch );
        temp = FO( temp, n--, branch );

        // printf("right f \t%x %x\n", left, temp);

        right ^= temp;

        // printf("left right round\t%x %x %i\n", left, right, n);
    }while( n>=0 );
    data[0]=left;
    data[1]=right;
}
/*---------------------------------------------------------------------
 * KeySchedule()
 * Build the key schedule. Most "key" operations use 16-bit
 * subkeys so we build u16-sized arrays that are "endian" correct.
 *---------------------------------------------------------------------*/
void KeySchedule(u16 *key, int branch )
{
    static u16 C[] = {
            0x0123,0x4567,0x89AB,0xCDEF, 0xFEDC,0xBA98,0x7654,0x3210 };
    u16 Kprime[8];
    int n;
    /* Now build the K'[] keys */
    for( n=0; n<8; ++n )
        Kprime[n] = (u16)(key[n] ^ C[n]);
    /* Finally construct the various sub keys */
    for( n=0; n<8; ++n )
    {
        KLi1[n][branch] = ROL16(key[n],1);
        KLi2[n][branch] = Kprime[(n+2)&0x7];
        KOi1[n][branch] = ROL16(key[(n+1)&0x7],5);
        KOi2[n][branch] = ROL16(key[(n+5)&0x7],8);
        KOi3[n][branch] = ROL16(key[(n+6)&0x7],13);
        KIi1[n][branch] = Kprime[(n+4)&0x7];
        KIi2[n][branch] = Kprime[(n+3)&0x7];
        KIi3[n][branch] = Kprime[(n+7)&0x7];
    }
}

void attack(){
    u32 sample_size = (u32)(pow(2, 24));
    u32 arbitraary = 0x6a6f6e61;
    u32 cons;
    u32 temp[8];
    u16 keyb[8], keyc[8], keyd[8];
    u16 keya[8] = {
            0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };
    unordered_multimap<u32,u32*> pairs;
    unordered_map<u32,u32*> Quartets;
    for (int i = 0; i < 8; ++i) {
        keyb[i] = keya[i];
        keyc[i] = keya[i];
        keyd[i] = keya[i];
    }
    keyb[2] = keya[2] ^ (u16) 0x8000;
    keyc[6] = keya[6] ^ (u16) 0x8000;
    keyd[2] = keyc[2] ^ (u16) 0x8000;
    KeySchedule(keya, 0);
    KeySchedule(keyb, 1);
    KeySchedule(keyc, 2);
    KeySchedule(keyd, 3);
    cons = FL(FO(arbitraary, 7, 1), 7, 1) ^ FL(FO(arbitraary ^ 0x00100000, 7, 3), 7, 3);
    srand48(time(NULL));
    for (int i = 0; i < sample_size; ++i) {
        pa[0] = (u32) mrand48();
        pa[1] = arbitraary;
        ca[0] = pa[0];
        ca[1] = pa[1];
        Kasumid(pa, 1);
        cb[0] = pa[0];
        cb[1] = pa[1] ^ 0x00100000;
        pb[0] = cb[0];
        pb[1] = cb[1];
        Kasumi(cb, 2);
        temp[0] = ca[0];
        temp[1] = ca[1];
        temp[2] = cb[0];
        temp[3] = cb[1];
        pairs.insert(make_pair( cb[1] ,temp));
    }
    srand48(time(NULL));
    for (int i = 0; i < sample_size; ++i) {
        pc[0] = (u32) mrand48();
        pc[1] = arbitraary;
        cc[0] = pc[0];
        cc[1] = pc[1];
        Kasumid(pc, 3);
        cd[0] = pc[0];
        cd[1] = pc[1] ^ 0x00100000;
        pd[0] = cd[0];
        pd[1] = cd[1];
        Kasumi(cd, 4);
        auto its = pairs.equal_range(cd[1] ^ 0x00100000);
        for (auto it = its.first; it != its.second; ++it){
            temp[0] = it->second[0];  // ca[0]
            temp[1] = it->second[1];  // ca[1]
            temp[2] = it->second[2];  // cb[0]
            temp[3] = it->second[3];  // cb[1]
            if (temp[0] ^ cc[0] == cons){
                temp[4] = cc[0];
                temp[5] = cc[1];
                temp[6] = cd[0];
                temp[7] = cd[1];
                Quartets.insert(make_pair(temp[0] ^ temp[4], temp));
            }
        }
    }
    cout << pairs.size();
    cout << "\n";
    cout << Quartets.size();
}

int main(){
    attack();
}