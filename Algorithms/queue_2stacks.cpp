#include <iostream>
#include <stack>
#include <utility>

template<class T>
class MyQueue {
private:
    std::stack<std::pair<T, T>> first_;
    std::stack<std::pair<T, T>> second_;
    int32_t size_;
    int32_t last_;
public:
    MyQueue() {
        size_ = 0;
        last_ = 0;
    }

    void Push(T val) {
        if (first_.empty()) {
            first_.push({val, val});
        } else {
            first_.push({val, std::min(val, first_.top().second)});
        }
        last_ = val;
        ++size_;
    }

    [[nodiscard]] int32_t Size() const {
        return size_;
    }

    [[nodiscard]] bool Empty() const {
        return Size() == 0;
    }

    void Pop() {
        if (Empty()) {
            throw std::out_of_range("Очередь пустая");
        }
        if (!second_.empty()) {
            second_.pop();
        } else {
            second_.push({first_.top().first, first_.top().first});
            first_.pop();
            while (!first_.empty()) {
                second_.push({first_.top().first, std::min(first_.top().first, second_.top().second)});
                first_.pop();
            }
            second_.pop();
        }
        --size_;
    }

    T Front() {
        if (Empty()) {
            throw std::out_of_range("Очередь пустая");
        }
        if (second_.empty()) {
            second_.push({first_.top().first, first_.top().first});
            first_.pop();
            while (!first_.empty()) {
                second_.push({first_.top().first, std::min(first_.top().first, second_.top().second)});
                first_.pop();
            }
        }
        return second_.top().first;
    }

    T Back() {
        if (Empty()) {
            throw std::out_of_range("Очередь пустая");
        }
        return last_;
    }

    void Clear() {
        size_ = 0;
        first_ = std::stack<std::pair<T, T>>();
        second_ = std::stack<std::pair<T, T>>();
    }

    T Min() {
        if (Empty()) {
            throw std::out_of_range("Очередь пуста");
        }
        if (!first_.empty()) {
            if (!second_.empty()) {
                return std::min(first_.top().second, second_.top().second);
            } else {
                return first_.top().second;
            }
        }
        return second_.top().second;
    }
};

int main() {
    try {
        MyQueue<int32_t> q;
        q.Push(1);
        q.Push(2);
        q.Push(3);
        std::cout << q.Front() << ' ' << q.Back() << ' ' << q.Min() << '\n';
        q.Pop();
        std::cout << q.Front() << ' ' << q.Back() << ' ' << q.Min() << '\n';
        q.Push(4);
        std::cout << q.Front() << ' ' << q.Back() << '\n';
        std::cout << q.Min();
    }
    catch (std::out_of_range &e) {
        std::cout << e.what();
    }
}
