#include "isolate_noise.h"

#include <TVector3.h>

#include "tdc.h"
#include "detector.h"

IsolateNoise::IsolateNoise()
: AnalysisTask()
{
    gDirectory->mkdir("noise");
    gDirectory->cd("noise");

    widths.push_back(new TH1F("tdc5strip8", "Width plot for TDC 5 Strip 8",
                              200, 0, 100));

    widths.push_back(new TH1F("tdc7strip8", "Width plot for TDC 7 Strip 8",
                              200, 0, 100));

    for (auto &width : widths)
    {
        width->GetXaxis()->SetTitle("Width (raw)");
        width->GetYaxis()->SetTitle("Counts");
    }
    
    gDirectory->cd("..");
}

IsolateNoise::~IsolateNoise()
{
}

void IsolateNoise::execute()
{ 
    for (auto dit = digit_store_->begin(); dit != digit_store_->end(); dit++)
    {
        widths[0]->Fill(dit->width(), dit->tdc() == 5);
        widths[1]->Fill(dit->width(), dit->tdc() == 7);
    }
}

void IsolateNoise::terminate()
{
    // gDirectory->pwd();
}