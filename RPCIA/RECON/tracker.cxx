#include "tracker.h"

Tracker::Tracker()
{
}

Tracker::~Tracker()
{
}

void Tracker::find_tracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const
{
    for (auto tit = track_store->begin(); tit != track_store->end(); tit++)
    {
        
    }
}

void Tracker::find_pretracks(Store<Cluster> *cluster_store, Store<Track> *track_store) const
{
    track_store->clear();
    // Find segments
    for (auto cpit = cluster_store->begin(); cpit != cluster_store->end(); cpit++)
    {
        for (auto testpit = next(cpit); testpit != cluster_store->end(); testpit++)
        {
            if (should_segment(**cpit, **testpit))
            {
                Track track;
                track.add(*cpit);
                track.add(*testpit);
                track.init();
                track_store->add(track);
            }
        }
    }

    // Combine segments
    for (auto tpit = track_store->begin(); tpit != track_store->end(); tpit++)
    {
        for (auto testpit = next(tpit); testpit != track_store->end(); testpit++)
        {
            if (should_combine_segments(**tpit, **testpit))
            {
                (*tpit)->merge(**testpit);
                track_store->remove(testpit--);
            }
        }
    }
}

void Tracker::set_muon_digits(Store<Track> *track_store)
{
    for (auto tpit = track_store->begin(); tpit != track_store->end(); tpit++)
    {
        for (auto cpit = (*tpit)->begin_clusters(); cpit != (*tpit)->end_clusters(); cpit++)
        {
            for (auto dpit = (*cpit)->digits_begin(); dpit != (*cpit)->digits_end(); dpit++)
            {
                (*dpit)->set_muon(true);
            }
        }
    }
}

bool Tracker::should_combine_segments(const Track &t1, const Track &t2) const
{
    Double_t angle = t1.plane().Angle(t2.plane());
    if (angle < 0.5) // if angle is less than pi/6
    {
        return true;
    }
    return false;
}

bool Tracker::should_segment(const Cluster &c1, const Cluster &c2) const
{
    // Must not be on same layer apart
    if (abs(c1.position()[2] - c2.position()[2]) == 0) 
    {
        return false;
    }

    // Must be going in same direction to create track segment
    if (c1.position()[1] == 0 && c2.position()[1] != 0)
    {
        return false;
    }
    if (c1.position()[0] == 0 && c2.position()[0] != 0)
    {
        return false;
    }

    // Must not jump more than one strip
    if (abs(c1.position()[0] - c2.position()[0]) > 1)
    {
        return false;
    }
    if (abs(c1.position()[1] - c2.position()[1]) > 1)
    {
        return false;
    }
    
    // Otherwise return true
    return true;
}