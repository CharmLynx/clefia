#include <string>     
#include <sstream>     
#include <iomanip>    
#include <cstdint>      

void hex_to_bytes(const string& hex_string, uint64_t digits[], int& count){

    int hex_length = hex_string.length();
    int padding = 8-(hex_length%8);
    string text = hex_string;
    if(padding!=0){
        text = string(padding, '0')+ text;
    }
    int text_length = text.length();
    string part(8,'0');
    int n=0;
    while(text_length>0){
        for(int i=0;i<8;i++){
                part.at(i)=text.at(i+n);
        }
        n+=8;
        text_length=text_length-8;
        uint32_t value = stoul(part, nullptr, 16);
        digits[count]=value;
        count++;
    }
}

void bytes_to_hex(uint64_t A[], string& a, int count){
    for(int i =0;i<count;i++){
        stringstream ss;
        ss << setfill('0') << setw(8) << hex << A[i];
        a += ss.str();
    }
}