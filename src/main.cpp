#include <iostream>
#include <ctime>
#include <algorithm>
#include "api_client.h"
#include "film_record.h"
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

    std::cout << "Deseja atualizar/baixar novos filmes? (1-Sim, 0-Nao): ";
    int update;
    std::cin >> update;

    if (update == 1) {
        std::cout << "Buscando na API...\n";
        auto filmes = client.getMovies(2); 
        
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
    while (opt != 4) {
        std::cout << "\n--- ROLETA DE FILMES ---\n";
        std::cout << "1. Buscar por Titulo (TRIE)\n";
        std::cout << "2. Roleta por Genero (Arquivo Invertido)\n";
        std::cout << "3. Listar Filme por ID\n";
        std::cout << "4. Sair\nOpcao: ";
        std::cin >> opt;

        if (opt == 1) {
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
                        break; 
                    }
                }
            }
        }
        else if (opt == 2) {
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
        }
        else if (opt == 3) {
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
        }
    }

    return 0;
}