
#include "globalIncludes.h"
#include "globalDefinitons.h"
/// Also include (almost) all header files
#include "regie.h"
#include "topology.h"
#include "spikelist.h"

using namespace std;

/*************************************************************************
**************************************************************************
* NoEvent
* An event-based simulation algorithm for networks of LIF, QIF or RF
* Neurons with dynamic synapses. Various statistically relevant variables
* can be measured in almost any Neuron.
*
* Author: Jannes Gladrow
* Bachelorthesis Summer 2012 Goettingen
* Georg-August University of Goettingen
*
* In the group of Prof. Marc Timme.
* Under supervision of Dipl.Phys. Hinrich Kiehlblock.
**************************************************************************
*************************************************************************/

// Initialize adjacency matrix
const Index dim = 2;

int main(int argc, char** argv)
{
    cout << "********* NoEvent v. "<<_VERSION <<"***********" << endl;
    cout << "***********************************"<< endl;
    cout << "Event based neural network simulator " <<endl << "by Jannes Gladrow, Goettingen Mai 2012."<<endl ;
    cout << "***********************************"<< endl;

    // End time of simulation
    Time tEnd = 200;
    unsigned int binNr = 10 ;
    // Parameters
    double I = -1 ;
    double c =  2.4;
    double tauM = 0.02;
    double tauS = 0.0685714;
    double u = 0.5 ;

    // Initialize the neurons.
    TypeZero neuron1 = TypeZero(1, 10);
    QIF neuron2 = QIF(2, tauM, I);
    //QIF neuron3 = QIF(3,tauM,I);

    // Initialize neuron list
    NeuroStack neurons ;
    neurons.push_back(&neuron1) ;
    neurons.push_back(&neuron2) ;
   // neurons.push_back(&neuron3) ;
    // Initialize Synapses
    ConstSynapse syn12 = ConstSynapse(((Edge){1,2}),c);
    //DeprSynapse syn13 = DeprSynapse(((Edge){1,3}),1 ,tauS,u);
    //DeprSynapse syn13 = DeprSynapse(((Edge){1,3}),c,tauS, u );

    // Initialize Synapselist
    SynapseStack synapses ;
    synapses.push_back(&syn12) ;
    //synapses.push_back(&syn23) ;

    // now initialize the top level classes
    Topology topology = Topology(neurons, synapses) ;
    SpikeList spikeList = SpikeList();
    Statistics statistics = Statistics() ;

    // Now add the neurons of interest to the statistics module
    //statistics.recSpikeTrain(neuron1) ;
    //statistics.clampNeuron(neuron2) ;
    //statistics.recSpikeTrain(neuron3) ;
    statistics.recSpikeTrain(neuron2) ;
    Evolution evolution = Evolution(&spikeList, &topology, &statistics, tEnd) ;

    // regie
    Regie regie(&evolution);
    regie.frequencyAnalysis(0.01, 50 , 0.05 , 20, tEnd, binNr, &neuron1, false) ;
    //write  the data to files
    //evolution.initSpikeList();
    //evolution.run();
    //statistics.outAll(true) ;
    std::cout << "Done." << std::endl ;
    return 0;
}
