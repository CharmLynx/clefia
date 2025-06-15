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

    // Перетворити на байти
    Byte plaintext[16]={0};
    Byte ciphertext[16]={0};
    Byte decrypted[16]={0};


    


    
    //обробка ключів
    cout<<"Введіть довжину ключа(128, 192, 256): ";
    int a;
    cin >> a;
    cout<<endl;
    
    Word kw[4];
    vector<Word> rk;
    clefia::CipherParameters params;

   
    switch(a){
    case 128:{
        string hex_key;
        ifstream inff("Key128.txt");
        if (!inff) {
            cerr << "Помилка відкриття файлу\n";
            return 1;
        }
        inff >> hex_key;
        Byte key[16]={0};
        int count_k=0;
        hex_to_bytes(hex_key, key, count_k);
        params = get_params(KeyLength::Bits128);
        rk.resize(params.numRoundKeys);
        GenerateRoundKeys128(key, kw, rk.data(), params.rounds);
        break;}
    case 192:{
        string hex_key;
        ifstream inff("Key192.txt");
        if (!inff) {
            cerr << "Помилка відкриття файлу\n";
            return 1;
        }
        inff >> hex_key;
        Byte key[24]={0};
        int count_k=0;
        hex_to_bytes(hex_key, key, count_k);
        params = get_params(KeyLength::Bits192);
        rk.resize(params.numRoundKeys);
        GenerateRoundKeys192(key, kw, rk.data());
        break;}
    case 256:{
        string hex_key;
        ifstream inff("Key256.txt");
        if (!inff) {
            cerr << "Помилка відкриття файлу\n";
            return 1;
        }
        inff >> hex_key;
        Byte key[32]={0};
        int count_k=0;
        hex_to_bytes(hex_key, key, count_k);
        params = get_params(KeyLength::Bits256);
        rk.resize(params.numRoundKeys);
        GenerateRoundKeys256(key, kw, rk.data());
        break;}
    default:
        cout<<"pomylka"<<endl;
    }


    //обробка вхідного тексту 
    string hex_plain;
    ifstream inf("Plaintext.txt");
    if (!inf) {
        cerr << "Помилка відкриття файлу\n";
        return 1;
    }
    inf >> hex_plain;


    if(hex_plain.length()%32!=0){
        cout<<"+00"<<endl;
        int len=32-hex_plain.length()%32;
        while(len!=0){
            hex_plain="0"+hex_plain;
            len--;
        }
        cout << hex_plain << endl;
    }


    //encrypt text
    /*string part_p={0};
    int count_p=0;
    int count = hex_plain.length()/32-1;
    

    ofstream("Ciphertext.txt"); 
    for(int i=0; i<=count; ++i){
        part_p=hex_plain.substr(32*i, 32);
        count_p=0;
        hex_to_bytes(part_p, plaintext, count_p);
        
        EncryptBlock(plaintext, ciphertext, kw, rk.data(), params.rounds);
        for(int j=0; j<16; ++j) ciphertext[j]=0;
        part_p = {0};


    }*/
    EncryptText(hex_plain, kw, rk, params.rounds);


    //зчитування шифротексту
    string hex_cipher;
    ifstream inff("Ciphertext.txt");
    if (!inff) {
        cerr << " Помилка відкриття файлу\n";
        return 1;
    }
    inff >> hex_cipher;

    if(hex_cipher.length()%32!=0){
        cout<<"ERROR"<<endl;
        return 1;        
    }
    


    //derypt text
    /*string part_c={0};
    int count_c=0;
    int count = hex_cipher.length()/32-1;
    
    ofstream("Decrypt.txt"); 
    for(int i=0; i<=count; ++i){
        part_c=hex_cipher.substr(32*i, 32);
        count_p=0;
        hex_to_bytes(part_c, ciphertext, count_c);
        
        DecryptBlock(ciphertext, decrypted, kw, rk.data(), params.rounds);
        for(int j=0; j<16; ++j) decrypted[j]=0;
        part_c = {0};


    }*/
    DecryptText(hex_cipher, kw, rk, params.rounds);
   
    

    // Вивід
    /*cout << "Plaintext:  ";
    for(int i=0;i<16;i++) cout << hex << setw(2) << setfill('0') << (int)plaintext[i];
    cout << "\nCiphertext: ";
    for(int i=0;i<16;i++) cout << hex << setw(2) << setfill('0') << (int)ciphertext[i];
    cout << "\nDecrypted:  ";
    for(int i=0;i<16;i++) cout << hex << setw(2) << setfill('0') << (int)decrypted[i];
    cout << endl;*/


   

    

    return 0;
}
