#include "inverted_index.h"
#include <fstream>
#include <iostream>

InvertedIndex::InvertedIndex(std::string nameSuffix) {
    filename_dict = "data/inv_dict_" + nameSuffix + ".dat";
    filename_post = "data/inv_post_" + nameSuffix + ".dat";
    
    std::ofstream f1(filename_dict, std::ios::app);
    std::ofstream f2(filename_post, std::ios::app);
}

void InvertedIndex::addRelation(int keyId, int movieId) {
    long current_head = -1;
    if (dictionary.count(keyId)) current_head = dictionary[keyId];

    PostingNode node = {movieId, current_head};

    std::ofstream file(filename_post, std::ios::binary | std::ios::app);
    long my_offset = file.tellp();
    file.write(reinterpret_cast<char*>(&node), sizeof(PostingNode));
    file.close();

    dictionary[keyId] = my_offset;
}

std::vector<int> InvertedIndex::getIds(int keyId) {
    std::vector<int> results;
    if (dictionary.find(keyId) == dictionary.end()) return results;

    long cursor = dictionary[keyId];
    std::ifstream file(filename_post, std::ios::binary);

    while (cursor != -1) {
        PostingNode node;
        file.seekg(cursor);
        file.read(reinterpret_cast<char*>(&node), sizeof(PostingNode));
        results.push_back(node.movie_id);
        cursor = node.next_offset;
    }
    return results;
}

void InvertedIndex::save() {
    std::ofstream file(filename_dict, std::ios::binary);
    size_t size = dictionary.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (auto const& [key, offset] : dictionary) {
        file.write(reinterpret_cast<const char*>(&key), sizeof(key));
        file.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    }
}

void InvertedIndex::load() {
    std::ifstream file(filename_dict, std::ios::binary);
    if (!file.is_open()) return;
    size_t size = 0;
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(size))) return;
    for (size_t i = 0; i < size; i++) {
        int key; long offset;
        file.read(reinterpret_cast<char*>(&key), sizeof(key));
        file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        dictionary[key] = offset;
    }
    std::cout << "Indice Invertido carregado.\n";
}