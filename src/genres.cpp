#include "genres.h"
#include <iostream>

void Genre::print() const {
    std::cout << "--- ID: " << id << " ---" << std::endl;
    std::cout << "--- Nome: " << name << " ---" << std::endl;
}