#include "analysis_task.h"

AnalysisTask::AnalysisTask(const char *name, int width, int height)
 : name_(name)
{
    canvas_ = new TCanvas(name_, "canvas", width, height);
}

AnalysisTask::~AnalysisTask()
{
    delete canvas_;
}

void AnalysisTask::connect_inputs(Store<Digit> *digits, Store<Cluster> *clusters, Store<Track> *tracks)
{
    digit_store_ = digits;
    cluster_store_ = clusters;
    track_store_ = tracks;
}

void AnalysisTask::set_output_dir(const char* outdir)
{
    outdir_ = outdir;
}

void AnalysisTask::init_task()
{
    gSystem->mkdir(Form("%s/%s", outdir_, name_), true);
    gDirectory->mkdir(name_);
    gDirectory->cd(name_);

    init();

    gDirectory->cd("..");
}

void AnalysisTask::execute_task()
{
    execute();
}

void AnalysisTask::terminate_task()
{
    canvas_->cd();

    terminate();

    canvas_->Close();
}

TH1F* AnalysisTask::create_1d_histogram(TString name, 
                                        TString title, 
                                        TString xaxis, 
                                        TString yaxis,
                                        int xmin, int xmax, 
                                        double xspacing)
{
    // Handle path in histogram name
    TPRegexp rpath(".*\\/");
    TPRegexp rbase("[^/]+$");
    TString path = name(rpath);
    TString basename = name(rbase);

    // Navigate to correct directory
    gDirectory->mkdir(path);
    gDirectory->cd(path);

    // Calculate number of bins
    int nbins = int((xmax - xmin) / xspacing);

    // Create histogram
    TH1F* hist = new TH1F(basename, title, nbins, xmin, xmax);
    plots_.push_back(hist);
    
    // Set axis titles
    hist->GetXaxis()->SetTitle(xaxis);
    hist->GetYaxis()->SetTitle(yaxis);

    // Un-navigate from directory
    gDirectory->cd("..");

    return hist;
}

TH2F* AnalysisTask::create_2d_histogram(TString name, 
                                        TString title, 
                                        TString xaxis, 
                                        TString yaxis,
                                        double xmin, double xmax, double xspacing,
                                        double ymin, double ymax, double yspacing)
{
    // Handle path in histogram name
    TPRegexp rpath(".*\\/");
    TPRegexp rbase("[^/]+$");
    TString path = name(rpath);
    TString basename = name(rbase);

    // Navigate to correct directory
    gDirectory->mkdir(path);
    gDirectory->cd(path);

    // Calculate number of bins
    int xbins = int((xmax - xmin) / xspacing);
    int ybins = int((ymax - ymin) / yspacing);

    // Create histogram
    TH2F* hist = new TH2F(basename, title, xbins, xmin, xmax, ybins, ymin, ymax);
    plots_.push_back(hist);
    
    // Set axis titles
    hist->GetXaxis()->SetTitle(xaxis);
    hist->GetYaxis()->SetTitle(yaxis);

    // Un-navigate from directory
    gDirectory->cd("..");

    return hist;
}

TProfile* AnalysisTask::create_2d_profile(TString name, 
                                      TString title, 
                                      TString xaxis, 
                                      TString yaxis,
                                      double xmin, double xmax, double xspacing,
                                      double ymin, double ymax)
{
    // Handle path in histogram name
    TPRegexp rpath(".*\\/");
    TPRegexp rbase("[^/]+$");
    TString path = name(rpath);
    TString basename = name(rbase);

    // Navigate to correct directory
    gDirectory->mkdir(path);
    gDirectory->cd(path);

    // Calculate number of bins
    int xbins = int((xmax - xmin) / xspacing);

    // Create histogram
    TProfile* prof = new TProfile(basename, title, xbins, xmin, xmax, ymin, ymax);
    plots_.push_back(prof);
    
    // Set axis titles
    prof->GetXaxis()->SetTitle(xaxis);
    prof->GetYaxis()->SetTitle(yaxis);

    // Un-navigate from directory
    gDirectory->cd("..");

    return prof;
}

void AnalysisTask::cd_grid(int tdc) const
{
    if (tdc < 3)
        canvas_->cd(tdc*3+1);
    else if (tdc < 5)
        canvas_->cd(tdc-1);
    else if (tdc < 7)
        canvas_->cd(tdc);
    else
        canvas_->cd(tdc+1);
}