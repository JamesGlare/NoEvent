/*********************************
* Library File
* Defining math functions etc.
*
*
**********************************/

#include "globalDefinitons.h"

/// Test floats library
 bool lib::isnan(Time value)
 {
    return value != value;

 }

template <class T>
T average(std::vector<T> vecT)
{
    T average  ; // this must be possible
   for(Index k = 0 ; k< vecT.size(); k++)
        average+= vecT[k]; // must implement +
   return average / vecT.size() ; // must implement /
} ;

 // requires #include <limits>
bool lib::isinf(Time value)
 {
 return std::numeric_limits<Time>::has_infinity &&
 value == std::numeric_limits<Time>::infinity();
 }

/// Math Library

double lib::atanh(const double& x)
{
    if ( std::abs(x) > 1.0)
        throw lib::MathException() ;
  return 1.0/2 *std::log((1.0+x)/(1.0-x)) ;
}

double lib::acoth(const double& x)
{
    if (std::abs(x) < 1.0 )
        throw lib::MathException() ;
    return 1.0/2 *std::log( (x+1.0)/(x-1.0) ) ;
}
