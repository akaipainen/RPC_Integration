#if !defined(MUON_DISTRIBUTION_H)
#define MUON_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

class MuonDistribution : public AnalysisTask
{
    std::vector<TH1F *> tdc_strip_rate_;
    std::vector<TH1F *> tdc_channel_rate_;
    
    const double& run_duration_; // Run duration in seconds

    // Change these to set size of printed canvas
    static const int w = 2;
    static const int h = 1;

public:
    MuonDistribution(const char* name, const double& input=1)
     : AnalysisTask(name, 500*w, 300*h)
     , run_duration_(input)
    { }

    ~MuonDistribution();

    void init()
    {
        gDirectory->mkdir("strip");
        gDirectory->cd("strip");
        for (int tdc = 0; tdc < 18; tdc++)
        {
            gDirectory->cd("rate");
            tdc_strip_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Muon Rate by Strip (tdc = %d)", tdc),
                                          32, 0, 32));
            tdc_strip_rate_.back()->GetXaxis()->SetTitle("Strip");
            tdc_strip_rate_.back()->GetYaxis()->SetTitle("Muon Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");

        gDirectory->mkdir("channel");
        gDirectory->cd("channel");
        for (int tdc = 0; tdc < 18; tdc++)
        {

            gDirectory->cd("rate");
            tdc_channel_rate_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Muon Rate by Channel (tdc = %d)", tdc),
                                            32, 0, 32));
            tdc_channel_rate_.back()->GetXaxis()->SetTitle("TDC Channel");
            tdc_channel_rate_.back()->GetYaxis()->SetTitle("Muon Rate [Hz]");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (dit->muon())
            {
                tdc_strip_rate_[dit->tdc()]->Fill(dit->strip(), 1.0/run_duration_);
                tdc_channel_rate_[dit->tdc()]->Fill(dit->channel(), 1.0/run_duration_);
            }
        }
    }

    void terminate()
    {
        canvas_->Divide(2, 1);

        gStyle->SetOptStat(11);
        for (int tdc = 0; tdc < 18; tdc++)
        {
            canvas_->cd(1);
            tdc_strip_rate_[tdc]->Draw();

            canvas_->cd(2);
            tdc_channel_rate_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // MUON_DISTRIBUTION_H
