#include "time_difference_from_average.h"

#include <TFitResult.h>

#include "detector.h"
#include "tdc.h"

TimeDifferenceFromAverage::TimeDifferenceFromAverage()
{
    gDirectory->mkdir("time_diff_avg");
    gDirectory->cd("time_diff_avg");
    
    create_outputs();

    gDirectory->cd("..");
}

TimeDifferenceFromAverage::~TimeDifferenceFromAverage()
{
}

void TimeDifferenceFromAverage::create_outputs()
{
    canvas_ = new TCanvas("time_diff_avg", "canvas", 2000, 600);
    canvas_->Divide(2, 1);

    gDirectory->mkdir("tdc");
    gDirectory->cd("tdc");

    for (int tdc = 0; tdc < 9; tdc++)
    {
        tdc_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Time difference from average (tdc = %d)", tdc),
                                     200/25*128, -100, 100));
        tdc_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        tdc_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");

    gDirectory->mkdir("eta");
    gDirectory->cd("eta");
    for (int layer = 0; layer < 3; layer++)
    {
        eta_.push_back(new TH1F(Form("eta_layer_%d", layer), Form("Time difference from average (eta layer = %d)", layer),
                                     200/25*128, -100, 100));
        eta_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        eta_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");

    gDirectory->mkdir("phi");
    gDirectory->cd("phi");
    for (int layer = 0; layer < 3; layer++)
    {
        phi_.push_back(new TH1F(Form("phi_layer_%d", layer), Form("Time difference from average (phi layer = %d)", layer),
                                     200/25*128, -100, 100));
        phi_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        phi_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");
}

void TimeDifferenceFromAverage::execute()
{
    // Calculate the mean time
    double mean_time = 0;
    int count = 0;
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        mean_time += TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
        count++;
    }
    mean_time /= count;

    // Fill the histogram with the time differences from the mean time
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        double time = TDC::combined_time_ns(dit->bcid_tdc(), dit->fine_time());
        tdc_[dit->tdc()]->Fill(time - mean_time);
        if (dit->direction())
        {
            auto temp = Detector::tdc_layer(dit->tdc());
            eta_[temp]->Fill(time - mean_time);
        }
        else
        {
            phi_[Detector::tdc_layer(dit->tdc())]->Fill(time - mean_time);
        }
    }
}

void TimeDifferenceFromAverage::terminate()
{
    canvas_->cd();
    TF1* formula = new TF1("full_width_3/4_max", "gaus");
    for (int tdc = 0; tdc < 9; tdc++)
    {
        // Create full width half max gaussian fit
        full_width_r_max(*(tdc_[tdc]), *formula, 0.5); // get full width 3/4 max
        TFitResultPtr r = tdc_[tdc]->Fit(formula, "SRQ0");
        double mean = r->Parameter(1);
        tdc_[tdc]->SetAxisRange(mean - 20, mean + 20);
        
        TF1 extrapolated(*formula);
        extrapolated.SetRange(mean - 20, mean + 20);
        extrapolated.SetLineStyle(3);

        gStyle->SetOptFit(1);
        gStyle->SetOptStat(11);

        canvas_->cd(1);
        tdc_[tdc]->Draw();
        formula->Draw("SAME");
        extrapolated.Draw("SAME");
        gPad->SetLogy(true);

        canvas_->cd(2);
        tdc_[tdc]->Draw();
        formula->Draw("SAME");
        extrapolated.Draw("SAME");
        gPad->SetLogy(false);

        canvas_->Print(Form("time_diff_avg_fwhm_tdc_%d.pdf", tdc));
        canvas_->Clear("D");
    }
    canvas_->Close();
}

void TimeDifferenceFromAverage::full_width_r_max(const TH1F &hist, TF1 &formula, const double r)
{
    double r_max_val = r * hist.GetMaximum();
    int lower_bin = hist.FindFirstBinAbove(r_max_val);
    int upper_bin = hist.FindLastBinAbove(r_max_val);
    double lower = hist.GetXaxis()->GetBinCenter(lower_bin);
    double upper = hist.GetXaxis()->GetBinCenter(upper_bin);
    formula.SetRange(lower, upper);
}