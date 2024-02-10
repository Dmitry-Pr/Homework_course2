#include <iostream>
#include <vector>
#include <cstdint>

// Элемент хеш-таблицы - список объектов с одним хешем
template<class KeyType, class ValueType>
struct Node {
    KeyType key_;
    ValueType value_;
    Node *next_;

    Node(KeyType, ValueType);
};

template<class KeyType, class ValueType>
Node<KeyType, ValueType>::Node(KeyType key, ValueType val) {
    key_ = key;
    value_ = val;
    next_ = nullptr;
}

// Хеш-таблица
template<class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
public:
    HashTable();

    explicit HashTable(Func);

    HashTable(size_t, double, Func func = std::hash<KeyType>());

    ~HashTable();

    void insert(KeyType, ValueType);

    ValueType *find(KeyType);

    void erase(KeyType);

    Node<KeyType, ValueType> &operator[](uint64_t);

    Node<KeyType, ValueType> at(uint64_t);

    size_t size() const;

    size_t capacity() const;

private:
    std::vector<Node<KeyType, ValueType> *> *table_;
    size_t cap_ = 100;
    size_t count_ = 0;
    double fullness_ = 0.5;
    Func func_;
};

template<class KeyType, class ValueType, class Func>
HashTable<KeyType, ValueType, Func>::HashTable() {
    table_ = new std::vector<Node<KeyType, ValueType> *>(cap_);
}

template<class KeyType, class ValueType, class Func>
HashTable<KeyType, ValueType, Func>::HashTable(Func func) {
    func_ = func;
    table_ = new std::vector<Node<KeyType, ValueType> *>(cap_);
}

template<class KeyType, class ValueType, class Func>
HashTable<KeyType, ValueType, Func>::HashTable(size_t cap, double fullness, Func func) {
    cap_ = cap;
    fullness_ = fullness;
    if (fullness <= 0 || fullness > 1) {
        fullness_ = 0.5;
    }
    func_ = func;
    table_ = new std::vector<Node<KeyType, ValueType> *>(cap_);
}

template<class KeyType, class ValueType, class Func>
HashTable<KeyType, ValueType, Func>::~HashTable() {
    if (table_ == nullptr) {
        return;
    }
    for (auto &chain: *table_) {
        if (chain != nullptr) {
            auto cur = chain;
            while (cur != nullptr) {
                auto next = cur->next_;
                delete cur;
                cur = next;
            }
        }
    }
    delete table_;
}

template<class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::insert(KeyType key, ValueType val) {
    size_t ind = func_(key) % cap_;
    auto node = new Node(key, val);
    count_++;
    if (table_->at(ind) == nullptr) {
        table_->at(ind) = node;
    } else {
        auto cur = table_->at(ind);
        while (cur->next_ != nullptr) {
            if (cur->key_ == key) {
                count_--;
                cur->value_ = val;
                delete node;
                return;
            }
            cur = cur->next_;
        }
        if (cur->key_ == key) {
            count_--;
            cur->value_ = val;
            delete node;
            return;
        }
        cur->next_ = node;
    }
    if (cap_ == 0) {
        return;
    }
    if (1.0 * count_ / cap_ > fullness_) {
        auto newTable = HashTable<KeyType, ValueType, Func>(cap_ * 2, fullness_, func_);
        for (auto &chain: *table_) {
            if (chain != nullptr) {
                auto cur = chain;
                while (cur != nullptr) {
                    auto next = cur->next_;
                    newTable.insert(cur->key_, cur->value_);
                    delete cur;
                    cur = next;
                }
            }
        }
        delete table_;
        table_ = newTable.table_;
        newTable.table_ = nullptr;
        cap_ *= 2;
    }
}

template<class KeyType, class ValueType, class Func>
ValueType *HashTable<KeyType, ValueType, Func>::find(KeyType key) {
    auto ind = func_(key) % cap_;
    if (table_->at(ind) == nullptr) {
        return nullptr;
    }
    auto cur = table_->at(ind);
    while (cur != nullptr) {
        if (cur->key_ == key) {
            return &cur->value_;
        }
        cur = cur->next_;
    }
    return nullptr;
}

template<class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::erase(KeyType key) {
    auto ind = func_(key) % cap_;
    if (table_->at(ind) == nullptr) {
        return;
    }
    auto cur = table_->at(ind);
    if (cur->key_ == key) {
        table_->at(ind) = cur->next_;
        delete cur;
        count_--;
        return;
    }
    auto prev = table_->at(ind);
    cur = cur->next_;
    while (cur != nullptr) {
        if (cur->key_ == key) {
            prev->next_ = cur->next_;
            delete cur;
            count_--;
            return;
        }
        prev = cur;
        cur = cur->next_;
    }
}

template<class KeyType, class ValueType, class Func>
Node<KeyType, ValueType> &HashTable<KeyType, ValueType, Func>::operator[](uint64_t hash) {
    if (hash >= cap_) {
        throw std::out_of_range("");
    }
    if (table_->at(hash) == nullptr) {
        throw std::runtime_error("");
    }
    return *table_->at(hash);
}

template<class KeyType, class ValueType, class Func>
Node<KeyType, ValueType> HashTable<KeyType, ValueType, Func>::at(uint64_t hash) {
    if (hash >= cap_) {
        throw std::out_of_range("");
    }
    if (table_->at(hash) == nullptr) {
        throw std::runtime_error("");
    }
    return *table_->at(hash);
}

template<class KeyType, class ValueType, class Func>
size_t HashTable<KeyType, ValueType, Func>::size() const {
    return count_;
}

template<class KeyType, class ValueType, class Func>
size_t HashTable<KeyType, ValueType, Func>::capacity() const {
    return cap_;
}

//int main() {
//    HashTable<int, int> table = HashTable<int, int>();
//    for (int i = 0; i < 60; i++) {
//        table.insert(i, i);
//    }
//    table.insert(1, 1);
//    table.insert(10, 2);
//    std::cout << table.capacity() << '\n';
//    std::cout << table.at(10).value_ << '\n' << table[10].value_;
//    table.erase(1);
//}