#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <string>

namespace chronocipher {

std::array<uint8_t,256> generateKey(); 

void blockify(const std::string& inputText, std::vector<char>& vBuffer);

void mutateCipher(std::array<uint8_t,256>& chronokey);

void encrypt_round1(std::array<char, 256>& block);
void decrypt_round1(std::array<char, 256>& block);
void bicryptRound2(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
void encryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
void decryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
}
