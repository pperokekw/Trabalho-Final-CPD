#include "film_record.h"
#include <fstream>
#include <cstring> // memset, strncpy
#include <filesystem> // C++17
#include <iostream>

namespace fs = std::filesystem;

//Construtor que cria a pasta data (pasta que vai ter os dados em disco)
FilmRecord::FilmRecord() {
    if (!fs::exists("data")) {
        fs::create_directory("data");
        std::cout << "[Sistema] Pasta 'data/' criada automaticamente." << std::endl;
    }
}

long FilmRecord::saveMovie(const Film& film) {
    std::ofstream fileMov(FILE_MOVIES, std::ios::binary | std::ios::app);
    std::ofstream fileOver(FILE_OVERVIEWS, std::ios::binary | std::ios::app);

    if (!fileMov.is_open() || !fileOver.is_open()) return -1;

    long ov_offset = fileOver.tellp();
    int ov_size = film.overview.size();
    fileOver.write(film.overview.c_str(), ov_size);
    fileOver.close();

    MovieData reg;
    std::memset(&reg, 0, sizeof(MovieData)); // Zera memória

    reg.id = film.id;
    reg.popularity = film.popularity;
    reg.vote_average = film.rating;
    reg.offset_overview = ov_offset;
    reg.overview_size = ov_size;

    std::strncpy(reg.title, film.title.c_str(), sizeof(reg.title) - 1);
    std::strncpy(reg.release_date, film.release_date.c_str(), sizeof(reg.release_date) - 1);

    long mov_offset = fileMov.tellp();
    fileMov.write(reinterpret_cast<char*>(&reg), sizeof(MovieData));
    fileMov.close();

    return mov_offset;
}

Film FilmRecord::readMovieAt(long offset) {
    Film f;
    std::ifstream file(FILE_MOVIES, std::ios::binary);
    
    if (file.is_open()) {
        MovieData reg;
        file.seekg(offset);
        file.read(reinterpret_cast<char*>(&reg), sizeof(MovieData));

        f.id = reg.id;
        f.title = reg.title;
        f.release_date = reg.release_date;
        f.rating = reg.vote_average;
        f.popularity = reg.popularity;
        
        f.overview = readOverview(reg.offset_overview, reg.overview_size);
    }
    return f;
}

std::string FilmRecord::readOverview(long offset, int size) {
    std::ifstream file(FILE_OVERVIEWS, std::ios::binary);
    std::string text = "";
    if (file.is_open() && size > 0) {
        text.resize(size);
        file.seekg(offset);
        file.read(&text[0], size);
    }
    return text;
}