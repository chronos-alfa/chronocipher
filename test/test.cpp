#include <cstdint>
#include <ios>
#include <iostream>
#include <array>
#include <cassert>
#include <fstream>
#include <numeric>
#include <string>
#include "../src/lib.h"

void testKeyGen() {
  auto myKey = chronocipher::generateKey();

  // Testing the generated key
  assert(myKey.size() == 256 && "Key is not of the correct size");
  assert((myKey[0] != 0 || myKey[255] != 255) && "Something went wrong with shuffling");

  // Save the key unless it already exists
  std::ifstream testKeyf("./test/testkey.key", std::ios::binary);
  if (!testKeyf.good()) {
    std::ofstream testKeyOut("./test/testkey.key", std::ios::binary);
    for(char c: myKey) {
      testKeyOut << c;
    }
    testKeyOut.close();
  }
  testKeyf.close();
  return;
}

void testBlockify() {
  const std::string inputText{"Hello world"};
  std::vector<char> myBuffer{};

  chronocipher::blockify(inputText, myBuffer);

  assert((myBuffer.size() % 256) == 0 && "Wrong size of the key");
  for (size_t i{0}; i < inputText.size(); ++i) {
    assert(inputText[i] == myBuffer[i] && "Wrong character found");
  }
  return;
}
 
void testRound1() {
  std::array<char, 256> myText{'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  std::array<char, 256> cipheredText{myText};

  chronocipher::encrypt_round1(cipheredText);

  for (size_t i{0}; i < 11; ++i) {
    assert(myText[i] != cipheredText[i] && "Round 1 encryption doesn't work");
  }

  chronocipher::decrypt_round1(cipheredText);

  for (size_t i{0}; i < 256; ++i) {
    assert(myText[i] == cipheredText[i] && "Round 1 decryption doesn't work");
  }

  return;
}

void testMutateCipher() {
  std::array<uint8_t, 256> myKey{};
  std::iota(myKey.begin(), myKey.end(), 0);
  assert(myKey[0] == 0 && "Couldn't create the proper key beginning.");
  assert(myKey[255] == 255 && "Couldn't create the proper key ending.");
  chronocipher::mutateCipher(myKey);
  assert(myKey[0] != 0 && "Couldn't mutate the key beginning");
  assert(myKey[255] != 255 && "Couldn't mutate the key ending.");

  return;
}

void testRound2() {
  std::array<uint8_t, 256> myKey = chronocipher::generateKey();

  std::array<char, 256> myText{'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  std::array<char, 256> cipheredText{myText};
  chronocipher::bicryptRound2(cipheredText, myKey);
  assert((myText[0] != cipheredText[1] || myText[1] != cipheredText[1]) && "Round 1 encryption didn't work");
  chronocipher::bicryptRound2(cipheredText, myKey);
  for (size_t i{0}; i < 256; ++i) {
    assert(myText[i] == cipheredText[i] && "Round 2 decryption didn't work");
  }
  return;
}

void testRound3() {
  std::array<uint8_t, 256> myKey = chronocipher::generateKey();
  chronocipher::mutateCipher(myKey);

  std::array<char, 256> myText{'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
  std::array<char, 256> cipherText{myText};

  chronocipher::encryptRound3(cipherText, myKey);

  assert((myText[0] != cipherText[0] || myText[1] != cipherText[1] || myText[2] != cipherText[2]) && "Round 3 encryption doesn't work");

  chronocipher::decryptRound3(cipherText, myKey);

  for(int i{0}; i < 256; ++i) {
    assert(myText[i] == cipherText[i] && "Round 3 decryption doesn't work");
  }

  return;
}

int main() {
  std::cout << "Test runner: \n";

  testMutateCipher();
  testKeyGen();
  testBlockify();
  testRound2();
  testRound3();

  std::cout << "All tests successful\n";
}
