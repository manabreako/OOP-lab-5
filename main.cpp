
#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;

// Перемещение значений
template <typename T>
struct ValueMover {
    static T moveValue(T value) {
        return value;
    }
};

// Специализация для строк
template <>
struct ValueMover<string> {
    static string moveValue(string value) {
        return std::move(value);
    }
};

// Шаблонный класс LRU Cache
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

    inline static size_t hits = 0;
    inline static size_t misses = 0;

public:
    // Получение элемента
    bool get(const K& key, V& value) {
        auto it = index.find(key);

        if (it == index.end()) {
            misses++;
            return false;
        }

        hits++;

        cache.splice(cache.begin(), cache, it->second);
        value = it->second->second;

        return true;
    }

    // Добавление элемента
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

    // Размер кэша
    size_t size() const {
        return cache.size();
    }

    // Очистка
    void clear() {
        cache.clear();
        index.clear();
    }

    // Статические функции
    static size_t getHits() {
        return hits;
    }

    static size_t getMisses() {
        return misses;
    }

    // Дружественная функция
    template <typename A, typename B, size_t N>
    friend ostream& operator<<(ostream& out,
                               const LRUCache<A, B, N>& obj);
};

// Вывод состояния кэша
template <typename K, typename V, size_t N>
ostream& operator<<(ostream& out, const LRUCache<K, V, N>& obj) {
    out << "LRU Cache:\n";

    for (const auto& item : obj.cache) {
        out << item.first << " : " << item.second << '\n';
    }

    return out;
}

// Массовое заполнение
template <typename K, typename V, size_t N>
void fillCache(LRUCache<K, V, N>& cache,
               const vector<pair<K, V>>& data) {
    for (const auto& item : data) {
        cache.put(item.first, item.second);
    }
}

// Шаблонная функция вывода
template <typename T>
void printItem(const T& value) {
    cout << value;
}

// Специализация для string
template <>
void printItem<string>(const string& value) {
    cout << "[Строка] " << value;
}

// Чтение данных из файла
template <typename K, typename V>
vector<pair<K, V>> readFile(const string& filename) {
    vector<pair<K, V>> data;

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка открытия файла!\n";
        return data;
    }

    K key;
    V value;

    while (file >> key >> value) {
        data.emplace_back(key, value);
    }

    return data;
}

// Главная функция
int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // Создание кэша на 5 элементов
    LRUCache<int, string, 5> cache;

    // Чтение 10 объектов из файла
    vector<pair<int, string>> data =
        readFile<int, string>("cache_data.txt");

    if (data.size() != 10) {
        cout << "В файле должно быть 10 объектов!\n";
        return 1;
    }

    // Заполнение кэша
    fillCache(cache, data);

    cout << "После заполнения:\n";
    cout << cache;

    // Проверка попадания
    string value;

    if (cache.get(8, value)) {
        cout << "\nНайден элемент: ";
        printItem<string>(value);
        cout << '\n';
    }

    // Проверка промаха
    cache.get(100, value);

    // Добавление нового элемента
    cache.put(11, "New element");

    cout << "\nПосле добавления нового элемента:\n";
    cout << cache;

    // Статистика
    cout << "\nРазмер кэша: " << cache.size() << '\n';
    cout << "Попадания: " << cache.getHits() << '\n';
    cout << "Промахи: " << cache.getMisses() << '\n';

    // Очистка
    cache.clear();

    cout << "\nПосле очистки: "
         << cache.size() << " элементов\n";

    return 0;
}
