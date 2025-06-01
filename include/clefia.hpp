#pragma once
#include <cstdint>

namespace clefia{

using Byte=uint8_t;
using Word=uint32_t;

enum class KeyLength:uint8_t{
    Bits128 = 16,
    Bits192 = 24,
    Bits256 = 32
};

struct CipherParameters{
    KeyLength keyLength;
    int rounds;
    int numRoundKeys;
};

CipherParameters get_params(KeyLength length);

void EncryptBlock(const Byte plaintext[16], Byte ciphertext[16], const Byte key[], KeyLength keyLen);
void DecryptBlock(const Byte ciphertext[16], Byte plaintext[16], const Byte key[], KeyLength keyLen);

}
