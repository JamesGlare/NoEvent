#include "globalDefinitons.h"
#include "evolution.h"
#include "neuron.h"
#include "synapse.h"
#include "statistics.h"

#ifndef REGIE_H
#define REGIE_H

/**************************************************************************
* Regie Class
*
* This class can initialize frequency analysis runs.
*************************************************************************/

class Regie
{
    public:
        Regie(Evolution* evol);
        void frequencyAnalysis(Time fStart, Time fEnd, Time fStep, Time tEquib, Time tEnd, unsigned int binNr, TypeZero* pumpNeuron,  bool verbose=false) ;

        Evolution* getEvolution();

    private:
         Evolution* evolution ;
};

#endif // REGIE_H
