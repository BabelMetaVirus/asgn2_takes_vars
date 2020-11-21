#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

class LinkedList{
    private:
    struct Node {
        std::string varName;
        int64_t value;
        struct Node* next;  
        Node(std::string _varName = "!", int _num = 0){
            this->varName = _varName;
            this->value = _num;
        }
    };
    Node* head;
    Node* tail;
    int size;
    LinkedList(void){
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
    }
    LinkedList(const LinkedList& _cpy){
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
        Node* it = _cpy.head;
        while(it){
            add(it->varName, it->value);
            it = it->next;
        }
    }
    LinkedList& operator=(const LinkedList& _cpy){
        if(&_cpy != this){
            this->head = nullptr;
            this->tail = nullptr;
            this->size = 0;
            Node* it = _cpy.head;
            while(it){
                add(it->varName, it->value);
                it = it->next;
            }
        }
        return *this;
    }
    void add(std::string varName, int64_t val){
        if(this->tail){
            this->tail->next = new Node(varName, val);
            this->tail = this->tail->next;
        }
        else{
            this->tail = new Node(varName, val);
            this->head = this->tail;
        }
        size++;
    }
    bool contains(std::string _varName){
        bool found = false;
        Node* it = this->head;
        while(it){
            if(it->varName == _varName){
                found = true;
                break;
            }
            it = it->next;
        }
        return found;
    }
};