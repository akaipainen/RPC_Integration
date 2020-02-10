#if !defined(HISTOGRAM_BUILDER_HPP)
#define HISTOGRAM_BUILDER_HPP

#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

class HistogramBuilder {
private:
    TFile *root_file;
    TTree *events_tree;

    std::vector<TH1 *> histograms;

    int num_hist;

public:
    // Constructor
    HistogramBuilder(TFile *root_file, std::string tree_name);

    // Add a histogram to the file
    void add_hist(TH1F * (* histogram_callback)(TTree *tree));

    // Adds multiple histograms to the file
    void add_hist(std::vector<TH1F *> (* histogram_callback)(TTree *tree));

    // Closes the histogram builder
    void close();

};

#endif // HISTOGRAM_BUILDER_HPP
