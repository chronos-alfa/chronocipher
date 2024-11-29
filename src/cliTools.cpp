#include "lib.h"
#include "cliTools.h"
#include <cstdint>
#include <cstring>
#include <string_view>
#include <string.h>
#include <iostream>
#include <fstream>

std::string_view getOptionValue(int argc, int& index, char** argv) {
  char* paramName{argv[index]};

  ++index;

  if (index >= argc) {
    ParamValueError error{};
    error.message = "Missing value of the option argument";
    error.parameterName = paramName;

    throw error;
  }

  return std::string_view{argv[index]};
}

CmdParameters parseArguments(int argc, char** argv) {
  CmdParameters params{};

  for (int i{1}; i < argc; ++i) {
    if (strcmp(argv[i], "-e") == 0) {
       params.encryptKeyFile = getOptionValue(argc, i, argv);
    } else if (strcmp(argv[i], "-d") == 0) {
      params.decryptKeyFile = getOptionValue(argc, i, argv);
    } else if (strcmp(argv[i], "-gr") == 0) {
      params.gr = true;
    } else if (strcmp(argv[i], "-gp") == 0) {
      params.passphrase = getOptionValue(argc, i, argv);
    } else {
      params.filepath = std::string_view(argv[i]);
    }
  }

  return params;
}

bool validateParams(const CmdParameters& params) {
  if (params.filepath == "") {
    std::cerr << "ERROR: Filepath parameter missing.\n";
    return false;
  }

  int validParamsCounter = 0;

  if (params.gr) ++validParamsCounter;
  if (params.passphrase.length() > 0) ++validParamsCounter;
  if (params.encryptKeyFile.length() > 0) ++validParamsCounter;
  if (params.decryptKeyFile.length() > 0) ++validParamsCounter;

  if (validParamsCounter == 1) return true;
  if (validParamsCounter > 1) {
    throw TooManyOptionsError{};
  }

  return false;
}

bool writeSymmetricKey(std::string_view filePath) {
  std::array<uint8_t, 256> key{chronocipher::generateKey()};

  std::ofstream ofb(filePath.data(), std::ios::binary);
  if (!ofb.is_open()) return false;

  ofb.write(reinterpret_cast<const char *>(&key), sizeof(key));

  if (ofb.fail()) return false;
  if (ofb.bad()) return false;

  ofb.close();

  return true;
}

bool writeSymmetricKey(std::string_view filePath, std::string_view passphrase) {
  std::array<uint8_t, 256> key{chronocipher::generateKey(passphrase.data())};

  std::ofstream ofb(filePath.data(), std::ios::binary);
  if (!ofb.is_open()) return false;

  ofb.write(reinterpret_cast<const char *>(&key), sizeof(key));

  if (ofb.fail()) return false;
  if (ofb.bad()) return false;

  ofb.close();

  return true;
}

bool readKey(std::string_view filePath, std::array<uint8_t, 256>& outputKey) {
  std::ifstream ifb(filePath.data(), std::ios::binary);
  if(ifb.bad() || ifb.fail()) return false;
  char buffer[256]{};
  ifb.read(buffer, 256);
  std::memcpy(outputKey.data(), buffer, 256);
  return true;
}
