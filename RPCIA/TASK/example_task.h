#if !defined(EXAMPLE_TASK_H)
#define EXAMPLE_TASK_H

#include "analysis_task.h"

#include <vector>

#include <TH1.h>

class ExampleTask : public AnalysisTask
{
    std::vector<TH1F *> tdc_;

    // Change these to set size of printed canvas
    static const int w = 1;
    static const int h = 1;

    // Uncomment and replace this if you need to feed input or return output from this task
    // std::vector<Digit>* input_;
    // std::vector<Digit>* output_;

public:
    ExampleTask(const char *name, void* input=nullptr, void* output=nullptr)
     : AnalysisTask(name, 500*w, 300*h)
    {
    }

    ~ExampleTask()
    {
        for (auto tdc = 0; tdc < 9; tdc++)
        {
            delete tdc_[tdc];
        }
    }

    void init()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // TODO: Replace this with creating your histograms
        }
    }

    void execute()
    {
        // TODO: Replace this with filling your analysis task
    }

    void terminate()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // Draw your histograms to the canvas using ->Draw()
            // Set other visual options here

            canvas_->Print(Form("%s/%s/tdc_%d.pdf", outdir_, name_, tdc)); // Save canvas to pdf
            canvas_->Clear("D"); // Clear the canvas for each TDC
        }
    }
};


#endif // EXAMPLE_TASK_H
