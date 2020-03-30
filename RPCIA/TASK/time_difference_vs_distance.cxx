#include "time_difference_vs_distance.h"

#include <TPad.h>

#include "detector.h"
#include "tdc.h"

TimeDifferenceVsDistance::TimeDifferenceVsDistance(/* args */)
{
    gDirectory->mkdir("time_diff_vs_dist");
    gDirectory->cd("time_diff_vs_dist");
    
    create_outputs();

    gDirectory->cd("..");
}

TimeDifferenceVsDistance::~TimeDifferenceVsDistance()
{
}

void TimeDifferenceVsDistance::create_outputs()
{
    canvas_ = new TCanvas("time_diff_vs_dist", "canvas", 2000, 600);
    canvas_->Divide(2, 1);

    gDirectory->mkdir("tdc");
    gDirectory->cd("tdc");
    
    for (int tdc = 0; tdc < 9; tdc++)
    {
        tdc_color_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Time difference vs distance (tdc = %d)", tdc),
                                      32, 0, 32, 1600, 0, 1600));
        tdc_color_.back()->SetOption("COLZ");

        tdc_profile_.push_back(new TProfile(Form("profile_tdc_%d", tdc), Form("Profile plot of time difference vs distance (tdc = %d)", tdc),
                                            32, 0, 32, 0, 1600));
    }

    gDirectory->cd("..");
}

void TimeDifferenceVsDistance::execute()
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

                TVector3 pos1 = Detector::position(dit1->tdc(), dit1->strip());
                TVector3 pos2 = Detector::position(dit2->tdc(), dit2->strip());
                
                int dist = std::abs(pos1[0]-pos2[0] + pos1[1]-pos2[1]);
                
                tdc_color_[dit1->tdc()]->Fill(dist, std::abs(time1 - time2));
                tdc_profile_[dit1->tdc()]->Fill(dist, std::abs(time1 - time2));
            }
        }
    }
}

void TimeDifferenceVsDistance::terminate()
{
    canvas_->cd();
    for (int tdc = 0; tdc < 9; tdc++)
    {
        canvas_->cd(1);
        tdc_color_[tdc]->Draw();
        gPad->SetLogz();

        canvas_->cd(2);
        tdc_profile_[tdc]->Draw();

        canvas_->Print(Form("time_diff_vs_dist_tdc_%d.pdf", tdc));
        canvas_->Clear("D");
    }
    canvas_->Close();
}