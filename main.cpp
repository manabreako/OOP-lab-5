
#include <iostream>
#include <list>
#include <unordered_map>
#include <string>

using namespace std;

template <typename K, typename V, size_t Capacity>
class LRUCache {
private:
    using Item = pair<K, V>;

    list<Item> cache;
    unordered_map<K, typename list<Item>::iterator> index;

public:
    void put(const K& key, const V& value) {
        auto it = index.find(key);

        if (it != index.end()) {
            it->second->second = value;
            cache.splice(cache.begin(), cache, it->second);
            return;
        }

        if (cache.size() == Capacity) {
            index.erase(cache.back().first);
            cache.pop_back();
        }

        cache.emplace_front(key, value);
        index[key] = cache.begin();
    }

    bool get(const K& key, V& value) {
        auto it = index.find(key);

        if (it == index.end()) {
            return false;
        }

        cache.splice(cache.begin(), cache, it->second);
        value = it->second->second;

        return true;
    }

    size_t size() const {
        return cache.size();
    }

    void clear() {
        cache.clear();
        index.clear();
    }
};

int main() {
    LRUCache<int, string, 5> cache;

    cache.put(1, "Apple");
    cache.put(2, "Banana");

    string value;

    if (cache.get(1, value)) {
        cout << "Found: " << value << '\n';
    }

    cout << "Cache size: " << cache.size() << '\n';

    return 0;
}
