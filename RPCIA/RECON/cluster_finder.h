#if !defined(CLUSTER_FINDER_H)
#define CLUSTER_FINDER_H

#include <vector>

#include "cluster.h"
#include "digit.h"
#include "store.h"

// Finds adjacent strips with hits 
// to find pre clusters in a single direction
//
// Full clusters will be found after associating tracks
// from each direction
class ClusterFinder
{
public:
    ClusterFinder();
    ~ClusterFinder();

    // Make the preclusters in the cluster store
    void make_preclusters(Store<Digit> *digit_store, Store<Cluster> *cluster_store) const;

private:
    // bool should_add(const Cluster& cluster, const Digit &digit) const;
    bool satisfy_constraints(const Digit &digit1, const Digit &digit2) const;
    
};

#endif // CLUSTER_FINDER_H
