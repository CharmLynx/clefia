#pragma once

#include <string>
#include <cstdint>

void hex_to_bytes(const std::string& hex_string, uint8_t digits[], int& count);
void bytes_to_hex(uint8_t A[], std::string& a, int count);
