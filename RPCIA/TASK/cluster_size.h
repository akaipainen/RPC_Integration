#if !defined(CLUSTER_SIZE_H)
#define CLUSTER_SIZE_H

#include "analysis_task.h"

#include <vector>

#include <TH1.h>

class ClusterSize : public AnalysisTask
{
    std::vector<TH1F *> tdc_;

public:
    ClusterSize(const char *name)
     : AnalysisTask(name, 500, 300)
    {
    }

    ~ClusterSize()
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
            tdc_.push_back(new TH1F(Form("tdc_%d", tdc), Form("Cluster size (tdc = %d)", tdc),
                                        16, 0, 16));
            tdc_.back()->GetXaxis()->SetTitle("Cluster size");
            tdc_.back()->GetYaxis()->SetTitle("Count");
        }
    }

    void execute()
    {
        for (auto cit = cluster_store_->begin(); cit != cluster_store_->end(); cit++)
        {
            for (auto t = cit->begin_tdcs(); t != cit->end_tdcs(); t++)
            {
                tdc_[*t]->Fill(cit->num_digits());
            }
        }  
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            gStyle->SetOptFit(1);
            gStyle->SetOptStat(11);

            tdc_[tdc]->Draw();
            gPad->SetLogy(true);

            // Save canvas to pdf
            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc)); 

            // Clear the canvas for each TDC
            canvas_->Clear("D"); 
        }
    }
};


#endif // CLUSTER_SIZE_H
