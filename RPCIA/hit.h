#if !defined(HIT_HPP)
#define HIT_HPP

#include <TObject.h>

class Hit : public TObject {
private:
    Int_t bcid_fpga;
	Int_t bcid_tdc;
	Int_t tdc;
	Int_t channel;
    Int_t strip;
	Int_t width;
	Int_t fine_time;
    Bool_t noise;

public:
    // Constructor
    Hit();
    Hit(Int_t tdc, Int_t channel, Int_t width, Int_t bcid_tdc, Int_t fine_time);

    // Print
    void Print(Option_t *option="") const;

    // For reading event in from stringstream
    friend std::istream& operator>>(std::istream &stream, Hit &hit);

    // Getters
    Int_t get_tdc() const;
    Int_t get_channel() const;
    Int_t get_strip() const;
    Int_t get_width() const;
    Int_t get_bcid_tdc() const;
    Int_t get_bcid_fpga() const;
    Int_t get_fine_time() const;
    Int_t get_time() const;
    Bool_t get_noise() const;

    // Set the fpga bcid receive time
    void set_bcid_fpga(int value);
    void set_noise(bool value);

    ClassDef(Hit, 1); // Hit structure
};

#endif // HIT_HPP