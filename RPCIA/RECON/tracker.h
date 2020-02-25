#if !defined(TRACKER_H)
#define TRACKER_H

#include "store.h"
#include "cluster.h"
#include "track.h"

class Tracker
{
private:
    

public:
    Tracker();
    ~Tracker();

    void find_tracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const;
    void find_pretracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const;

private:
    // Returns whether two clusters should form a segment
    bool should_segment(const Cluster &c1, const Cluster &c2) const;

    // Returns whether two segments should merge into a single track
    bool should_combine_segments(const Track &t1, const Track &t2) const;
};


#endif // TRACKER_H
