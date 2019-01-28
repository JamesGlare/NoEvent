#include "regie.h"

Regie::Regie(Evolution* _evolution)
: evolution(_evolution) {
    //ctor

};

void Regie::frequencyAnalysis(Time fStart, Time fEnd, Time fStep, Time tEquib, Time tEnd, unsigned int binNr, TypeZero* pumpNeuron, bool verbose)
{
    // Step 1: Reset the network

    // Step 2: Set frequency of pump Neuron
    Time freq = fStart ;
    Statistics* stats = evolution->getStatistics() ;
    while( freq <= fEnd )
    {
        // Reset the network
        evolution->setSystemTime( 0.0 );
        evolution->networkReset();
        stats->resetStatistics() ;
        // set the frequency for the neuron.
        pumpNeuron->setFreq(freq);
        // Initialize the spikeList
        evolution->initSpikeList() ;
        // run this simulation
        evolution->run() ;
        // write the average frequencies
        stats->writeAverageFreq(freq, tEquib,binNr,tEnd) ;
        freq+= fStep; // increment frequency
        if(verbose)
            stats->outAll(true) ;
    };
};
