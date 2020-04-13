#if !defined(MUON_TIMING_DISTRIBUTION_H)
#define MUON_TIMING_DISTRIBUTION_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TH1.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TString.h>

#include "tdc.h"

class MuonTimingDistribution : public AnalysisTask
{
private:
    std::vector<TH1D *> tdc_;

    // Change these to set size of printed canvas
    static const int w = 2;
    static const int h = 1;

public:
    MuonTimingDistribution(const char *name)
     : AnalysisTask(name, 500*w, 300*h)
    { }

    ~MuonTimingDistribution() 
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete tdc_[tdc];
        }
    }

    void init()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdc_.push_back(new TH1D(Form("tdc_%d", tdc), Form("Time difference of muon hits from muon hit average (tdc = %d)", tdc),
                                        200/25*128, -100, 100)); // binning = number of ticks
            tdc_.back()->GetXaxis()->SetTitle("Time difference (ns)");
            tdc_.back()->GetYaxis()->SetTitle("Count");
        }
    }

    void execute()
    {
        // Calculate the mean time
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
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            if (dit->muon())
            {
                double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                if (dit->direction())
                {
                    auto test = time - mean_time_eta;
                    tdc_[dit->tdc()]->Fill(time - mean_time_eta);
                }
                else
                {
                    auto test = time - mean_time_phi;
                    tdc_[dit->tdc()]->Fill(time - mean_time_phi);
                }
            }
        }
    }

    void terminate()
    {
        canvas_->Divide(2, 1);

        TF1* formula = new TF1("full_width_3/4_max", "gaus");
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // Create full width half max gaussian fit
            full_width_r_max(*(tdc_[tdc]), *formula, 0.25); // get full width 3/4 max
            TFitResultPtr r = tdc_[tdc]->Fit(formula, "SQR0");
            // TFitResultPtr r = tdc_[tdc]->Fit("gaus", "SQ0"); // Save Quiet 0=NoPlot

            double mean = r->Parameter(1);
            tdc_[tdc]->SetAxisRange(mean - 20, mean + 20);
            
            // Resize plot to zoom in to +/- 20ns around mean
            TF1 extrapolated(*formula);
            extrapolated.SetRange(mean - 20, mean + 20);
            extrapolated.SetLineStyle(3);

            // Set stat options for legend
            gStyle->SetOptFit(1);
            gStyle->SetOptStat(11);

            // Draw log scale plot
            canvas_->cd(1);
            tdc_[tdc]->Draw();
            formula->Draw("SAME");
            extrapolated.Draw("SAME");
            gPad->SetLogy(true);

            // Draw linear scale plot
            canvas_->cd(2);
            tdc_[tdc]->Draw();
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


#endif // MUON_TIMING_DISTRIBUTION_H
