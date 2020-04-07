#if !defined(EVENT_HITS_ON_STRIP_H)
#define EVENT_HITS_ON_STRIP_H

#include "analysis_task.h"

#include <vector>
#include <unordered_map>

#include <TCanvas.h>
#include <TH2F.h>

class EventHitsOnStrip : public AnalysisTask
{
private:
    std::vector<TH2F *> tdc_;

public:
    EventHitsOnStrip(const char* name)
     : AnalysisTask(name)
    { }

    ~EventHitsOnStrip()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete tdc_[tdc];
        }
    }

    void init()
    {
        canvas_->Divide(1, 1);

        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdc_.push_back(new TH2F(Form("tdc_%d", tdc), Form("Number of hits in an event (tdc = %d)", tdc),
                                    32, 0, 32, 10, 0, 10));
            tdc_.back()->SetOption("COLZ");
            tdc_.back()->GetXaxis()->SetTitle("Strip");
            tdc_.back()->GetYaxis()->SetTitle("Count");
        }
    }

    void execute()
    {
        std::unordered_map<int, int> counts;
        for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
        {
            counts[dit->tdc() * 32 + dit->strip()] += 1;
        }

        for (auto it = counts.begin(); it != counts.end(); it++)
        {
            tdc_[it->first / 32]->Fill(it->first % 32, it->second);
        }
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            canvas_->cd(1);
            tdc_[tdc]->Draw();
            // gPad->SetLogz();

            // canvas_->cd(2);
            // profile_tdc_[tdc]->Draw();

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc));
            canvas_->Clear("D");
        }
    }
};


#endif // EVENT_HITS_ON_STRIP_H
