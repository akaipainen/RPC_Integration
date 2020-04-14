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
    static const int w = 3;
    static const int h = 3;

public:
    NoiseDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
     , num_events_(0)
    { }

    ~NoiseDistribution();

    void set_style(TH1* hist)
    {
        // Customize style
        hist->SetFillColor(16); // Bar color
        hist->SetLineColor(kBlack); // Error bar color
        hist->SetBarWidth(0.8); // Set bar width
        hist->SetBarOffset(0.1); 

        for (int i = 0; i < 32; i++)
        {
            hist->GetXaxis()->SetBinLabel(i+1, Form("%d", i));
        }
    }

    void init()
    {
        // Create histograms
        for (int tdc = 0; tdc < 18; tdc++)
        {
            tdc_strip_rate_.push_back(
                create_1d_histogram(Form("strip/rate/tdc_%d", tdc),
                                    Form("Noise Rate by Strip (tdc = %d)", tdc),
                                    "Strip",
                                    "Noise Rate [Hz]",
                                    0, 32, 1)
            );
            set_style(tdc_strip_rate_.back());

            tdc_channel_rate_.push_back(
                create_1d_histogram(Form("channel/rate/tdc_%d", tdc),
                                    Form("Noise Rate by Channel (tdc = %d)", tdc),
                                    "Channel",
                                    "Noise Rate [Hz]",
                                    0, 32, 1)
            );
            set_style(tdc_channel_rate_.back());
        }
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
        canvas_->Divide(3, 3);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_strip_rate_[tdc]->Draw("BAR");
        }
        canvas_->Print(Form("%s/%s/strip_rate.pdf", outdir_, name_));
        canvas_->Clear("D");

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_channel_rate_[tdc]->Draw("BAR");
        }
        canvas_->Print(Form("%s/%s/channel_rate.pdf", outdir_, name_));
        canvas_->Clear("D");

    }
};


#endif // NOISE_DISTRIBUTION_H
