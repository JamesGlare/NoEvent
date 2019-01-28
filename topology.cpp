#include "topology.h"

// Sorting algorithm for neuroStacks
bool neuroSort(Neuron* i, Neuron* j)
{
    return (*i).getPosition() < (*j).getPosition() ;
};
bool synapseSort(Synapse* i, Synapse* j)
{
    Edge iEdge = (*i).getEdge() ;
    Edge jEdge = (*j).getEdge() ;

    if (iEdge.i < jEdge.i)
        return true ;
    else if (iEdge.i== jEdge.i)
        return iEdge.j < jEdge.j ;
    else
        return false ;
};

Topology::Topology(  NeuroStack _neurons,  SynapseStack _synapses )
{
    // This constructor has to construct an adjancency matrix
    // for convenience out of the given neuron and synapse matrices.

    // save the size of the stack
    netSize = _neurons.size() ;
    synNr = _synapses.size() ;

    // Create the vector<Neuron*> instances
    // create also the vector<Synapse*> instances
    NeuroStack t ;
    SynapseStack s;
    neurons = t;
    synapses = s;

    // sort the neurostack according to sorting function
    std::sort(_neurons.begin(), _neurons.end(), neuroSort) ;
    neurons = _neurons ; // FLAT COPIES OF THE NEURONS !

    // now sort the synapse vector
    std::sort(_synapses.begin(), _synapses.end(), synapseSort) ;
    synapses =_synapses ; // FLAT COPIES OF THE SYNAPSES!!!!

    // create adjacencymatrix, since its a dynamically constructed
    // 2d array we have to create it step by step.
    adjacencyMatrix = new Index* [netSize];
    for(Index i =0; i < netSize; i++)
    {
        adjacencyMatrix[i] = new Index[netSize] ;
    }

    // Initialize the whole matrix with zeroes.
    for(Index j = 0; j < netSize; j++)
        for(Index k = 0; k < netSize; k++)
        adjacencyMatrix[j][k] = 0 ;

    // Go through the synapses
    for(Index i = 0 ; i < synNr; i++)
    {
        Synapse syn = *(_synapses[i]) ;

        Edge edge = syn.getEdge() ;

        adjacencyMatrix[edge.i-1][edge.j-1] = 1 ;
    }

};

// We have to define a destructor explicitly for this
// class.
Topology::~Topology()
{
    for(Index i = 0; i < netSize; i++)
    {
        delete [] adjacencyMatrix[i] ;
    }
    delete [] adjacencyMatrix ;
}
// Neighbors are those neurons to whom there is a connection.
// Index position is 1 based!!
NeuroStack Topology::getNeighbors(Index position)
{
    NeuroStack neighbors;
    for(Index i = 0; i < netSize; i++)
    {
        /**********************
        * Die adjancecy matrix gibt gerichtete
        * Verbindungen in dem Netzwerk an.
        *
        *    1 2 3 4 5 ...
        * 1 0 0 0 0 0 ..
        * 2 1 0 1 0 1 ..
        * 3 0 1 0 0 1 ...
        * 4 1 1 1 1 0 ....
        ***********************/
        if(adjacencyMatrix[position-1][i] == 1)
        {
            // save the address of the neurons in the vector
            neighbors.push_back( neurons[i] ) ;
        }
    }
    return neighbors ;
}
// Index i is one based.
// This methods returns
SynapseStack Topology::getEdges(Index i)
{
        // now create an vector of synapse pointers
        SynapseStack synEdges ;

        // add the Synapses to the array
        for(Index j = 0; j <  synNr; j++)
        {
            Edge edge = synapses[j]->getEdge() ;
            if (edge.i == i)
                synEdges.push_back( synapses[j] ) ;
        }
        // The order of this stack
        // corresponds to an ascending order
        // in the edge.j indices.
        return synEdges ;
};

Index Topology::getNetSize()
{
    return netSize ;
};

Index Topology::getSynNr()
{
  return synapses.size();
};

/// Like all array getter methods
/// this methods uses 1-based
/// indices as argument.
Neuron* Topology::getNeuron(Index i)
{
    return neurons[i-1] ;
}
