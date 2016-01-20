
ATLAS: Adaptable Tool Library for Advanced Simulation

Copyright 2015 University of Central Florida



SUMMARY
=======

This library provides many fundamental capabilities used in creating
virtual environment simulations.  It includes elements such as vectors,
matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
HLA, Bluetooth), and XML processing.  It also includes some extensions
to allow similar code to work in Linux and in Windows.  Note that support
for iOS and Android development is also included.

This code has been released under the Apache License Version 2.0 (see 
licensing information below) and is provided by the University of
Central Florida and the U.S. Army's Army Research Laboratory Simulation
and Training Technology Center (under the Military Open Simulator
Enterprise Strategy, or MOSES, program).  Individual contributors can
be found in the CONTRIBUTORS.txt file included in this archive.


COMPILING
=========

This wrapper has a number of (optional) dependencies:

- libxml2 (and libiconv in Windows)
- libuuid
- bluez
- HLA RTI
- msinttypes (Windows only)

Note that Bluetooth, HLA RTI, UUID and XML support can be enabled/disabled.

This library uses the scons build tool (www.scons.org).

To compile:

scons enableBluetooth=yes bluetoothPath=<path_to_bluetooth_root> enableRTI=yes rtiPath=<path_to_rti_root> enableUUID=yes uuidPath=<path_to_libuuid_root> enableXML=yes xmlPath=<path_to_libxml2_root>

Examples:

scons enableBluetooth=yes bluetoothPath=/usr enableRTI=yes rtiPath=/usr/local/rtis_D30G enableUUID=yes uuidPath=/usr enableXML=yes xmlPath=/usr

scons enableBluetooth=no enableRTI=yes rtiPath=C:\rtis-D30G enableUUID=yes enableXML=yes xmlPath=C:\libxml2-2.9.2 iconvPath=C:\libiconv-1.14


LICENSE
=======

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


CHANGE LOG
==========

v1.0.1   Including copyright, licensing, etc.

v1.0.0   Initial version (not for public release).

