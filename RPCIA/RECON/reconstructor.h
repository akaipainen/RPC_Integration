#if !defined(RECONSTRUCTOR_H)
#define RECONSTRUCTOR_H

class DigitMaker;
class DigitStore;
// class TriggerStore;
// class TrackStore;
// class ClusterStore;
class RawReader;
class TTree;

class Reconstructor {
private:
    DigitMaker *digit_maker_;

    DigitStore *digit_store_;
    // TriggerStore *trigger_store_;
    // TrackStore *track_store_;
    // ClusterStore *cluster_store_;

public:
    Reconstructor();
    ~Reconstructor();

    // Convert raw data into digit
    void convert_digits(RawReader *raw_reader, DigitStore *digit_store) const;

    // Run reconstruction
    void reconstruct(RawReader *raw_reader, TTree *digits_tree) const;

    // Write the digit and cluster information to Tree
    void fill_tree(DigitStore *digit_store, TTree &digits_tree) const;

private:
    DigitStore * get_digit_store();
    DigitStore * get_trigger_store();

    void create_digit_maker();

};

#endif // RECONSTRUCTOR_H
