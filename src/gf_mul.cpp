#include <cstdint>
#include <iostream>
#include <iomanip>

using namespace std;

uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint16_t res = 0;
    uint16_t aa = a;
    for (int i = 0; i < 8; ++i) {
        if (b & 1)
            res ^= aa;

        aa <<= 1;
        if (aa & 0x100)
            aa ^= 0x11D;

        b >>= 1;
    }
    return res & 0xFF;
}

