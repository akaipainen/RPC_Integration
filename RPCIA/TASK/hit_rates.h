#if !defined(HIT_RATES_H)
#define HIT_RATES_H

#include "analysis_task.h"

#include <vector>

#include <TCanvas.h>
#include <TH1.h>

class HitRates : public AnalysisTask
{
private:
    TCanvas *canvas_;

    std::vector<TH1F *> tdc_;

public:
    HitRates();
    ~HitRates();

    void create_outputs();

    void execute();

    void terminate();
};


#endif // HIT_RATES_H
