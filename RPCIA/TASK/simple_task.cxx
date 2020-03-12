#include "simple_task.h"

#include <TVector3.h>

#include "tdc.h"
#include "detector.h"

SimpleTask::SimpleTask()
: AnalysisTask()
{
    gDirectory->mkdir("simple");
    gDirectory->cd("simple");

    time_hit_diff_from_average = new TH1F("time_diff_avg", "Time difference from average time for all hits",
                                          3200, -1600, 1700);
    time_hit_diff_from_average->GetXaxis()->SetTitle("Time difference from event average (ns)");
    time_hit_diff_from_average->GetYaxis()->SetTitle("Count");

    time_hit_diff_all_pairs = new TH1F("time_diff_pairs", "Time difference between all pairs of hits",
                                       3200, -1600, 1600);
    time_hit_diff_all_pairs->GetXaxis()->SetTitle("Time difference between two events (ns)");
    time_hit_diff_all_pairs->GetYaxis()->SetTitle("Count");

    time_vs_distance_hit_diff = new TH2F("time_dist_diff", "Absolute time difference vs distance for all parallel hits",
                                         1600, 0, 1600, 64, 0, 64);
    time_vs_distance_hit_diff->GetXaxis()->SetTitle("Time difference between two parallel hits in an event (ns)");
    time_vs_distance_hit_diff->GetYaxis()->SetTitle("Strip distance (ignoring vertical layer distance)");

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
        mean_time += TDC::combined_time_ns((*dit)->bcid_tdc(), (*dit)->fine_time());
    }
    mean_time /= digit_store_->size();

    // Fill the histogram with the time differences from the mean time
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        double time = TDC::combined_time_ns((*dit)->bcid_tdc(), (*dit)->fine_time());
        time_hit_diff_from_average->Fill(time - mean_time);
    }

    // Fill the histogram with the time difference of all pairs of hits
    for (auto dpit1 = digit_store_->begin(); dpit1 != digit_store_->end(); dpit1++)
    {
        auto dit1 = *dpit1;
        for (auto dpit2 = next(dpit1); dpit2 != digit_store_->end(); dpit2++)
        {
            auto dit2 = *dpit2;
            double time1 = TDC::combined_time_ns(dit1->bcid_tdc(), dit1->fine_time());
            double time2 = TDC::combined_time_ns(dit2->bcid_tdc(), dit2->fine_time());
            if (Detector::parallel(dit1->tdc(), dit2->tdc()))
            {
                TVector3 pos1 = Detector::position(dit1->tdc(), dit1->strip());
                TVector3 pos2 = Detector::position(dit2->tdc(), dit2->strip());
                int dist = std::abs(pos1[0]-pos2[0] + pos1[1]-pos2[1]);
                time_vs_distance_hit_diff->Fill(std::abs(time1 - time2), dist);
            }
            time_hit_diff_all_pairs->Fill(time1 - time2);
        }
    }
}

void SimpleTask::terminate()
{
    // gDirectory->pwd();
}