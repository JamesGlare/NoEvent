#include "neuron.h"

/// Every non-virtual function at least has to be defined somewhere
/// otherwise a compiler error (undefined reference to vtable)

const Time startTime = 0.0 ;

// Definitions of the motherclass functions
Neuron::Neuron(Index i, const State _thresholdValue, const State _resetValue)
: thresholdValue(_thresholdValue), resetValue(_resetValue), position(i)
{
    // Indices must be greater equal zero.
    if(i <=0.0)
        throw lib::IndexException() ;

    reset(startTime);
    // Set internal flags
    continuousTimeReport =false ;
    spikeReport = false ;
    // Set the time variable of this neuron
};

Index Neuron::getPosition()
{
    return  position ;
};

State Neuron::getState()
{
    return state ;
}

// Implementation of updateState
void Neuron::updateState(Time t, State incomingSpike)
{
    Time T = t - timeOfState ;
    // Step 1: refresh old state to current state
    state = evolve(state, T);
    // Step 2: add the spike to the state and refresh time.
    state += incomingSpike;
    // The estimation of the spike time
    // is done by the evolution.
    timeOfState = t ;
};

// Implementation of the reset function.
void Neuron::reset(Time t)
{
    state = this->resetValue ;
    timeOfState = t ;
};


/******************************
Leaky-Integrate-and-fire Neuron
 - constructor
 - estimateSpikeTime
 - free evolution
*******************************/

// LIf properties
const State LIFthreshold = 1.0 ;
const State LIFreset = 0.0 ;

// Constructor of LIF
LIF::LIF(Index i, double _tauM, double _I) : Neuron(i, LIFthreshold, LIFreset), tauM(_tauM), I(_I)
{
} ;

// Estimation function for the LIF
Time LIF::estimateSpikeTime(){
    // Consider the case, that this neuron already wants to spike now.
    if (state > thresholdValue)
    {
        return 0.0 ;
    }
    else if (I <= 1.0)
    {
        return _INFTY ;
    }
    return tauM*std::log((I-state)/(I-thresholdValue)) ;
};


State LIF::evolve(State state, Time T)
{
    return (state - I)*std::exp(-T/tauM) + I ;
}
/***********************************
Quadratic-Integrate-and-fire Neuron
 - constructor
 - estimateSpikeTime
 - free evolution
************************************/
// QIF properties
const State QIFthresholdValue = 1000000000 ;
const State QIFresetValue = -1000000000 ;

//const State QIFthresholdValue = 1.5 ;
//const State QIFresetValue =  -1.5;
// Constructor of LIF
// Constructor of LIF
QIF::QIF(Index i, double _tauM, double _I) : Neuron(i, QIFthresholdValue, QIFresetValue), tauM(_tauM), I(_I)
{
} ;

State QIF::evolve(State state, Time T)
{
    if( I < 0.0 )
    {
        double J = std::sqrt(-I);
        if ( std::abs(state) <= J)
        {
            return J*std::tanh(lib::atanh( state/J)-J/tauM*T )  ;
        }
        else
        {
            return J*1.0 / std::tanh(lib::acoth(state/J)-J/tauM*T );
        }

    }
    else
    {
        double J = std::sqrt(I);
        return  J*std::tan(J/tauM*T+std::atan(state/J )) ;
    }
};
// Estimation function for the LIF
Time QIF::estimateSpikeTime()
{
    // Consider the case, that this neuron already wants to spike now.
    if (state > thresholdValue)
    {
        return 0.0 ;
    }
    if(I < 0.0  )
    {
        double J = std::sqrt(-I) ;
        if(state > J)
        {
            return tauM/J * lib::acoth(state/J) ;
        }
        else
        {
            return _INFTY ;
        }
    }
    else
    {
        return tauM / std::sqrt(I) * (M_PI/2.0 - std::atan(state/std::sqrt(I))) ;
    }
    return 0.0 ;
} ;

/***********************************
 TypeZero Population
- constructor
 - estimateSpikeTime
 - free evolution
************************************/
TypeZero::TypeZero(Index _i, Time _freq)
: Neuron(_i,0,0), freq(_freq){};

const double epsilon = 0.00001 ;

Time TypeZero::estimateSpikeTime()
{
    Time Tout = 1.0/freq ;
    Time est = std::floor(timeOfState*freq)*Tout - timeOfState ;
    return est < epsilon ? Tout : est ;
};
State TypeZero::evolve(State state, Time t)
{
    return 0.0 ;
};

void TypeZero::setFreq(Time _freq)
{
    freq = _freq ;
};
Time TypeZero::getFreq()
{
    return freq ;
}

/***********************************
 Resonate and fire neurons
- constructor
 - estimateSpikeTime
 - free evolution
************************************/
