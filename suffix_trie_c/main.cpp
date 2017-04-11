#include "suffix_trie.h"
int main() {  
  //std::string str="XMADAMYXACXACD";  
  std::string str="aabac";
  SuffixTrie mytree;  
  if (!mytree.CreateTree(str)) {
    std::cout << "fail" << std::endl;  
  }
  else {  
    mytree.Show_Out();  
  } 
  mytree.DestoryTree();  
  return 0;  
} 


/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
