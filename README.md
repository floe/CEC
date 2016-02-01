# CEC
Arduino library for HDMI CEC communication

Original code at: https://code.google.com/archive/p/cec-arduino/ (c) Phil Burr and Andrew N. Carr

This is an Arduino library which implements the HDMI v1.3a CEC wire protocol which allows communication with HDMI CEC capable devices. A typical usage scenario would be a Home Theater PC environment which uses HDMI but does not support CEC. This would allow the HTPC to communicate with other HDMI CEC equipment.

Note: many HDMI sinks also want +5V supplied on the hot-plug detect (HPD) pin before they start using the port (not shown in schematic).
