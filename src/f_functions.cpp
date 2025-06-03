#include "f_functions.hpp"
#include "sboxes.hpp"
#include "gf_mul.hpp"

uint32_t F0(uint32_t X, uint32_t RK){
    uint32_t T = X^RK;

    uint8_t T0 = (T>>24)&0xFF;
    uint8_t T1 = (T>>16)&0xFF;
    uint8_t T2 = (T>>8)&0xFF;
    uint8_t T3 = T&0xFF;

    uint8_t V[4] = {
        S0[T0],  
        S1[T1],  
        S0[T2],  
        S1[T3]   
    };

    uint8_t Y[4] = {0};

    for (int i=0; i<4; ++i) {
        Y[i] = gf_mul(M0[i][0], V[0]) ^
               gf_mul(M0[i][1], V[1]) ^
               gf_mul(M0[i][2], V[2]) ^
               gf_mul(M0[i][3], V[3]);
    }

    return (Y[0]<<24)|(Y[1]<<16)|(Y[2]<<8)|Y[3];
}

uint32_t F1(uint32_t X, uint32_t RK){
    uint32_t T = X^RK;

    uint8_t T0 = (T>>24)&0xFF;
    uint8_t T1 = (T>>16)&0xFF;
    uint8_t T2 = (T>>8)&0xFF;
    uint8_t T3 = T&0xFF;

    uint8_t V[4] = {
        S1[T0],  
        S0[T1],  
        S1[T2],  
        S0[T3]   
    };

    uint8_t Y[4] = {0};

    for (int i=0; i<4; ++i) {
        Y[i] = gf_mul(M1[i][0], V[0])^
               gf_mul(M1[i][1], V[1])^
               gf_mul(M1[i][2], V[2])^
               gf_mul(M1[i][3], V[3]);
    }

    return (Y[0]<<24)|(Y[1]<<16)|(Y[2]<<8)|Y[3];
}
