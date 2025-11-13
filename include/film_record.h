#ifndef FILM_RECORD_H
#define FILME_RECORD_H

class FilmRecord{
  public:

    bool active;
    int id;
    double popularity;
    double rating;

    char title[101];
    char release_date[11];

    long offset_overview;

  private:
    void writeMovies(movieList);
};

#endif
