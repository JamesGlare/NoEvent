#include "globalDefinitons.h"
#include "spikelist.h"
#include "topology.h"

#ifndef EVOLUTION_H
#define EVOLUTION_H

/**************************************************************************
* Evolution Class
* Initializes the spikelist at the beginning of the simulation,
* then works through the spikelist and keeps it up to date during
* the simulation. It carries out the spikes, calculating the spikestrength
* with the help of the synpase table and the neurons stored in topology.
*************************************************************************/


class Evolution
{
    public:
        Evolution(SpikeList* spikeList, Topology* topology, Statistics* statistics, Time tEnd);

        // Initialize the SpikeList
        void initSpikeList() ;

        // Global network reset
        void networkReset();
        //simulate
        void run() ;

        // Getter & setter function for the system time
        Time getSystemTime();
        void setSystemTime(Time tSys) ;

        Statistics* getStatistics();

    private:
        // Simulation specific variables
        Time tSys ;
        Time tEnd ;
        Topology* topology ;
        SpikeList* spikeList ;
        Statistics* statistics ;
        // workStep
        void eventStep() ;
};

#endif // EVOLUTION_H
