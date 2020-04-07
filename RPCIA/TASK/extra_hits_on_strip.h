#if !defined(EXTRA_HITS_ON_STRIP_H)
#define EXTRA_HITS_ON_STRIP_H

#include "analysis_task.h"

#include <vector>
#include <unordered_set>

#include <TCanvas.h>
#include <TH2F.h>
#include <TProfile.h>

#include "tdc.h"

class ExtraHitsOnStrip : public AnalysisTask
{
private:
    std::vector<TH2F *> color_tdc_;
    std::vector<TProfile *> profile_tdc_;

public:
    ExtraHitsOnStrip(const char* name)
     : AnalysisTask(name, 1000, 300)
    { }

    ~ExtraHitsOnStrip()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete color_tdc_[tdc];
            delete profile_tdc_[tdc];
        }
        
    }

    void init()
    {
        canvas_->Divide(2, 1);
        
        for (int tdc = 0; tdc < 9; tdc++)
        {
            color_tdc_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Time between first and second hit (tdc = %d)", tdc),
                                            32, 0, 32, 100, 0, 100));
            color_tdc_.back()->SetOption("COLZ");
            color_tdc_.back()->GetXaxis()->SetTitle("Strip");
            color_tdc_.back()->GetYaxis()->SetTitle("Time (ns)");

            profile_tdc_.push_back(new TProfile(Form("profile_tdc_%d", tdc), Form("Time between first and second hit (tdc = %d)", tdc),
                                                     32, 0, 32, 0, 100));
            profile_tdc_.back()->GetXaxis()->SetTitle("Strip");
            profile_tdc_.back()->GetYaxis()->SetTitle("Time (ns)");
        }
    }

    void execute()
    {
        std::unordered_set<int> visited;
    
        for (auto dit1 = digit_store_->begin(); dit1 != digit_store_->end(); dit1++)
        {
            if (visited.find(dit1->tdc()*32 + dit1->strip()) != visited.end())
                continue;
            else
                visited.insert(dit1->tdc()*32 + dit1->strip());

            for (auto dit2 = next(dit1); dit2 != digit_store_->end(); dit2++)
            {
                if (dit1->tdc() == dit2->tdc() && dit1->strip() == dit2->strip())
                {
                    double time1 = TDC::combined_time_ns(dit1->bcid_tdc(), dit1->fine_time());
                    double time2 = TDC::combined_time_ns(dit2->bcid_tdc(), dit2->fine_time());
                    
                    if (time2 - time1 < 100)
                    {
                        color_tdc_[dit1->tdc()]->Fill(dit1->strip(), time2 - time1);
                        profile_tdc_[dit1->tdc()]->Fill(dit1->strip(), time2 - time1);
                        
                        // Only do this for the first and second
                        break;
                    }
                }
            }
        }
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            canvas_->cd(1);
            color_tdc_[tdc]->Draw();
            // gPad->SetLogz();

            canvas_->cd(2);
            profile_tdc_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }

};


#endif // EXTRA_HITS_ON_STRIP_H
