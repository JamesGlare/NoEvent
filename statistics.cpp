#include "statistics.h"


/// Parameters
const double resolution = 0.005 ;
const std::string spikeTrainPrefix = "spikeTrain_";
const std::string clampDataPrefix = "clampData_" ;
const std::string freqAveragePrefix= "frequencies" ;
const std::string fileExt = ".dat" ;


    // Since we have to check for doubles being zero,
    // we have to define an epsilon
const Time epsilon = 0.00001 ;

Index relateKeys(Index i, Indices indices)
{
    for(Index k = 0; k < indices.size(); k++)
        if(i== indices[k])
            return k ;
};

Statistics::~Statistics()
{
    //dtor
}

Statistics::Statistics()
{
    // Initiailisiere die Listen
        NetworkSpikeTrain _netSpikeTrains ;
        netSpikeTrains = _netSpikeTrains ;

        Indices _whichToClamp ;
        whichToClamp = _whichToClamp ;

        Indices _whichSpikeTrain ;
        whichSpikeTrain = _whichSpikeTrain ;

        NetworkClampRecords _netClampRecords;
        netClampRecords = _netClampRecords ;
} ;

void Statistics::clampNeuron( Neuron neuron)
{
    Index i = neuron.getPosition() ;
    whichToClamp.push_back(i) ;
    ClampRecords cr;
    netClampRecords.push_back(cr ) ;
};
void Statistics::recSpikeTrain( Neuron neuron )
{
    Index i = neuron.getPosition() ;
   whichSpikeTrain.push_back(i) ;
   SpikeTrain st ;
   netSpikeTrains.push_back(st); // just add a spiketrain - the order doesnt matter here,
} ;
bool isEqual(Index a, Index b)
{ return a == b ;};

bool Statistics::neuronIsClamped(Index j)
{
    bool alreadyRecorded = false;
    for(Index i = 0; i < whichToClamp.size(); i++)
        if(whichToClamp[i]== j)
            alreadyRecorded = true ;
    return alreadyRecorded ;
};
bool Statistics::spikesAreRecorded(Index j)
{
    bool alreadyRecorded = false;
    for(Index i = 0; i < whichSpikeTrain.size(); i++)
        if(whichSpikeTrain[i]== j)
            alreadyRecorded = true ;
    return alreadyRecorded ;
};
void Statistics::addSpike( Spike spike )
{
    if( spikesAreRecorded(spike.i))
    // The spikes are added in time order anyway
        netSpikeTrains[ relateKeys(spike.i, whichSpikeTrain ) ].push_back( spike ) ;
};
void Statistics::addClampData( Neuron& toClamp, Time tf)
{
    if(neuronIsClamped(toClamp.getPosition())){
        // Get the container of neuron i
        ClampRecords* rec = &netClampRecords[ relateKeys(toClamp.getPosition(), whichToClamp )] ;
        // first check if container is empty
        if(rec->empty())
            rec->push_back((ClampData){ 0.0, toClamp.resetValue}) ;
        // now run through the free evolution
        Time t = resolution;
        Time ti = rec->at(rec->size()-1).t;
        while(t < tf - ti )
        {
            Index i = rec->size() ;
            rec->push_back((ClampData){ t + ti ,toClamp.evolve( rec->at( i-1 ).s, resolution) } ) ;
            t += resolution ;
        };
        rec->push_back((ClampData){tf, toClamp.getState()}) ;
    };
};
void Statistics::recReset(Neuron& toClamp, Time tReset)
{
    // before we reset the neuron we have to reeconstruct
    // the clampData that has accumulated
    addClampData(toClamp, tReset) ;
    // now reset the neurons clamp data
    if(neuronIsClamped(toClamp.getPosition())){
        ClampRecords* rec = &netClampRecords[relateKeys( toClamp.getPosition(), whichToClamp) ] ;
        rec->push_back((ClampData){ tReset, toClamp.resetValue });
    }
};

