#if !defined(TRACK_H)
#define TRACK_H

#include <vector>

#include <TVector3.h>

#include "cluster.h"

class Track : public TObject
{
private:
    Int_t unique_id_;
    static Int_t id_counter_;

    std::vector<Cluster> clusters_;
    Int_t num_clusters_;

    std::vector<Double_t> data_x_; // one dimension of data
    std::vector<Double_t> data_z_; // height (layer)

    Bool_t two_coords_;
    Bool_t direction_;

    Bool_t has_plane_;
    TVector3 plane_;

public:
    Track();

    // Get the trigger id
    Int_t trigger_id() const;

    // Get the direction 
    // If has two coordinates, return -1
    Int_t direction() const;

    // Add a cluster to this track
    void add(const Cluster &cluster);

    // Calculate the plane of this pretrack
    void init();
    
    // Get the plane this pretrack is in
    TVector3 plane() const;

    // Merge two vectors together
    void merge(const Track &track);

private:
    // void delete_data();
    // void fill_position(const TVector3 &position);

    ClassDef(Track, 1);

};


#endif // TRACK_H
