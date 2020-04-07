#if !defined(TIME_DIFFERENCE_FROM_AVERAGE_H)
#define TIME_DIFFERENCE_FROM_AVERAGE_H

#include "analysis_task.h"

#include <vector>
#include <iostream>

#include <TH1.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TString.h>

#include "tdc.h"

class TimeDifferenceFromAverage : public AnalysisTask
{
private:
    std::vector<double> *input_;
    std::vector<double> *output_;

    std::vector<TH1D *> tdc_;
    // std::vector<TH1D *> eta_;
    // std::vector<TH1D *> phi_;

public:
    TimeDifferenceFromAverage(const char *name, std::vector<double> *input=nullptr, std::vector<double> *output=nullptr)
     : AnalysisTask(name, 1000, 300)
     , input_(input)
     , output_(output)
    { }

    ~TimeDifferenceFromAverage()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete tdc_[tdc];
        }
    }

    void init()
    {
        canvas_->Divide(2, 1);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdc_.push_back(new TH1D(Form("tdc_%d", tdc), Form("Time difference from average (tdc = %d)", tdc),
                                        200/25*128, -100, 100));
            tdc_.back()->GetXaxis()->SetTitle("Time difference from direction average (ns)");
            tdc_.back()->GetYaxis()->SetTitle("Count");
        }

        // for (int layer = 0; layer < 3; layer++)
        // {
        //     eta_.push_back(new TH1D(Form("eta_%d", layer), Form("Time difference from average (eta layer = %d)", layer),
        //                                 200/25*128, -100, 100));
        //     eta_.back()->GetXaxis()->SetTitle("Time difference from direction average (ns)");
        //     eta_.back()->GetYaxis()->SetTitle("Count");
        // }

        // for (int layer = 0; layer < 3; layer++)
        // {
        //     phi_.push_back(new TH1D(Form("phi_%d", layer), Form("Time difference from average (phi layer = %d)", layer),
        //                                 200/25*128, -100, 100));
        //     phi_.back()->GetXaxis()->SetTitle("Time difference from direction average (ns)");
        //     phi_.back()->GetYaxis()->SetTitle("Count");
        // }
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
            if (dit->direction())
            {
                mean_time_eta += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                if (input_) mean_time_eta -= (*input_)[dit->tdc()]; // correct offset
                count_eta++;
            }
            else
            {
                mean_time_phi += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
                if (input_) mean_time_phi -= (*input_)[dit->tdc()]; // correct offset
                count_phi++;
            }
            
        }
        mean_time_eta /= count_eta;
        mean_time_phi /= count_phi;

        // Fill the histogram with the time differences from the mean time
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
            if (input_) time -= (*input_)[dit->tdc()]; // correct offset
            if (dit->direction())
            {
                if (count_eta == 1) continue;
                auto test = time - mean_time_eta;
                tdc_[dit->tdc()]->Fill(time - mean_time_eta);
                // eta_[Detector::tdc_layer(dit->tdc())]->Fill(time - mean_time_eta);
            }
            else
            {
                if (count_phi == 1) continue;
                auto test = time - mean_time_phi;
                tdc_[dit->tdc()]->Fill(time - mean_time_phi);
                // phi_[Detector::tdc_layer(dit->tdc())]->Fill(time - mean_time_phi);
            }
        }
    }

    void terminate()
    {
        TF1* formula = new TF1("full_width_3/4_max", "gaus");
        if (output_) output_->resize(9);
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // Create full width half max gaussian fit
            full_width_r_max(*(tdc_[tdc]), *formula, 0.25); // get full width 3/4 max
            TFitResultPtr r = tdc_[tdc]->Fit(formula, "SRQ0");
            double mean = r->Parameter(1);
            if (output_) (*output_)[tdc] = mean; // save mean offset
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


#endif // TIME_DIFFERENCE_FROM_AVERAGE_H
