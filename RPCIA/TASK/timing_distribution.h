#if !defined(TIMING_DISTRIBUTION_H)
#define TIMING_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TH1.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TString.h>

#include "tdc.h"
#include "detector.h"
#include "summary_hist.h"
#include "cluster_finder.h"

class TimingDistribution : public AnalysisTask
{
private:
    SummaryHist<TH1F> muon_candidate_;

    int spark_counter_;

    // Change these to set size of printed canvas
    static const int w = 2;
    static const int h = 1;

public:
    TimingDistribution(const char *name)
     : AnalysisTask(name, 500*w, 300*h)
     , muon_candidate_("muon_candidate")
     , spark_counter_(0)
    { }

    ~TimingDistribution() 
    { }

    void init()
    {
        muon_candidate_.init(9, 200/25*128, -100, 100);
    }

    void execute()
    {
        // Spark filter
        for (auto &cluster : *cluster_store_)
        {
            // If there exists a cluster size greater than 8,
            // skip this event
            if (cluster.num_digits() > 8)
            {
                spark_counter_++;
                return;
            }
        }

        // Calculate the mean time 
        // Use all muons (clusters in time and space)
        double mean_time_eta = 0;
        double mean_time_phi = 0;
        int count_eta = 0;
        int count_phi = 0;
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (dit->muon())
            {
                if (dit->direction())
                {
                    mean_time_eta += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                    count_eta++;
                }
                else
                {
                    mean_time_phi += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                    count_phi++;
                }
            }
        }
        mean_time_eta /= count_eta;
        mean_time_phi /= count_phi;

        // Fill the histogram with the time differences from the mean time
        // Only use clusters in space (add hits which are adjacent to clusters in time)
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (dit->muon())
            {
                double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                if (dit->direction())
                {
                    auto test = time - mean_time_eta;
                    muon_candidate_[dit->tdc()].Fill(time - mean_time_eta);
                }
                else
                {
                    auto test = time - mean_time_phi;
                    muon_candidate_[dit->tdc()].Fill(time - mean_time_phi);
                }
            }
            else
            {
                for (auto testit = digit_store_->begin(); testit != digit_store_->end(); testit++)
                {
                    if (Detector::strip_adjacent(testit->tdc(), testit->strip(),
                                                 dit->tdc(), dit->strip()))
                    {
                        double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                        if (dit->direction())
                        {
                            auto test = time - mean_time_eta;
                            muon_candidate_[dit->tdc()].Fill(time - mean_time_eta);
                        }
                        else
                        {
                            auto test = time - mean_time_phi;
                            muon_candidate_[dit->tdc()].Fill(time - mean_time_phi);
                        }
                    }
                }
            }
        }
    }

    void terminate()
    {
        std::cout << "Number of sparks: " << spark_counter_ << std::endl;

        canvas_->Divide(2, 1);

        

        TF1* formula = new TF1("full_width_3/4_max", "gaus");
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // Create full width half max gaussian fit
            full_width_r_max(muon_candidate_[tdc], *formula, 0.25); // get full width 3/4 max
            TFitResultPtr r = muon_candidate_[tdc].Fit(formula, "SQR0");
            // TFitResultPtr r = muon_candidate_[tdc]->Fit("gaus", "SQ0"); // Save Quiet 0=NoPlot

            double mean = r->Parameter(1);
            muon_candidate_[tdc].SetAxisRange(mean - 20, mean + 20);
            
            // Resize plot to zoom in to +/- 20ns around mean
            TF1 extrapolated(*formula);
            extrapolated.SetRange(mean - 20, mean + 20);
            extrapolated.SetLineStyle(3);

            // Set stat options for legend
            gStyle->SetOptFit(1);
            gStyle->SetOptStat(11);

            // Draw log scale plot
            canvas_->cd(1);
            muon_candidate_[tdc].Draw();
            formula->Draw("SAME");
            extrapolated.Draw("SAME");
            gPad->SetLogy(true);

            // Draw linear scale plot
            canvas_->cd(2);
            muon_candidate_[tdc].Draw();
            formula->Draw("SAME");
            extrapolated.Draw("SAME");
            gPad->SetLogy(false);

            // Save canvas to pdf
            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc)); 
            canvas_->Clear("D");
        }
    }

private:
    void full_width_r_max(const TH1 &hist, TF1 &formula, const double r=0.5)
    {
        double r_max_val = r * hist.GetMaximum();
        int lower_bin = hist.FindFirstBinAbove(r_max_val);
        int upper_bin = hist.FindLastBinAbove(r_max_val);
        double lower = hist.GetXaxis()->GetBinCenter(lower_bin);
        double upper = hist.GetXaxis()->GetBinCenter(upper_bin);
        formula.SetRange(lower, upper);
    }
};


#endif // TIMING_DISTRIBUTION_H
