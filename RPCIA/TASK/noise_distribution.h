#if !defined(NOISE_DISTRIBUTION_H)
#define NOISE_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TCanvas.h>
#include <TH1.h>

class NoiseDistribution : public AnalysisTask
{
    SummaryHist<TH1F> strip_rate_;
    SummaryHist<TH1F> channel_rate_;
    
    int num_events_; // Number of events

    // Change these to set size of printed canvas
    static const int w = 3;
    static const int h = 3;

public:
    NoiseDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
     , strip_rate_("strip_rate")
     , channel_rate_("channel_rate")
     , num_events_(0)
    { }

    ~NoiseDistribution();

    void init()
    {
        strip_rate_.init(9, 32, 0, 32);
        channel_rate_.init(9, 32, 0, 32);
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (!dit->muon())
            {
                strip_rate_[dit->tdc()].Fill(dit->strip());
                channel_rate_[dit->tdc()].Fill(dit->channel());
            }
        }
        num_events_++;
    }

    void terminate()
    {
        strip_rate_.configure("XNUMS");
        channel_rate_.configure("XNUMS");

        strip_rate_.for_each(&TH1F::Scale, 1/(num_events_*0.0016), "");
        channel_rate_.for_each(&TH1F::Scale, 1/(num_events_*0.0016), "");

        strip_rate_.configure_titles("Strip", "Noise Hits [KHz]");
        channel_rate_.configure_titles("Channel", "Noise Hits [KHz]");

        strip_rate_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_rate.pdf", outdir_, name_));
        canvas_->Clear();

        channel_rate_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_rate.pdf", outdir_, name_));
        canvas_->Clear();
    }
};


#endif // NOISE_DISTRIBUTION_H
