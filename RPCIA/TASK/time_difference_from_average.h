#if !defined(TIME_DIFFERENCE_FROM_AVERAGE_H)
#define TIME_DIFFERENCE_FROM_AVERAGE_H

#include "analysis_task.h"

#include <vector>

#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>

class TimeDifferenceFromAverage : public AnalysisTask
{
private:
    TCanvas * canvas_;

    std::vector<TH1F *> tdc_;
    std::vector<TH1F *> eta_;
    std::vector<TH1F *> phi_;

public:
    TimeDifferenceFromAverage();
    ~TimeDifferenceFromAverage();

    void create_outputs();

    void execute();

    void terminate();

private:
    void full_width_r_max(const TH1F &hist, TF1 &formula, const double r=0.5);
};


#endif // TIME_DIFFERENCE_FROM_AVERAGE_H
