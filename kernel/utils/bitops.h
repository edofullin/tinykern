#pragma once

// bit set from the right, bval either 0 or 1
#define BIT_SET(val, bitnum, bval) ((val & ~(1 << bitnum)) | (bval << bitnum))