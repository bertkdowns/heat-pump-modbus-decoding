The idea behind this repository is to have an arduino uno with the StandardFirmata script installed, and then use this to control the arduino. It will listen to mqtt events and update the pins appropriately.

This adds 3 mqtt topics:
valve/write
vsd1/write
vsd2/write
which each should be a number input.