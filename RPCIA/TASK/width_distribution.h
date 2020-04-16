#if !defined(WIDTH_DISTRIBUTION_H)
#define WIDTH_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH2.h>
#include <TProfile.h>

#include "tdc.h"
#include "summary_hist.h"

class WidthDistribution : public AnalysisTask
{
    // All hits
    SummaryHist<TH2F> strip_all_;
    SummaryHist<TH2F> channel_all_;

    // Muon hits
    SummaryHist<TH2F> strip_muon_;
    SummaryHist<TH2F> channel_muon_;

    // Noise hits
    SummaryHist<TH2F> strip_noise_;
    SummaryHist<TH2F> channel_noise_;

    // Cluster size
    SummaryHist<TH2F> cluster_size_;

    // Number of hits
    SummaryHist<TH2F> num_hits_;

    static const int w = 3;
    static const int h = 3;

public:
    WidthDistribution(const char* name)
     : AnalysisTask(name, 500*w, 300*h)
     , strip_all_("strip_all")
     , channel_all_("channel_all")
     , strip_muon_("strip_muon")
     , channel_muon_("channel_muon")
     , strip_noise_("strip_noise")
     , channel_noise_("channel_noise")
     , cluster_size_("cluster_size")
     , num_hits_("num_hits")
    { }

    ~WidthDistribution();

    void init()
    {
        strip_all_.init(9, 32, 0, 32, 128, 0, 128);
        channel_all_.init(9, 32, 0, 32, 128, 0, 128);
        strip_muon_.init(9, 32, 0, 32, 128, 0, 128);
        channel_muon_.init(9, 32, 0, 32, 128, 0, 128);
        strip_noise_.init(9, 32, 0, 32, 128, 0, 128);
        channel_noise_.init(9, 32, 0, 32, 128, 0, 128);
        cluster_size_.init(9, 32, 0, 32, 128, 0, 128);
        num_hits_.init(9, 32, 0, 32, 128, 0, 128);
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            // all
            strip_all_[dit->tdc()].Fill(dit->strip(), dit->width());
            channel_all_[dit->tdc()].Fill(dit->channel(), dit->width());
            if (dit->muon()) // muon
            {
                strip_muon_[dit->tdc()].Fill(dit->strip(), dit->width());
                channel_muon_[dit->tdc()].Fill(dit->channel(), dit->width());
            }
            else // noise
            {
                strip_noise_[dit->tdc()].Fill(dit->strip(), dit->width());
                channel_noise_[dit->tdc()].Fill(dit->channel(), dit->width());
            }
            // num hits
            num_hits_[dit->tdc()].Fill(digit_store_->size(), dit->width());
        }
        for (auto cit = cluster_store_->begin(); cit != cluster_store_->end(); cit++)
        {
            for (auto dit = cit->begin_digits(); dit != cit->end_digits(); dit++)
            {
                cluster_size_[dit->tdc()].Fill(cit->num_digits(), dit->width());
            }
            
        }
    }

    void terminate()
    {
        strip_all_.configure("XNUMS COLZ");
        channel_all_.configure("XNUMS COLZ");
        strip_muon_.configure("XNUMS COLZ");
        channel_muon_.configure("XNUMS COLZ");
        strip_noise_.configure("XNUMS COLZ");
        channel_noise_.configure("XNUMS COLZ");
        cluster_size_.configure("SEP COLZ");
        num_hits_.configure("SEP COLZ");

        strip_all_.configure_titles("Strip", "Width");
        channel_all_.configure_titles("Strip", "Width");
        strip_muon_.configure_titles("Strip", "Width");
        channel_muon_.configure_titles("Strip", "Width");
        strip_noise_.configure_titles("Strip", "Width");
        channel_noise_.configure_titles("Strip", "Width");
        cluster_size_.configure_titles("Cluster Size", "Width");
        num_hits_.configure_titles("Number of Hits", "Width");

        strip_all_.draw(canvas_);
        canvas_->Print(Form("%s/%s/strip_all.pdf", outdir_, name_));
        canvas_->Clear();

        channel_all_.draw(canvas_);
        canvas_->Print(Form("%s/%s/channel_all.pdf", outdir_, name_));
        canvas_->Clear();

        strip_muon_.draw(canvas_);
        canvas_->Print(Form("%s/%s/strip_muon.pdf", outdir_, name_));
        canvas_->Clear();

        channel_muon_.draw(canvas_);
        canvas_->Print(Form("%s/%s/channel_muon.pdf", outdir_, name_));
        canvas_->Clear();

        strip_noise_.draw(canvas_);
        canvas_->Print(Form("%s/%s/strip_noise.pdf", outdir_, name_));
        canvas_->Clear();

        channel_noise_.draw(canvas_);
        canvas_->Print(Form("%s/%s/channel_noise.pdf", outdir_, name_));
        canvas_->Clear();

        cluster_size_.draw(canvas_);
        canvas_->Print(Form("%s/%s/cluster_size.pdf", outdir_, name_));
        canvas_->Clear();

        num_hits_.draw(canvas_);
        canvas_->Print(Form("%s/%s/num_hits.pdf", outdir_, name_));
        canvas_->Clear();
    }
};


#endif // WIDTH_DISTRIBUTION_H
