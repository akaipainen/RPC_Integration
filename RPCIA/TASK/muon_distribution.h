#if !defined(MUON_DISTRIBUTION_H)
#define MUON_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

#include "summary_hist.h"

class MuonDistribution : public AnalysisTask
{
    SummaryHist<TH1F> strip_rate_;
    SummaryHist<TH1F> channel_rate_;
    
    const double& run_duration_; // Run duration in seconds

    // Change these to set size of printed canvas
    static const int w = 3;
    static const int h = 3;

public:
    MuonDistribution(const char* name, const double& input=1)
     : AnalysisTask(name, 500*w, 300*h)
     , strip_rate_("strip_rate")
     , channel_rate_("channel_rate")
     , run_duration_(input)
    { }

    ~MuonDistribution();

    void init()
    {
        strip_rate_.init(9, 32, 0, 32);
        channel_rate_.init(9, 32, 0, 32);
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (dit->muon())
            {
                strip_rate_[dit->tdc()].Fill(dit->strip(), 1.0/run_duration_);
                channel_rate_[dit->tdc()].Fill(dit->channel(), 1.0/run_duration_);
            }
        }
    }

    void terminate()
    {
        strip_rate_.configure("XNUMS");
        channel_rate_.configure("XNUMS");

        strip_rate_.configure_titles("Strip", "Muon Hits [Hz]");
        channel_rate_.configure_titles("Channel", "Muon Hits [Hz]");

        strip_rate_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_rate.pdf", outdir_, name_));
        canvas_->Clear();

        channel_rate_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_rate.pdf", outdir_, name_));
        canvas_->Clear();
    }
};


#endif // MUON_DISTRIBUTION_H
