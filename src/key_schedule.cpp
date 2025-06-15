#include "key_schedule.hpp"
#include "gfn.hpp"
#include "sboxes.hpp"
#include <cstdint>
#include <iomanip> 
#include <iostream>
#include <cstring>

using namespace std;

namespace clefia {

const uint32_t CON128[60] = {
    0xf56b7aeb, 0x994a8a42, 0x96a4bd75, 0xfa854521,
    0x735b768a, 0x1f7abac4, 0xd5bc3b45, 0xb99d5d62,
    0x52d73592, 0x3ef636e5, 0xc57a1ac9, 0xa95b9b72,
    0x5ab42554, 0x369555ed, 0x1553ba9a, 0x7972b2a2,
    0xe6b85d4d, 0x8a995951, 0x4b550696, 0x2774b4fc,
    0xc9bb034b, 0xa59a5a7e, 0x88cc81a5, 0xe4ed2d3f,
    0x7c6f68e2, 0x104e8ecb, 0xd2263471, 0xbe07c765,
    0x511a3208, 0x3d3bfbe6, 0x1084b134, 0x7ca565a7,
    0x304bf0aa, 0x5c6aaa87, 0xf4347855, 0x9815d543,
    0x4213141a, 0x2e32f2f5, 0xcd180a0d, 0xa139f97a,
    0x5e852d36, 0x32a464e9, 0xc353169b, 0xaf72b274,
    0x8db88b4d, 0xe199593a, 0x7ed56d96, 0x12f434c9,
    0xd37b36cb, 0xbf5a9a64, 0x85ac9b65, 0xe98d4d32,
    0x7adf6582, 0x16fe3ecd, 0xd17e32c1, 0xbd5f9f66,
    0x50b63150, 0x3c9757e7, 0x1052b098, 0x7c73b3a7
};
const uint32_t CON192[84] = {
    0xc6d61d91, 0xaaf73771, 0x5b6226f8, 0x374383ec,
    0x15b8bb4c, 0x799959a2, 0x32d5f596, 0x5ef43485,
    0xf57b7acb, 0x995a9a42, 0x96acbd65, 0xfa8d4d21,
    0x735f7682, 0x1f7ebec4, 0xd5be3b41, 0xb99f5f62,
    0x52d63590, 0x3ef737e5, 0x1162b2f8, 0x7d4383a6,
    0x30b8f14c, 0x5c995987, 0x2055d096, 0x4c74b497,
    0xfc3b684b, 0x901ada4b, 0x920cb425, 0xfe2ded25,
    0x710f7222, 0x1d2eeec6, 0xd4963911, 0xb8b77763,
    0x524234b8, 0x3e63a3e5, 0x1128b26c, 0x7d09c9a6,
    0x309df106, 0x5cbc7c87, 0xf45f7883, 0x987ebe43,
    0x963ebc41, 0xfa1fdf21, 0x73167610, 0x1f37f7c4,
    0x01829338, 0x6da363b6, 0x38c8e1ac, 0x54e9298f,
    0x246dd8e6, 0x484c8c93, 0xfe276c73, 0x9206c649,
    0x9302b639, 0xff23e324, 0x7188732c, 0x1da969c6,
    0x00cd91a6, 0x6cec2cb7, 0xec7748d3, 0x8056965b,
    0x9a2aa469, 0xf60bcb2d, 0x751c7a04, 0x193dfdc2,
    0x02879532, 0x6ea666b5, 0xed524a99, 0x8173b35a,
    0x4ea00d7c, 0x228141f9, 0x1f59ae8e, 0x7378b8a8,
    0xe3bd5747, 0x8f9c5c54, 0x9dcfaba3, 0xf1ee2e2a,
    0xa2f6d5d1, 0xced71715, 0x697242d8, 0x055393de,
    0x0cb0895c, 0x609151bb, 0x3e51ec9e, 0x5270b089
};
const uint32_t CON256[92] = {
    0x0221947e, 0x6e00c0b5, 0xed014a3f, 0x8120e05a,
    0x9a91a51f, 0xf6b0702d, 0xa159d28f, 0xcd78b816,
    0xbcbde947, 0xd09c5c0b, 0xb24ff4a3, 0xde6eae05,
    0xb536fa51, 0xd917d702, 0x62925518, 0x0eb373d5,
    0x094082bc, 0x6561a1be, 0x3ca9e96e, 0x5088488b,
    0xf24574b7, 0x9e64a445, 0x9533ba5b, 0xf912d222,
    0xa688dd2d, 0xcaa96911, 0x6b4d46a6, 0x076cacdc,
    0xd9b72353, 0xb596566e, 0x80ca91a9, 0xeceb2b37,
    0x786c60e4, 0x144d8dcf, 0x043f9842, 0x681edeb3,
    0xee0e4c21, 0x822fef59, 0x4f0e0e20, 0x232feff8,
    0x1f8eaf20, 0x73af6fa8, 0x37ceffa0, 0x5bef2f80,
    0x23eed7e0, 0x4fcf0f94, 0x29fec3c0, 0x45df1f9e,
    0x2cf6c9d0, 0x40d7179b, 0x2e72ccd8, 0x42539399,
    0x2f30ce5c, 0x4311d198, 0x2f91cf1e, 0x43b07098,
    0xfbd9678f, 0x97f8384c, 0x91fdb3c7, 0xfddc1c26,
    0xa4efd9e3, 0xc8ce0e13, 0xbe66ecf1, 0xd2478709,
    0x673a5e48, 0x0b1bdbd0, 0x0b948714, 0x67b575bc,
    0x3dc3ebba, 0x51e2228a, 0xf2f075dd, 0x9ed11145,
    0x417112de, 0x2d5090f6, 0xcca9096f, 0xa088487b,
    0x8a4584b7, 0xe664a43d, 0xa933c25b, 0xc512d21e,
    0xb888e12d, 0xd4a9690f, 0x644d58a6, 0x086cacd3,
    0xde372c53, 0xb216d669, 0x830a9629, 0xef2beb34,
    0x798c6324, 0x15ad6dce, 0x04cf99a2, 0x68ee2eb3
};

void Sigma(uint32_t L[4]){
    uint64_t hi = ((uint64_t)L[0]<<32) | L[1];
    uint64_t lo = ((uint64_t)L[2]<<32) | L[3];

    __uint128_t X = ((__uint128_t)hi<<64) | lo;
    __uint128_t x_0_6 = (X>>(128-7)) & 0x7F;
    __uint128_t x_7_63 = (X>>(128-64)) & 0x1FFFFFFFFFFFFFFULL;
    __uint128_t x_64_120 = (X>>7) & 0x1FFFFFFFFFFFFFFULL;
    __uint128_t x_121_127 = X&0x7F;
    __uint128_t Y = (x_7_63<<71) | (x_121_127<<64) | (x_0_6<<57) | x_64_120;

    uint64_t Y_hi = Y>>64;
    uint64_t Y_lo = Y & 0xFFFFFFFFFFFFFFFFULL;

    L[0] = (Y_hi>>32) & 0xFFFFFFFF;
    L[1] = Y_hi & 0xFFFFFFFF;
    L[2] = (Y_lo>>32) & 0xFFFFFFFF;
    L[3] = Y_lo & 0xFFFFFFFF;
}

void GenerateRoundKeys128(const Byte key[16], Word kw[4], Word rk[], int rounds){
    Word K[4], L[4];

    for(int i=0; i<4; ++i){
        K[i] = (key[4*i]<<24) |(key[4*i+1]<<16) | (key[4*i+2]<<8) | key[4*i+3];
    }

    GFN4_r(CON128, K, L, 12);

    kw[0] = K[0];
    kw[1] = K[1];
    kw[2] = K[2];
    kw[3] = K[3];

    for(int i=0; i<9; ++i){
        uint32_t T[4];

        for(int j=0; j<4; ++j){
            T[j] = L[j]^CON128[24+4*i+j];
        }
        Sigma(L);

        if((i%2)==1){
            for(int j=0; j<4; ++j){
                T[j]^= K[j];
            }
        }

        for(int j=0; j<4; ++j){
            rk[4*i+j] = T[j];
        }
    }
}

void GenerateRoundKeys192(const uint8_t key[24], uint32_t kw[4], uint32_t rk[44]){
    uint32_t K[6];
    for(int i=0; i<6; ++i){
        K[i] = (static_cast<uint32_t>(key[4*i+0])<<24) | (static_cast<uint32_t>(key[4*i+1])<<16) |
               (static_cast<uint32_t>(key[4*i+2])<<8)  | (static_cast<uint32_t>(key[4*i+3]));
    }

    uint32_t KL[4] = { K[0], K[1], K[2], K[3] };
    uint32_t KR[4] = { K[4], K[5], K[0]^0xFFFFFFFF, K[1]^0xFFFFFFFF };

    uint32_t input[8];
    for(int i=0; i<4; ++i){
        input[i] = KL[i];
        }
    for(int i=0; i<4; ++i){
        input[i + 4] = KR[i];
        }

    uint32_t L[8];
    GFN8_r(CON192, input, L, 10);
    uint32_t LL[4], LR[4];
    memcpy(LL, L, 4*sizeof(uint32_t));
    memcpy(LR, L+4, 4*sizeof(uint32_t));

    uint32_t WK[4];
    for(int i=0; i<4; ++i){
        WK[i] = KL[i]^KR[i];
        kw[i] = WK[i];
    }

    for(int i=0; i<=10; ++i){
        uint32_t T[4];
        const uint32_t* con_ptr = &CON192[40+4*i];

        if((i%4)==0 || (i%4)==1){
            for(int j=0; j<4; ++j){
                T[j] = LL[j]^con_ptr[j];
                }

            if((i%2)==1){
                for (int j=0; j<4; ++j){
                    T[j]^=KR[j];
                }
            }

            Sigma(LL);
        }
        else{
            for(int j=0; j<4; ++j)
                T[j] = LR[j]^con_ptr[j];

            if((i%2)==1){
                for(int j=0; j<4; ++j){
                    T[j]^=KL[j];
                }
            }

            Sigma(LR);
        }

        for(int j=0; j<4; ++j){
            rk[4*i+j] = T[j];
        }
    }

}

void GenerateRoundKeys256(const uint8_t key[32], uint32_t kw[4], uint32_t rk[52]){
    uint32_t K[8];
    for(int i=0; i<8; ++i){
        K[i] = (static_cast<uint32_t>(key[4*i+0])<<24) | (static_cast<uint32_t>(key[4*i+1])<<16) |
               (static_cast<uint32_t>(key[4*i+2])<<8)  | (static_cast<uint32_t>(key[4*i+3]));
    }

    uint32_t KL[4] = { K[0], K[1], K[2], K[3] };
    uint32_t KR[4] = { K[4], K[5], K[6], K[7] };

    uint32_t input[8];
    for(int i=0; i<4; ++i){
        input[i] = KL[i];
    }
    for(int i=0; i<4; ++i){
        input[i+4] = KR[i];
        }

    uint32_t L[8];
    GFN8_r(CON256, input, L, 10);

    uint32_t LL[4], LR[4];
    memcpy(LL, L, 4*sizeof(uint32_t));
    memcpy(LR, L+4, 4*sizeof(uint32_t));

    for(int i=0; i<4; ++i) {
        kw[i] = KL[i]^KR[i];
    }

    for(int i=0; i<=12; ++i){
        uint32_t T[4];
        const uint32_t* con_ptr = &CON256[40+4*i];
        if(((i%4)==0) || ((i%4)==1)){
            for(int j=0; j<4; ++j){
                T[j] = LL[j]^con_ptr[j];
                }

            if((i%2)==1){
                for (int j=0; j<4; ++j){
                    T[j]^= KR[j];
                    }
            }

            Sigma(LL);
        }
        else{
            for(int j=0; j<4; ++j){
                T[j] = LR[j]^con_ptr[j];
            }

            if((i%2)==1){
                for (int j=0; j<4; ++j){
                    T[j]^= KL[j];
                }
            }

            Sigma(LR);
        }

        for (int j=0; j<4; ++j){
            rk[4*i+j] = T[j];
            }
    }
}
}




