#if !defined(STRIP_H)
#define STRIP_H

#include <TObject.h>

#include "digit.h"

// Combines Digit and ChamberConfig data
// to provide information about a hit strip
class Strip : public TObject
{
public:
    enum Direction
    {
        ETA = 0,
        PHI = 1
    };

private:
    Int_t tdc_;
    Int_t layer_;
    Int_t channel_;
    Int_t strip_;
    Int_t width_raw_; // raw value from tdc
    Direction direction_;
    Float_t width_;   // computed value from resolution

public:
    Strip();
    Strip(Int_t tdc, Int_t channel, Int_t width);
    Strip(const Digit &digit);
    ~Strip();

    // Returns whether two strips are neighbors
    static Bool_t are_neighbors(const Strip &first, const Strip &second);

    // Returns whether two strips are parallel
    static Bool_t are_parallel(const Strip &first, const Strip &second);

    // Return tdc
    Int_t tdc() const;

    // Return tdc channel number
    Int_t channel() const;
    
    // Return chamber strip number
    Int_t strip() const;
    
    // Return raw width of pulse from tdc
    Int_t width_raw() const;
    
    // Return  direction of this strip (ETA or PHI)
    Direction direction() const;
    
    // Return width of pulse in ns
    Float_t width() const;


};

#endif // STRIP_H
