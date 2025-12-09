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


ApiClient::ApiClient(std::string api_key) {
    this->m_api_key = api_key;
}

std::vector<Film> ApiClient::getMovies(int pages) {
    std::vector<Film> films;
    
    for (int page = 1; page <= pages; ++page) {
        std::cout << "[API] Baixando pagina " << page << "..." << std::endl;

        cpr::Response r = cpr::Get(
            cpr::Url{m_base_url + "/discover/movie"},
            cpr::Parameters{
                {"api_key", m_api_key},
                {"language", "pt-BR"},
                {"sort_by", "popularity.desc"},
                {"page", std::to_string(page)}},
            cpr::Timeout{10000}
        );

        if (r.status_code == 200) {
            try {
                
                json data = json::parse(r.text);

                for (const auto& item : data["results"]) {
                    Film f;
                    
                    f.id = item.value("id", 0);
                    f.title = item.value("title", "Sem Titulo");
                    f.release_date = item.value("release_date", "");
                    f.overview = item.value("overview", "");
                    f.popularity = item.value("popularity", 0.0);
                    f.rating = item.value("vote_average", 0.0);

                    if (item.contains("genre_ids") && item["genre_ids"].is_array()) {
                        for (int g_id : item["genre_ids"]) {
                            f.genres.push_back(g_id);
                        }
                    }

                    films.push_back(f);
                }
            } catch (const std::exception& e) {
                std::cerr << "[Erro JSON] " << e.what() << std::endl;
            }
        } else {
            std::cerr << "[Erro API] Status Code: " << r.status_code << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return films;
}

std::vector<Genre> ApiClient::getGenres() {
    std::vector<Genre> genres;

    cpr::Response r = cpr::Get(
        cpr::Url{m_base_url + "/genre/movie/list"},
        cpr::Parameters{
            {"api_key", m_api_key},
            {"language", "pt-BR"}},
        cpr::Timeout{10000}
    );

    if (r.status_code == 200) {
        try {
            json data = json::parse(r.text);
            for (const auto& item : data["genres"]) {
                Genre g;
                g.id = item.value("id", 0);
                g.name = item.value("name", "Desconhecido");
                genres.push_back(g);
            }
        } catch (const std::exception& e) {
            std::cerr << "[Erro JSON Generos] " << e.what() << std::endl;
        }
    }

    return genres;
}