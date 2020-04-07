#if !defined(TIME_DIFFERENCE_VS_DISTANCE_H)
#define TIME_DIFFERENCE_VS_DISTANCE_H

#include "analysis_task.h"

#include <vector>
#include <unordered_set>

#include <TCanvas.h>
#include <TH2F.h>
#include <TProfile.h>

#include "detector.h"
#include "tdc.h"

class TimeDifferenceVsDistance : public AnalysisTask
{
private:
    std::vector<TH2F *> tdc_color_;
    // std::vector<TH2F *> eta_color_;
    // std::vector<TH2F *> phi_color_;

    std::vector<TProfile *> tdc_profile_;
    // std::vector<TProfile *> eta_profile_;
    // std::vector<TProfile *> phi_profile_;

public:
    TimeDifferenceVsDistance(const char* name)
     : AnalysisTask(name, 1000, 300)
    { }

    ~TimeDifferenceVsDistance()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete tdc_color_[tdc];
            delete tdc_profile_[tdc];
        }
    }

    void init()
    {
        canvas_->Divide(2, 1);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdc_color_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Time difference between pairs of hits at a given distance apart (tdc = %d)", tdc),
                                        32, 0, 32, 128, 0, 25));
            tdc_color_.back()->SetOption("COLZ");
            tdc_color_.back()->GetXaxis()->SetTitle("Distance");
            tdc_color_.back()->GetYaxis()->SetTitle("Time (ns)");

            tdc_profile_.push_back(new TProfile(Form("profile_tdc_%d", tdc), Form("Time difference between pairs of hits at a given distance apart (tdc = %d)", tdc),
                                                32, 0, 32, 0, 25));
            tdc_color_.back()->GetXaxis()->SetTitle("Distance");
            tdc_color_.back()->GetYaxis()->SetTitle("Time (ns)");
        }
    }

    void execute()
    {
        // Find the first hit on each strip, ignore rest
        std::unordered_set<int> visited;
        Store<Digit> *first_hits = new Store<Digit>(true);
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (visited.find(dit->tdc()*32 + dit->strip()) != visited.end())
                continue;
            else
                visited.insert(dit->tdc()*32 + dit->strip());
                first_hits->add(*dit);
        }
        
        for (auto dit1 = first_hits->begin(); dit1 != first_hits->end(); dit1++)
        {
            for (auto dit2 = next(dit1); dit2 != first_hits->end(); dit2++)
            {
                // Only do for matching tdcs
                if (dit1->tdc() == dit2->tdc())
                {
                    double time1 = TDC::combined_time_ns(dit1->bcid_tdc(), dit1->fine_time());
                    double time2 = TDC::combined_time_ns(dit2->bcid_tdc(), dit2->fine_time());

                    // Must be within 25ns to be considered even remotely a hit
                    if (std::abs(time2 - time1) > 25)
                    {
                        continue;
                    }

                    TVector3 pos1 = Detector::position(dit1->tdc(), dit1->strip());
                    TVector3 pos2 = Detector::position(dit2->tdc(), dit2->strip());
                    
                    int dist = std::abs(pos1[0]-pos2[0] + pos1[1]-pos2[1]);
                    auto time = std::abs(time2 - time1);
                    
                    tdc_color_[dit1->tdc()]->Fill(dist, time);
                    tdc_profile_[dit1->tdc()]->Fill(dist, time);
                }
            }
        }
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            canvas_->cd(1);
            tdc_color_[tdc]->Draw();
            gPad->SetLogz();

            canvas_->cd(2);
            tdc_profile_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }

};


#endif // TIME_DIFFERENCE_VS_DISTANCE_H
