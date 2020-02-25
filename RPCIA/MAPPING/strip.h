#if !defined(STRIP_H)
#define STRIP_H

#include <TVector3.h>

class Strip : public TObject
{
private:
    Int_t tdc_;
    Int_t strip_;
    Bool_t direction_;

    TVector3 position_;

public:
    Strip(Int_t tdc, Int_t strip);
    ~Strip();
};


#endif // STRIP_H
