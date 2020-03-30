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
    for (auto cit = cluster_store->begin(); cit != cluster_store->end(); cit++)
    {
        for (auto testit = next(cit); testit != cluster_store->end(); testit++)
        {
            if (should_segment(*cit, *testit))
            {
                Track track;
                track.add(cit.get());
                track.add(testit.get());
                track.init();
                track_store->add(track);
            }
        }
    }

    // Combine segments
    for (auto tit = track_store->begin(); tit != track_store->end(); tit++)
    {
        for (auto testit = next(tit); testit != track_store->end(); testit++)
        {
            if (should_combine_segments(*tit, *testit))
            {
                tit->merge(*testit);
                track_store->remove(testit--);
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