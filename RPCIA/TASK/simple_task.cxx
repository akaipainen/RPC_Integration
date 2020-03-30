#include "simple_task.h"

#include <iostream>

#include <TVector3.h>

#include "tdc.h"
#include "detector.h"

SimpleTask::SimpleTask()
: AnalysisTask()
{
    gDirectory->mkdir("simple");
    gDirectory->cd("simple");

    gDirectory->mkdir("time_diff_avg");
    gDirectory->cd("time_diff_avg");

    for (Int_t i = 0; i < 11; i++) // 9 tdcs total + 2 directions (eta/phi)
    {
        if (i == 9)
        {
            time_hit_diff_from_average.push_back(new TH1F("phi", "Time difference from average time for all hits phi",
                                                          200*5, -100, 100));
        }
        else if (i == 10)
        {
            time_hit_diff_from_average.push_back(new TH1F("eta", "Time difference from average time for all hits eta",
                                                          200*5, -100, 100));
        } else
        {
            time_hit_diff_from_average.push_back(new TH1F(TString::Format("tdc_%d", i), TString("Time difference from average time for all hits TDC=").Append('0'+i).Data(),
                                                          200*5, -100, 100));
        }
        
        time_hit_diff_from_average.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        time_hit_diff_from_average.back()->GetYaxis()->SetTitle("Count");
    }


    gDirectory->cd("..");

    time_hit_diff_all_pairs = new TH1F("time_diff_pairs", "Time difference between all pairs of hits",
                                       3200, -1600, 1600);
    time_hit_diff_all_pairs->GetXaxis()->SetTitle("Time difference between two events (ns)");
    time_hit_diff_all_pairs->GetYaxis()->SetTitle("Count");

    time_vs_distance_hit_diff = new TH2F("time_dist_diff", "Absolute time difference vs distance for all pairs of parallel hits",
                                         64, 0, 64, 1600, 0, 1600);
    time_vs_distance_hit_diff->SetOption("COLZ");
    time_vs_distance_hit_diff->GetXaxis()->SetTitle("Strip distance (ignoring vertical layer distance)");
    time_vs_distance_hit_diff->GetYaxis()->SetTitle("Time difference between two parallel hits in an event (ns)");

    time_vs_distance_profile = new TProfile("time_dist_profile", "Profile plot of absolute time difference vs distance for all parallel hits",
                                            64, 0, 64, 0, 1600);
    time_vs_distance_profile->GetXaxis()->SetTitle("Strip distance (ignoring vertical layer distance)");
    time_vs_distance_profile->GetYaxis()->SetTitle("Average time difference between two parallel hits in an event (ns)");

    gDirectory->cd("..");
}

SimpleTask::~SimpleTask()
{
}

void SimpleTask::execute()
{
    // Calculate the mean time
    double mean_time = 0;
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        if (dit->tdc()*32 + dit->strip() == 168 ||
            dit->tdc()*32 + dit->strip() == 232)
        {
            continue;
        }
        mean_time += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
    }
    mean_time /= digit_store_->size();

    // Fill the histogram with the time differences from the mean time
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        if (dit->tdc()*32 + dit->strip() == 168 ||
            dit->tdc()*32 + dit->strip() == 232)
        {
            continue;
        }
        double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
        time_hit_diff_from_average[dit->tdc()]->Fill(time - mean_time);
        time_hit_diff_from_average[9+dit->direction()]->Fill(time - mean_time);
    }

    // Fill the histogram with the time difference of all pairs of hits
    for (auto dit1 = digit_store_->begin(); dit1 != digit_store_->end(); dit1++)
    {
        if (dit1->tdc()*32 + dit1->strip() == 168 ||
            dit1->tdc()*32 + dit1->strip() == 232)
        {
            continue;
        }
        
        for (auto dit2 = next(dit1); dit2 != digit_store_->end(); dit2++)
        {
            if (dit2->tdc()*32 + dit2->strip() == 168 ||
                dit2->tdc()*32 + dit2->strip() == 232)
            {
                continue;
            }

            double time1 = TDC::combined_time_ns(dit1->bcid_tdc(), dit1->fine_time());
            double time2 = TDC::combined_time_ns(dit2->bcid_tdc(), dit2->fine_time());
            if (Detector::parallel(dit1->tdc(), dit2->tdc()))
            {
                TVector3 pos1 = Detector::position(dit1->tdc(), dit1->strip());
                TVector3 pos2 = Detector::position(dit2->tdc(), dit2->strip());
                int dist = std::abs(pos1[0]-pos2[0] + pos1[1]-pos2[1]);
                time_vs_distance_hit_diff->Fill(dist, std::abs(time1 - time2));
                time_vs_distance_profile->Fill(dist, std::abs(time1 - time2));
            }
            time_hit_diff_all_pairs->Fill(time1 - time2);
        }
    }
}

void SimpleTask::terminate()
{
    // gDirectory->pwd();
}