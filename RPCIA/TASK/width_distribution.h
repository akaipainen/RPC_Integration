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
    static const int w = 3;
    static const int h = 3;

public:
    WidthDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
    { }

    ~WidthDistribution();

    void init()
    {
        for (int tdc = 0; tdc < 18; tdc++)
        {
            tdc_color_strip_.push_back(
                create_2d_histogram(Form("strip/color/tdc_%d", tdc),
                                    Form("Width vs. Strip (tdc = %d)", tdc),
                                    "Strip",
                                    "Width [ns]",
                                    0, 32, 1, 
                                    0, 128, 1)
            );
            tdc_color_strip_.back()->SetOption("COLZ");

            tdc_profile_strip_.push_back(
                create_2d_profile(Form("strip/profile/tdc_%d", tdc),
                                  Form("Width vs. Strip (tdc = %d)", tdc),
                                  "Strip",
                                  "Width [ns]",
                                  0, 32, 1, 
                                  0, 128)
            );

            tdc_color_channel_.push_back(
                create_2d_histogram(Form("channel/color/tdc_%d", tdc),
                                    Form("Width vs. Channel (tdc = %d)", tdc),
                                    "Channel",
                                    "Width [ns]",
                                    0, 32, 1, 
                                    0, 128, 1)
            );
            tdc_color_channel_.back()->SetOption("COLZ");

            tdc_profile_channel_.push_back(
                create_2d_profile(Form("channel/profile/tdc_%d", tdc),
                                  Form("Width vs. Channel (tdc = %d)", tdc),
                                  "Channel",
                                  "Width [ns]",
                                  0, 32, 1, 
                                  0, 128)
            );
        }
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
        canvas_->Divide(3, 3);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_color_strip_[tdc]->Draw();
        }
        canvas_->Print(Form("%s/%s/strip_color.pdf", outdir_, name_));
        canvas_->Clear("D");

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_profile_strip_[tdc]->Draw();
        }
        canvas_->Print(Form("%s/%s/strip_profile.pdf", outdir_, name_));
        canvas_->Clear("D");

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_color_channel_[tdc]->Draw();
        }
        canvas_->Print(Form("%s/%s/channel_color.pdf", outdir_, name_));
        canvas_->Clear("D");

        for (int tdc = 0; tdc < 9; tdc++)
        {
            cd_grid(tdc);
            tdc_profile_channel_[tdc]->Draw();
        }
        canvas_->Print(Form("%s/%s/channel_profile.pdf", outdir_, name_));
        canvas_->Clear("D");
    }
};


#endif // WIDTH_DISTRIBUTION_H