void Statistics::outSpikeTrain(std::ostream &stream, Index i)
{
    SpikeTrain st = netSpikeTrains[relateKeys(i, whichSpikeTrain)] ;
    for(Index i = 0; i < st.size(); i++)
        stream << st[i].i << " " << st[i].t <<std::endl ;
} ;

void Statistics::outClampData(std::ostream &stream, Index i)
{
    ClampRecords cr = netClampRecords[relateKeys(i, whichToClamp)] ;
    for( Index i = 0; i < cr.size(); i++ )
        stream << cr[i].s << " " << cr[i].t << std::endl ;
} ;

void Statistics::outAll(bool _yieldISI=true)
{
    // First out the spikeTrain data
    for(Index i=0; i < netSpikeTrains.size(); i++)
    {
        Index nrIndex = whichSpikeTrain[i] ;
        SpikeTrain* st = &netSpikeTrains[relateKeys(nrIndex, whichSpikeTrain)] ;
        // Complicated conversion int -> string
        std::string fileName ;
        std::stringstream conv ;
        conv << nrIndex ;
        // Write to file
        std::fstream outFile( fileName.append(spikeTrainPrefix).append( conv.str()).append(fileExt).c_str()  , std::fstream::out);

        for(Index k=0; k<st->size();k++ )
        {
            Time out = 0.0 ;
            if(_yieldISI)
            {
                out = k > 0 ? st->at(k).t - st->at(k-1).t : st->at(k).t - 0.0 ;
            }
            else
            {
                out = st->at(k).t;
            }
                outFile <<st->at(k).t <<"\t" << out  << std::endl ;
        }
        outFile.close();
    }
    // Put out the clampdata
    for(Index i=0; i < netClampRecords.size(); i++)
    {
        Index nrIndex = whichToClamp[i];
        ClampRecords* recs = &netClampRecords[relateKeys(i,whichToClamp)] ;
        std::string fileName ;
        std::stringstream conv;
        conv << nrIndex ;
        std::fstream outFile(fileName.append(clampDataPrefix).append(conv.str()).append(fileExt).c_str(), std::fstream::out) ;

        for(Index k = 0; k < recs->size(); k++)
        {
            outFile<< recs->at(k).t << "\t" << recs->at(k).s << std::endl ;
        }
    }
} ;


Bins Statistics::binning(unsigned int binNr, const SpikeTrain* st, Time tEnd, Time tEquib)
{
    // 1. Create a vector of bins, the size of the vector is the number of bins.
    Bins bins (binNr) ;

    if( st->empty()){
        return bins ;
    }
    //2 . Calculate the bin length / timestep, take the max time in spiketrain as right bound
    // since sometimes the simulation stops afterwards
    Time maxISI = 0.0 ;
    for(Index l=1; l < st->size(); l++)
    {
        Time temp = st->at(l).t - st->at(l-1).t ;
        maxISI = st->at(l-1).t > tEquib && temp > maxISI ? temp :maxISI ;
    }

    Time dt = maxISI / binNr ; // greatest possible ISI / binNr
    // 3. Fill the second entry of struct Bin{ Time binAverage;  Index entries;}
    for(Index k=1; k < st->size(); k++)
    {
        if( st->at(k-1).t > tEquib)
        { // check for equilibrium time
            Time temp = (st->at(k).t -st->at(k-1).t ) / dt  ;
            // ask for tEquilibrium and the correctness of the bin index.
            Index i =  temp > 1.0 && temp <= binNr  ? ((Index) temp) - 1 : 0 ;
            bins[i].entries += 1 ;
        }
    }
    // 4. Fill the bin average entry of the struct Bins
    for(Index k=1; k < st->size(); k++)
    {
        if( st->at(k-1).t > tEquib)
        { // check for equilibrium time
            Time temp = (st->at(k).t -st->at(k-1).t ) / dt  ;
            // ask for tEquilibrium and the correctness of the bin index.
            Index i =  temp > 1.0 && temp <= binNr  ? ((Index) temp) -  1: 0 ;
            bins[i].binAverage += (st->at(k).t -st->at(k-1).t ) / bins[i].entries ;
        }
    } // the bin average
    return bins ;
} ;

