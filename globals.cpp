// globals.cpp

#include "globals.h"

int animationSpeed = 50;
int brightness = 255;

Effect effects[] = {
  {"rainbow", rainbow},
  {"chasing_rainbow", chasingRainbow},
  {"flashing_amber_white_sequence", flashingAmberWhiteSequence}
};
int numberOfEffects = sizeof(effects) / sizeof(effects[0]);