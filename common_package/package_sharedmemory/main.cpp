#include "tc_shm.h"
#include <iostream>

int main() {  
  taf::TC_Shm tc_shmx, tc_shmy;
  tc_shmx.init(4, 1234, true);
  tc_shmy.init(4, 1234, false);
  int *p1 = (int *)tc_shmx.getPointer();
  int *p2 = (int *)tc_shmy.getPointer();
  *p1 = 10;
  std::cout << "p2 = [" << *p2 << "]" << std::endl;
    
  return 0;
} 


/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
