#include "VarList.h"
VarNode* VarList::Find(const std::string& _name) const{
    VarNode* it = this->head;
    while(it){
        if(it->data.name == _name){
            return it;
        }
        it = it->next;
    }
    return nullptr;
}
void VarList::Store(const std::string& _name, const int64_t _val){
    // Allocate the node.
    VarNode* n = new VarNode();
    n->data.name = _name;
    n->data.value = _val;
    n->next = nullptr;
    // Start iterating.
    VarNode* p = this->head;
    // The list is empty.
    if(!p){
        this->head = n;
    }
    // The variable is lexicographically before the head.
    else if(_name < p->data.name){
        n->next = p;
        this->head = n;
    }
    // We need to find the node that comes directly before the new value.
    else{
        while(p->next && p->next->data.name < _name){
            p = p->next;
        }
        n->next = p->next;
        p->next = n;
    }
    // We added a new tail.
    if(this->tail == p){
        this->tail = n;
    }
    // Increment the size.
    size++;
}
VarList::VarList(void){
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;
}
VarList::~VarList(void){
    this->Clear();
}
int VarList::Size(void) const{
    return this->size;
}
void VarList::Clear(void){
    while(this->head){
        VarNode* d = this->head;
        this->head = d->next;
        delete d;
    }
    this->head = nullptr;
    this->tail = nullptr;
    this->size = 0;
}

bool VarList::Contains(const std::string& _name) const{
    return this->Find(_name) != nullptr;
}
bool VarList::AddVar(const std::string& _name, const int64_t _val){
    if(!this->Contains(_name)){
        this->Store(_name, _val);
        return true;
    }
    return false;
}

bool VarList::SetVar(const std::string& _name, const int64_t _val){
    VarNode* tgt = this->Find(_name);
    if(tgt){
        tgt->data.value = _val;
        return true;
    }
    else{
        return this->AddVar(_name, _val);
    }
}
bool VarList::DelVar(const std::string& _name){
    VarNode* tgt = this->Find(_name);
    if(tgt != nullptr){
        if(size == 1){
            Clear();
        }
        else{
            VarNode* p = this->head;
            while(p->next != tgt){
                p = p->next;
            }
            p->next = tgt->next;
            if(this->tail == tgt){
                this->tail = p;
            }
            delete tgt;
        }
    }
    return false;
}
bool VarList::GetVar(const std::string& _name, int64_t& _val) const{
    VarNode* tgt = this->Find(_name);
    if(tgt != nullptr){
        _val = tgt->data.value;
        return true;
    }
    return false;
}

void VarList::GetVarData(VarEntry* rec) const{
    if(this->size > 0){
        int idx = 0;
        VarNode* it = this->head;
        while(it){
            rec[idx].name = it->data.name;
            rec[idx].value = it->data.value;
            idx++;
            it = it->next;
        }
    }
}

void VarList::SetVarData(VarEntry* _rec, const int num){
    for(int i = 0; i < num; i++){
        this->SetVar(_rec[i].name,_rec[i].value);
    }
}