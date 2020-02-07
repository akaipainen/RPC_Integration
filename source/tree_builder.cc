#include <iostream>
#include <vector>

#include <TTree.h>
#include <TFile.h>
#include <TClassTable.h>

#include "event.hpp"
#include "tree_builder.hpp"
#include "event_builder.hpp"

TreeBuilder::TreeBuilder() : live_time(0) {
    file = new TFile("2test.root", "recreate");
    event_tree = new TTree("events", "Event information");

    event = new Event();

    event_tree->Branch("events_split", "Event", &event, 16000, 2);    // split
    event_tree->Branch("live_time", &live_time, "live_time/I");

    // event_tree->Branch("events_no_split", "Event", &event, 16000, 0); // no split
}

TreeBuilder::~TreeBuilder() {
    delete file;
}

void TreeBuilder::add_event(Event &_event) {
    event_builder.add_event(_event);
    
    if (event_builder.get_range() > 10) {
        Event _event = event_builder.get_event();
        event = &_event;
        event_tree->Fill();
    }
}

void TreeBuilder::write_tree() {
    // Write the remaining events to the tree
    while (event_builder.get_num_events() > 0) {
        Event _event = event_builder.get_event();
        event = &_event;
        event_tree->Fill();
    }
    
    event_tree->Write();
    file->Close();
}