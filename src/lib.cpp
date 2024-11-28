#include <cstddef>
#include <numeric>
#include <random>
#include <array>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <string>
#include "lib.h"

namespace chronocipher {

std::array<unsigned char,256> generateKey() {
  auto mt = std::mt19937(std::random_device{}());
  std::array<uint8_t,256> myKey{};
  std::iota(myKey.begin(), myKey.end(), 0);
  assert(myKey[0] == 0 && "Beginning of the key is wrong");
  assert(myKey[255] == 255 && "End of the key is wrong");

  std::shuffle(myKey.begin(), myKey.end(), mt);

  return myKey;
}

void mutateCipher(std::array<uint8_t,256>& chronokey) {
  size_t rotateCount{chronokey[chronokey[0]]};
  size_t permuteCount{chronokey[chronokey[1]]};

  if (rotateCount == 0) rotateCount = 7;
  if (permuteCount == 0) permuteCount = 13;

  for (size_t i{0}; i < rotateCount; ++i) {
    for (size_t j{0}; j < permuteCount; ++j) {
      std::next_permutation(chronokey.begin(), chronokey.end());
    }
    std::rotate(chronokey.begin(), chronokey.begin()+rotateCount-i, chronokey.end());
  }

  return;
}

void blockify(const std::string& inputText, std::vector<char>& vBuffer) {
  size_t newSize{256};

  while (newSize < inputText.size()) {
    newSize += 256;
  }

  vBuffer.resize(newSize);
  std::fill(vBuffer.begin(), vBuffer.end(), 0x00);

  for (size_t i{0}; i < inputText.size(); ++i) {
    vBuffer[i] = inputText[i];
  }

  return;
}

void encrypt_round1(std::array<char, 256>& block) {
  const std::array<char, 256> originalBlock{block};

  for (size_t i{0}; i<128; ++i) {
    size_t i2 = i+128;

    std::bitset<8> a0{static_cast<uint8_t>(originalBlock[i])};
    std::bitset<8> a1{static_cast<uint8_t>(originalBlock[i2])};

    std::bitset<8> b0{static_cast<uint8_t>(originalBlock[i])};
    std::bitset<8> b1{static_cast<uint8_t>(originalBlock[i2])};

    a0 << 4;
    a1 >> 4;
    char c0{static_cast<char>((a0|a1).to_ulong())};
    block[i] = c0;

    b0 >> 4;
    b1 << 4;
    char c1{static_cast<char>((b0|b1).to_ulong())};
    block[i2] = c1;
  }

  return;
}

void decrypt_round1(std::array<char, 256>& block) {
  const std::array<char, 256> originalBlock{block};

  for (size_t i{0}; i<128; ++i) {
    size_t i2 = i+128;

    std::bitset<8> a0{static_cast<uint8_t>(originalBlock[i])};
    std::bitset<8> a1{static_cast<uint8_t>(originalBlock[i2])};

    std::bitset<8> b0{static_cast<uint8_t>(originalBlock[i])};
    std::bitset<8> b1{static_cast<uint8_t>(originalBlock[i2])};

    a0 >> 4;
    a1 << 4;
    char c0{static_cast<char>((a0|a1).to_ulong())};
    block[i] = c0;

    b0 << 4;
    b1 >> 4;
    char c1{static_cast<char>((b0|b1).to_ulong())};
    block[i2] = c1;
  }

  return;
}

void bicryptRound2(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey) {
  for (size_t i{0}; i < 256; ++i) {
    block[i]^=chronokey[i];
  }

  return;
}

void encryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey) {
  std::array<char, 256> originalBlock{block};

  for(size_t i{0}; i < 256; ++i) {
    block[i] = originalBlock[chronokey[i]];
  }

  return;
}

void decryptRound3(std::array<char, 256>& block, const std::array<uint8_t, 256>& chronokey) {
  std::array<char, 256> cipheredBlock{block};

  for (size_t i{0}; i < 256; ++i) {
    block[chronokey[i]] = cipheredBlock[i];
  }

  return;
}

void fullEncrypt(std::array<char, 256>& block, std::array<uint8_t, 256> chronokey) {
  encrypt_round1(block);
  mutateCipher(chronokey);
  bicryptRound2(block, chronokey);
  mutateCipher(chronokey);
  encryptRound3(block, chronokey);
  mutateCipher(chronokey);

  return;
}

void fullDecrypt(std::array<char, 256>&block, std::array<uint8_t, 256> chronokey) {
  mutateCipher(chronokey);
  std::array<uint8_t, 256> keyR2{chronokey};
  mutateCipher(chronokey);
  std::array<uint8_t, 256> keyR3{chronokey};
  mutateCipher(chronokey); // Key is now ready for another block
  
  decryptRound3(block, keyR3);
  bicryptRound2(block, keyR2);
  decrypt_round1(block);

  return;
}
}
