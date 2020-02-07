#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include <string>
#include <fstream>
#include <bitset>
#include <sstream>
#include <vector>

// #include "packet_reader.hpp"
#include "event.hpp"
#include "event_builder.hpp"
#include "histogram_builder.hpp"
#include "histograms.hpp"
// #include "tree_builder.hpp"

using namespace std;

void print_help() {
    cout << "-f, --file: Filename of network data (should be .dat)" << endl
         << "-p, --packets: Number of packets to process" << endl
         << "-m, --mode: Measurement mode (Should be `leading`, `pair`, or `trailing`)" << endl
         << "-n --no_trigger: Disable trigger" << endl
         << "-h​,​ --help​: Displays this help message" << endl;
}

struct Options {
    string file;
    int packets;
    int mode;
    bool trigger;
};

Options parse_options(int argc, char *argv[]) {
    opterr = true;
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "file",       required_argument, nullptr, 'f' },
        { "packets",    required_argument, nullptr, 'p' },
        { "mode",       required_argument, nullptr, 'm' },
        { "no_trigger", no_argument,       nullptr, 'n' },
        { "help",       no_argument,       nullptr, 'h' },
        { nullptr,      0,                 nullptr, '\0'}
    };

    Options options = {
        "",   // file
        0,    // packets
        0, // mode
        true  // trigger
    };
    
    while ((choice = getopt_long(argc, argv, "f:th", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'f':
                options.file = optarg;
                break;

            case 'p':
                options.packets = atoi(optarg);
                break;

            case 'm':
                options.mode = atoi(optarg);
                break;

            case 't':
                options.trigger = false;
                break;
                
            case 'h':
                print_help();
                exit(0);
                
            default:
                cerr << "Invalid option: " << choice << '\n';
                exit(1);
        }
    }
    
    return options;
};

int main(int argc, char *argv[]) {
    Options options = parse_options(argc, argv);

    ifstream file(options.file);
    if (!file.is_open()) {
        throw "File not found.";
    }

    TFile *root_file = new TFile("official.root", "recreate");

    EventBuilder eb(root_file);
    Event event;
    while(file >> event) {
        eb.add_event(event);
    }
    eb.close();

    HistogramBuilder hb(root_file, "tree");
    hb.add_hist(histogram::noise_rate_per_strip);
    hb.close();

    root_file->Close();

    return 0;
}