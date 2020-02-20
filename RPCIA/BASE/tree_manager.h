#if !defined(TREE_MANAGER_H)
#define TREE_MANAGER_H

#include <TTree.h>
#include <TObject.h>
#include <TString.h>

#include "digit.h"

class TreeManager {
public:
    TreeManager();
    ~TreeManager();

    void add_class_name(TTree& tree, const char* pattern, const char* class_name) const;

    Bool_t make_branch(TTree& tree, const char* storeClassName,
                      const char* branchClassName, const char* branchName, 
                      void *address, int bufferSize = 4000, int splitLevel = 99) const;

    // template <typename T>
    Bool_t make_branch(TTree& tree, const char* branchName, const char *store_class_name,
                       std::vector<Digit> *address, int buffer_size = 4000, int split_level = 99) const;
  
    Bool_t set_address(TTree& tree, const char* branchName, void* address) const;
    
    TObject* create_object(const TTree& tree, const char* detail) const;
    
    void update_branch_statuses(TTree& tree, const char* pattern) const;

private:
    TString get_class_name(const TTree& tree, const char* pattern,
                           bool makeDefault) const;
    
    TString default_class_name(const char* treename, const char* pattern) const;
};

#endif // TREE_MANAGER_H
