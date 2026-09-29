#pragma once
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cstddef>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        explicit Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    template <bool IsConst>
    class ListIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const T*, T*>;
        using reference         = std::conditional_t<IsConst, const T&, T&>;

    private:
        Node* ptr;
        // Итератор end() - это nullptr, поэтому для --end() нужно знать список,
        // чтобы перейти к его хвосту
        const DoublyLinkedList* list;

        friend DoublyLinkedList;
        template <bool> friend class ListIterator;

        ListIterator(Node* p, const DoublyLinkedList* l) : ptr(p), list(l) {}

    public:
        ListIterator() : ptr(nullptr), list(nullptr) {}

        template <bool C = IsConst, typename = std::enable_if_t<C>>
        ListIterator(const ListIterator<false>& other) : ptr(other.ptr), list(other.list) {}

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

        ListIterator& operator--() {
            ptr = (ptr == nullptr) ? list->tail : ptr->prev;
            return *this;
        }
        ListIterator operator--(int) {
            ListIterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool operator==(const ListIterator& a, const ListIterator& b) { return a.ptr == b.ptr; }
        friend bool operator!=(const ListIterator& a, const ListIterator& b) { return a.ptr != b.ptr; }
    };

    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;

    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        try {
            for (Node* cur = other.head; cur != nullptr; cur = cur->next) {
                push_back(cur->data);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            DoublyLinkedList tmp(other);
            swap(tmp);
        }
        return *this;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    // O(1)
    void push_front(const T& value) {
        Node* node = new Node(value);
        if (head == nullptr) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        ++count;
    }

    // O(1)
    void push_back(const T& value) {
        Node* node = new Node(value);
        if (tail == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++count;
    }

    // O(1)
    void pop_front() {
        if (head == nullptr) throw std::underflow_error("DoublyLinkedList::pop_front: список пуст");
        Node* tmp = head;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
        --count;
    }

    // O(1): в отличие от односвязного списка, предыдущий узел известен сразу
    void pop_back() {
        if (tail == nullptr) throw std::underflow_error("DoublyLinkedList::pop_back: список пуст");
        Node* tmp = tail;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
        --count;
    }

    T& front() {
        if (head == nullptr) throw std::underflow_error("DoublyLinkedList::front: список пуст");
        return head->data;
    }
    const T& front() const {
        if (head == nullptr) throw std::underflow_error("DoublyLinkedList::front: список пуст");
        return head->data;
    }

    T& back() {
        if (tail == nullptr) throw std::underflow_error("DoublyLinkedList::back: список пуст");
        return tail->data;
    }
    const T& back() const {
        if (tail == nullptr) throw std::underflow_error("DoublyLinkedList::back: список пуст");
        return tail->data;
    }

    // O(n), возвращает end(), если элемент не найден
    iterator find(const T& value) {
        Node* cur = head;
        while (cur != nullptr && !(cur->data == value)) cur = cur->next;
        return iterator(cur, this);
    }
    const_iterator find(const T& value) const {
        Node* cur = head;
        while (cur != nullptr && !(cur->data == value)) cur = cur->next;
        return const_iterator(cur, this);
    }

    // O(n), удаляет первое вхождение
    bool remove(const T& value) {
        iterator it = find(value);
        if (it == end()) return false;
        erase(it);
        return true;
    }

    // O(1): вставка перед pos (как в STL). Для end() - вставка в конец
    iterator insert(iterator pos, const T& value) {
        if (pos.ptr == nullptr) {
            push_back(value);
            return iterator(tail, this);
        }
        if (pos.ptr == head) {
            push_front(value);
            return iterator(head, this);
        }
        Node* node = new Node(value);
        node->prev = pos.ptr->prev;
        node->next = pos.ptr;
        pos.ptr->prev->next = node;
        pos.ptr->prev = node;
        ++count;
        return iterator(node, this);
    }

    // O(1): вставка после pos
    iterator insert_after(iterator pos, const T& value) {
        if (pos.ptr == nullptr) throw std::invalid_argument("DoublyLinkedList::insert_after: итератор end()");
        return insert(iterator(pos.ptr->next, this), value);
    }

    // O(1): удаление узла, возвращает итератор на следующий элемент
    iterator erase(iterator pos) {
        if (pos.ptr == nullptr) throw std::out_of_range("DoublyLinkedList::erase: итератор end()");
        Node* node = pos.ptr;
        Node* nextNode = node->next;

        if (node->prev != nullptr) node->prev->next = node->next;
        else head = node->next;

        if (node->next != nullptr) node->next->prev = node->prev;
        else tail = node->prev;

        delete node;
        --count;
        return iterator(nextNode, this);
    }

    // O(n)
    T& getAt(int index) {
        return nodeAt(index)->data;
    }
    const T& getAt(int index) const {
        return nodeAt(index)->data;
    }

    // O(n): у каждого узла меняются местами prev и next
    void reverse() {
        Node* cur = head;
        while (cur != nullptr) {
            std::swap(cur->prev, cur->next);
            cur = cur->prev; // после обмена бывший next лежит в prev
        }
        std::swap(head, tail);
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

    void swap(DoublyLinkedList& other) noexcept {
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

    void printReverse() const {
        std::cout << "[";
        for (Node* cur = tail; cur != nullptr; cur = cur->prev) {
            std::cout << cur->data;
            if (cur->prev != nullptr) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    iterator begin() { return iterator(head, this); }
    iterator end() { return iterator(nullptr, this); }
    const_iterator begin() const { return const_iterator(head, this); }
    const_iterator end() const { return const_iterator(nullptr, this); }
    const_iterator cbegin() const { return const_iterator(head, this); }
    const_iterator cend() const { return const_iterator(nullptr, this); }

private:
    // Обход с ближайшего конца: не более n/2 шагов
    Node* nodeAt(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("DoublyLinkedList::getAt: индекс вне диапазона");
        }
        Node* cur;
        if (index < count / 2) {
            cur = head;
            for (int i = 0; i < index; ++i) cur = cur->next;
        } else {
            cur = tail;
            for (int i = count - 1; i > index; --i) cur = cur->prev;
        }
        return cur;
    }
};