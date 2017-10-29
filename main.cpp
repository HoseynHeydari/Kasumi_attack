#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "kasumi.h"
#include <unordered_map>

using namespace std;

extern u32 pa[2], pb[2], pc[2];
extern u32 pd[2], cb[2], ca[2];
extern u32 cc[2], cd[2];

bool Identifier(){
    bool iden;
    iden = (ca[0] ^ FL(FO(ca[1], 7 ), 7) == cc[0] ^ FL(FO(cc[1], 7 ), 7));
    return iden;
}


int main(void){
    u32 sample_size = (u32)(pow(2, 24));
    u32 arbitraary = 0x6a6f6e61;
    u32 temp1[4];
    u32 temp2[8];
    u16 keyb[8], keyc[8], keyd[8];
    u16 keya[8] = {
            0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };
    unordered_map<u16,u32*> pairs;
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
    srand48(time(NULL));
    for (int i = 0; i < sample_size; ++i) {
        pa[0] = (u32) mrand48();
        pa[1] = arbitraary;
        ca[0] = pa[0];
        ca[1] = pa[1];
        Kasumid(pa);
        cb[0] = pa[0];
        cb[1] = pa[1] ^ 0x00100000;
        pb[0] = cb[0];
        pb[1] = cb[1];
        Kasumi(cb);
        temp1[0] = ca[0];
        temp1[1] = ca[1];
        temp1[2] = cb[0];
        temp1[3] = cb[1];
        pairs[cb[1] ^ 0xFF] = temp1;
    }
    for (int i = 0; i < sample_size; ++i) {
        pc[0] = (u32) mrand48();
        pc[1] = arbitraary;
        cc[0] = pc[0];
        cc[1] = pc[1];
        Kasumid(pc);
        cd[0] = pc[0];
        cd[1] = pc[1] ^ 0x00100000;
        pd[0] = cd[0];
        pd[1] = cd[1];
        Kasumi(cd);
        if (true){
            Quartets[ca[0] ^ cc[0]] = temp2;
        }
    }
    return 0;
}
