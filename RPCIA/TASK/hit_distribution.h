#if !defined(HIT_DISTRIBUTION_H)
#define HIT_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

class HitDistribution : public AnalysisTask
{
    std::vector<TH1F *> tdc_strip_rate_;
    std::vector<TH1F *> tdc_channel_rate_;
    
    std::vector<TH1F *> tdc_strip_count_;
    std::vector<TH1F *> tdc_channel_count_;
    
    const double& run_duration_; // Run duration in seconds

    // Change these to set size of printed canvas
    static const int w = 3;
    static const int h = 3;

public:
    HitDistribution(const char* name, const double& input=1)
     : AnalysisTask(name, 500*w, 300*h)
     , run_duration_(input)
    { }

    ~HitDistribution();

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
            tdc_strip_count_.push_back(
                create_1d_histogram(Form("strip/count/tdc_%d", tdc),
                                    Form("Total Hit Count by Strip (tdc = %d)", tdc),
                                    "Strip",
                                    "Hit Count",
                                    0, 32, 1)
            );
            set_style(tdc_strip_count_.back());
                
            tdc_strip_rate_.push_back(
                create_1d_histogram(Form("strip/rate/tdc_%d", tdc),
                                    Form("Hit Rate by Strip (tdc = %d)", tdc),
                                    "Strip",
                                    "Hit Rate [Hz]",
                                    0, 32, 1)
            );
            set_style(tdc_strip_rate_.back());

            tdc_channel_count_.push_back(
                create_1d_histogram(Form("channel/count/tdc_%d", tdc),
                                    Form("Total Hit Count by Channel (tdc = %d)", tdc),
                                    "Channel",
                                    "Hit Count",
                                    0, 32, 1)
            );
            set_style(tdc_channel_count_.back());
                
            tdc_channel_rate_.push_back(
                create_1d_histogram(Form("channel/rate/tdc_%d", tdc),
                                    Form("Hit Rate by Channel (tdc = %d)", tdc),
                                    "Channel",
                                    "Hit Rate [Hz]",
                                    0, 32, 1)
            );
            set_style(tdc_channel_rate_.back());
        }
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            tdc_strip_count_[dit->tdc()]->Fill(dit->strip());
            tdc_strip_rate_[dit->tdc()]->Fill(dit->strip(), 1.0/run_duration_);

            tdc_channel_count_[dit->tdc()]->Fill(dit->channel());
            tdc_channel_rate_[dit->tdc()]->Fill(dit->channel(), 1.0/run_duration_);
        }
    }

    void terminate()
    {
        canvas_->Divide(3, 3);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_strip_count_[tdc]->Draw("BAR");
        }
        canvas_->Print(Form("%s/%s/strip_count.pdf", outdir_, name_));
        canvas_->Clear("D");
        
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
            tdc_channel_count_[tdc]->Draw("BAR");
        }
        canvas_->Print(Form("%s/%s/channel_count.pdf", outdir_, name_));
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


#endif // HIT_DISTRIBUTION_H
