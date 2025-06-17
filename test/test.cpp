#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <fstream>

#include "clefia.hpp"
#include "key_schedule.hpp"
#include "utils.hpp"
#include "f_functions.hpp"
#include "sboxes.hpp"
#include "gf_mul.hpp"

using namespace std;
using namespace clefia;

int main(){

    Word kw[4];
    vector<Word> rk;
    clefia::CipherParameters params;

    string hex_plain = "000102030405060708090a0b0c0d0e0f";
    
    if(hex_plain.length()%32!=0){
        int len=32-hex_plain.length()%32;
        while(len!=0){
            hex_plain="0"+hex_plain;
            len--;
        }
        cout << hex_plain << endl;
    }

    //ключ 128
    string hex_key_128 = "ffeeddccbbaa99887766554433221100";
    
    Byte key_128[16]={0};
    int count_k=0;
    hex_to_bytes(hex_key_128, key_128, count_k);
    params = get_params(KeyLength::Bits128);
    rk.resize(params.numRoundKeys);
    GenerateRoundKeys128(key_128, kw, rk.data(), params.rounds);

    

    EncryptText(hex_plain, kw, rk, params.rounds);

    string hex_cipher;
    ifstream inff("Ciphertext.txt");
    if (!inff) {
        cerr << " Помилка відкриття файлу\n";
        return 1;
    }
    inff >> hex_cipher;

    string hex_cipher_128 = "de2bf2fd9b74aacdf1298555459494fd";

    cout<<"128 encrypt | ";
    if(hex_cipher==hex_cipher_128){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }

    DecryptText(hex_cipher, kw, rk, params.rounds);

    string hex_plain_128;
    ifstream infff("Decrypt.txt");
    if (!infff) {
        cerr << " Помилка відкриття файлу\n";
        return 1;
    }
    infff >> hex_plain_128;
    
    cout<<"128 decrypt | ";
    if(hex_plain_128==hex_plain){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }
    cout<<"----------------"<<endl;

    //ключ 192
    string hex_key_192 = "ffeeddccbbaa99887766554433221100f0e0d0c0b0a09080";
    
    Byte key_192[24]={0};
    count_k=0;
    hex_to_bytes(hex_key_192, key_192, count_k);
    params = get_params(KeyLength::Bits192);
    rk.resize(params.numRoundKeys);
    GenerateRoundKeys192(key_192, kw, rk.data());

    EncryptText(hex_plain, kw, rk, params.rounds);
    
    hex_cipher.clear();
    ifstream inff1("Ciphertext.txt");
    if (!inff1) {
        cerr << " Помилка відкриття файлу\n";
        return 1;
    }
    inff1 >> hex_cipher;

    string hex_cipher_192 = "e2482f649f028dc480dda184fde181ad";
    cout<<"192 encrypt | ";
    if(hex_cipher==hex_cipher_192){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }

    DecryptText(hex_cipher, kw, rk, params.rounds);

    string hex_plain_192;
    ifstream infff1("Decrypt.txt");
    if (!infff1) {
        cerr << " Помилка відкриття файлу\n";
    }
    infff1 >> hex_plain_192;

    cout<<"192 decrypt | ";
    if(hex_plain_192==hex_plain){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }
    cout<<"----------------"<<endl;

    //ключ 256
    string hex_key_256 = "ffeeddccbbaa99887766554433221100f0e0d0c0b0a090807060504030201000";

    Byte key_256[32]={0};
    count_k=0;
    hex_to_bytes(hex_key_256, key_256, count_k);
    params = get_params(KeyLength::Bits256);
    rk.resize(params.numRoundKeys);
    GenerateRoundKeys256(key_256, kw, rk.data());

    EncryptText(hex_plain, kw, rk, params.rounds);

    hex_cipher.clear();
    ifstream inff2("Ciphertext.txt");
    if (!inff2) {
        cerr << " Помилка відкриття файлу\n";
        return 1;
    }
    inff2 >> hex_cipher;

    string hex_cipher_256 = "a1397814289de80c10da46d1fa48b38a";
    cout<<"256 encrypt | ";
    if(hex_cipher==hex_cipher_256){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }

    DecryptText(hex_cipher, kw, rk, params.rounds);

    string hex_plain_256;
    ifstream infff2("Decrypt.txt");
    if (!infff2) {
        cerr << " Помилка відкриття файлу\n";
    }
    infff2 >> hex_plain_256;

    cout<<"256 decrypt | ";
    if(hex_plain_256==hex_plain){
        cout<<"ok"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }


    
    return 0;
}
