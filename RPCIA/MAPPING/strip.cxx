#include "strip.h"

#include "detector.h"

Strip::Strip(Int_t tdc, Int_t strip)
 : tdc_(tdc)
 , strip_(strip)
{
}

Strip::~Strip()
{
}

Int_t Strip::tdc()
{
    return tdc_;
}

Int_t Strip::strip()
{
    return strip_;
}

Bool_t Strip::direction()
{
    return Detector::tdc_direction(tdc_);
}

TVector3 Strip::position()
{
    return Detector::position(tdc_, strip_);
}