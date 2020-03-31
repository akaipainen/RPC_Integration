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
    std::vector<double> *input_;
    std::vector<double> *output_;
    const char *name_;

    TCanvas * canvas_;

    std::vector<TH1D *> tdc_;
    std::vector<TH1D *> eta_;
    std::vector<TH1D *> phi_;

    friend class TimeDifferenceFromAverageCorrected;

public:
    TimeDifferenceFromAverage(const char *name, std::vector<double> *input=nullptr, std::vector<double> *output=nullptr);
    ~TimeDifferenceFromAverage();

    void create_outputs();

    void execute();

    void terminate();

private:
    void full_width_r_max(const TH1 &hist, TF1 &formula, const double r=0.5);
};


#endif // TIME_DIFFERENCE_FROM_AVERAGE_H
