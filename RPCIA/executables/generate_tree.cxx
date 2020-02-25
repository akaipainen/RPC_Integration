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

#include <TTree.h>

// using json = nlohmann::json;

void print_help() {
    std::cout << "Usage: ./gen_tree config_file num_packets" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    Loader *loader = new Loader();
    loader->open("official_.root");
    TTree *tree = loader->get_tree_loader();

    Reconstructor rec;
    rec.set_input_file(argv[1]);
    rec.set_pair_mode(false);
    rec.set_tree(*tree);
    rec.run();

    loader->write();

    delete loader;

    return 0;
}