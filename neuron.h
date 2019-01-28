#ifndef NEURON_H
#define NEURON_H

// Includes
#include "globalDefinitons.h"

/****************************************
* Neuron Class
* The central class describing neural
* network dynamics.
* Jannes Gladrow Mai, 2012
*****************************************/

class Neuron
{
    public:
        inline virtual ~Neuron(){};
        // updateState Function
        // Update the state of this neuron. Needsthe spike strength.
        // This function assumes that this Neuron receives a spike at time t,
        // its internal state then catches up from the last time it was updated.
        // If a 0 is given, the state is only updated
        // according to the free time evolution.
        // Time t: Actual system time
        // state incoming spike.
        void updateState (Time t, State incomingSpike);

        // This function is called by the evolution to estimate
        // the spike timing of this neuron.
        Time virtual estimateSpikeTime (){};

        // Reset function
        void reset (Time t);

        // get Position
        Index getPosition();
        // Get state of the membrane potential
        State getState();

        // The eolution of the individual neurons
        // has to be implemented in the subclass.
        // this function pointer is given to
        // statistics class.
        virtual State evolve(State state, Time period){};

        // Constants defined - publicly accessible.
         const State thresholdValue ;
         const State resetValue ;


    protected:
        Neuron (Index i, const State thresholdValue, const State resetValue);

        // internal flags
        bool continuousTimeReport ;
        bool spikeReport  ;

        // The internal state and timestamp of this state.
        Time timeOfState ;
        State state ;

    private :
        Index position ;
};

// Define vector of neurons as neurostack
typedef std::vector<Neuron*> NeuroStack;

/// Inherited Neuron classes
class LIF : public Neuron
{
    public:
        LIF(Index i, double tauM, double I);
        Time estimateSpikeTime () ;

        State evolve(State state, Time period);
    private:
        // additional parameters
        const double tauM ;
        const double I;
};

class QIF : public Neuron
{
     public:
        QIF(Index i, double tauM, double I);
        Time estimateSpikeTime () ;

        State evolve(State state, Time period);
     private:
         // parameters
        const double tauM ;
        const double I ;
};

class RF : public Neuron
{
     public:
        RF(Index  i, double w, double l);
        Time estimateSpikeTime () ;

        void reset() ;

        State evolve(State state, Time period) ;
     private:
        State evolve(Time period);
        State j; // this model has anotther state variable

        const State jReset ;
        const double w; // omega
        const double l; // leak constant

};


/********************************************
* TypeZero Neuron
* This neuron type is used to simulate external
* populations of neurons.
*******************************************/
class TypeZero :  public Neuron
{
    public:
        TypeZero(Index i, Time freq);
        Time estimateSpikeTime () ;

        State evolve(State state, Time period);

        void setFreq(Time freq) ;
        Time getFreq() ;
    private:
        Time freq ;
};

#endif // NEURON_H
