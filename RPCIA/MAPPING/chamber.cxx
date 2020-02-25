#include "chamber.h"

Chamber::Chamber(int layer, bool eta, bool bis7, int tdc_l, int tdc_r)
 : layer_(layer)
 , eta_direction_(eta)
 , bis7_(bis7)
{
    tdcs_[0] = tdc_l;
    tdcs_[1] = tdc_r;
}

bool Chamber::first(int tdc) const
{
    return tdcs_[0] == tdc;
}

bool Chamber::strip_adjacent(int tdc1, int strip1,
                       int tdc2, int strip2) const
{
    if (tdc1 == tdc2)
    {
        return abs(strip1 - strip2) < 2;
    }
    else if (tdc1 == tdcs_[0] && tdc2 == tdcs_[1])
    {
        return strip1 == 0 && strip2 == 31;
    }
    else if (tdc1 == tdcs_[1] && tdc2 == tdcs_[0])
    {
        return strip1 == 31 && strip2 == 0;
    }
    return false;
}

bool Chamber::eta_direction() const
{
    return eta_direction_;
}

bool Chamber::bis7() const
{
    return bis7_;
}

int Chamber::layer() const
{
    return layer_;
}