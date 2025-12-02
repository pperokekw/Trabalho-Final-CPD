#include "primary_index.h"
#include <fstream>
#include <iostream>

void PrimaryIndex::add(int id, long offset) {
    index_map[id] = offset;
}

long PrimaryIndex::getOffset(int id) {
    if (index_map.count(id)) return index_map[id];
    return -1;
}

void PrimaryIndex::save() {
    std::ofstream file(FILENAME, std::ios::binary);
    size_t size = index_map.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    
    for (auto const& [id, offset] : index_map) {
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        file.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    }
}

void PrimaryIndex::load() {
    std::ifstream file(FILENAME, std::ios::binary);
    if (!file.is_open()) return;

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    
    for (size_t i = 0; i < size; i++) {
        int id;
        long offset;
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        index_map[id] = offset;
    }
    std::cout << "Indice Primario carregado: " << size << " entradas.\n";
}