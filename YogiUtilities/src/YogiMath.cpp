
#include "YogiMath.h"


namespace YogiMath {


long
powi( long nBase, long nExp )
{
    long nResult = 1;

    if ( nExp < 0 )
    {
        // under normal conditions this would
        // create a fraction
        nResult = 0;
    }
    else
    {
        // inefficient but it works
        while ( 0 < nExp-- )
            nResult *= nBase;
    }

    return nResult;
}


}  // namespace YogiMath
