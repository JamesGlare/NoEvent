#include "spikelist.h"

bool spikeSort(Spike firstSpike, Spike secondSpike)
{
    Time first = firstSpike.t ;
    Time second = secondSpike.t;
    return first < second ;
};

SpikeList::SpikeList()
{
    //ctor
    //Create list
    Spikes s ;
    spikes = s;
};

void SpikeList::addEstimate(Spike s)
{
     // only accept those spikes which will occur in finite time
     if(! lib::isinf(s.t) && ! lib::isnan (s.t) )
     {
    // if there is already a spike announced, delete that one,
    // since this information deprecated.
        deleteSpike(s.i) ;
        spikes.push_back(s);
        sortSpikes();
     }

};
void SpikeList::clearList()
{
    spikes.clear() ;
};

// Delete the stored spike of the neuron
// with the index k.
void SpikeList::deleteSpike(Index k)
{
    Spikes::iterator it;
    for(it = spikes.begin(); it != spikes.end(); it++)
        if((*it).i == k)
            it = spikes.erase(it) ;
};

void SpikeList::sortSpikes()
{
    spikes.sort(spikeSort) ;
};

// count the number of non infinity spikes
// in the spikelist
unsigned int SpikeList::spikesToCome()
{
  unsigned int toCome = 0;
  Spikes::iterator it;
  for( it = spikes.begin(); it != spikes.end(); it++ )
        toCome++;
    return toCome ;
};

bool SpikeList::hasNext()
{
    if(spikes.empty())
        return false;
    else if ( ! lib::isinf (spikes.front().t) && ! lib::isnan(spikes.front().t) )
        return true;
    else
        return false  ;

};

Spike SpikeList::yieldNext()
{
    /// IMPORTANT:
    /// DONT CALL THIS FUNCTION IN CASE
    /// WITHOUT HAVING CALLED hasNext() FIRST !
    // By convention C++ lists
    // sort their elements
    // in ascending order
    Spike next = spikes.front();
    spikes.pop_front(); // delete first element
    return next ;
};
