#if !defined(CHAMBER_H)
#define CHAMBER_H

#include <vector>

class Chamber
{
private:
    int layer_;
    bool eta_direction_;
    bool bis7_;
    int tdcs_[2]; // second index is -1 if alone
    
public:
    Chamber(int layer, bool eta, bool bis7, int tdc_l, int tdc_r = -1);

    // returns if this tdc comes first in the order
    bool first(int tdc) const;

    // Returns true if two strips are adjacent
    // Accounting for tdcs next to each other
    bool strip_adjacent(int tdc1, int strip1,
                        int tdc2, int strip2) const;

    // Get the direction (is eta?)
    bool eta_direction() const;

    // Get the sub detector (is bis7?)
    bool bis7() const;

    // Get the layer
    int layer() const;

private:

};

#endif // CHAMBER_H