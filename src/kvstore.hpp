// This is blueprint of kvstore class implemenatation in kvstore.cpp
// The .hpp file provides abstraction and acts as the contract. 
// It’s what the user includes to use the class. The .cpp file 
// holds the internal logic and stays hidden — encapsulating the implementation
// user -> .hpp 
//  compiler ->both


// Header Guards 
#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <unordered_map>
#include <string>
#include <optional>

#include <mutex>

class KeyValueStore {
    std::unordered_map<std::string, std::string> store;
    std::mutex mtx;  // protects store
    const std::string filename; // data file
public:
    KeyValueStore(const std::string& fname);
    void load_from_disk();
    void persist_to_disk();
    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    bool del(const std::string& key);
};

#endif
