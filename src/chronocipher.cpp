#include <array>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string_view>
#include <string.h>
#include "lib.h"

enum ReturnCodes {
  success,
  missingParams,
  invalidParams,
  missingParamsValue,
  tooManyParams,
  writeError
};

void printUsage() {
  std::cout << "USAGE:\n";
  std::cout << "chronocipher <option> <filepath>\n";
  std::cout << "\n";
  std::cout << "ARGUMENTS\n";
  std::cout << "filepath  Path to the file\n";
  std::cout << "\n";
  std::cout << "OPTIONS\n";
  std::cout << "-gr  Generate random key to the <filepath>\n";
  std::cout << "-gp PASSPHRASE  Generate random key to the <filepath> from a passphrase\n";
  std::cout << "-e KEYFILE  Encrypt the file in the <filepath> with key from the KEYFILE\n";
  std::cout << "-d KEYFILE  Decrypt the file in the <filepath> with key from the KEYFILE\n";
  std::cout << "\n";

  return;
}

struct CmdParameters {
  std::string_view filepath{};
  bool gr{false};
  std::string_view passphrase{};
  std::string_view encryptKeyFile{};
  std::string_view decryptKeyFile{};
};

struct ParamValueError {
  std::string_view message;
  std::string_view parameterName;
};

struct TooManyOptionsError {
  std::string_view tooManyMessage{"Too many options detected."};
};

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

bool writeGeneratorKey(std::string_view filePath) {
  std::array<uint8_t, 256> key{chronocipher::generateKey()};

  std::ofstream ofb(filePath.data(), std::ios::binary);
  if (!ofb.is_open()) return false;

  ofb.write(reinterpret_cast<const char *>(&key), sizeof(key));

  if (ofb.fail()) return false;
  if (ofb.bad()) return false;

  ofb.close();

  return true;
}

bool writeGeneratorKey(std::string_view filePath, std::string_view passphrase) {
  std::array<uint8_t, 256> key{chronocipher::generateKey(passphrase.data())};

  std::ofstream ofb(filePath.data(), std::ios::binary);
  if (!ofb.is_open()) return false;

  ofb.write(reinterpret_cast<const char *>(&key), sizeof(key));

  if (ofb.fail()) return false;
  if (ofb.bad()) return false;

  ofb.close();

  return true;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printUsage();
    return ReturnCodes::missingParams;
  }

  try {
    const auto params = parseArguments(argc, argv);
    const bool isValid{validateParams(params)};

    if (!isValid) {
      printUsage();
      return ReturnCodes::invalidParams;
    }

    std::string_view filePath{params.filepath};

    if (params.gr) {
      if(writeGeneratorKey(filePath)) {
        std::cout << "Successfully written down the key to " << filePath << ".\n";
        return ReturnCodes::success;
      } else {
        std::cerr << "Couldn't write down the key to the file";
        return ReturnCodes::writeError;
      } 
    }

    if (params.passphrase.length() > 0) {
      if(writeGeneratorKey(filePath, params.passphrase)) {
        std::cout << "Successfully written down the key to " << filePath << ".\n";
        return ReturnCodes::success;
      } else {
        std::cerr << "Couldn't write down the key to the file";
        return ReturnCodes::writeError;
      }
    }

  } catch(ParamValueError err) {
    std::cerr << "ERROR: " << err.message << '\n';
    std::cerr << "- argument in question: " << err.parameterName << '\n';
    printUsage();

    return ReturnCodes::missingParamsValue;
  } catch(TooManyOptionsError err) {
    std::cerr << "ERROR: " << err.tooManyMessage << '\n';
    printUsage();

    return ReturnCodes::tooManyParams;
  }

  printUsage();
  return ReturnCodes::invalidParams;
}
