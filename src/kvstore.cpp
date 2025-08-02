#include "kvstore.hpp"
#include <fstream>
#include <sstream>


KeyValueStore::KeyValueStore(const std::string& fname) : filename(fname) {}

void KeyValueStore::load_from_disk() {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            store[key] = value;
        }
    }
}

void KeyValueStore::persist_to_disk() {
    std::ofstream file(filename, std::ios::trunc); // overwrite
    for (const auto& [key, value] : store) {
        file << key << "=" << value << "\n";
    }
}


void KeyValueStore::put(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(mtx);
    store[key] = value;
    persist_to_disk();
}

std::optional<std::string> KeyValueStore::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = store.find(key);
    return (it != store.end()) ? std::optional<std::string>(it->second) : std::nullopt;
}

bool KeyValueStore::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    bool erased = store.erase(key) > 0;
    if (erased) persist_to_disk();
    return erased;
}