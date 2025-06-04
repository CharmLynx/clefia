#include <string>     
#include <sstream>     
#include <iomanip>    
#include <cstdint>   

#include "utils.hpp"
#include "clefia.hpp"


using namespace std;

void hex_to_bytes(const string& hex_string, uint8_t output[], int& count){
    count = 0;
    for(size_t i = 0; i < hex_string.length(); i += 2){
        string byte_str = hex_string.substr(i, 2);
        output[count++] = static_cast<uint8_t>(stoi(byte_str, nullptr, 16));
    }
}


void bytes_to_hex(uint8_t A[], string& a, int count){
    for(int i =0;i<count;i++){
        stringstream ss;
        ss << setfill('0') << setw(8) << hex << A[i];
        a += ss.str();
    }
}