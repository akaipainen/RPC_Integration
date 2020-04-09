// #include <cstdlib>
#include <iostream>
// #include <getopt.h>
// #include <string>
// #include <fstream>
// #include <bitset>
// #include <sstream>
// #include <vector>

// #include "nlohmann/json.hpp"

// #include "event.h"
// #include "event_builder.h"
// #include "histogram_builder.h"
// #include "histograms.h"
// #include "utils.h"

#include "loader.h"
#include "reconstructor.h"
#include "analysis_manager.h"

#include "time_difference_from_average.h"
#include "time_difference_vs_distance.h"
#include "extra_hits_on_strip.h"
#include "hit_distribution.h"
#include "muon_distribution.h"
#include "noise_distribution.h"
#include "event_hits_on_strip.h"
#include "cluster_size.h"
#include "event_display.h"

#include <TTree.h>

// using json = nlohmann::json;

void print_help() {
    std::cout << "Usage: ./gen_tree config_file num_packets" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    Loader *loader = new Loader();
    loader->open("official_leading.root");
    TTree *tree = new TTree("tree", "Tree info");

    Reconstructor rec;
    rec.set_input_file(argv[1]);
    rec.set_pair_mode(std::atoi(argv[2]));
    rec.set_tree(*tree);
    // rec.add_noise(169);
    // rec.add_noise(233);
    rec.run();

    AnalysisManager mgr;
    mgr.init(tree);
    std::vector<double> *time_offsets = new std::vector<double>();
    std::vector<int> *event_display = new std::vector<int>{251, 273};
    double run_duration = 60; //seconds
    mgr.add_task(new HitDistribution("hit_distribution", 60));
    mgr.add_task(new MuonDistribution("muon_distribution", 60));
    mgr.add_task(new NoiseDistribution("noise_distribution"));
    mgr.add_task(new TimeDifferenceFromAverage("time_diff_avg", nullptr, time_offsets));
    mgr.add_task(new EventDisplay("event_display", event_display));
    mgr.add_task(new ClusterSize("cluster_size"));
    mgr.add_task(new TimeDifferenceVsDistance("time_difference_vs_distance"));
    mgr.add_task(new ExtraHitsOnStrip("extra_hits_on_strip"));
    mgr.add_task(new EventHitsOnStrip("event_hits_on_strip"));
    // mgr.add_task(new IsolateNoise());
    mgr.run();

    AnalysisManager mgr2;
    mgr2.init(tree);
    mgr2.add_task(new TimeDifferenceFromAverage("time_diff_avg_offset", time_offsets, nullptr));
    
    mgr2.run();

    loader->write();

    delete loader;

    return 0;
}