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
    static const int w = 2;
    static const int h = 2;

public:
    HitDistribution(const char* name, const double& input=1)
     : AnalysisTask(name, 500*w, 300*h)
     , run_duration_(input)
    { }

    ~HitDistribution();

    void init()
    {
        gDirectory->mkdir("strip");
        gDirectory->cd("strip");
        gDirectory->mkdir("count");
        gDirectory->mkdir("rate");
        for (int tdc = 0; tdc < 18; tdc++)
        {
            gDirectory->cd("count");
            tdc_strip_count_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Total Hit Count by Strip (tdc = %d)", tdc),
                                          32, 0, 32));
            tdc_strip_count_.back()->GetXaxis()->SetTitle("Strip");
            tdc_strip_count_.back()->GetYaxis()->SetTitle("Hit Count");
            gDirectory->cd("..");

            gDirectory->cd("rate");
            tdc_strip_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Hit Rate by Strip (tdc = %d)", tdc),
                                          32, 0, 32));
            tdc_strip_rate_.back()->GetXaxis()->SetTitle("Strip");
            tdc_strip_rate_.back()->GetYaxis()->SetTitle("Hit Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");

        gDirectory->mkdir("channel");
        gDirectory->cd("channel");
        gDirectory->mkdir("count");
        gDirectory->mkdir("rate");
        for (int tdc = 0; tdc < 18; tdc++)
        {    
            gDirectory->cd("count");
            tdc_channel_count_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Total Hit Count by Channel (tdc = %d)", tdc),
                                            32, 0, 32));
            tdc_channel_count_.back()->GetXaxis()->SetTitle("TDC Channel");
            tdc_channel_count_.back()->GetYaxis()->SetTitle("Hit Count");
            gDirectory->cd("..");

            gDirectory->cd("rate");
            tdc_channel_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Hit Rate by Channel (tdc = %d)", tdc),
                                            32, 0, 32));
            tdc_channel_rate_.back()->GetXaxis()->SetTitle("TDC Channel");
            tdc_channel_rate_.back()->GetYaxis()->SetTitle("Hit Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");
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
        canvas_->Divide(2, 2);

        gStyle->SetOptStat(11);
        for (int tdc = 0; tdc < 18; tdc++)
        {
            canvas_->cd(1);
            tdc_strip_count_[tdc]->Draw();

            canvas_->cd(2);
            tdc_strip_rate_[tdc]->Draw("E");
            
            canvas_->cd(3);
            tdc_channel_count_[tdc]->Draw();

            canvas_->cd(4);
            tdc_channel_rate_[tdc]->Draw("E");

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // HIT_DISTRIBUTION_H
