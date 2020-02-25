#include <vector>

#include "event.hpp"

void NoiseRate() {
    std::vector<TH1F *> hist(3, new TH1F("noise", "noise rate", 100, -3, 3));

    TFile *f = TFile::Open("../build/2test.root");
    if (f == 0) {
        printf("Error opening file\n");
        return;
    }
    TTree *tree = (TTree *) f->Get("events");

    Event *event;
    int num_entries;

    tree->SetBranchAddress("events.hits", &event);
    num_entries = tree->GetEntries();

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 1000); 
    c1->Divide(1, 3);

    for (Int_t i = 0; i < num_entries; ++i) {
        tree->GetEntry(i);
        hist[hit.get_tdc()]->Fill(event.get_strip(), hit;
    }

    for (int i = 0; i < hist.size(); i++) {
        hist[i]->Scale()
        hist[i]->Draw();
    }
}