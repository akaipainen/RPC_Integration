#if !defined(EVENT_BUILDER_HPP)
#define EVENT_BUILDER_HPP

#include <list>

#include <TFile.h>
#include <TTree.h>

#include "event.hpp"
#include "utils.hpp"

class EventBuilder {
private:
    TFile *root_file;  // TFile from external scope
    TTree *event_tree; // TTree file
    Event *event;      // Event object to fill TTree file with

    std::list<Event> master_events; // Master list of events
	utils::NoisyRolloverCounter event_counter; // Event counter to assign true event ids

public:
    // Constructor
    EventBuilder(TFile *root_file);

    // Add an event to the event builder
    void add_event(Event event);

    // Close the event builder
    void close();

private:
    // Write an event to the TFile
    void write_event();

};

#endif // EVENT_BUILDER_HPP
