/******************************************************************************************************/
/********************** Bitwise boolean: 2 bytes store 16 boolean values (bits) ***********************/
/******************************************************************************************************/

#include "mem.h"

void _SetTrue(bool_16&var, const unsigned char index)
{
    var = var | 1<<index;
}

void _SetFalse(bool_16&var, const unsigned char index)
{
    var = var & ~(1<<index);
}

bool _GetValue(bool_16 var, const unsigned char index)
{
    return var & 1<<index;
}

void _SetTrueSafe(bool_16&var, const unsigned char index)
{
    if(index > 15)
		return;
    var = var | 1<<index;
}

void _SetFalseSafe(bool_16&var, const unsigned char index)
{
    if(index > 15)
		return;
    var = var & ~(1<<index);
}

bool _GetValueSafe(bool_16 var, const unsigned char index)
{
    if(index > 15)
		return false;
    return var & 1<<index;
}

void _SetAllTrue(bool_16&var)
{
    var = (bool_16)~0;
}

void _SetAllFalse(bool_16&var)
{
    var = 0;
}
