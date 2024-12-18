#include <iostream>
#include <stdexcept>
#include <random>
#include <unordered_set>
#include <cmath>

// Node structure
template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T value) : data(value), next(nullptr) {}
};

// Circular linked list
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    size_t size;

    void copy_list(const LinkedList& other) {
        if (other.size == 0) return;
        Node<T>* current = other.head;
        do {
            push_tail(current->data);
            current = current->next;
        } while (current != other.head);
    }

    void clear_list() {
        while (size > 0) {
            pop_head();
        }
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        copy_list(other);
    }

    ~LinkedList() {
        clear_list();
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear_list();
            copy_list(other);
        }
        return *this;
    }

    LinkedList(size_t n, T min_val, T max_val) : head(nullptr), tail(nullptr), size(0) {
        std::mt19937 gen(42); // Random seed
        std::uniform_int_distribution<T> dist(min_val, max_val);
        for (size_t i = 0; i < n; ++i) {
            push_tail(dist(gen));
        }
    }


    void push_tail(T value) {
        Node<T>* new_node = new Node<T>(value);
        if (!tail) {
            head = tail = new_node;
            new_node->next = head;
        }
        else {
            new_node->next = head;
            tail->next = new_node;
            tail = new_node;
        }
        ++size;
    }
    void pop_tail() {
        if (size == 0) throw std::underflow_error("List is empty.");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = head;
        }
        --size;
    }

    void pop_head() {
        if (size == 0) throw std::underflow_error("List is empty.");
        Node<T>* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            tail->next = head;
        }
        delete temp;
        --size;
    }

    void delete_node(T value) {
        while (size > 0 && head->data == value) {
            pop_head();
        }
        if (size == 0) return;
        Node<T>* current = head;
        do {
            if (current->next->data == value) {
                Node<T>* temp = current->next;
                current->next = temp->next;
                if (temp == tail) {
                    tail = current;
                }
                delete temp;
                --size;
            }
            else {
                current = current->next;
            }
        } while (current != tail);
    }

    Node<T>* get_head() const {
        return head;
    }

    size_t get_size() const {
        return size;
    }
    void push_head(const LinkedList& other) {
        if (!other.head) return;

        LinkedList<T> temp;

        Node<T>* current = other.head;
        do {
            temp.push_tail(current->data);
            current = current->next;
        } while (current != other.head);

        current = this->head;
        if (current) {
            do {
                temp.push_tail(current->data);
                current = current->next;
            } while (current != this->head);
        }

        *this = temp;
    }

    void push_tail(const LinkedList& other) {
        Node<T>* current = other.head;
        if (!current) return;
        do {
            push_tail(current->data);
            current = current->next;
        } while (current != other.head);
    } 
    T operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of range.");
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range.");
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
        if (list.size == 0) return os;
        Node<T>* current = list.head;
        do {
            os << current->data << " ";
            current = current->next;
        } while (current != list.head);
        return os;
    }
};

// Generate lucky numbers using the sieve algorithm
template<typename T>
LinkedList<T> lucky_numbers(int n) {
    LinkedList<T> lucky_numbers;
    for (int i = 1; i <= n; ++i) {
        lucky_numbers.push_tail(i);
    }

    int step = 2;
    while (step <= lucky_numbers.get_size()) {
        Node<T>* current = lucky_numbers.get_head();
        int index = 1;

        LinkedList<T> to_delete;
        do {
            if (index % step == 0) {
                to_delete.push_tail(current->data);
            }
            current = current->next;
            ++index;
        } while (current != lucky_numbers.get_head());

        Node<T>* delete_current = to_delete.get_head();
        do {
            lucky_numbers.delete_node(delete_current->data);
            delete_current = delete_current->next;
        } while (delete_current != to_delete.get_head() && to_delete.get_size() > 0);

        if (step < lucky_numbers.get_size()) {
            Node<T>* step_node = lucky_numbers.get_head();
            for (int i = 1; i < step; ++i) {
                step_node = step_node->next;
            }
            step = step_node->data;
        }
        else {
            break;
        }
    }

    return lucky_numbers;
}

// Generate a list of unlucky numbers
template<typename T>
LinkedList<T> unlucky_numbers(const LinkedList<T>& lucky_numbers, int n) {
    LinkedList<T> all_numbers;
    for (int i = 1; i <= n; ++i) {
        all_numbers.push_tail(i);
    }

    LinkedList<T> unlucky_numbers;
    Node<T>* current = all_numbers.get_head();
    do {
        bool is_lucky = false;
        Node<T>* lucky_current = lucky_numbers.get_head();
        do {
            if (current->data == lucky_current->data) {
                is_lucky = true;
                break;
            }
            lucky_current = lucky_current->next;
        } while (lucky_current != lucky_numbers.get_head());

        if (!is_lucky) {
            unlucky_numbers.push_tail(current->data);
        }
        current = current->next;
    } while (current != all_numbers.get_head());

    return unlucky_numbers;
}




