#if !defined(EVENT_DISPLAY_H)
#define EVENT_DISPLAY_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TH2.h>

#include "tdc.h"
#include "summary_hist.h"

class EventDisplay : public AnalysisTask
{
    std::vector<int> *input_;
    SummaryHist<TH2F> muon_;
    SummaryHist<TH2F> noise_;
    int counter_;

    int muon_hits_;
    int noise_hits_;

    std::vector<int> muon_hits_tdc_;
    std::vector<int> noise_hits_tdc_;

public:
    EventDisplay(const char *name, std::vector<int> *input=nullptr)
     : AnalysisTask(name, 500, 300)
     , input_(input)
     , muon_("muon")
     , noise_("noise")
     , counter_(0)
     , muon_hits_(0)
     , noise_hits_(0)
    { }

    ~EventDisplay()
    { }

    void init()
    {
        muon_.init(9, 32, 0, 32, 50/25*128, 0, 50);
        noise_.init(9, 32, 0, 32, 50/25*128, 0, 50);

        muon_.configure("XNUMS");
        noise_.configure("XNUMS");

        muon_.configure_titles("Strip", "Time [ns]");
        noise_.configure_titles("Strip", "Time [ns]");

        muon_.for_each(&TH2F::SetFillColor, short(kRed));
        noise_.for_each(&TH2F::SetFillColor, short(kBlue));
    }

    void execute()
    {
        int trigger_id = digit_store_->begin()->trigger_id();
        
        // Get first time
        double first_time = TDC::combined_time_ns(digit_store_->begin()->bcid_tdc(),
                                                  digit_store_->begin()->fine_time());
        for (auto dit = next(digit_store_->begin()); dit != digit_store_->end(); dit++)
        {
            double time = TDC::combined_time_ns(dit->bcid_tdc(),
                                                dit->fine_time());
            if (time < first_time) first_time = time;
        }
        first_time -= 5;

        if (counter_ < 100 - input_->size() ||
            std::find(input_->begin(), input_->end(), trigger_id) != input_->end())
        {
            // Reset histograms
            for (auto tdc = 0; tdc < 9; tdc++) {
                muon_[tdc].Reset("ICESM");
                noise_[tdc].Reset("ICESM");
            }
            muon_hits_ = 0;
            noise_hits_ = 0;

            // Fill histograms
            for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
            {
                if (dit->muon()) 
                {
                    muon_[dit->tdc()].Fill(dit->strip(), TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())-first_time);
                    muon_hits_++;
                }
                else 
                {
                    noise_[dit->tdc()].Fill(dit->strip(), TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())-first_time);
                    noise_hits_++;
                }
            }
            
            if (noise_hits_ > 0)
            {
                std::cout << noise_hits_ << "/" << noise_hits_+muon_hits_ << std::endl;
                // Draw to canvases
                if (noise_hits_ == 0) muon_.draw(canvas_, 0, "BOX");
                else if (muon_hits_ == 0) noise_.draw(canvas_, 0, "BOX");
                else 
                {
                    muon_.draw(canvas_, 0, "BOX");
                    noise_.draw(canvas_, 0, "BOX SAME", true);
                }
            
                canvas_->Print(Form("%s/%s/%d.pdf", outdir_, name_, trigger_id));
                canvas_->Clear();
                counter_++;
            }
        }
    }

    // All data is saved in terminate
    void terminate()
    { }
};


#endif // EVENT_DISPLAY_H
