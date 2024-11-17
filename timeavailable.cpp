#include "timeavailable.h"

void timeavailable(struct timeval *t) {
    Serial.println("Got time adjustment from NTP!");
    printLocalTime();
}