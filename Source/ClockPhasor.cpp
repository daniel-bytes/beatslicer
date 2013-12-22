#include "ClockPhasor.h"
#include <cmath>

#define TICKS_PER_BAR 4

void ClockPhasor::setPhase(double clockPosition)
{
	phase = (float)fmod(clockPosition / TICKS_PER_BAR, 1.0);
}