#include "hit_rates.h"

HitRates::HitRates()
{
    gDirectory->mkdir("hit_rates");
    gDirectory->cd("hit_rates");
    
    create_outputs();

    gDirectory->cd("..");
}

HitRates::~HitRates()
{
}

void HitRates::create_outputs()
{
    canvas_ = new TCanvas("hit_rates", "canvas", 1000, 600);
    canvas_->Divide(1, 1);

    gDirectory->mkdir("tdc");
    gDirectory->cd("tdc");
    
    for (int tdc = 0; tdc < 9; tdc++)
    {
        tdc_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Hit rate vs strip (tdc = %d)", tdc),
                                32, 0, 32));
        tdc_.back()->GetXaxis()->SetTitle("Strip");
        tdc_.back()->GetYaxis()->SetTitle("Count");
    }

    gDirectory->cd("..");
}

void HitRates::execute()
{
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        tdc_[dit->tdc()]->Fill(dit->strip());
    }
}

void HitRates::terminate()
{
    canvas_->cd();
    for (int tdc = 0; tdc < 9; tdc++)
    {
        canvas_->cd(1);
        tdc_[tdc]->Draw();
        // gPad->SetLogz();

        // canvas_->cd(2);
        // profile_tdc_[tdc]->Draw();

        canvas_->Print(Form("hit_rates_tdc_%d.pdf", tdc));
        canvas_->Clear("D");
    }
    canvas_->Close();
}