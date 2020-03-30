#if !defined(TDC_ANALYSIS_TASK_H)
#define TDC_ANALYSIS_TASK_H

#include "analysis_task.h"

#include <vector>

#include <TObject.h>

class TDCAnalysisTask : public AnalysisTask
{
private:
    std::vector<TObject> objects_;

public:
    TDCAnalysisTask();
    ~TDCAnalysisTask();

    void execute();
    void terminate();

    virtual void execute_tdc() = 0;
};


#endif // TDC_ANALYSIS_TASK_H
