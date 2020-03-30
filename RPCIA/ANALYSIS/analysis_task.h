#if !defined(ANALYSIS_TASK_H)
#define ANALYSIS_TASK_H

#include <vector>

#include "store.h"
#include "digit.h"
#include "cluster.h"
#include "track.h"
#include "strip.h"

class AnalysisTask
{
protected:
    Store<Digit> *digit_store_;
    Store<Cluster> *cluster_store_;
    Store<Track> *track_store_;

public:
    AnalysisTask();
    ~AnalysisTask();

    // Connect the input stores
    void connect_inputs(Store<Digit> *digits, Store<Cluster> *clusters, Store<Track> *tracks);

    // Execute one step of analysis task
    // Fill output objects with current data in stores
    virtual void execute() = 0; // OVERWRITE

    // End analysis task
    // Save outputs to file
    virtual void terminate() = 0; // OVERWRITE

};

#endif // ANALYSIS_TASK_H
