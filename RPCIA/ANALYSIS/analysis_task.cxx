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