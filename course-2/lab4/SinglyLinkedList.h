#pragma once
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cstddef>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    // IsConst = true даёт const_iterator, false - обычный iterator
    template <bool IsConst>
    class ListIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

    private:
        Node* ptr;

        friend SinglyLinkedList;
        template <bool> friend class ListIterator;

        // Создавать итератор из узла может только сам список
        explicit ListIterator(Node* p) : ptr(p) {}

    public:
        ListIterator() : ptr(nullptr) {}

        // Неявное преобразование iterator -> const_iterator
        template <bool C = IsConst, typename = std::enable_if_t<C>>
        ListIterator(const ListIterator<false>& other) : ptr(other.ptr) {}

        reference operator*() const { return ptr->data; }
        pointer operator->() const { return &ptr->data; }

        ListIterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        ListIterator operator++(int) {
            ListIterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        // friend-функции, чтобы можно было сравнивать iterator с const_iterator
        friend bool operator==(const ListIterator& a, const ListIterator& b) { return a.ptr == b.ptr; }
        friend bool operator!=(const ListIterator& a, const ListIterator& b) { return a.ptr != b.ptr; }
    };

    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;

    SinglyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    SinglyLinkedList(const SinglyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        try {
            for (Node* cur = other.head; cur != nullptr; cur = cur->next) {
                push_back(cur->data);
            }
        } catch (...) {
            // Деструктор не вызывается, если конструктор бросил исключение,
            // поэтому уже созданные узлы освобождаем вручную
            clear();
            throw;
        }
    }

    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    ~SinglyLinkedList() { clear(); }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            SinglyLinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    // O(1)
    void push_front(const T& value) {
        Node* node = new Node(value);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
        ++count;
    }

    // O(1) благодаря tail
    void push_back(const T& value) {
        Node* node = new Node(value);
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++count;
    }

    // O(1)
    void pop_front() {
        if (head == nullptr) throw std::underflow_error("SinglyLinkedList::pop_front: список пуст");
        Node* tmp = head;
        head = head->next;
        delete tmp;
        --count;
        if (head == nullptr) tail = nullptr;
    }

    // O(n): чтобы найти предпоследний узел, нужно пройти весь список
    void pop_back() {
        if (head == nullptr) throw std::underflow_error("SinglyLinkedList::pop_back: список пуст");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* cur = head;
            while (cur->next != tail) cur = cur->next;
            delete tail;
            tail = cur;
            tail->next = nullptr;
        }
        --count;
    }

    // O(1)
    T& front() {
        if (head == nullptr) throw std::underflow_error("SinglyLinkedList::front: список пуст");
        return head->data;
    }
    const T& front() const {
        if (head == nullptr) throw std::underflow_error("SinglyLinkedList::front: список пуст");
        return head->data;
    }

    // O(1)
    T& back() {
        if (tail == nullptr) throw std::underflow_error("SinglyLinkedList::back: список пуст");
        return tail->data;
    }
    const T& back() const {
        if (tail == nullptr) throw std::underflow_error("SinglyLinkedList::back: список пуст");
        return tail->data;
    }

    // O(n), возвращает end(), если элемент не найден
    iterator find(const T& value) {
        Node* cur = head;
        while (cur != nullptr && !(cur->data == value)) cur = cur->next;
        return iterator(cur);
    }
    const_iterator find(const T& value) const {
        Node* cur = head;
        while (cur != nullptr && !(cur->data == value)) cur = cur->next;
        return const_iterator(cur);
    }

    // O(n), удаляет первое вхождение
    bool remove(const T& value) {
        if (head == nullptr) return false;
        if (head->data == value) {
            pop_front();
            return true;
        }
        Node* cur = head;
        while (cur->next != nullptr && !(cur->next->data == value)) cur = cur->next;
        if (cur->next == nullptr) return false;

        Node* toDelete = cur->next;
        cur->next = toDelete->next;
        if (toDelete == tail) tail = cur;
        delete toDelete;
        --count;
        return true;
    }

    // O(1): вставка после узла, на который указывает pos
    iterator insert_after(iterator pos, const T& value) {
        if (pos.ptr == nullptr) throw std::invalid_argument("SinglyLinkedList::insert_after: итератор end()");
        Node* node = new Node(value);
        node->next = pos.ptr->next;
        pos.ptr->next = node;
        if (pos.ptr == tail) tail = node;
        ++count;
        return iterator(node);
    }

    // O(1): удаление узла после pos, возвращает итератор на следующий за удалённым
    iterator erase_after(iterator pos) {
        if (pos.ptr == nullptr || pos.ptr->next == nullptr) {
            throw std::out_of_range("SinglyLinkedList::erase_after: нечего удалять");
        }
        Node* toDelete = pos.ptr->next;
        pos.ptr->next = toDelete->next;
        if (toDelete == tail) tail = pos.ptr;
        delete toDelete;
        --count;
        return iterator(pos.ptr->next);
    }

    // O(n)
    T& getAt(int index) {
        return nodeAt(index)->data;
    }
    const T& getAt(int index) const {
        return nodeAt(index)->data;
    }

    // O(n): разворот перенаправлением указателей, без создания новых узлов
    void reverse() {
        Node* prev = nullptr;
        Node* cur = head;
        tail = head;
        while (cur != nullptr) {
            Node* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        head = prev;
    }

    void clear() {
        while (head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        count = 0;
    }

    void swap(SinglyLinkedList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }

    int size() const { return count; }
    bool empty() const { return head == nullptr; }

    void print() const {
        std::cout << "[";
        for (Node* cur = head; cur != nullptr; cur = cur->next) {
            std::cout << cur->data;
            if (cur->next != nullptr) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator cbegin() const { return const_iterator(head); }
    const_iterator cend() const { return const_iterator(nullptr); }

private:
    Node* nodeAt(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("SinglyLinkedList::getAt: индекс вне диапазона");
        }
        Node* cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur;
    }
};