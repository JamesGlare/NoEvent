

#include "evolution.h"

/// Some parameters for the run

// maybe you need some warm up time
const Time tSysStart = 0.0 ;

Evolution::Evolution(SpikeList* _spikeList, Topology* _topology, Statistics* _statistics, Time _tEnd)
: spikeList(_spikeList), topology(_topology), statistics(_statistics), tEnd(_tEnd){
    //ctor
    tSys = tSysStart ;
};

// This function initializes every neurons and adds its estimate
// to the eventList.
void Evolution::initSpikeList()
{
    Index n = topology->getNetSize() ;
    // iterate neurons
    for(Index i = 1 ; i <= n; i++)
    {
        Neuron* neuron = topology->getNeuron(i);
        Spike spike = {neuron->estimateSpikeTime()  + tSys, neuron->getPosition()} ;
        spikeList->addEstimate(spike) ;
    }
};

// The fundamental stepping function
// void eventStep
// This function receives the next event from
// the spikeList, carries out the spike at the neuron
// and the synapse. The spike is automatically deleted
// from the list, all receiving neurons give their new estimate,
// the internal system time is updated
// and the next step can be processed.
void Evolution::eventStep()
{
    // 1. Get the next spike event.
    Spike spike = spikeList->yieldNext() ;
    Time now = spike.t ; // now is the sum of the estimated time of the spikeevent + systemtime
    // 2. Go to the neuron.
    Neuron* spiking = topology->getNeuron( spike.i ) ;
    // 2.1 Reset this neuron.
    spiking->reset( now );
    // 3. Get the neighbor neurons receiving the spike.
    NeuroStack receivers = topology->getNeighbors( spike.i ) ;
    // 4. Now get the corresponding synapses
    SynapseStack edges = topology->getEdges(spike.i) ;
    //std::cout << "Spiking: "<< "Neuron " << spiking->getPosition() << " @ t=" << now << std::endl ;
    // 5. So now for each adjacent neuron, do the following steps
    for(Index j = 0 ; j < receivers.size(); j++)
    {
        // the trick is here, that both containers
        // are correpondingly sorted.
        Neuron* receiver = receivers[j] ;
        Synapse* corrEdge = edges[j];
        // 5.1 Get the postsynaptic potential
        // and automatically update the neurons state
        State EPSP = corrEdge->getPSP( now ) ;
        // 5.2 Now apply the spike to the neuron
        receiver->updateState(now, EPSP) ;
        // 5.3 Now ask for a new estimate of the receiving neuron.
        //std::cout << "Receiving: "<< "Neuron " << receiver->getPosition() << " @ EPSP=" << EPSP<< std::endl ;
        Spike spike = {receiver->estimateSpikeTime() + now , receiver->getPosition()} ;
        spikeList->addEstimate(spike) ;
        // 5.4 *************** ClampSTATISTICS ************
        statistics->addClampData(*receiver, now) ;
    }
    // 6. Ask the spiking neuron for a new estimate
    Spike newSpike = {spiking->estimateSpikeTime()+now, spiking->getPosition()};
    spikeList->addEstimate( newSpike ) ;
    // 7. SPIKE TRAIN STATISTICS AND Clamp Statistics.
    statistics->recReset(*spiking, now) ;
    statistics->addSpike(spike) ;
    // 8. update the system time
    tSys = now ;
}

/// Starts the simulation.
void Evolution::run()
{
    while(tSys < tEnd && spikeList->hasNext())
    {
        eventStep();
    }
};

Time Evolution::getSystemTime()
{
  return tSys ;
};

void Evolution::setSystemTime(Time _tSys)
{
    tSys = _tSys ;
};
Statistics* Evolution::getStatistics()
{
    return statistics ;
}
void Evolution::networkReset()
{
    spikeList->clearList();
    // reset neurons
    for(Index i=0; i < topology->getNetSize(); i++)
    {
        topology->getNeuron(i+1)->reset(0.0); // assuming continuously distributed neural indices & set time to zero
        SynapseStack synapses = topology->getEdges(i+1);
        for(Index k =0; k < synapses.size(); k++)
        {
          synapses[k]->reset(true);
        };
    }
};

