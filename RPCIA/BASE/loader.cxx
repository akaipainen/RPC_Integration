#include "loader.h"

Loader::Loader() { }

Loader::~Loader() {
    delete out_file_;
}

void Loader::open(const char* filename) {
    try {
        out_file_ = new TFile(filename, "recreate");
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void Loader::write() {
    out_file_->Write();
}

TTree * Loader::get_tree_loader() {
    digit_tree_ = new TTree("tree", "Tree info");
    return digit_tree_;
}