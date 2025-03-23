#include <iostream>

const double LOAD_FACTOR_MAX = 1.0;   // When full, resize
const double LOAD_FACTOR_MIN = 0.25; // When too empty, shrink
const double MULTIPLIER = 0.6180339887; // Used for multiplication hashing

struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
    
    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
public:
    Node* head;
    Node* tail;

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insert(int key, int value) {
        Node* newNode = new Node(key, value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void remove(int key) {
        Node* current = head;
        while (current) {
            if (current->key == key) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                delete current;
                return;
            }
            current = current->next;
        }
    }

    Node* find(int key) {
        Node* current = head;
        while (current) {
            if (current->key == key) return current;
            current = current->next;
        }
        return nullptr;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }
};

class HashTable {
private:
    DoublyLinkedList** table;
    int capacity;
    int size;

    int hash(int key) {
        double fractionalPart = key * MULTIPLIER - int(key * MULTIPLIER);
        return int(capacity * fractionalPart);
    }

    void resize(int newCapacity) {
        DoublyLinkedList** oldTable = table;
        int oldCapacity = capacity;
        
        table = new DoublyLinkedList*[newCapacity]();
        for (int i = 0; i < newCapacity; i++)
            table[i] = new DoublyLinkedList();
        
        capacity = newCapacity;
        size = 0;
        
        for (int i = 0; i < oldCapacity; i++) {
            Node* current = oldTable[i]->head;
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
            delete oldTable[i];
        }
        delete[] oldTable;
    }

public:
    HashTable(int initCapacity = 8) : capacity(initCapacity), size(0) {
        table = new DoublyLinkedList*[capacity]();
        for (int i = 0; i < capacity; i++)
            table[i] = new DoublyLinkedList();
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++)
            delete table[i];
        delete[] table;
    }

    void insert(int key, int value) {
        if (size >= capacity * LOAD_FACTOR_MAX)
            resize(capacity * 2);
        
        int index = hash(key);
        if (!table[index]->find(key)) {
            table[index]->insert(key, value);
            size++;
        }
    }

    void remove(int key) {
        int index = hash(key);
        if (table[index]->find(key)) {
            table[index]->remove(key);
            size--;
        }
        if (size <= capacity * LOAD_FACTOR_MIN && capacity > 8)
            resize(capacity / 2);
    }

    int get(int key) {
        int index = hash(key);
        Node* node = table[index]->find(key);
        if (node)
            return node->value;
        throw std::runtime_error("Key not found");
    }

    void display() {
        for (int i = 0; i < capacity; i++) {
            std::cout << "Bucket " << i << ": ";
            Node* current = table[i]->head;
            while (current) {
                std::cout << "(" << current->key << ", " << current->value << ") -> ";
                current = current->next;
            }
            std::cout << "NULL\n";
        }
    }
};

int main() {
    HashTable ht;
    ht.insert(1, 100);
    ht.insert(2, 200);
    ht.insert(3, 300);
    ht.insert(10, 400);
    
    ht.display();
    
    std::cout << "Value for key 2: " << ht.get(2) << "\n";
    
    ht.remove(2);
    ht.display();
    
    return 0;
}


