#if !defined(ANALYSIS_TASK_H)
#define ANALYSIS_TASK_H

#include <vector>

#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TPRegexp.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

#include "store.h"
#include "digit.h"
#include "cluster.h"
#include "track.h"
#include "strip.h"

class AnalysisTask
{
protected:
    std::vector<TH1*> plots_; // all plots are saved here

    Store<Digit> *digit_store_;
    Store<Cluster> *cluster_store_;
    Store<Track> *track_store_;

    TCanvas *canvas_;

    const char *name_;
    const char *outdir_;

public:
    AnalysisTask(const char *name="analysis_task", int width=500, int height=300);
    ~AnalysisTask();

    // Connect the input stores
    void connect_inputs(Store<Digit> *digits, Store<Cluster> *clusters, Store<Track> *tracks);
    
    // Connect the input stores
    void set_output_dir(const char* outdir);

    // Define output objects (run once at beginning) - wrapper
    void init_task();

    // Fill output objects with current data in stores (run at each event step) - wrapper
    void execute_task();

    // Save outputs to file (run at end) - wrapper
    void terminate_task();

protected:
    // OVERWRITE WITH IMPLEMENTATIONS

    virtual void init() = 0;

    virtual void execute() = 0;
    
    virtual void terminate() = 0;

    // Helper methods to use

    TH1F* create_1d_histogram(TString name, 
                              TString title, 
                              TString xaxis, 
                              TString yaxis,
                              int xmin, int xmax, double xspacing);

    TH2F* create_2d_histogram(TString name, 
                              TString title, 
                              TString xaxis, 
                              TString yaxis,
                              double xmin, double xmax, double xspacing,
                              double ymin, double ymax, double yspacing);

    TProfile* create_2d_profile(TString name, 
                              TString title, 
                              TString xaxis, 
                              TString yaxis,
                              double xmin, double xmax, double xspacing,
                              double ymin, double ymax);

    void cd_grid(int tdc) const;

};

#endif // ANALYSIS_TASK_H
