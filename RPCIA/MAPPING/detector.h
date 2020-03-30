#if !defined(DETECTOR_H)
#define DETECTOR_H

#include <vector>
#include <unordered_map>

#include <TVector3.h>

#include "chamber.h"

class Detector
{
private:
    static Detector instance_;

    std::vector<Chamber *> chambers_;
    std::unordered_map<int, Chamber *> tdc_to_chamber_;
    
public:
    // Delete the copy and assignemnt operators
    Detector(Detector const& copy) = delete;
    Detector & operator=(Detector const &copy) = delete;

    // Get the singleton instance of Detector
    static Detector & get();

    // Returns whether the two strips are adjacent
    static bool strip_adjacent(int tdc1, int strip1,
                               int tdc2, int strip2);

    // Returns whether two tdcs are parallel
    static bool parallel(int tdc1, int tdc2);

    // Get the direction of the tdc
    static bool tdc_direction(int tdc);

    // Get the layer of the tdc
    static int tdc_layer(int tdc);

    // Get the absolute position of the tdc/strip
    static TVector3 position(int tdc, int strip);

private:
    Detector();
    ~Detector();

    bool strip_adjacent_imp(int tdc1, int strip1, int tdc2, int strip2) const;
    bool parallel_imp(int tdc1, int tdc2) const;
    bool tdc_direction_imp(int tdc) const;
    int tdc_layer_imp(int tdc) const;
    TVector3 position_imp(int tdc, int strip) const;
    
    // Add a tdc/chamber combination
    void add_chamber(int order, bool eta, bool bis7, int tdc1, int tdc2=-1);
};


#endif // DETECTOR_H
