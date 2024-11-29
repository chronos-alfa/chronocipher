#include <iostream>
#include <string_view>
#include "cliTools.h"

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

  return;
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
      if(writeSymmetricKey(filePath)) {
        std::cout << "Successfully written down the key to " << filePath << ".\n";
        return ReturnCodes::success;
      } else {
        std::cerr << "Couldn't write down the key to the file";
        return ReturnCodes::writeError;
      } 
    }

    if (params.passphrase.length() > 0) {
      if(writeSymmetricKey(filePath, params.passphrase)) {
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
