#if !defined(TIME_DIFFERENCE_VS_DISTANCE_H)
#define TIME_DIFFERENCE_VS_DISTANCE_H

#include "analysis_task.h"

#include <vector>
#include <unordered_set>

#include <TCanvas.h>
#include <TH2F.h>
#include <TProfile.h>

class TimeDifferenceVsDistance : public AnalysisTask
{
private:
    TCanvas *canvas_;

    std::vector<TH2F *> tdc_color_;
    std::vector<TH2F *> eta_color_;
    std::vector<TH2F *> phi_color_;

    std::vector<TProfile *> tdc_profile_;
    std::vector<TProfile *> eta_profile_;
    std::vector<TProfile *> phi_profile_;

public:
    TimeDifferenceVsDistance();
    ~TimeDifferenceVsDistance();

    void create_outputs();

    void execute();

    void terminate();

};


#endif // TIME_DIFFERENCE_VS_DISTANCE_H
