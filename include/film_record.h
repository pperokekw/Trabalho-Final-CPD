#ifndef FILM_RECORD_H
#define FILME_RECORD_H

struct FilmRecord{

  bool active;
  int id;
  double popularity;
  double rating;

  char title[101];
  char release_date[11];

  long offset_overview;
};

#endif
