#include "films.h"
#include <iostream>
#include <iomanip>

void Film::print() const {
    std::cout << "--------------------------------------------------" << std::endl;
    
    std::string ano = (release_date.size() >= 4) ? release_date.substr(0, 4) : "????";
    
    std::cout << title << " (" << ano << ")" << std::endl;
    std::cout << "ID: " << id << " | Popularidade: " << std::fixed << std::setprecision(2) << popularity;
    std::cout << " | Nota: " << rating << "/10" << std::endl;
    
    if (overview.size() > 150) {
        std::cout << "Sinopse: " << overview.substr(0, 150) << "..." << std::endl;
    } else {
        std::cout << "Sinopse: " << overview << std::endl;
    }
    
    std::cout << "Generos: [ ";
    for (int g : genres) {
        std::cout << g << " ";
    }
    std::cout << "]" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}