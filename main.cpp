#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kasumi.h"
#include <map>
#include <unordered_map>

using namespace std;

int main(void){
    u16 keyb[8], keya[8] = {
            0x9900, 0xAABB, 0xCCDD, 0xEEFF, 0x1122, 0x3344, 0x5566, 0x7788
    };
//    unordered_map
    keyb[2] = keya[2] ^ (u16) 0x8000;
    KeySchedule(keya);
    srand48(time(NULL));
    u32 ca[64][2], cb[64][2], pa[64][2], pb[64][2];
    for (int i = 0; i < 64; ++i) {
        pa[i] = {(u32) mrand48(), 0x6a6f6e61};
        ca[i][0] = pa[i][0];
        ca[i][1] = pa[i][1];
        Kasumid(pa[i]);
        cb[i][0] = pa[i][0];
        cb[i][1] = pa[i][1] ^ 0x00100000;
        pb[i][0] = cb[i][0];
        pb[i][1] = cb[i][1];
        Kasumi(cb[i]);
    }
    return 0;
}
