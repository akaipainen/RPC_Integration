#if !defined(HIT_DISTRIBUTION_H)
#define HIT_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

#include "summary_hist.h"
#include "layer_hist.h"

class HitDistribution : public AnalysisTask
{
    SummaryHist<TH1F> strip_rate_all_;
    SummaryHist<TH1F> channel_rate_all_;
    
    SummaryHist<TH1F> strip_count_all_;
    SummaryHist<TH1F> channel_count_all_;
    
    SummaryHist<TH1F> strip_rate_muon_;
    SummaryHist<TH1F> channel_rate_muon_;
    
    SummaryHist<TH1F> strip_rate_noise_;
    SummaryHist<TH1F> channel_rate_noise_;

    LayerHist<TH1F> event_hits_;
    
    const double& run_duration_; // Run duration in seconds

    int num_events_;

    // Change these to set size of printed canvas
    static const int w = 3;
    static const int h = 3;

public:
    HitDistribution(const char* name, const double& input=1)
     : AnalysisTask(name, 500*w, 300*h)
     , strip_rate_all_("strip_rate_all")
     , channel_rate_all_("channel_rate_all")
     , strip_count_all_("strip_count_all")
     , channel_count_all_("channel_count_all")
     , strip_rate_muon_("strip_rate_muon")
     , channel_rate_muon_("channel_rate_muon")
     , strip_rate_noise_("strip_rate_noise")
     , channel_rate_noise_("channel_rate_noise")
     , event_hits_("event_hits")
     , run_duration_(input)
     , num_events_(0)
    { }

    ~HitDistribution();

    void init()
    {
        strip_rate_all_.init(9, 32, 0, 32);
        channel_rate_all_.init(9, 32, 0, 32);

        strip_count_all_.init(9, 32, 0, 32);
        channel_count_all_.init(9, 32, 0, 32);

        strip_rate_muon_.init(9, 32, 0, 32);
        channel_rate_muon_.init(9, 32, 0, 32);

        strip_rate_noise_.init(9, 32, 0, 32);
        channel_rate_noise_.init(9, 32, 0, 32);

        event_hits_.init(9, 128, 0, 128);
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            // all
            strip_rate_all_[dit->tdc()].Fill(dit->strip(), 1.0/run_duration_);
            channel_rate_all_[dit->tdc()].Fill(dit->channel(), 1.0/run_duration_);
            
            strip_count_all_[dit->tdc()].Fill(dit->strip());
            channel_count_all_[dit->tdc()].Fill(dit->channel());

            if (dit->muon()) // muon
            {
                strip_rate_muon_[dit->tdc()].Fill(dit->strip(), 1.0/run_duration_);
                channel_rate_muon_[dit->tdc()].Fill(dit->channel(), 1.0/run_duration_);
            }
            else { // noise
                strip_rate_noise_[dit->tdc()].Fill(dit->strip());
                channel_rate_noise_[dit->tdc()].Fill(dit->channel());
            }
        }
        event_hits_[3*(1-Detector::tdc_direction(digit_store_->begin()->tdc()))+Detector::tdc_layer(digit_store_->begin()->tdc())].Fill(digit_store_->size());
        num_events_++;
    }

    void terminate()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        strip_rate_all_.configure("XNUMS");
        channel_rate_all_.configure("XNUMS");
        strip_count_all_.configure("XNUMS");
        channel_count_all_.configure("XNUMS");
        strip_rate_muon_.configure("XNUMS");
        channel_rate_muon_.configure("XNUMS");
        strip_rate_noise_.configure("XNUMS");
        channel_rate_noise_.configure("XNUMS");

        strip_rate_all_.configure_titles("Strip", "All Hits [Hz]");
        channel_rate_all_.configure_titles("Channel", "All Hits [Hz]");
        strip_count_all_.configure_titles("Strip", "All Hits [Count]");
        channel_count_all_.configure_titles("Channel", "All Hits [Count]");
        strip_rate_muon_.configure_titles("Strip", "Muon Hits [Hz]");
        channel_rate_muon_.configure_titles("Channel", "Muon Hits [Hz]");
        strip_rate_noise_.configure_titles("Strip", "Noise Hits [KHz]");
        channel_rate_noise_.configure_titles("Channel", "Noise Hits [KHz]");

        event_hits_.configure_titles("Number of hits in event", "Count");

        // Scale noise counts to rates
        strip_rate_noise_.for_each(&TH1F::Scale, 1/(num_events_*0.0016), "");
        channel_rate_noise_.for_each(&TH1F::Scale, 1/(num_events_*0.0016), "");

        // All hits
        strip_rate_all_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_rate_all.pdf", outdir_, name_));
        canvas_->Clear();

        channel_rate_all_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_rate_all.pdf", outdir_, name_));
        canvas_->Clear();

        strip_count_all_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_count_all.pdf", outdir_, name_));
        canvas_->Clear();

        channel_count_all_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_count_all.pdf", outdir_, name_));
        canvas_->Clear();
        
        // Muon hits
        strip_rate_muon_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_rate_muon.pdf", outdir_, name_));
        canvas_->Clear();

        channel_rate_muon_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_rate_muon.pdf", outdir_, name_));
        canvas_->Clear();

        // Noise hits
        strip_rate_noise_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/strip_rate_noise.pdf", outdir_, name_));
        canvas_->Clear();

        channel_rate_noise_.draw(canvas_, 0, "BAR E0");
        canvas_->Print(Form("%s/%s/channel_rate_noise.pdf", outdir_, name_));
        canvas_->Clear();

        // Event hits
        event_hits_.draw(canvas_, 0, "", true);
        canvas_->Print(Form("%s/%s/event_hits.pdf", outdir_, name_));
        canvas_->Clear();
    }
};


#endif // HIT_DISTRIBUTION_H
