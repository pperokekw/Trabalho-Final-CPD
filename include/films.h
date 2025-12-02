#ifndef FILMS_H
#define FILMS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

struct MovieData {
    int id;
    char title[150];
    char release_date[11];   // YYYY-MM-DD
    double popularity;
    double vote_average;
    long offset_overview;  
    int overview_size;       
};

class Film {
public:
    int id;
    std::string title;
    std::string release_date;
    std::string overview;
    double popularity;
    double rating;
    std::vector<int> genres; //ID dos Generos

    Film() : id(0), popularity(0.0), rating(0.0) {}

    void print() const;
};

#endif