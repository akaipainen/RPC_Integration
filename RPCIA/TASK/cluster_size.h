#if !defined(CLUSTER_SIZE_H)
#define CLUSTER_SIZE_H

#include "analysis_task.h"

#include <vector>

#include <TH1.h>

#include "layer_hist.h"
#include "detector.h"

class ClusterSize : public AnalysisTask
{
    std::vector<TH1F *> tdc_;
    LayerHist<TH1F> layer_;

public:
    ClusterSize(const char *name)
     : AnalysisTask(name, 500, 300)
     , layer_("layer")
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
        layer_.init(9, 32, 0, 32);
        layer_.configure_titles("Cluster Size", "Count");

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
            layer_[3*(1-Detector::tdc_direction(*cit->begin_tdcs())) + cit->position()[2]].Fill(cit->num_digits());
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
        canvas_->Clear();

        layer_.draw(canvas_, 0, "", true);
        canvas_->Print(Form("%s/%s/layer.pdf", outdir_, name_));
        canvas_->Clear();
    }
};


#endif // CLUSTER_SIZE_H
