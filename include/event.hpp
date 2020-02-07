#if !defined(EVENT_HPP)
#define EVENT_HPP

#include <sstream>

#include <TObject.h>
#include <TClonesArray.h>

#include "hit.hpp"

class Event : public TObject {
private:
    // Event specific
	Int_t id;
    Int_t mode;
	Int_t num_hits;
    Int_t bcid;
    Int_t felix_counter;

    TClonesArray hits;

public:
    // Constructor
    Event();
    Event(Int_t id, Int_t mode, Int_t num_hits);

    // Destructor
    ~Event();

    // Print
    void Print(Option_t *option="") const;

    // Getters
    Int_t get_id() const;
    Int_t get_mode() const;
    Int_t get_num_hits() const;
    Int_t get_num_noise_hits() const;
    Hit * get_hit(int i) const;

    // Setters
    void set_id(int value);

    // For reading event in from stringstream
    friend std::istream& operator>>(std::istream &stream, Event &event);

    // Merge two events together
    void merge(Event event);

    // Find the track hits and the noise hits
    void find_track();

    ClassDef(Event, 1); // Event structure
};

#endif // EVENT_HPP