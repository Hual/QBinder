/******************************************************************************************************/
/********************** Bitwise boolean: 2 bytes store 16 boolean values (bits) ***********************/
/******************************************************************************************************/

#pragma once

#define SetTrue(var, index) _SetTrueSafe(var, index)
#define SetFalse(var, index) _SetFalseSafe(var, index)
#define GetValue(var, index) _GetValueSafe(var, index)

typedef unsigned short bool_16;

void _SetTrue(bool_16&, const unsigned char);
void _SetFalse(bool_16&, const unsigned char);
bool _GetValue(bool_16, const unsigned char);
void _SetTrueSafe(bool_16&, const unsigned char);
void _SetFalseSafe(bool_16&, const unsigned char);
bool _GetValueSafe(bool_16, const unsigned char);
void _SetAllTrue(bool_16&);
void _SetAllFalse(bool_16&);
