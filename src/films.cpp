#include "films.h"
#include <iostream>
#include <iomanip>

void Film::print() const{
  std::cout << "--- " << title << "(" << release_date.substr(0, 4) << ") ---" << std::endl;
  std::cout << "ID: " << id << " | Popularidade: " << popularity << std::endl;
  std::cout << "Nota: " << std::fixed << std::setprecision(1) << rating << "/10.0" << std::endl;
  std::cout << "Sinopse: " << overview.substr(0,100) << "..." << std::endl;
  std::cout << "------------------------------" << std::endl;
}
