#include "strip.h"

#include "detector.h"

Strip::Strip(Int_t tdc, Int_t strip)
 : tdc_(tdc)
 , strip_(strip)
{
    direction_ = Detector::tdc_direction(tdc);
    position_ = Detector::position(tdc, strip);
}

Strip::~Strip()
{
}