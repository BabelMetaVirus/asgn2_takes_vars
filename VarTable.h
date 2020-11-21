#ifndef _VARTABLE_H_
#define _VARTABLE_H_
#include "City.h"
#include "VarList.h"
class VarTable{
    private:
    VarList* buckets;
    uint64 width;
    int size;
    void Clear(void);                                   //
    uint64 HashFunc(const std::string&) const;             // TODO: Update with actual hash method.
    public:
    VarTable(int _width = 16);                          //
    ~VarTable(void);                                    //
    int Width(void) const;                        //
    int Size(void) const;                         //
    bool Contains(const std::string&) const; // changed to const
                                             // due to error received
    bool AddVar(const std::string&, const int64_t);
    bool SetVar(const std::string&, const int64_t);
    bool DelVar(const std::string&); //
    bool GetVar(const std::string&, int64_t&) const;
    void GetVarData(VarEntry*) const;
    void SetVarData(VarEntry*, const int num);
};

#endif