// This function must be called after the Evolution has run the simulation
/*void Statistics::writeAverageFreq(Time fIn, Time tEquib, unsigned int binNr, Time tEnd)
{
    std::string fileName ;
    std::fstream outFile; // typically this file already exists
    outFile.open( fileName.append(freqAveragePrefix).append( fileExt ).c_str()  , std::fstream::out|std::fstream::app );

    outFile << fIn ;
    Frequencies freqs ; // vector which is to be written
    Time freq ;
    Time variance ;
    Time totalFrequencyAverage ;
    // go through each recorded spike train
    for(Index k = 0; k < netSpikeTrains.size(); k++)
    {
        SpikeTrain* st = &netSpikeTrains[k] ;
        freq = 0.0 ;
        variance = 0.0 ;
        totalFrequencyAverage = 0.0 ;
        // get the bins of the k-th spiketrain
        Bins bins = binning(binNr, st ,tEnd,tEquib) ;

        Index eqSpikeNr = 0 ;
        // calculate the total frequency average for this run
        for(Index l = 0; l < bins.size(); l++)
        {
            eqSpikeNr+= bins[l].entries ; // total nr of spikes after tEquib
        }
        totalFrequencyAverage = eqSpikeNr/(tEnd-tEquib) ;

        for(Index j = 0 ; j < bins.size(); j++)
        {                        // Use the formular <g(X)> = Sum_i g(Xi) * P(Xi)
            freq +=  bins[j].binAverage > 0.0 && bins[j].entries>0 ?(1.0 / bins[j].binAverage) * (((double) bins[j].entries)/ eqSpikeNr  ) : 0 ;
            variance += bins[j].binAverage > 0.0 && bins[j].entries>0 ?  std::pow( bins[j].binAverage- totalFrequencyAverage ,2.0) : 0 ;
        }

        variance *= 1.0/ (binNr*( binNr-1 ) ) ;
        freqs.push_back( {freq, variance} ) ; // save these values
    }
    for(Index k = 0; k < freqs.size(); k++ )
    {
        outFile  << "\t" << freqs[k].mean  << "\t"<< freqs[k].var << "\t" << totalFrequencyAverage ;
    }
    outFile << std::endl ;
    outFile.close() ; // close the stream again
};*/
void Statistics::writeAverageFreq(Time fIn, Time tEquib, unsigned int binNr, Time tEnd)
{
    std::string fileName ;
    std::fstream outFile; // typically this file already exists
    outFile.open( fileName.append(freqAveragePrefix).append( fileExt ).c_str()  , std::fstream::out|std::fstream::app );

    outFile << fIn ;
    Frequencies freqs ; // vector which is to be written

    Time totalFrequencyAverage ;
    // go through each recorded spike train
    for(Index k = 0; k < netSpikeTrains.size(); k++)
    {
        SpikeTrain* st = &netSpikeTrains[k] ;

        totalFrequencyAverage = 0.0 ;

        Index eqSpikeNr = 0 ;
        // calculate the total frequency average for this run
        for(Index l = 0; l < st->size(); l++)
        {
            eqSpikeNr+= st->at(l).t > tEquib ? 1 : 0 ; // total nr of spikes after tEquib
        }
        totalFrequencyAverage = eqSpikeNr/(tEnd-tEquib) ;

        freqs.push_back( {totalFrequencyAverage, 0} ) ; // save these values
    }
    for(Index k = 0; k < freqs.size(); k++ )
    {
        outFile  << "\t" << freqs[k].mean  ;//<< "\t"<< freqs[k].var  ;
    }
    outFile << std::endl ;
    outFile.close() ; // close the stream again
};

void Statistics::resetStatistics()
{
    for(Index i = 0; i < netSpikeTrains.size(); i++)
        netSpikeTrains[i].clear();
    for(Index j=0; j < netClampRecords.size(); j++)
        netClampRecords[j].clear() ;
} ;
