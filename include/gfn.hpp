#pragma once
#include <cstdint>


void GFN4_r(const uint32_t rk[], const uint32_t input[4], uint32_t output[4], int rounds);
void GFN4_inv(const uint32_t input[4], uint32_t output[4], const uint32_t rk[], int rounds);
void GFN8_r(const uint32_t rk[], const uint32_t input[8], uint32_t output[8], int rounds);


