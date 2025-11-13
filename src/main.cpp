#include <iostream>
#include "films.h"
#include "api_client.h"
#include "genres.h"
#include <vector>

int main(){

  const std::string API_KEY = "ce4997dc14a0b4f70ddc709656625b41";
  const std::string API_URL = "https://api.themoviedb.org/3";
  ApiClient client(API_KEY);


  std::vector<Film> filmes = client.getMovies(1);
  std::vector<Genre> genres = client.getGenres();

  if (filmes.empty()){
    std:: cerr<< "deu erro dawg" << std::endl;
    return 1;
 }

  for(const Film& f: filmes) {
    f.print();
  }
  for(const Genre& g: genres){
    g.print();
  }
  


  return 0;
}
