#ifndef API_CLIENT_H
#define API_CLIENT_H
#include "genres.h"
#include <string>
#include <vector>
#include "films.h"

class ApiClient{

  private:
    const std::string m_base_url = "https://api.themoviedb.org/3";
    std::string m_api_key;

  public:
    ApiClient(std::string api_key);

    std::vector<Film> getMovies(int pages);
    std::vector<Genre> getGenres();
};
#endif
