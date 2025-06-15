#include "clefia.hpp"
#include "gfn.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cstring>


#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <fstream>
using namespace std;

namespace clefia {

CipherParameters get_params(KeyLength length){
    switch(length){
        case KeyLength::Bits128:
            return {length, 18, 36};
        case KeyLength::Bits192:
            return {length, 22, 44};
        case KeyLength::Bits256:
            return {length, 26, 52};
        default:
            return {KeyLength::Bits128, 18, 36};
    }
}

void EncryptBlock(const Byte pt[16], Byte ct[16], const Word kw[4], const Word rk[], int rounds){
    Word x[4];
    //ofstream("Ciphertext.txt"); 

    for (int i=0; i<4; ++i){
        x[i] = (pt[4*i]<<24) | (pt[4*i+1]<<16) | (pt[4*i+2]<<8) | pt[4*i+3];
    }

    x[1]^= kw[0];
    x[3]^= kw[1];

    Word y[4];
    GFN4_r(rk, x, y, rounds);

    y[1]^= kw[2];
    y[3]^= kw[3];

    for (int i=0; i<4; ++i){
        ct[4*i+0] = (y[i]>>24) & 0xFF;
        ct[4*i+1] = (y[i]>>16) & 0xFF;
        ct[4*i+2] = (y[i]>>8) & 0xFF;
        ct[4*i+3] = y[i] & 0xFF;
    }
    ofstream outf("Ciphertext.txt", ios::app);
    for(int i=0;i<16;i++){
        outf << hex << setw(2) << setfill('0') << (int)ct[i];
    }
	outf.close(); 
}

void EncryptText(const string& hex_plain, const Word kw[4], const vector<Word>& rk, int rounds){
    Byte plaintext[16], ciphertext[16];
    int count_p = 0;
    int block_count = hex_plain.length()/32-1;

    ofstream("Ciphertext.txt"); 
    for (int i = 0; i <= block_count; ++i){
        string part_p = hex_plain.substr(32*i, 32);
        count_p = 0;
        hex_to_bytes(part_p, plaintext, count_p);

        EncryptBlock(plaintext, ciphertext, kw, rk.data(), rounds);
        memset(ciphertext, 0, 16);
    }
}

void DecryptBlock(const Byte ct[16], Byte pt[16], const Word kw[4], const Word rk[], int rounds){
    Word x[4];

    for (int i=0; i<4; ++i){
        x[i] = (ct[4*i]<<24) | (ct[4*i+1]<<16) | (ct[4*i+2]<<8) | ct[4*i+3];
    }

    x[1]^= kw[2];
    x[3]^= kw[3];

    Word y[4];
    GFN4_inv(x, y, rk, rounds);

    y[1]^= kw[0];
    y[3]^= kw[1];

    for (int i=0; i<4; ++i){
        pt[4*i+0] = (y[i]>>24) & 0xFF;
        pt[4*i+1] = (y[i]>>16) & 0xFF;
        pt[4*i+2] = (y[i]>>8) & 0xFF;
        pt[4*i+3] = y[i] & 0xFF;
    }
    ofstream outf("Decrypt.txt", ios::app);
    for(int i=0;i<16;i++) {
        outf << hex << setw(2) << setfill('0') << (int)pt[i];
    }
	outf.close(); 
}
void DecryptText(const string& hex_cipher, const Word kw[4], const vector<Word>& rk, int rounds){
    Byte ciphertext[16], decrypted[16];
    int count_c = 0;
    int block_count = hex_cipher.length()/32-1;

    ofstream("Decrypt.txt"); 

    for (int i=0; i<=block_count; ++i){
        string part_c = hex_cipher.substr(32*i, 32);
        count_c = 0;
        hex_to_bytes(part_c, ciphertext, count_c);

        DecryptBlock(ciphertext, decrypted, kw, rk.data(), rounds);
        memset(decrypted, 0, 16);
    }
}

}
