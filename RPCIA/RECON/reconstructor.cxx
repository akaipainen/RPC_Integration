#include "reconstructor.h"

#include <TTree.h>

Reconstructor::Reconstructor() {
    raw_reader_ = new RawReader();

    digit_maker_ = new DigitMaker();
    cluster_finder_ = new ClusterFinder();
    tracker_ = new Tracker();
    
    digit_store_ = new Store<Digit>();
    cluster_store_ = new Store<Cluster>();
    track_store_ = new Store<Track>();
}

Reconstructor::~Reconstructor() {
    delete raw_reader_;

    delete digit_maker_;
    delete cluster_finder_;
    delete tracker_;
    
    delete digit_store_;
    delete cluster_store_;
    delete track_store_;
}

void Reconstructor::set_input_file(const char *dat_file)
{
    raw_reader_->open(dat_file);
    raw_reader_->next(); // Initialize raw_reader
}

void Reconstructor::set_pair_mode(const bool val)
{
    RawDecoder::set_pair_mode(false);
}

void Reconstructor::set_tree(TTree &tree)
{
    tree_ = &tree;
    digit_store_->connect(tree);
    cluster_store_->connect(tree);
    track_store_->connect(tree);
}

void Reconstructor::run()
{
    while (!raw_reader_->is_done())
    {
        // Load one event into the reconstructor
        digit_maker_->raw_to_digits(raw_reader_, digit_store_); // fill digit_store_

        // Find pre clusters for tracking
        cluster_finder_->make_preclusters(digit_store_, cluster_store_); // fill cluster_store_
        
        // Track the preclusters to find pretracks
        tracker_->find_pretracks(cluster_store_, track_store_);

        // Find track intersections to determine muons
        // Merge common tracks
        // Merge common clusters
            // complete track_store_
            // complete cluster_store_

        // Set the muon digits
        tracker_->set_muon_digits(track_store_);

        tree_->Fill();
    }
    
}

// void Reconstructor::reconstruct(RawReader *raw_reader, TTree *digits_tree)
// {
//     fill_tree(get_trigger_store(), *digits_tree);
// }
