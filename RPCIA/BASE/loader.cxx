#include "loader.h"

#include <iostream>

Loader::Loader() { }

Loader::~Loader() {
    out_file_->Close();
    delete out_file_;
}

void Loader::open(const char* filename) {
    out_file_ = new TFile(filename, "recreate");
    // std::cout << out_file_->GetSize() << std::endl;
    // out_file_->Draw();
}

void Loader::write() {
    out_file_->Write();
}

TTree * Loader::get_tree_loader() {
    digit_tree_ = (TTree *) out_file_->Get("tree");
    std::cout << digit_tree_ << std::endl;
    if (digit_tree_)
    {
        std::cout << "Found existing tree." << std::endl;
        return digit_tree_;
    }
    std::cout << "Creating new tree." << std::endl;
    digit_tree_ = new TTree("tree", "Tree info");
    return digit_tree_;
}