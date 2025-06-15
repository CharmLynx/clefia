#pragma once
#include "clefia.hpp"

namespace clefia {
void GenerateRoundKeys128(const Byte key[16], Word kw[4], Word rk[], int rounds);
void GenerateRoundKeys192(const Byte key[24], Word kw[4], Word rk[44]);
void GenerateRoundKeys256(const Byte key[24], Word kw[4], Word rk[44]);

}
