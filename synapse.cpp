#include "synapse.h"


/// Implementation of the
/// synapse base class.

// Convention:
// All Synapses start with full
// capability and ressources.

const State stateZero = 1.0 ;
const Time resetTime = 0.0 ;

Synapse::Synapse(Edge _edge, double _c) : edge(_edge), c(_c)
{
    //ctor
    // Set time to zero
    reset( true );
};
// Getter method for edge.
Edge Synapse::getEdge()
{
    return edge ;
};
// Getter Method for the synpase
double Synapse::getC()
{
    return c ;
};

void Synapse::reset(bool TimeReset)
{
    if(TimeReset)
        timeOfState = resetTime;

    state = stateZero ;
};

/// Implementation of the derived
/// Depressing Synapse class.

DeprSynapse::DeprSynapse( Edge _edge, double _c, double _tauS, double _u) : Synapse(_edge, _c), tauS(_tauS), u(_u)
{
};

void DeprSynapse::freeEvolution(Time t)
{
    Time T = t-timeOfState ;
    state = (state-1.0)*std::exp(-T/tauS) ;
    state += 1.0 ;
    // Set the new time then.
    timeOfState = t ;
};

State DeprSynapse::getPSP(Time t)
{
    // Evolve the state until
    // the point in time where
    // the Action Potential arrives
    freeEvolution(t);
    State tempState = state ;

    // Then add the spike
    state -= u*state ;
    return c*tempState ; // connectivity times ressources
};

/// Implementation of the constSynpase Class

ConstSynapse::ConstSynapse(Edge _edge, double _c) : Synapse(_edge, _c)
{

} ;
void ConstSynapse::freeEvolution(Time t)
{
    timeOfState = t ;
} ;
State ConstSynapse::getPSP(Time t)
{
   return c;
};

/// Implementation of the derived
/// Depressing and facilitating synapse class.

DeprFacilSynapse::DeprFacilSynapse(Edge _edge, double _c, double _tauS, double _tauF, double _u )
: Synapse(_edge, _c), tauS(_tauS), tauF(_tauF), u(_u)
{
    U = 0.0 ;
};

void DeprFacilSynapse::freeEvolution(Time t)
{
   Time T = t -timeOfState;
    // update the utilization variable first
    U *=std::exp(-T/tauS) ;
    // now update the state variable x
    state = (state-1.0)*std::exp(-T/tauS) ;
    state += 1.0 ;
};
State DeprFacilSynapse::getPSP(Time t)
{
   freeEvolution(t);
   // Save the state in a temp variable
   State tempState = state ;
    // Add the spike to the potential.
    U += u*(1.0-U);
    state -= U*state ;
    return c*tempState ; // connectivity times ressources
};
