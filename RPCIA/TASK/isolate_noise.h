#if !defined(ISOLATE_NOISE_H)
#define ISOLATE_NOISE_H

#include <vector>

#include <TH1.h>

#include "analysis_task.h"

class IsolateNoise : public AnalysisTask
{
private:
    std::vector<TH1F *> widths;

public:
    IsolateNoise();
    ~IsolateNoise();

    virtual void execute();
    virtual void terminate();
};


#endif // ISOLATE_NOISE_H
