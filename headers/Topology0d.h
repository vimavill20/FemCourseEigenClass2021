//
//  Topology1d.h
//  FemSC
//
//  Created by Philippe Devloo on 03/04/18.
//

#ifndef Topology0d_h
#define Topology0d_h

#include "DataTypes.h"
#include "IntRule1d.h"

class Topology0d
{
public:
    
    // Type definition for integration rule 1D
    typedef IntRule1d LocIntRule;
    
    // Number of sides associated with 1D elements
    const static int nSides = 1;

    // Number of corner nodes associated with 1D elements
    const static int nCorners = 1;
    
    // Dimension of 1D elements
    const static int Dimension = 0;
    
    // Number of nodes associated with a side
    static int NSideNodes(int side);
    
    // Local node index of a node associated with a side
    static int SideNodeLocIndex(int side, int node);
    
    // Return the enumerated element type
    static MElementType Type();
};

#endif /* Topology1d_h */
