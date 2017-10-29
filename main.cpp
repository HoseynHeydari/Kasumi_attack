#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "kasumi.h"
#include <unordered_map>

using namespace std;

int main(void){
    u32 sample_size = (u32)(pow(2, 24));
    u32 arbitraary = 0x6a6f6e61;
    u32 temp[4];
    u16 keyb[8], keyc[8], keyd[8];
    u16 keya[8] = {
            0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };
    unordered_map<u16,u32*> pairs1;
    unordered_map<u16,u32*> pairs2;
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
    u32 pa[2], pb[2], pc[2], pd[2], cb[2], ca[2], cc[2], cd[2];
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
        temp[0] = ca[0];
        temp[1] = ca[1];
        temp[2] = cb[0];
        temp[3] = cb[1];
        pairs1[cb[1] ^ 0xFF] = temp;
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
        temp[0] = cc[0];
        temp[1] = cc[1];
        temp[2] = cd[0];
        temp[3] = cd[1];
        pairs2[cd[1] ^ 0xFF] = temp;
    }
    return 0;
}
