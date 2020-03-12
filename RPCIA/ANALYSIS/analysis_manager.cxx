#include "analysis_manager.h"

AnalysisManager::AnalysisManager()
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
}

void AnalysisManager::add_task(AnalysisTask *task)
{
    task->connect_inputs(digit_store_, cluster_store_, track_store_);
    tasks_.push_back(task);
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
        task->execute();
    }
}

void AnalysisManager::terminate_tasks()
{
    for (auto &task : tasks_)
    {
        task->terminate();
    }
}