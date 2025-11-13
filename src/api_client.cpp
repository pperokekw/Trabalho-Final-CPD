#include "api_client.h"
#include "films.h"
#include "genres.h"
#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string API_KEY = "ce4997dc14a0b4f70ddc709656625b41";
const std::string API_URL = "https://api.themoviedb.org/3";


ApiClient::ApiClient(std::string api_key){
  this->m_api_key = api_key;
}

std::vector<Film> ApiClient::getMovies(int paginas){
  std::vector<Film> films;

  std::cout << "[Obtendo Filmes] Obtendo " << paginas << " paginas de " << paginas << " paginas..." << std::endl;

  for(int pagina = 1; pagina<=paginas; ++pagina){

    cpr::Parameters params = {
      {"api_key", m_api_key},
      {"language","pt-BR"},
      {"sort_by","popularity.desc"},
      {"page",std::to_string(pagina)}
    };

    cpr::Response res = cpr::Get(cpr::Url{m_base_url + "/discover/movie"}, params);


    if (res.status_code != 200){

      std::cerr << "Erro, falha na pagina: " << pagina << ". Status: " << res.status_code << std::endl;
      std::cerr << "Resposta da api: " << res.text << std::endl;
      std::cerr << "Mensagem CPR: " << res.error.message << std::endl;

      continue;
    }


    try{

      json data = json::parse(res.text);

      for (const auto& item : data["results"]){

        Film f;


        f.id = item.value("id",-1);
        f.title = item.value("title", "Sem Titulo");
        f.release_date = item.value("release_date","N/A");
        f.overview = item.value("overview","Sem sinopse disponível.");
        f.popularity = item.value("popularity",0.0);
        f.rating = item.value("vote_average",0.0);
        for(const auto& genre : item["genre_ids"]){
          f.genres.push_back(genre);
        }


        films.push_back(f);
      }
      std::cout << "Pagina " << pagina << "/" << paginas << " processada." << std::endl;

    } catch(json::parse_error& e){
      std::cerr << "Erro ao fazer parse do Json na pagina " << pagina << std::endl;
      continue;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  std::cout << "Coleta Finalizada. Total de " << films.size() << " filmes encontrados." << std::endl;

  return films;
}


std::vector<Genre> ApiClient::getGenres(){
  std::vector<Genre> genres;
  
  cpr::Parameters params = {
    {"api_key", m_api_key},
    {"language","pt"}
  }

  cpr::Response res = cpr::Get(cpr::Url{m_base_url + "/genre/movie/list"},params);

  try{

    json data = json::parse(res.text);
    for(const auto& genre : data["genres"]){
      Genre g;
      g.id = genre.value("id");
      g.name = genre.value("name");
      genres.push_back(g);
    }
  }catch(json::parse_error& e){
    std::cerr << "Erro ao baixar gêneros." << std::endl;
    continue;
  }

  return genres;

}
