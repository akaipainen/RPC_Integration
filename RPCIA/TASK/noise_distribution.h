#if !defined(NOISE_DISTRIBUTION_H)
#define NOISE_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TCanvas.h>
#include <TH1.h>

class NoiseDistribution : public AnalysisTask
{
    std::vector<TH1F *> tdc_strip_rate_;
    std::vector<TH1F *> tdc_channel_rate_;
    
    int num_events_; // Number of events

    // Change these to set size of printed canvas
    static const int w = 2;
    static const int h = 1;

public:
    NoiseDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
     , num_events_(0)
    { }

    ~NoiseDistribution();

    void init()
    {
        gDirectory->mkdir("strip");
        gDirectory->cd("strip");
        gDirectory->mkdir("count");
        gDirectory->mkdir("rate");
        for (int tdc = 0; tdc < 18; tdc++)
        {
            gDirectory->cd("rate");
            tdc_strip_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Noise Rate by Strip (tdc = %d)", tdc),
                                          32, 0, 32));
            tdc_strip_rate_.back()->GetXaxis()->SetTitle("Strip");
            tdc_strip_rate_.back()->GetYaxis()->SetTitle("Noise Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");

        gDirectory->mkdir("channel");
        gDirectory->cd("channel");
        gDirectory->mkdir("count");
        gDirectory->mkdir("rate");
        for (int tdc = 0; tdc < 18; tdc++)
        {

            gDirectory->cd("rate");
            tdc_channel_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Noise Rate by Channel (tdc = %d)", tdc),
                                            32, 0, 32));
            tdc_channel_rate_.back()->GetXaxis()->SetTitle("TDC Channel");
            tdc_channel_rate_.back()->GetYaxis()->SetTitle("Noise Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (!dit->muon())
            {
                tdc_strip_rate_[dit->tdc()]->Fill(dit->strip());
                tdc_channel_rate_[dit->tdc()]->Fill(dit->channel());
            }
        }
        num_events_++;
    }

    void terminate()
    {
        canvas_->Divide(2, 1);

        printf("Number of events: %d", num_events_);

        gStyle->SetOptStat(11);
        for (int tdc = 0; tdc < 18; tdc++)
        {
            canvas_->cd(1);
            tdc_strip_rate_[tdc]->Scale(1/(0.0000016*num_events_));
            tdc_strip_rate_[tdc]->Draw();

            canvas_->cd(2);
            tdc_channel_rate_[tdc]->Scale(1/(0.0000016*num_events_));
            tdc_channel_rate_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // NOISE_DISTRIBUTION_H
