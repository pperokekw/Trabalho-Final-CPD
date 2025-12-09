#include <iostream>
#include <ctime>
#include <fstream>
#include <algorithm>
#include "api_client.h"
#include "films_record.h"
#include "primary_index.h"
#include "inverted_index.h"
#include "trie.h"

std::vector<int> intersect(std::vector<int> a, std::vector<int> b) {
    std::vector<int> res;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(res));
    return res;
}

std::vector<Genre> load_genres(ApiClient& client) {
    std::vector<Genre> genres;
    std::string path = "data/genres.dat";
    std::ifstream in(path, std::ios::binary);
    if (in.is_open()) {
        size_t count = 0;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        for (size_t i = 0; i < count; ++i) {
            Genre g;
            in.read(reinterpret_cast<char*>(&g.id), sizeof(g.id));
            size_t name_len = 0;
            in.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
            g.name.resize(name_len);
            in.read(&g.name[0], name_len);
            genres.push_back(g);
        }
        return genres;
    }
    
    std::cout << "Baixando lista de generos...\n";
    genres = client.getGenres();
    
    std::ofstream out(path, std::ios::binary);
    size_t count = genres.size();
    out.write(reinterpret_cast<char*>(&count), sizeof(count));
    for (const auto& g : genres) {
        out.write(reinterpret_cast<const char*>(&g.id), sizeof(g.id));
        size_t name_len = g.name.size();
        out.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
        out.write(g.name.c_str(), name_len);
    }
    return genres;
}

int main() {
    std::srand(std::time(nullptr));

    const std::string API_KEY = "ce4997dc14a0b4f70ddc709656625b41";
    ApiClient client(API_KEY);

    FilmRecord recorder;
    PrimaryIndex pIndex;
    InvertedIndex genreIndex("genres");
    Trie titleIndex;

    pIndex.load();
    genreIndex.load();
    titleIndex.load();
    
    auto genres = load_genres(client);

    std::cout << "Deseja atualizar/baixar novos filmes? (1-Sim, 0-Nao): " << std::flush;
    int update;
    std::cin >> update;

    if (update == 1) {
        std::cout << "Quantas paginas deseja buscar? (20 resultados por pagina) ";
        int pages;
        std::cin >> pages;
        std::cout << "Buscando na API...\n";
        auto filmes = client.getMovies(pages); 
        
        if (!filmes.empty()) {
            std::cout << "Salvando e Indexando...\n";
            for (const auto& f : filmes) {
                if (pIndex.getOffset(f.id) != -1) continue;

                long offset = recorder.saveMovie(f);
                
                pIndex.add(f.id, offset);
                titleIndex.insert(f.title, f.id);
                for (int g : f.genres) genreIndex.addRelation(g, f.id);
            }
            
            pIndex.save();
            genreIndex.save();
            titleIndex.save();
            std::cout << "Dados salvos!\n";
        }
    }

    // [NOVO] Menu Interativo (Requisito 5)
    int opt = 0;
    while (opt != 5) {
        std::cout << "\n--- ROLETA DE FILMES ---\n";
        std::cout << "1. Buscar por Titulo (TRIE)\n";
        std::cout << "2. Roleta por Genero (Arquivo Invertido)\n";
        std::cout << "3. Listar Filme por ID\n";
        std::cout << "4. Listar Todos os Filmes\n";
        std::cout << "5. Sair\nOpcao: ";
        std::cin >> opt;

        switch (opt) {
            case 1: {
                std::string termo;
                std::cout << "Digite o inicio do nome: ";
                std::cin >> termo;
                auto ids = titleIndex.search(termo);
                std::cout << ids.size() << " filmes encontrados.\n";
                
                int count = 0;
                for (int id : ids) {
                    long off = pIndex.getOffset(id);
                    if (off != -1) {
                        Film f = recorder.readMovieAt(off);
                        f.print();
                        count++;
                        if (count >= 5) { // Paginação simples
                            std::cout << "(...mais resultados...)\n";
                            std::cout << "Deseja exibir mais resultados? \nOpcao: ";
                            int more;
                            std::cin >> more;
                            if (more == 1) {
                                count = 0;
                            } else {
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 2: {
                std::cout << "\n--- GENEROS DISPONIVEIS ---\n";
                for(const auto& g : genres) {
                     std::cout << g.id << " - " << g.name << "\n";
                }
                int genId;
                std::cout << "ID do Genero (ex: 28 Acao, 35 Comedia): ";
                std::cin >> genId;
                auto ids = genreIndex.getIds(genId);
                
                if (!ids.empty()) {
                    int vencedor = ids[std::rand() % ids.size()];
                    std::cout << "\n*** FILME SORTEADO ***\n";
                    long off = pIndex.getOffset(vencedor);
                    Film f = recorder.readMovieAt(off);
                    f.print();
                    std::cout << "Sinopse: " << f.overview << "\n";
                } else {
                    std::cout << "Nenhum filme desse genero encontrado.\n";
                }
                break;
            }
            case 3: {
                int id;
                std::cout << "Digite o ID: ";
                std::cin >> id;
                long off = pIndex.getOffset(id);
                if (off != -1) {
                     Film f = recorder.readMovieAt(off);
                     f.print();
                     std::cout << "Sinopse completa: " << f.overview << "\n";
                } else {
                    std::cout << "Filme nao encontrado.\n";
                }
                break;
            }
            case 4: {
                std::cout << "\n--- LISTA DE FILMES ---\n";
                std::cout << std::left << std::setw(10) << "ID" << " | " << "Titulo" << "\n";
                std::cout << "--------------------------------\n";
                
                const auto& index = pIndex.getIndex();
                for (const auto& [id, offset] : index) {
                    Film f = recorder.readMetadataAt(offset);
                    std::cout << std::left << std::setw(10) << f.id << " | " << f.title << "\n";
                }
                std::cout << "Total: " << index.size() << " filmes.\n";
                break;
            }
            case 5: {
                std::cout << "Saindo...\n";
                break;
            }
            default: {
                std::cout << "Opcao invalida. Por favor, escolha uma opcao entre 1 e 5.\n";
                std::cin.clear();
                break;
            }
        }
    }

    return 0;
}