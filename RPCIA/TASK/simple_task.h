#if !defined(SIMPLE_TASK_H)
#define SIMPLE_TASK_H

#include <vector>

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

#include "analysis_task.h"

class SimpleTask : public AnalysisTask
{
private:
    std::vector<TH1F *> time_hit_diff_from_average;
    TH1F * time_hit_diff_all_pairs;
    TH2F *time_vs_distance_hit_diff;
    TProfile *time_vs_distance_profile;
    
public:
    SimpleTask();
    ~SimpleTask();

    virtual void execute();
    virtual void terminate();
};


#endif // SIMPLE_TASK_H
