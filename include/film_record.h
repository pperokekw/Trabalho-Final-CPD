#ifndef FILM_RECORD_H
#define FILM_RECORD_H

#include <vector>
#include <string>
#include "films.h"

class FilmRecord {
public:
    //Construtor para verificar e/ou criar pasta data
    FilmRecord();
    
    //mostra a posição onde o filme foi alocado
    long saveMovie(const Film& film);

    // acesso direto
    Film readMovieAt(long offset);

private:
    //le resumo
    std::string readOverview(long offset, int size);

    const std::string FILE_MOVIES = "data/movies.dat";
    const std::string FILE_OVERVIEWS = "data/overviews.dat";
}; 

#endif