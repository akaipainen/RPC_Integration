#include "tracker.h"

#include <iostream>

Tracker::Tracker()
{
}

Tracker::~Tracker()
{
}

void Tracker::find_tracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const
{
}

void Tracker::find_pretracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const
{
    track_store->clear();

    // Find segments
    for (auto cit = cluster_store->begin(); cit != cluster_store->end(); cit++)
    {
        for (auto testcit = next(cit); testcit != cluster_store->end(); testcit++)
        {
            if (should_segment(*cit, *testcit))
            {
                Track track;
                track.add(cit.get());
                track.add(testcit.get());
                track.init();
                track_store->add(track);
            }
        }
    }


    // Combine segments
    for (auto tit = track_store->begin(); tit != track_store->end(); tit++)
    {
        for (auto testtit = next(tit); testtit != track_store->end(); testtit++)
        {
            if (should_combine_segments(*tit, *testtit))
            {
                tit->merge(*testtit);
                track_store->remove(testtit--);
            }
        }
    }
}

void Tracker::set_muon_digits(Store<Track> *track_store)
{
    for (auto tit = track_store->begin(); tit != track_store->end(); tit++)
    {
        for (auto cit = tit->begin_clusters(); cit != tit->end_clusters(); cit++)
        {
            for (auto dit = cit->begin_digits(); dit != cit->end_digits(); dit++)
            {
                dit->set_muon(true);
            }
        }
    }
}

bool Tracker::should_combine_segments(const Track &t1, const Track &t2) const
{
    // if angle between track trajectories is greater than pi/6 (30 degrees), return false
    Double_t angle = t1.plane().Angle(t2.plane());
    if (angle > 0.5) 
    {
        return false;
    }

    // Merging two tracks must share at least one cluster
    // This will successfully combine all clusters since 
    // we only have three layers for clusters to be on
    // and two overlapping track segments will share a track
    // on one layer
    bool share_a_cluster = false; 
    for (auto tit = t1.begin_clusters(); tit != t1.end_clusters(); tit++)
    {
        for (auto testtit = t2.begin_clusters(); testtit != t2.end_clusters(); testtit++)
        {
            // If clusters are the same pointer
            if (tit.get() == testtit.get())
            {
                share_a_cluster = true;
                break;
            }
        }
        if (share_a_cluster) break;
    }
    if (!share_a_cluster) return false;

    return true;
}

bool Tracker::should_segment(const Cluster &c1, const Cluster &c2) const
{
    // cluster.position yields (eta strip, phi strip, layer) vector

    // Clusters must be on different layers
    if (abs(c1.position()[2] - c2.position()[2]) == 0) 
    {
        return false;
    }

    // Clusters must be in same direction
    if (c1.position()[1] == 0 && c2.position()[1] != 0)
    {
        return false;
    }
    if (c1.position()[0] == 0 && c2.position()[0] != 0)
    {
        return false;
    }

    // Clusters' center of charge must not 
    // be more than one strip apart
    if (abs(c1.position()[0] - c2.position()[0]) >= 2)
    {
        return false;
    }
    if (abs(c1.position()[1] - c2.position()[1]) >= 2)
    {
        return false;
    }
    
    // Otherwise return true
    return true;
}