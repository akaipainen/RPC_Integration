#if !defined(WIDTH_DISTRIBUTION_H)
#define WIDTH_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH2.h>
#include <TProfile.h>

class WidthDistribution : public AnalysisTask
{
    std::vector<TH2F*> tdc_color_strip_;
    std::vector<TProfile*> tdc_profile_strip_;

    std::vector<TH2F*> tdc_color_channel_;
    std::vector<TProfile*> tdc_profile_channel_;

    // Change these to set size of printed canvas
    static const int w = 2;
    static const int h = 2;

public:
    WidthDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
    { }

    ~WidthDistribution();

    void init()
    {
        gDirectory->mkdir("strip");
        gDirectory->cd("strip");
        gDirectory->mkdir("color");
        gDirectory->mkdir("profile");
        for (int tdc = 0; tdc < 18; tdc++)
        {
            gDirectory->cd("color");
            tdc_color_strip_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Width vs. Strip (tdc = %d)", tdc),
                                                32, 0, 32, 128, 0, 128));
            tdc_color_strip_.back()->GetXaxis()->SetTitle("Strip");
            tdc_color_strip_.back()->GetYaxis()->SetTitle("Width");
            tdc_color_strip_.back()->SetOption("COLZ");
            gDirectory->cd("..");

            gDirectory->cd("profile");
            tdc_profile_strip_.push_back(new TProfile(Form("tdc_%d", tdc), Form("Width vs. Strip (tdc = %d)", tdc),
                                         32, 0, 32, 0, 128));
            tdc_profile_strip_.back()->GetXaxis()->SetTitle("Strip");
            tdc_profile_strip_.back()->GetYaxis()->SetTitle("Width");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");

        gDirectory->mkdir("channel");
        gDirectory->cd("channel");
        gDirectory->mkdir("color");
        gDirectory->mkdir("profile");
        for (int tdc = 0; tdc < 18; tdc++)
        {
            gDirectory->cd("color");
            tdc_color_channel_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Width vs. Channel (tdc = %d)", tdc),
                                                  32, 0, 32, 128, 0, 128));
            tdc_color_channel_.back()->GetXaxis()->SetTitle("Channel");
            tdc_color_channel_.back()->GetYaxis()->SetTitle("Width");
            tdc_color_channel_.back()->SetOption("COLZ");
            gDirectory->cd("..");

            gDirectory->cd("profile");
            tdc_profile_channel_.push_back(new TProfile(Form("tdc_%d", tdc), Form("Width vs. Channel (tdc = %d)", tdc),
                                                        32, 0, 32, 0, 128));
            tdc_profile_channel_.back()->GetXaxis()->SetTitle("Channel");
            tdc_profile_channel_.back()->GetYaxis()->SetTitle("Width");
            gDirectory->cd("..");
        }
        gDirectory->cd("..");
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            tdc_color_strip_[dit->tdc()]->Fill(dit->strip(), dit->width());
            tdc_profile_strip_[dit->tdc()]->Fill(dit->strip(), dit->width());

            tdc_color_channel_[dit->tdc()]->Fill(dit->channel(), dit->width());
            tdc_profile_channel_[dit->tdc()]->Fill(dit->channel(), dit->width());
        }
    }

    void terminate()
    {
        canvas_->Divide(2, 2);

        gStyle->SetOptStat(11);
        for (int tdc = 0; tdc < 18; tdc++)
        {
            canvas_->cd(1);
            tdc_color_strip_[tdc]->Draw();

            canvas_->cd(2);
            tdc_profile_strip_[tdc]->Draw();
            
            canvas_->cd(3);
            tdc_color_channel_[tdc]->Draw();

            canvas_->cd(4);
            tdc_profile_channel_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // WIDTH_DISTRIBUTION_H
