December 2018 - Library First Release
(December 2018) - Library Latest Release for ADE9078

Copyright (C) The Regents of the University of California, 2018

Released into the public domain under creative commons share-alike license. This is an example C++ library for Arduino for operating the ADE9078 3-Phase AC Line measurement IC over SPI and tested with the Arduino Uno and C++ Arduino-style compiler for the Espressif ESP8266. This library in current form was shown to work with the Arduino Uno and the Espressif ESP8266.  See note below about Espressif ESP32 compatibility - a working forked example is provided is in the ESP32 folder in this project.  This ESP32 extension example was developed by Luis Contreras as part of the CalPlug EVSE Project - https://github.com/CalPlug/EVSE_Smart_Charger and presented in this project as functional example working with the ESP32.

Background of Operation
--------------------------------------------------------------------------------

The ADE9078 uses registers for communication.  Depending on the type, this can be a 16 bit or 32 bit register.  Communication uses sequential bytes using typical SPI byte-wise communication.  There are redundancies in how registers are accessed via different bit values for registers.  For addresses, the same command may be represented in multiple bit versions (eg a 16bit or 32 bit). When you call on a certain address, the corresponding value is returned with the value bit size corresponding to the 16 or 32 bit address called on. The responding value returns as the function called and of the same bit size.  Refer to the documentation!  For Debug, the Serial Monitor runs at 115200 baudrate when the reporting DEBUG functionality is activated.  Uncomment IFDEF statements in the library for library operation debug.
