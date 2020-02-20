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

#include "raw_reader.h"
#include "loader.h"
#include "digit_store.h"
#include "digit_maker.h"

#include <TTree.h>

// using json = nlohmann::json;

void print_help() {
    std::cout << "Usage: ./gen_tree config_file num_packets" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    RawReader *raw_reader = new RawReader();
    raw_reader->open(argv[1]);
    raw_reader->next();

    Loader *loader = new Loader();
    loader->open("official_.root");
    TTree *tree = loader->get_tree_loader();

    DigitStore *digit_store = new DigitStore(); // Stores hits of a single event
    digit_store->connect(*tree); // Connect to the branch of TTree

    // ClusterStore *cluster_store = ClusterStore::create();
    // cluster_store->connect(tree);
    
    DigitMaker *digit_maker = new DigitMaker(); 
    while (!raw_reader->is_done()) { // Has more events to load
        digit_maker->raw_to_digits(raw_reader, digit_store); // Load digits into event
        
        tree->Fill(); // Fill event into tree
        digit_store->clear();
    }
    loader->write();

    delete loader;
    delete raw_reader;
    delete digit_maker;

    return 0;
}