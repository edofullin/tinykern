#pragma once

#include "types.h"

void kalloc_init();

void* kmallocp();
void kmfree(void* page);