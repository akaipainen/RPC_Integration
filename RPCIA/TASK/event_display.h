#if !defined(EVENT_DISPLAY_H)
#define EVENT_DISPLAY_H

#include "analysis_task.h"

#include <vector>

#include <TH2.h>

#include "tdc.h"

class EventDisplay : public AnalysisTask
{
    std::vector<int> *input_;
    std::vector<TH2F *> tdc_;
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
            delete tdc_[tdc];
        }
    }

    void init()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            tdc_.push_back(new TH2F(Form("tdc_%d", tdc), "Event",
                                    32, 0, 32, 50/25*128, 0, 50));
            tdc_.back()->GetXaxis()->SetTitle("Strip");
            tdc_.back()->GetYaxis()->SetTitle("Time");
            tdc_.back()->SetLineColor(kRed);
        }
    }

    void execute()
    {
        int trigger_id = digit_store_->begin()->trigger_id();
        double first_time = TDC::combined_time_ns(digit_store_->begin()->bcid_tdc(),
                                                  digit_store_->begin()->fine_time());

        if (counter_ < 50 - input_->size() ||
            std::find(input_->begin(), input_->end(), trigger_id) != input_->end())
        {
            // Reset histograms
            for (auto tdc = 0; tdc < 9; tdc++) tdc_[tdc]->Reset("ICESM");

            for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
            {
                tdc_[dit->tdc()]->Fill(dit->strip(), TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time())-first_time);
            }
            
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

                tdc_[tdc]->SetTitle(Form("Event %d (tdc=%d)", trigger_id, tdc));
                tdc_[tdc]->Draw("BOX");
                
                // tdc_[tdc]->Reset();
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
