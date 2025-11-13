#include "genres.h"
#include <iostream>

void Genre::print() const{
  std::cout << "--- " name << " ---" << std::endl;
  std::cout << "--- ID: " << id << " ---" << std::endl;
}
