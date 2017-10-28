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
    u16 keyb[8];
    u16 keya[8] = {
            0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };
    unordered_map<u16,u32[4]> pairs;
    keyb[2] = keya[2] ^ (u16) 0x8000;
    KeySchedule(keya);
    srand48(time(NULL));
    u32 pa[2], cb[2], ca[2], pb[2];
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
        pairs.insert(cb[1] ^ 0xFF,{ca[0],ca[1],cb[0],cb[1]});
    }
    return 0;
}
