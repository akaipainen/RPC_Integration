#include <sstream>
#include <iostream>

#include "event.hpp"

ClassImp(Event);

Event::Event() : id(0), mode(0), num_hits(0), hits("Hit", 0) { }

Event::Event(Int_t id, Int_t mode, Int_t num_hits)
: id(id), mode(mode), num_hits(num_hits), hits("Hit", num_hits) { }

Event::~Event() {
    hits.Clear();
}

void Event::Print(Option_t *) const {
    printf("Event ID: %5d | Num. Hits: %2d | Mode: %5d\n", 
           get_id(), get_num_hits(), get_mode());
    hits.Print();
}

Int_t Event::get_id() const { return id; }
Int_t Event::get_mode() const { return mode; }
Int_t Event::get_num_hits() const { return num_hits; }
Int_t Event::get_num_noise_hits() const { 
    int num = 0;
    for (size_t i = 0; i < num_hits; i++) {
        Hit *hit = (Hit *) hits[i];
        num += hit->get_noise();
    }
    return num;
}
Hit * Event::get_hit(int i) const {
    return (Hit *) hits[i];
}

void Event::set_id(int value) { id = value; }

std::istream& operator>>(std::istream &stream, Event &event) {    
    std::string word;
    // Find the start of the next packet
    while (stream >> word) {
        if (word == ">>>") break;
    }

    if (stream.eof()) {
        return stream;
    }
    

    // If failed to find start of new packet, quit
    if (word != ">>>") {
        stream.setstate(std::ios::failbit);
        return stream;
    }
    
    // Get the number of words in data packet
    // Word will be something like "size=24"
    for (int i = 0; i < 4; ++i) {
        stream >> word;
    }
    event.num_hits = (std::stoi(word.substr(5, word.size() - 5)) - 20) / 4;
    // std::cout << "Word: " << word << " | Num: " << event.num_hits << std::endl;

    // Skip packet header to get to data
    for (int i = 0; i < 13; ++i) {
        stream >> word;
    }

    // Clear previous data in event
    event.hits.Clear();

    // For each hit, collect data
    Hit hit;
    for (int i = 0; i < event.num_hits; ++i) {
        stream >> hit;
        
        Hit *_hit = (Hit *) event.hits.ConstructedAt(i);
        std::swap(*_hit, hit);
    }

    // Collect the FPGA data in final bits
    unsigned long b1, b2, b3, b4, b5;
    stream >> std::hex >> b1 >> b2 >> b3 >> b4 >> b5;
    event.id = b2;
    event.bcid = b3 << 4 | b4;
    event.felix_counter = b5;

    for (int i = 0; i < event.num_hits; i++) {
        ((Hit *) event.hits[i])->set_bcid_fpga(event.bcid);
    }

    return stream;
}

void Event::merge(Event event) {
    assert(id == event.id);
    assert(mode == event.mode);
    
    for (int i = 0; i < event.num_hits; ++i) {
        Hit *hit = ((Hit *) event.hits[i]);
        Hit *_hit = (Hit *) hits.ConstructedAt(i + num_hits);
        std::swap(*hit, *_hit);
    }
    num_hits += event.num_hits;
    event.hits.Clear();
}

void Event::find_track() {
    for (int i = 0; i < num_hits; ++i) {
        Hit *hit1 = ((Hit *) hits[i]);
        for (int j = i + 1; j < num_hits; ++j) {
            Hit *hit2 = ((Hit *) hits[j]);

            if (abs(hit1->get_strip() - hit2->get_strip()) <= 1) { // Spatial correlation within 1 strip
                if (abs(hit1->get_time() - hit2->get_time()) < 50) { // Time correlation within 50 * 0.195 ~ 10 ns
                    hit1->set_noise(false);
                    hit2->set_noise(false);
                }
            }
        }
    }
}