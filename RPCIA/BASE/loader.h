#if !defined(LOADER_H)
#define LOADER_H

#include <iostream>

#include <TFile.h>
#include <TTree.h>

class Loader {
private:
    TFile *out_file_;
    
    TTree *digit_tree_;

public:
    Loader();
    ~Loader();

    // Open a file for tree output
    void open(const char* filename);

    // Write trees to file
    void write();

    // Get the tree pointer to fill
    TTree * get_tree_loader();

private:
    void create_digit_loader();
};

#endif // LOADER_H
