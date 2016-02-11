# CEC
Arduino library for HDMI CEC communication

Original code at: https://code.google.com/archive/p/cec-arduino/ (c) Phil Burr and Andrew N. Carr

This is an Arduino library which implements the HDMI v1.3a CEC wire protocol which allows communication with HDMI CEC capable devices. A typical usage scenario would be a Home Theater PC environment which uses HDMI but does not support CEC. This would allow the HTPC to communicate with other HDMI CEC equipment.

I have tested the example with a somewhat dated Philips TV (40PFL5605K), and can now use my remote control for Kodi etc. Other TVs may have different quirks - if it doesn't work, connect something that does work (e.g. a FireTV), enable promiscuous mode and observe the traffic on the serial console. Unknown messages can be directly pasted into CEC-O-MATIC (http://www.cec-o-matic.com/) to decode them without having to dig through the spec.

Note: many HDMI sinks also want +5V supplied on the hot-plug detect pin (HPD, pin 19) before they start using the port. The bold numbers in the schematic on the right correspond to the HDMI pin numbers where the signals should be connected (see also https://en.wikipedia.org/wiki/HDMI).
