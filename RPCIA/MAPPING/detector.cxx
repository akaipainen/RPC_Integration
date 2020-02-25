#include "detector.h"

#include <unordered_map>

Detector Detector::instance_;

Detector::Detector()
{
    add_chamber(0, true, true, 0);
    add_chamber(1, true, true, 1);
    add_chamber(2, true, true, 2);
    add_chamber(0, false, true, 3, 4);
    add_chamber(1, false, true, 5, 6);
    add_chamber(2, false, true, 7, 8);
}

Detector::~Detector()
{
    for (auto &chamber : chambers_)
    {
        delete chamber;
    }
    
}

Detector & Detector::get()
{
    return instance_;
}

// ############# Implementations ##############

bool Detector::strip_adjacent_imp(int tdc1, int strip1, int tdc2, int strip2) const
{
    if (tdc_to_chamber_.at(tdc1) != tdc_to_chamber_.at(tdc2))
    {
        return false;
    }
    return tdc_to_chamber_.at(tdc1)->strip_adjacent(tdc1, strip1,
                                                    tdc2, strip2);
}

bool Detector::parallel_imp(int tdc1, int tdc2) const
{
    return tdc_to_chamber_.at(tdc1)->eta_direction() == tdc_to_chamber_.at(tdc2)->eta_direction();
}

bool Detector::tdc_direction_imp(int tdc) const
{
    return tdc_to_chamber_.at(tdc)->eta_direction();
}

TVector3 Detector::position_imp(int tdc, int strip) const
{
    TVector3 vec;
    Chamber *c = tdc_to_chamber_.at(tdc);
    vec[c->eta_direction()] = !c->first(tdc) * 32 + strip;
    vec[1 - c->eta_direction()] = -1;
    vec[2] = c->layer();
    return vec;
}

// ############# Static functions ##############

bool Detector::strip_adjacent(int tdc1, int strip1, int tdc2, int strip2)
{
    return Detector::get().strip_adjacent_imp(tdc1, strip1, tdc2, strip2);
}

bool Detector::parallel(int tdc1, int tdc2)
{
    return Detector::get().parallel_imp(tdc1, tdc2);
}

bool Detector::tdc_direction(int tdc)
{
    return Detector::get().tdc_direction_imp(tdc);
}

TVector3 Detector::position(int tdc, int strip)
{
    return Detector::get().position_imp(tdc, strip);
}

// ############# Helper functions ##############

void Detector::add_chamber(int layer, bool eta, bool bis7, int tdc1, int tdc2)
{
    Chamber *c = new Chamber(layer, eta, bis7, tdc1, tdc2);
    tdc_to_chamber_[tdc1] = c;
    if (tdc2 != -1)
    {
        tdc_to_chamber_[tdc2] = c;
    }
    chambers_.push_back(c);
}