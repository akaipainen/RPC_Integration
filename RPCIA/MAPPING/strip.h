#if !defined(STRIP_H)
#define STRIP_H

#include <TVector3.h>

class Strip : public TObject
{
private:
    Int_t tdc_;
    Int_t strip_;

public:
    Strip(Int_t tdc, Int_t strip);
    ~Strip();

    bool operator==(const Strip &rhs)
    {
        return tdc_ == rhs.tdc_ && strip_ == rhs.strip_;
    }

    bool operator!=(const Strip &rhs)
    {
        return !operator==(rhs);
    }

    Int_t tdc();
    Int_t strip();
    Bool_t direction();
    TVector3 position();

};


#endif // STRIP_H
