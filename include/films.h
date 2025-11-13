#ifndef FILM_H
#define FILM_H

#include <string>
#include <vector>

class Film{
  public:
    int id;
    std::string title;
    std::string release_date;
    std::string overview;
    std::vector<int> genres;
    double popularity;
    double rating;

    void print() const;
};

#endif
