#ifndef _VARLIST_H_
#define _VARLIST_H_
#include <string>

struct VarEntry{
    std::string name;
    int64_t value;
};

struct VarNode{
    VarEntry data;
    VarNode* next;
};

class VarList{
    private:
    VarNode* head;
    VarNode* tail;
    int size;
    VarNode* Find(const std::string&) const; //
    void Store(const std::string&, const int64_t); //
    public:
    VarList(void); //
    ~VarList(void); //
    int Size(void) const; //
    void Clear(void); //
    bool Contains(const std::string&) const; //
    bool AddVar(const std::string&, const int64_t); //
    bool SetVar(const std::string&, const int64_t); //
    bool DelVar(const std::string&); //
    bool GetVar(const std::string&, int64_t&) const; //
    void GetVarData(VarEntry*) const; //
    void SetVarData(VarEntry*, const int num); //
};
#endif