
#include <iostream>
#include <list>
#include <unordered_map>
#include <string>

using namespace std;

// Базовый шаблонный класс LRU Cache
template <typename K, typename V, size_t Capacity>
class LRUCache {
private:
    using Item = pair<K, V>;

    // Список элементов кэша
    list<Item> cache;

    // Таблица для быстрого поиска
    unordered_map<K, typename list<Item>::iterator> index;

public:
    // Конструктор
    LRUCache() {
        cout << "LRU Cache created\n";
    }
};

// Главная функция
int main() {
    LRUCache<int, string, 5> cache;

    cout << "Program started\n";

    return 0;
}
