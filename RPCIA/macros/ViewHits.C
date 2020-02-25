#include <stdio.h>


void ViewHits() {
    TFile *f = TFile::Open("../build/2test.root");
    if (f == 0) {
        printf("Error opening file\n");
        return;
    }

    auto c = new TCanvas();

    TTree *events = (TTree *) f->Get("events");

    events->SetMarkerStyle(8);
    events->SetMarkerColor(1);
    events->Draw("tdc:strip", "id == 2100 && noise");
    events->SetMarkerColor(2);
    events->Draw("tdc:strip", "id == 2100 && !noise");
}