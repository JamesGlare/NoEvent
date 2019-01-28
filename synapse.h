#ifndef SYNAPSE_H
#define SYNAPSE_H

/// Includes
#include "globalDefinitons.h"
#include "statistics.h"
/*************************************
* Synapse Classes
*  - Base Class implements structure
*  of synaptic interaction.
*
**************************************/

// namespace edge
class Synapse
{
    public:
        Synapse(Edge _edge, double _c);

        // Get Postsynaptic potential
        // returns the strength of a postsynaptic potential
        // generated at this synapse.
        // Also updates the State of the synapse
        // automatically.
        State virtual getPSP( Time t ){};

        // Getter method for the Edge
        Edge getEdge();
        double getC();

        // reset synapse
        void reset(bool TimeReset = false);

    protected:
        State state;
        Time timeOfState ;
         // Connectivity c
        // is inherent to all synapse types
        const double c ;
        // purely abstract function
        // describes free time evolution
        void virtual freeEvolution(Time t){};

    private:
        // Edge is tupel signaling the direction of the synapse.
        const Edge edge;
};
/// Define a container with synapses
typedef std::vector<Synapse*> SynapseStack;

// just a classic synapse
class ConstSynapse : public Synapse
{
     public:
        ConstSynapse(Edge edge, double _c);
        State getPSP(Time t) ;
    private:
        void freeEvolution(Time t);
};

/// Child classes of the synapse.
// Depressing Synapse
class DeprSynapse : public Synapse
{
    public:
        DeprSynapse(Edge edge, double _c, double _tauS, double _u);
        State getPSP(Time t) ;
    private:
        // parameters
        const double tauS;
        const double u;
        void freeEvolution(Time t);
};

// Depressing but also facilitating synapse
class DeprFacilSynapse : public Synapse
{
    public:
        DeprFacilSynapse(Edge edge, double _c, double _tauS, double _tauF, double _u);
        State getPSP(Time t) ;
    private:
        // paramters
        const double tauS;
        const double tauF;
        const double u;
        // this class of synapse
        // needs another state variable
        State U ;
        void freeEvolution(Time t);
};

#endif // SYNAPSE_H
