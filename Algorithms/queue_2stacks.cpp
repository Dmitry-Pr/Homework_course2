#include <iostream>
#include <stack>

template<class T>
class MyQueue {
private:
    std::stack<T> first_;
    std::stack<T> second_;
    int32_t size_;
public:
    MyQueue() {
        size_ = 0;
    }

    void Push(T val) {
        first_.push(val);
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
            --size_;
        } else {
            while (!first_.empty()) {
                second_.push(first_.top());
                first_.pop();
            }
            if (!second_.empty()) {
                second_.pop();
                --size_;
            }
        }
    }

    T Front() {
        if (Empty()) {
            throw std::out_of_range("Очередь пустая");
        }
        if (!second_.empty()) {
            return second_.top();
        } else {
            while (!first_.empty()) {
                second_.push(first_.top());
                first_.pop();
            }
            return second_.top();
        }
    }

    T Back() {
        if (Empty()) {
            throw std::out_of_range("Очередь пустая");
        }
        if (!first_.empty()) {
            return first_.top();
        } else {
            while (!second_.empty()) {
                first_.push(second_.top());
                second_.pop();
            }
            return first_.top();
        }
    }

    void Clear() {
        size_ = 0;
        first_ = std::stack<T>();
        second_ = std::stack<T>();
    }

    T Min() {
        if (Empty()) {
            throw std::out_of_range("Очередь пуста");
        }
        while (!second_.empty()) {
            first_.push(second_.top());
            second_.pop();
        }
        T min = first_.top();
        while (!first_.empty()) {
            if (min > first_.top()) {
                min = first_.top();
            }
            second_.push(first_.top());
            first_.pop();
        }
        return min;
    }
};

int main() {
    try {
        MyQueue<int32_t> q;
        q.Push(1);
        q.Push(2);
        q.Push(3);
        std::cout << q.Front() << ' ' << q.Back() << '\n';
        q.Pop();
        std::cout << q.Front() << ' ' << q.Back() << '\n';
        q.Push(4);
        std::cout << q.Front() << ' ' << q.Back() << '\n';
        std::cout << q.Min();
    }
    catch (std::out_of_range &e) {
        std::cout << e.what();
    }
}
