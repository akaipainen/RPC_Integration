#if !defined(RECONSTRUCTOR_H)
#define RECONSTRUCTOR_H

#include <vector>

#include <TTree.h>

#include "digit_maker.h"
#include "cluster_finder.h"
#include "tracker.h"

#include "store.h"
#include "raw_reader.h"
#include "raw_decoder.h"

#include "digit.h"
#include "cluster.h"
#include "track.h"

class Reconstructor {
private:
    RawReader *raw_reader_;
    
    DigitMaker *digit_maker_;
    ClusterFinder *cluster_finder_;
    Tracker *tracker_;

    Store<Digit> *digit_store_;
    Store<Cluster> *cluster_store_;
    Store<Track> *track_store_;

    TTree *tree_;

    std::vector<int> *noisy_strips_;

public:
    Reconstructor();
    ~Reconstructor();

    // Set input file
    void set_input_file(const char *dat_file);

    // Set pair mode
    void set_pair_mode(const bool val);

    // Set TTree to output to
    void set_tree(TTree &tree);

    // Add a noisy Strip to ignore in reconstruction and analysis
    void add_noise(const int tdc, const int strip);

    // Add a noisy Strip to ignore in reconstruction and analysis
    void add_noise(const int strip_id);

    // Run full reconstruction and save to tree
    void run();

};

#endif // RECONSTRUCTOR_H
