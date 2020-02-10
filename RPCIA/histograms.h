#include <vector>
#include <iostream>
#include <string>

#include <TTree.h>
#include <TH1.h>
#include <TClonesArray.h>

#include "event.h"
#include "hit.h"

namespace histogram 
{

TH1F * test(TTree *tree) {
    TH1F *source = new TH1F("source", "source hist",100,-3,3);
    source->FillRandom("gaus",1000);
    TH1F *final = new TH1F("final","final hist",100,-3,3);

    for (Int_t i=0;i<10000;i++) {
        final->Fill(source->GetRandom());
    }
    return final;
} 

std::vector<TH1F *> noise_rate_per_strip(TTree *tree) {
    const double timewindow = 0.0000016; // trigger event time window
    std::vector<TH1F *> hist;
    for (int i = 0; i < 3; ++i) {
        hist.push_back(new TH1F(utils::sprintf("tdc%d_strip_noise_rate", i).c_str(), 
                                utils::sprintf("TDC %d noise rate per strip", i).c_str(), 
                                32, 0, 32));
    }

    Event *event = 0;
    tree->SetBranchAddress("events", &event);

    int num_events = tree->GetEntries();
    int total_hits = 0;

    for (int i = 0; i < num_events; i++) {
        tree->GetEntry(i);
        int num_hits = event->get_num_hits();
        for (int j = 0; j < num_hits; j++) {
            Hit *hit = event->get_hit(j);
            hist[hit->get_tdc()]->Fill(hit->get_strip(), hit->get_noise() / (timewindow * num_events));
        }
    }

    // Format the histogram

    for (int i = 0; i < 3; i++) {
        hist[i]->SetStats(0);
        hist[i]->SetOption("hist bar");
        hist[i]->SetBarWidth(0.65);
        hist[i]->SetFillColor(0);
        hist[i]->SetLineColor(1);
        hist[i]->GetXaxis()->SetTitle("Strip Number (0-31)");
        hist[i]->GetYaxis()->SetTitle("Noise rate (Hz)");
        hist[i]->GetXaxis()->SetNdivisions(31);
    }
    
    return hist;
}

}