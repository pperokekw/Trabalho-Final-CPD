#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <map>
#include <string>

// Nó da lista encadeada em disco
struct PostingNode {
    int movie_id;
    long next_offset;
};

// Índice Invertido (Gênero -> Lista de Filmes)
class InvertedIndex {
public:
    InvertedIndex(std::string nameSuffix); // ex: "genres"
    
    void addRelation(int keyId, int movieId);
    std::vector<int> getIds(int keyId);
    
    void save();
    void load();

private:
    std::string filename_dict;
    std::string filename_post;
    std::map<int, long> dictionary; // KeyID -> Head Offset
};

#endif