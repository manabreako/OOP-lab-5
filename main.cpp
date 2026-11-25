
#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
struct ValueMover {
    static T moveValue(T value) {
        return value;
    }
};

template <>
struct ValueMover<string> {
    static string moveValue(string value) {
        return std::move(value);
    }
};

template <typename K, typename V, size_t Capacity>
class LRUCache {
    static_assert(is_arithmetic_v<K> || is_same_v<K, string>,
                  "Unsupported key type");

    static_assert(is_arithmetic_v<V> || is_same_v<V, string>,
                  "Unsupported value type");

    static_assert(Capacity > 0, "Capacity must be positive");

private:
    using Item = pair<K, V>;

    list<Item> cache;
    unordered_map<K, typename list<Item>::iterator> index;

public:
    void put(const K& key, V value) {
        auto it = index.find(key);

        if (it != index.end()) {
            it->second->second =
                ValueMover<V>::moveValue(std::move(value));

            cache.splice(cache.begin(), cache, it->second);
            return;
        }

        if (cache.size() == Capacity) {
            index.erase(cache.back().first);
            cache.pop_back();
        }

        cache.emplace_front(
            key, ValueMover<V>::moveValue(std::move(value))
        );

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

    template <typename A, typename B, size_t N>
    friend ostream& operator<<(ostream& out,
                               const LRUCache<A, B, N>& obj);
};

// Дружественная функция вывода
template <typename K, typename V, size_t N>
ostream& operator<<(ostream& out, const LRUCache<K, V, N>& obj) {
    out << "LRU Cache:\n";

    for (const auto& item : obj.cache) {
        out << item.first << " : " << item.second << '\n';
    }

    return out;
}

// Массовое заполнение кэша
template <typename K, typename V, size_t N>
void fillCache(LRUCache<K, V, N>& cache,
               const vector<pair<K, V>>& data) {
    for (const auto& item : data) {
        cache.put(item.first, item.second);
    }
}

// Обычная шаблонная функция
template <typename T>
void printItem(const T& value) {
    cout << value;
}

// Специализация для string
template <>
void printItem<string>(const string& value) {
    cout << "[Строка] " << value;
}

int main() {
    LRUCache<int, string, 5> cache;

    cache.put(1, "Apple");
    cache.put(2, "Banana");

    vector<pair<int, string>> data = {
        {3, "Orange"},
        {4, "Potato"}
    };

    fillCache(cache, data);

    cout << cache;

    return 0;
}
