#include "tree_manager.h"

#include <TTree.h>
#include <TBranch.h>
#include <TString.h>

#include <iostream>

#include "object_pair.h"
#include "digit.h"

TreeManager::TreeManager() { }

TreeManager::~TreeManager() { }

void TreeManager::add_class_name(TTree& tree, const char* pattern, const char* class_name) const {
    // If class name is not already in tree, add it
    TString test(get_class_name(tree, pattern, false));
    if (test.Length() == 0) { 
        TList *user_info = tree.GetUserInfo();
        user_info->Add(new ObjectPair(new TObjString(pattern),
                                      new TObjString(class_name),
                                      kTRUE, kTRUE));
    }
}

bool TreeManager::make_branch(TTree& tree, 
                              const char* store_class_name,
                              const char* branch_class_name,
                              const char* branch_name, 
                              void* address,
                              int buffer_size, 
                              int split_level) const {
    add_class_name(tree, branch_name, store_class_name);
    TBranch *branch = tree.Branch(branch_name, branch_class_name, address, buffer_size, split_level);
    return (branch != nullptr);
}

// template <typename T>
Bool_t TreeManager::make_branch(TTree& tree, const char* branch_name, const char *store_class_name,
                                std::vector<Digit> *address, int buffer_size, int split_level) const {
    add_class_name(tree, branch_name, store_class_name);
    TBranch *branch = tree.Branch(branch_name, address, buffer_size, split_level);
}

// template Bool_t TreeManager::make_branch<Digit>(TTree&, const char*, const char*, std::vector<Digit>, int, int) const;

bool TreeManager::set_address(TTree& tree, const char* branchName, 
                                    void* address) const {
    TBranch* branch = tree.GetBranch(branchName);
    if (branch) {
        branch->SetAddress(address);
        return kTRUE;
    }
    return kFALSE;
}

TObject * TreeManager::create_object(const TTree& tree, const char* detail) const {
    TString class_name(get_class_name(tree, detail, true));
    TClass *class_ptr = TClass::GetClass(class_name.Data());
    if (!class_ptr) {
        return 0x0;
    }
    return reinterpret_cast<TObject*>(class_ptr->New());
}

void TreeManager::update_branch_statuses(TTree& tree, const char* pattern) const {
    /// Loop over tree branches and set their status if their name matches
    /// the pattern : 
    /// - zero if branch address is null and 
    /// - one otherwise
    /// This will avoid memory leak, if we set the address to only part
    /// of the branches before doing a tree.GetEvent(i)
    ///
    /// WARNING : this is a time consuming operation, as we loop over branches
    /// at least twice (one here, and the TTree::SetBranchStatus is itself
    /// a loop). So use only when necessary.
    ///
    
    TIter next(tree.GetListOfBranches());
    TBranch* branch;
    
    while ((branch = static_cast<TBranch*>(next()))) {
        TString bname(branch->GetName());
        if (bname.Contains(pattern)) {
            tree.SetBranchStatus(Form("%s*",branch->GetName()),1);
        }
        else {
            if (!branch->GetAddress()) {
                tree.SetBranchStatus(Form("%s*",branch->GetName()),0);
            }
        }
    }
}


TString TreeManager::get_class_name(const TTree& tree, const char* pattern,
                                  bool make_default) const {
    /// Find out, using the TTree::UserInfo, the classname corresponding to 
    /// pattern.
    /// If makeDefault=true and we cannot find the pattern in the UserInfo,
    /// we return DefaultClassName(pattern)
    ///
    
    TTree& vtree = const_cast<TTree&>(tree); // not pretty, but the GetUserInfo is not const...
    
    TIter next(vtree.GetUserInfo());
    
    TObject* object;
    
    while ((object = next())) {
        ObjectPair* pair = static_cast<ObjectPair*>(object);
        TString key = (static_cast<TObjString*>(pair->First()))->String();
        TString value = (static_cast<TObjString*>(pair->Second()))->String();
        if (key.Contains(pattern, TString::kIgnoreCase)) {
            return value;
        }
    }
    
    if (make_default) return default_class_name(tree.GetName(), pattern);
    
    return "";
}

TString TreeManager::default_class_name(const char* tree_name, const char* pattern) const {
    /// For backward compatibility only. Decides, based on the tree name and a 
    /// pattern, which store class should be used.
    
    TString name(tree_name);
    TString spattern(pattern);
    spattern.ToUpper();

    if ( spattern.Contains("DIGIT") ) return "DigitStore";
    if ( spattern.Contains("TRIGGER" ) ) return "TriggerTrackStore";
    if ( spattern.Contains("TRACK") ) return "TrackStore";
    if ( spattern.Contains("CLUSTER") ) return "ClusterStore";
  
    return "";
}