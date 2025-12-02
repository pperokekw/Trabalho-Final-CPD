#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>

struct TrieNode {
    char character;
    int movie_id; 
    long first_child;
    long next_sibling;
    TrieNode(char c = '\0') : character(c), movie_id(-1), first_child(-1), next_sibling(-1) {}
};

class Trie {
public:
    Trie();
    void insert(const std::string& title, int movieId);
    std::vector<int> search(const std::string& prefix);
    
    void save();
    void load();

private:
    std::vector<TrieNode> nodes;
    long createNode(char c);
    void collect(long index, std::vector<int>& results);
    const std::string FILENAME = "data/titles.trie";
};

#endif