#pragma once

#include <string>
#include <cstdint>


void hex_to_bytes(const std::string& hex_string, uint64_t digits[], int& count);

void bytes_to_hex(uint64_t A[], std::string& a, int count);
