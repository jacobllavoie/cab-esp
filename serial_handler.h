// serial_handler.h

#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <Arduino.h>

void handleSerialInput();
void processSerialCommand(String command);

#endif