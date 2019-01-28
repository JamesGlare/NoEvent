#include "globalDefinitons.h"
#include "topology.h"


#ifndef SPIKELIST_H
#define SPIKELIST_H


/***************************************
* Spikelist Class
* Sorted list as storage for the
* Spikes occuring in the network.
* Only one spike per Neuron is stored
* in the list, after a neuron has received a
* Spike, it automatically reports to the list
* the new estimate for the spiking.
****************************************/

typedef std::list<Spike> Spikes;

class SpikeList
{
    public:
        // Ctor
        SpikeList();

        // Add Estimate function
        // adds Spike s into the list
        // and calls sortSpikes.
        // If a spike with a
        // timestamp  that is already stored
        // is to be inserted
        // the function always prioritizes
        // the one already inserted and detains the inserted one.
        void addEstimate(Spike s);

        bool hasNext() ;
        // YieldNext function
        // Yields the next function
        // and deletes the returned spike from the list
        Spike yieldNext();
        // how many spikes are still to be processed ?
        unsigned int spikesToCome();
        // clear the list
        void clearList() ;

    private:
        // the central list
        Spikes spikes ;
        // Pointer to the topology class
        Topology* topology ;

        // Private methods
        void sortSpikes();
        void deleteSpike(Index k);
};

#endif // SPIKELIST_H
