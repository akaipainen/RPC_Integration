#include <iterator>

#include "event_builder.hpp"

EventBuilder::EventBuilder(TFile *root_file)
 : root_file(root_file), event_counter(256) {
    event_tree = new TTree("tree", "Event information"); 
    event = 0;
    event_tree->Branch("events", "Event", &event, 16000, 2); // Branch for events
}

void EventBuilder::add_event(Event event) {
    event.set_id(event_counter.get(event.get_id()));

    if (master_events.empty() || event.get_id() > master_events.back().get_id()) {
        master_events.push_back(event);
    } else {
        auto it = master_events.end();
        do {
            std::advance(it, -1);
        } while (it->get_id() > event.get_id());

        // Check to see if this id exists
        if (it->get_id() == event.get_id()) {
            it->merge(event);
        } else {
            master_events.insert(next(it), event);
        }
    }

    // Keep master events small by writing after 10 event ids have passed
    if (master_events.back().get_id() - master_events.front().get_id() > 10) {
        write_event();
    }
}

void EventBuilder::close() {
    // Write the remaining events to the TTree
    while (master_events.size() > 0) {
        write_event();
    }
    event_tree->Write(); // Write the event_tree to the TFile
}

void EventBuilder::write_event() {
    Event _event = master_events.front();
    master_events.pop_front();
    _event.find_track(); // Find the muons/noise in event
    event = &_event; // Assign event filler to current event
    event_tree->Fill(); // Fill the event in the TTree
}
