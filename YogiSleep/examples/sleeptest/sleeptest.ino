#include "YogiSleep.h"


YogiSleep g_tSleep;


void
setup()
{
    g_tSleep.prepareSleep();
    g_tSleep.powerDown();
}


void
loop()
{}
