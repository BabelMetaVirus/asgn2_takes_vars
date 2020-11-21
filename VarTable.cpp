#include "VarTable.h"

void VarTable::Clear(void){
    if(this->buckets){
        for(uint64_t i = 0; i < this->width; i++){
            this->buckets[i].Clear();
        }
        delete [] this->buckets;
    }
    this->buckets = nullptr;
    this->size = 0;
    this->width = 0;
}
uint64 VarTable::HashFunc(const std::string& _name) const{
    uint64 chc = CityHash64(_name.c_str(), _name.length());
    uint64 hc = chc % this->width;
    return hc;
}

VarTable::VarTable(int _width){
    this->width = _width;
    this->size = 0;
    this->buckets = new VarList[this->width];
}

VarTable::~VarTable(void){
    this->Clear();
}

int VarTable::Width(void) const{
    return this->width;
}
int VarTable::Size(void) const{
    return this->size;
}
bool VarTable::Contains(const std::string& _name) const{
    if(this->buckets){
        uint64 hc = this->HashFunc(_name);
        return this->buckets[hc].Contains(_name);
    }
    return false;
}
bool VarTable::AddVar(const std::string& _name, const int64_t _val){
    bool bFlag = false;
    if(this->buckets){
        uint64 hc = this->HashFunc(_name);
        this->size -= this->buckets[hc].Size();
        bFlag = this->buckets[hc].AddVar(_name, _val);
        this->size += this->buckets[hc].Size();
    }
    return bFlag;
}
bool VarTable::SetVar(const std::string& _name, const int64_t _val){
    bool bFlag = false;
    if(this->buckets){
        uint64 hc = this->HashFunc(_name);
        this->size -= this->buckets[hc].Size();
        bFlag = this->buckets[hc].SetVar(_name, _val);
        this->size += this->buckets[hc].Size();
    }
    return bFlag;
}
bool VarTable::DelVar(const std::string& _name){
    bool bFlag = false;
    if(this->buckets){
        uint64 hc = this->HashFunc(_name);
        this->size -= this->buckets[hc].Size();
        bFlag = this->buckets[hc].DelVar(_name);
        this->size += this->buckets[hc].Size();
    }
    return bFlag;
}
bool VarTable::GetVar(const std::string& _name, int64_t& _val) const{
    if(this->buckets){
        uint64 hc = this->HashFunc(_name);
        return this->buckets[hc].GetVar(_name, _val);
    }
    return false;
}
void VarTable::GetVarData(VarEntry* _rec) const{
    int offset = 0;
    for(uint64_t i = 0; i < this->width; i++){
        this->buckets[i].GetVarData((_rec + offset));
        offset += this->buckets[i].Size();
    }
}
void VarTable::SetVarData(VarEntry* _rec, const int num){
    if(this->buckets){
        for(int i = 0; i < num; i++){
            this->AddVar(_rec[i].name, _rec[i].value);
        }
    }
    
}