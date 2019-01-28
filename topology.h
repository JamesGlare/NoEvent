
#include "globalDefinitons.h"
#include "neuron.h"
#include "synapse.h"

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

/***************************************************
* Topology Class
* "Knows" about the structure of the network.
* Holds pointers to all neurons and synapses.
* The objects have to be created (and stored)
* outside of the networking algorithms, e.g.
* in the main function.
****************************************************/
class Topology
{
    public:
        // public constructor
        Topology( NeuroStack _neurons,  SynapseStack _synapses) ;
        // Dtor
        ~Topology();
        // getNeighbors functions: Yields pointers to all neighbouring neurons.
        NeuroStack getNeighbors(Index position);
        // get a pointer to neuron with index i
        Neuron* getNeuron(Index i);
        // get all synapses directed from i->j
        SynapseStack getEdges(Index i);
        // get the size of the network
        Index getNetSize();
        Index getSynNr();

    private:
         Index** adjacencyMatrix ;
         NeuroStack neurons;
         SynapseStack synapses ;
         Index netSize;
         Index synNr ;
};

#endif // TOPOLOGY_H
