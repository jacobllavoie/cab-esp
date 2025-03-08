// effects.cpp

#include "config.h"
#include "globals.h"

// Effect array (updated to exclude omitted effects)
Effect effects[] = {
  {"rainbow", rainbow},
  {"chasing_rainbow", chasingRainbow},
  {"flashing_amber_white_sequence", flashingAmberWhiteSequence}
};
int numberOfEffects = sizeof(effects) / sizeof(effects[0]);