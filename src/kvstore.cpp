#include "kvstore.hpp"

void KeyValueStore::put(const std::string& key, const std::string& value){
    store[key] = value ;
}

std::optional<std::string> KeyValueStore::get(const std::string& key) {
    auto it = store.find(key);
    return (it != store.end()) ? std::optional<std::string>(it->second) : std::nullopt;
}


bool KeyValueStore::del(const std::string& key) {
    return store.erase(key) > 0;
}