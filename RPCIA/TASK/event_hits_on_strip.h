#if !defined(EVENT_HITS_ON_STRIP_H)
#define EVENT_HITS_ON_STRIP_H

#include "analysis_task.h"

#include <vector>
#include <unordered_map>

#include <TCanvas.h>
#include <TH2F.h>

class EventHitsOnStrip : public AnalysisTask
{
private:
    TCanvas *canvas_;

    std::vector<TH2F *> tdc_;

public:
    EventHitsOnStrip();
    ~EventHitsOnStrip();

    void create_outputs();

    void execute();

    void terminate();
};


#endif // EVENT_HITS_ON_STRIP_H
