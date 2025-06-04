#include "gfn.hpp"
#include "f_functions.hpp"

void GFN4_r(const uint32_t rk[], const uint32_t input[4], uint32_t output[4], int rounds){
    uint32_t t[4];
    for (int i=0; i<4; ++i){
        t[i] = input[i];
    }

    for (int i=0; i<rounds; ++i) {
        uint32_t f0in = t[0];
        uint32_t f1in = t[2];
        uint32_t rk0  = rk[2*i];
        uint32_t rk1  = rk[2*i+1];

        uint32_t f0out = F0(rk0, f0in);
        uint32_t f1out = F1(rk1, f1in);

        t[1]^= f0out;
        t[3]^= f1out;

        uint32_t temp = t[0];
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = temp;
    }

    output[0] = t[3];
    output[1] = t[0];
    output[2] = t[1];
    output[3] = t[2];
}

void GFN4_inv(const uint32_t input[4], uint32_t output[4], const uint32_t rk[], int rounds) {
    uint32_t t[4];
    for (int i=0; i<4; ++i){
        t[i] = input[i];
    }

    for (int i=0; i<rounds; ++i) {
        int idx = 2*(rounds-i-1);
        uint32_t rk0 = rk[idx];
        uint32_t rk1 = rk[idx+1];

        uint32_t f0in = t[0];
        uint32_t f1in = t[2];

        uint32_t f0out = F0(rk0, f0in);
        uint32_t f1out = F1(rk1, f1in);

        t[1]^= f0out;
        t[3]^= f1out;

        uint32_t temp = t[3];
        t[3] = t[2];
        t[2] = t[1];
        t[1] = t[0];
        t[0] = temp;
    }

    output[0] = t[1];
    output[1] = t[2];
    output[2] = t[3];
    output[3] = t[0];
}

void GFN8_r(const uint32_t rk[], const uint32_t input[8], uint32_t output[8], int rounds){
    uint32_t t[8];
    for (int i=0; i<8; ++i){
        t[i] = input[i];
    }

    for (int r = 0; r < rounds; ++r) {
        t[1]^= F0(rk[4*r+0], t[0]);
        t[3]^= F1(rk[4*r+1], t[2]);
        t[5]^= F0(rk[4*r+2], t[4]);
        t[7]^= F1(rk[4*r+3], t[6]);

        uint32_t temp = t[0];
        for (int i=0; i<7; ++i){
            t[i] = t[i+1];
        }
        t[7] = temp;
    }

    output[0] = t[7];
    output[1] = t[0];
    output[2] = t[1];
    output[3] = t[2];
    output[4] = t[3];
    output[5] = t[4];
    output[6] = t[5];
    output[7] = t[6];
}
