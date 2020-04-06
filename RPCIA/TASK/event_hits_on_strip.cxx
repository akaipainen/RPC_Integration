#include "event_hits_on_strip.h"

EventHitsOnStrip::EventHitsOnStrip(/* args */)
{
    gDirectory->mkdir("event_hits_on_strip");
    gDirectory->cd("event_hits_on_strip");
    
    create_outputs();

    gDirectory->cd("..");
}

EventHitsOnStrip::~EventHitsOnStrip()
{
}

void EventHitsOnStrip::create_outputs()
{
    canvas_ = new TCanvas("event_hits_on_strip", "canvas", 1000, 600);
    canvas_->Divide(1, 1);

    gDirectory->mkdir("tdc");
    gDirectory->cd("tdc");
    
    for (int tdc = 0; tdc < 9; tdc++)
    {
        tdc_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Number of hits in an event (tdc = %d)", tdc),
                                32, 0, 32, 10, 0, 10));
        tdc_.back()->SetOption("COLZ");
        tdc_.back()->GetXaxis()->SetTitle("Strip");
        tdc_.back()->GetYaxis()->SetTitle("Count");
    }

    gDirectory->cd("..");
}

void EventHitsOnStrip::execute()
{
    std::unordered_map<int, int> counts;
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        counts[dit->tdc() * 32 + dit->strip()] += 1;
    }

    for (auto it = counts.begin(); it != counts.end(); it++)
    {
        tdc_[it->first / 32]->Fill(it->first % 32, it->second);
    }
}

void EventHitsOnStrip::terminate()
{
    canvas_->cd();
    for (int tdc = 0; tdc < 9; tdc++)
    {
        canvas_->cd(1);
        tdc_[tdc]->Draw();
        // gPad->SetLogz();

        // canvas_->cd(2);
        // profile_tdc_[tdc]->Draw();

        canvas_->Print(Form("event_hits_on_strip_tdc_%d.pdf", tdc));
        canvas_->Clear("D");
    }
    canvas_->Close();
}