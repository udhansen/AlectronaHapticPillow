#ifndef TIMEAVAILABLE_H
#define TIMEAVAILABLE_H

#include <Arduino.h>
#include "displayConfig.h"

// Function declarations
void timeavailable(struct timeval *t);
void printLocalTime();

#endif
