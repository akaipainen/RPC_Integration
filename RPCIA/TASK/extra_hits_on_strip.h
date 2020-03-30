#if !defined(EXTRA_HITS_ON_STRIP_H)
#define EXTRA_HITS_ON_STRIP_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH2F.h>
#include <TProfile.h>

class ExtraHitsOnStrip : public AnalysisTask
{
private:
    TCanvas *canvas_;

    std::vector<TH2F *> color_tdc_;
    std::vector<TProfile *> profile_tdc_;

public:
    ExtraHitsOnStrip(/* args */);
    ~ExtraHitsOnStrip();

    void create_outputs();

    void execute();

    void terminate();

};


#endif // EXTRA_HITS_ON_STRIP_H
