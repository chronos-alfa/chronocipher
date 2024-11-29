#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <string>

namespace chronocipher {

std::array<uint8_t,256> generateKey(); 
std::array<unsigned char,256> generateKey(const std::string& passphrase);

void blockify(const std::string& inputText, std::vector<char>& vBuffer);

void mutateCipher(std::array<uint8_t,256>& chronokey);

void encryptRound1(std::array<char, 256>& block);
void decryptRound1(std::array<char, 256>& block);
void bicryptRound2(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
void encryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
void decryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey);
void fullEncrypt(std::array<char, 256>& block, std::array<uint8_t, 256> chronokey);
void fullDecrypt(std::array<char, 256>& block, std::array<uint8_t, 256> chronokey);
}
