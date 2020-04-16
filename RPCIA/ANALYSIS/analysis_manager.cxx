#include "analysis_manager.h"

AnalysisManager::AnalysisManager()
 : outdir_("output")
{
    digit_store_ = new Store<Digit>();
    cluster_store_ = new Store<Cluster>();
    track_store_ = new Store<Track>();
}

AnalysisManager::~AnalysisManager()
{
    delete digit_store_;
    delete cluster_store_;
    delete track_store_;
}

void AnalysisManager::init(TTree *tree)
{
    tree_ = tree;
    digit_store_->connect(*tree_);
    cluster_store_->connect(*tree_);
    track_store_->connect(*tree_);

    // Set global styles
    gStyle->SetHistMinimumZero();
    // gStyle->SetOptStat(0);
    // gStyle->SetOptFit(0);
}

void AnalysisManager::add_task(AnalysisTask *task)
{
    task->connect_inputs(digit_store_, cluster_store_, track_store_);
    tasks_.push_back(task);

    task->set_output_dir(outdir_);
    task->init_task();
}

void AnalysisManager::set_output_dir(const char* outdir)
{
    outdir_ = outdir;
}

void AnalysisManager::run()
{
    for (int i = 0; i < tree_->GetEntries(); i++)
    {
        tree_->GetEntry(i);
        execute_tasks();
    }
    terminate_tasks();
}

void AnalysisManager::execute_tasks()
{
    for (auto &task : tasks_)
    {
        task->execute_task();
    }
}

void AnalysisManager::terminate_tasks()
{
    for (auto &task : tasks_)
    {
        task->terminate_task();
    }
}