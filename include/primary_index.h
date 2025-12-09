#ifndef PRIMARY_INDEX_H
#define PRIMARY_INDEX_H

#include <map>
#include <string>

class PrimaryIndex {
public:
    void add(int id, long offset);
    long getOffset(int id);
    const std::map<int, long>& getIndex() const;
    void save();
    void load();

private:
    std::map<int, long> index_map;
    const std::string FILENAME = "data/primary.idx";
};

#endif