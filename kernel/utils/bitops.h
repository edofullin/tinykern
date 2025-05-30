#pragma once

// bit set from the right, bval either 0 or 1
#define BIT_SET_VAL(val, bitnum, bval) ((val & ~(1ull << bitnum)) | (((uint64)bval) << bitnum))
#define BIT_SET(val, bitnum) (val | (1ull << bitnum)) 
#define BIT_RESET(val, bitnum) (val & ~(1ull << bitnum)) 