#ifndef FILM_H
#define FILM_H

#include <string>

class Film{
  public:
    int id;
    std::string title;
    std::string release_date;
    std::string overview;
    double popularity;
    double rating;

    void print() const;
};

#endif
