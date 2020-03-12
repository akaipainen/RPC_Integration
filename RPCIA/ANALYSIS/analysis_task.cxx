#include "analysis_task.h"

AnalysisTask::AnalysisTask()
{
}

AnalysisTask::~AnalysisTask()
{
    delete digit_store_;
    delete cluster_store_;
    delete track_store_;
}

void AnalysisTask::connect_inputs(Store<Digit> *digits, Store<Cluster> *clusters, Store<Track> *tracks)
{
    digit_store_ = digits;
    cluster_store_ = clusters;
    track_store_ = tracks;
}
