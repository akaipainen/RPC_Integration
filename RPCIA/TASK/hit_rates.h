#if !defined(HIT_RATES_H)
#define HIT_RATES_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

class HitRates : public AnalysisTask
{
    std::vector<TH1F *> tdc_;

public:
    HitRates(const char* name)
     : AnalysisTask(name, 500, 300)
    { }

    ~HitRates();

    void init()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdc_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Hit rate vs strip (tdc = %d)", tdc),
                                    32, 0, 32));
            tdc_.back()->GetXaxis()->SetTitle("Strip");
            tdc_.back()->GetYaxis()->SetTitle("Count");
        }
    }

    void execute()
    {
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            tdc_[dit->tdc()]->Fill(dit->strip());
        }
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            canvas_->cd(1);
            tdc_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // HIT_RATES_H
