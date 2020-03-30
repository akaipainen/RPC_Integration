#include "tdc_analysis_task.h"

TDCAnalysisTask::TDCAnalysisTask()
{   
}

TDCAnalysisTask::~TDCAnalysisTask()
{
}

void TDCAnalysisTask::execute()
{
    for (int i = 0; i < 9; i++)
    {
        execute_tdc(i);
    }
    
}