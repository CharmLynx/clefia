#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

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

void EncryptBlock(const Byte plaintext[16], Byte ciphertext[16], const Word kw[4], const Word rk[], int rounds);
void DecryptBlock(const Byte ciphertext[16], Byte plaintext[16], const Word kw[4], const Word rk[], int rounds);
void EncryptText(const string& hex_plain, const Word kw[4], const vector<Word>& rk, int rounds);
void DecryptText(const string& hex_cipher, const Word kw[4], const vector<Word>& rk, int rounds);

}
