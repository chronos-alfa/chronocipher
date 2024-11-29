#pragma once
#include<string_view>

struct ParamValueError {
  std::string_view message;
  std::string_view parameterName;
};

struct TooManyOptionsError {
  std::string_view tooManyMessage{"Too many options detected."};
};

struct CmdParameters {
  std::string_view filepath{};
  bool gr{false};
  std::string_view passphrase{};
  std::string_view encryptKeyFile{};
  std::string_view decryptKeyFile{};
};

std::string_view getOptionValue(int argc, int& index, char** argv);
CmdParameters parseArguments(int argc, char** argv);
bool validateParams(const CmdParameters& params);
bool writeSymmetricKey(std::string_view filePath);
bool writeSymmetricKey(std::string_view filePath, std::string_view passphrase);

