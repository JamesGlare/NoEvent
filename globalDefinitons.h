#include "globalIncludes.h"

#ifndef _GLOBALDEF
#define _GLOBALDEF
/// Global Definitions


typedef  long double Time;
/// Convention: Indices have to be counted
/// as one based.
typedef unsigned int Index;
typedef long double State ;
// Convention:
// First Index defines the starting point,
// the next index defines the ending point.
// (i,j) = i -> j

typedef struct Edge
{
    Index i;
    Index j;
} Edge;
typedef unsigned int mode;

typedef struct Spike
{
    Time t;
    Index i;
} Spike ;


/// Precompiler Definitions
#define _INFTY std::numeric_limits<long double>::infinity( )
#define _VERSION 0.9

namespace lib {
/// Function declarations
    class MathException {};
    class IndexException{};
    double atanh(const double& x) ;
    double acoth(const double& x) ;

    bool isnan(Time value);
    bool isinf(Time value);

    template <class T>
    T average(std::vector<T> vecT);
};
#endif
