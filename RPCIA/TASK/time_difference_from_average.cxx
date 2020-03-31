#include "time_difference_from_average.h"

#include <TFitResult.h>

#include "detector.h"
#include "tdc.h"

TimeDifferenceFromAverage::TimeDifferenceFromAverage(const char *name, std::vector<double> *input, std::vector<double> *output)
 : input_(input)
 , output_(output)
 , name_(name)
{
    gDirectory->mkdir(name);
    gDirectory->cd(name);
    
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
        tdc_.push_back(new TH1D(Form("tdc_%d", tdc), Form("Time difference from average (tdc = %d)", tdc),
                                     200/25*128, -100, 100));
        tdc_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        tdc_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");

    gDirectory->mkdir("eta");
    gDirectory->cd("eta");
    for (int layer = 0; layer < 3; layer++)
    {
        eta_.push_back(new TH1D(Form("eta_layer_%d", layer), Form("Time difference from average (eta layer = %d)", layer),
                                     200/25*128, -100, 100));
        eta_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        eta_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");

    gDirectory->mkdir("phi");
    gDirectory->cd("phi");
    for (int layer = 0; layer < 3; layer++)
    {
        phi_.push_back(new TH1D(Form("phi_layer_%d", layer), Form("Time difference from average (phi layer = %d)", layer),
                                     200/25*128, -100, 100));
        phi_.back()->GetXaxis()->SetTitle("Time difference from event average (ns)");
        phi_.back()->GetYaxis()->SetTitle("Count");
    }
    gDirectory->cd("..");
}

void TimeDifferenceFromAverage::execute()
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

    // mean_time_eta = (mean_time_eta / 0.1953125) * 0.1953125;
    // mean_time_phi = (mean_time_phi / 0.1953125) * 0.1953125;

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
            eta_[Detector::tdc_layer(dit->tdc())]->Fill(time - mean_time_eta);
        }
        else
        {
            if (count_phi == 1) continue;
            auto test = time - mean_time_phi;
            tdc_[dit->tdc()]->Fill(time - mean_time_phi);
            phi_[Detector::tdc_layer(dit->tdc())]->Fill(time - mean_time_phi);
        }
    }
}

void TimeDifferenceFromAverage::terminate()
{
    canvas_->cd();
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

        canvas_->Print(Form("%s_fwhm_tdc_%d.pdf", name_, tdc));
        canvas_->Clear("D");
    }
    canvas_->Close();
}

void TimeDifferenceFromAverage::full_width_r_max(const TH1 &hist, TF1 &formula, const double r)
{
    double r_max_val = r * hist.GetMaximum();
    int lower_bin = hist.FindFirstBinAbove(r_max_val);
    int upper_bin = hist.FindLastBinAbove(r_max_val);
    double lower = hist.GetXaxis()->GetBinCenter(lower_bin);
    double upper = hist.GetXaxis()->GetBinCenter(upper_bin);
    formula.SetRange(lower, upper);
}