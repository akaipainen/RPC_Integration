#include <vector>
#include <iostream>
#include <string>

#include <TTree.h>
#include <TH1.h>
#include <THStack.h>
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

THStack * noise_to_muon_width(TTree *tree) {
    THStack *compare_widths = new THStack("compare_noise_to_muon_width", "Compare widths of noise to muon hits");
    TH1F *noise_widths = new TH1F("noise", "Noise widths", 128, 0, 128);
    noise_widths->SetFillColor(kBlack);
    TH1F *muon_widths = new TH1F("muon", "Muon widths", 128, 0, 128);
    muon_widths->SetFillColor(kRed);

    Event *event = 0;
    tree->SetBranchAddress("events", &event);

    int num_events = tree->GetEntries();
    int total_hits = 0;

    for (int i = 0; i < num_events; i++) {
        tree->GetEntry(i);
        int num_hits = event->get_num_hits();
        for (int j = 0; j < num_hits; j++) {
            Hit *hit = event->get_hit(j);
            if (hit->get_noise()) {
                noise_widths->Fill(hit->get_width());
            } else {
                muon_widths->Fill(hit->get_width());
            }
        }
    }

    compare_widths->Add(noise_widths);
    compare_widths->Add(muon_widths);
    return compare_widths;
}

TH1F * fpga_tdc_bcid_latency(TTree *tree) {
    TH1F *hist = new TH1F("fpga_tdc_bcid_latency", "Latency between the HPTDC BCID and the FPGA BCID", 100, 0, 128);
    
    Event *event = 0;
    tree->SetBranchAddress("events", &event);

    int num_events = tree->GetEntries();
    int total_hits = 0;

    for (int i = 0; i < num_events; i++) {
        tree->GetEntry(i);
        int num_hits = event->get_num_hits();
        for (int j = 0; j < num_hits; j++) {
            Hit *hit = event->get_hit(j);
            if (!hit->get_noise()) {
                hist->Fill(hit->get_bcid_fpga() - hit->get_bcid_tdc());
            }
        }
    }
    return hist;
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
        // hist[i]->SetOption("bar");
        // hist[i]->SetBarWidth(0.65);
        hist[i]->SetOption("E1");
        hist[i]->SetFillColor(0);
        hist[i]->SetLineColor(1);
        hist[i]->GetXaxis()->SetTitle("Strip Number (0-31)");
        hist[i]->GetYaxis()->SetTitle("Noise rate (Hz)");
        hist[i]->GetXaxis()->SetNdivisions(32);
    }
    
    return hist;
}

}