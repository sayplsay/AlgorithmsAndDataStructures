#include "tc_base64.h"
#include <iostream>

int main() {  
  taf::TC_Base64 tc_base64;
  std::string input = "hello world!";
  std::string output = tc_base64.encode(input);
  std::cout << "Input = [" << input << "], Encode = [" << output << "]" << std::endl;
  std::string decode = tc_base64.decode(output);
  std::cout << "Decode = [" << decode << "]" <<  std::endl;

  return 0;
} 


/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
