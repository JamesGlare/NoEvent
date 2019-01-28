
#ifndef STATISTICS_H
#define STATISTICS_H

#include "globalDefinitons.h"
#include "neuron.h"

/*****************************************************
* Statistics Module
* The statistics class is used to clamp the neurons,
* that is, record their time evolution, store and process
* it and write it to a file.
* Author: Jannes Gladrow, May 2012.
******************************************************/

// Special construct used
// in the statistics class.

// We use this data type to
// store the information
// of how to reconstruct
// the neurons temporal membrane fluctuation.
typedef struct ClampData
{
    Time t;
    State s;
} ClampData ;

typedef struct freqStat
{
    Time mean ;
    Time var ;
} freqStat;
typedef struct Bin
{
    Time binAverage;
    Index entries;
} Bin;
// A lot of containers to be declared
typedef std::vector<Spike> SpikeTrain; // a typical spike train (of hopefully only a single neuron)
typedef std::vector<SpikeTrain> NetworkSpikeTrain ; // The combined measurement of the whole network.
typedef std::vector<ClampData> ClampRecords ; // What do we meausure ? And when ?
typedef std::vector<ClampRecords> NetworkClampRecords ; // all ClampRecords of all neurons, which are clamped
typedef std::vector<Index> Indices ; // inidice container
typedef std::vector<freqStat> Frequencies ; // this vector contains all frequency information, sorted according to indices.
typedef std::vector<Bin> Bins ; // just a different name for vector of {double, Time}, the double is considered as probability.


class Statistics
{
    public:
        // Public Ctor
        Statistics();
        ~Statistics();
        // This function takes Neurons and clamps
        // them throughout the simulation.
        void clampNeuron( Neuron i ) ;
        void recSpikeTrain( Neuron i );

        void addSpike( Spike spike );
        void addClampData( Neuron& toClamp, Time tf);
        void recReset(Neuron& toClamp, Time tReset) ;
        void outSpikeTrain(std::ostream &stream, Index i);
        void outClampData(std::ostream &stream, Index i) ;
        void outAll(bool _yieldISI) ;

        // Functions especially designed for the regie class
        void writeAverageFreq(Time fIn, Time tEquib, unsigned int binNr, Time tEnd) ;
        void resetStatistics() ;

    private:
         // Binning analysis
        Bins binning(unsigned int binNr, const SpikeTrain* st, Time tEnd, Time tEquib);

        // a lot of containers ...
        NetworkSpikeTrain netSpikeTrains ;
        NetworkClampRecords netClampRecords;
        Indices whichToClamp ;
        Indices whichSpikeTrain ;

        Time tStart; // Time where recording starts

        bool neuronIsClamped(Index i);
        bool spikesAreRecorded(Index i);
};

#endif // STATISTICS_H
