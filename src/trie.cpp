#include "trie.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Trie::Trie() { nodes.push_back(TrieNode()); }

long Trie::createNode(char c) {
    nodes.push_back(TrieNode(c));
    return nodes.size() - 1;
}

void Trie::insert(const std::string& title, int movieId) {
    long current = 0;
    for (char c : title) {
        char key = std::toupper(c);
        long child = nodes[current].first_child;
        long prev = -1;
        bool found = false;

        while (child != -1) {
            if (nodes[child].character == key) {
                current = child;
                found = true;
                break;
            }
            prev = child;
            child = nodes[child].next_sibling;
        }

        if (!found) {
            long newNode = createNode(key);
            if (prev == -1) nodes[current].first_child = newNode;
            else nodes[prev].next_sibling = newNode;
            current = newNode;
        }
    }
    nodes[current].movie_id = movieId;
}

void Trie::collect(long index, std::vector<int>& results) {
    if (index == -1) return;
    if (nodes[index].movie_id != -1) results.push_back(nodes[index].movie_id);
    collect(nodes[index].first_child, results);
    collect(nodes[index].next_sibling, results);
}

std::vector<int> Trie::search(const std::string& prefix) {
    std::vector<int> results;
    long current = 0;
    for (char c : prefix) {
        char key = std::toupper(c);
        long child = nodes[current].first_child;
        bool found = false;
        while (child != -1) {
            if (nodes[child].character == key) {
                current = child;
                found = true;
                break;
            }
            child = nodes[child].next_sibling;
        }
        if (!found) return results;
    }
    collect(nodes[current].first_child, results);
    if (nodes[current].movie_id != -1) results.push_back(nodes[current].movie_id);
    return results;
}

void Trie::save() {
    std::ofstream file(FILENAME, std::ios::binary);
    size_t size = nodes.size();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(nodes.data()), size * sizeof(TrieNode));
}

void Trie::load() {
    std::ifstream file(FILENAME, std::ios::binary);
    if (!file.is_open()) return;
    size_t size = 0;
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(size))) return;
    nodes.resize(size);
    file.read(reinterpret_cast<char*>(nodes.data()), size * sizeof(TrieNode));
    std::cout << "Indice TRIE carregado.\n";
}