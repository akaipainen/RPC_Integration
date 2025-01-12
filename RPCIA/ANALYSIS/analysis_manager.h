#if !defined(ANALYSIS_MANAGER_H)
#define ANALYSIS_MANAGER_H

#include <vector>

#include <TTree.h>
#include <TFile.h>

#include "analysis_task.h"
#include "loader.h"

#include "strip.h"
#include "store.h"
#include "digit.h"
#include "cluster.h"
#include "track.h"

class AnalysisManager 
{
private:
    TTree *tree_;
    TFile *out_;
    std::vector<AnalysisTask *> tasks_;

    Store<Digit> *digit_store_;
    Store<Cluster> *cluster_store_;
    Store<Track> *track_store_;

    const char* outdir_;

public:
    AnalysisManager();
    ~AnalysisManager();

    // Initialize the AnalysisManager
    // Set the input tree
    void init(TTree *tree);

    // Add an analysis task
    void add_task(AnalysisTask *task);
    
    // Add an analysis task
    void set_output_dir(const char* outdir);

    // Run all the analysis tasks
    // Save output to output file
    void run();

private:
    // Execute all tasks on current entry
    void execute_tasks();

    // Terminate all tasks
    void terminate_tasks();

};

#endif // ANALYSIS_MANAGER_H
