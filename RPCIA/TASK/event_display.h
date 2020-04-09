#if !defined(EVENT_DISPLAY_H)
#define EVENT_DISPLAY_H

#include "analysis_task.h"

#include <vector>

#include <TH2.h>

#include "tdc.h"

class EventDisplay : public AnalysisTask
{
    std::vector<int> *input_;
    std::vector<TH2F *> muon_;
    std::vector<TH2F *> noise_;
    int counter_;

public:
    EventDisplay(const char *name, std::vector<int> *input=nullptr)
     : AnalysisTask(name, 500, 300)
     , input_(input)
     , counter_(0)
    { }

    ~EventDisplay()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete muon_[tdc];
            delete noise_[tdc];
        }
    }

    void init()
    {
        canvas_->Divide(3, 3);

        for (auto tdc = 0; tdc < 9; tdc++)
        {
            muon_.push_back(new TH2F(Form("muon_tdc_%d", tdc), "Event",
                                    32, 0, 32, 50/25*128, 0, 50));
            muon_.back()->GetXaxis()->SetTitle("Strip");
            muon_.back()->GetYaxis()->SetTitle("Time");
            muon_.back()->SetLineColor(kRed);
            
            noise_.push_back(new TH2F(Form("noise_tdc_%d", tdc), "Event",
                                    32, 0, 32, 50/25*128, 0, 50));
            noise_.back()->GetXaxis()->SetTitle("Strip");
            noise_.back()->GetYaxis()->SetTitle("Time");
            noise_.back()->SetLineColor(kBlue);
        }
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

        if (counter_ < 50 - input_->size() ||
            std::find(input_->begin(), input_->end(), trigger_id) != input_->end())
        {
            // Reset histograms
            for (auto tdc = 0; tdc < 9; tdc++) {
                muon_[tdc]->Reset("ICESM");
                noise_[tdc]->Reset("ICESM");
            }

            // Fill histograms
            for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
            {
                if (dit->muon()) muon_[dit->tdc()]->Fill(dit->strip(), TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())-first_time);
                else noise_[dit->tdc()]->Fill(dit->strip(), TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())-first_time);
            }
            
            // Draw to canvases
            for (auto tdc = 0; tdc < 9; tdc++)
            {
                if (tdc < 3)
                    canvas_->cd(tdc*3+1);
                else if (tdc < 5)
                    canvas_->cd(tdc-1);
                else if (tdc < 7)
                    canvas_->cd(tdc);
                else
                    canvas_->cd(tdc+1);

                muon_[tdc]->SetTitle(Form("Event %d (tdc=%d)", trigger_id, tdc));
                muon_[tdc]->Draw("BOX");
                noise_[tdc]->Draw("BOX SAME");
                
                // muon_[tdc]->Reset();
            }
            canvas_->Print(Form("%s/%s/%d.pdf", outdir_, name_, trigger_id));
            canvas_->Clear("D");
            counter_++;
        }
    }

    // All data is saved in terminate
    void terminate()
    { }
};


#endif // EVENT_DISPLAY_H
