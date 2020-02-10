#include "histogram_builder.h"

HistogramBuilder::HistogramBuilder(TFile *root_file, std::string tree_name)
 : root_file(root_file), num_hist(0) {
    events_tree = (TTree *) root_file->Get(tree_name.c_str());
}

void HistogramBuilder::add_hist(TH1F * (* histogram_callback)(TTree *tree)) {
    histograms.push_back(histogram_callback(events_tree));
    events_tree->ResetBranchAddresses();
}

void HistogramBuilder::add_hist(std::vector<TH1F *> (* histogram_callback)(TTree *tree)) {
    std::vector<TH1F *> hists = histogram_callback(events_tree);
    for (auto hist : hists) {
        histograms.push_back(hist);
    }
    events_tree->ResetBranchAddresses();
}

void HistogramBuilder::close() {
    for (int i = 0; i < histograms.size(); i++) {
        histograms[i]->Write();
    }
}