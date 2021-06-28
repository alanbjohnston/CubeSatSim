This spreadsheet is for analyzing the CubeSatSim APRS and CW telemetry ONLY.

The best telemetry for the CubeSatSim is FSK which uses the AMSAT FoxTelem software to automatically demodulate, decode, and graph the data.

For more information see https://github.com/alanbjohnston/CubeSatSim/wiki/3.-Ground-Station

A version for Google Docs is available for copying and download at:

 http://cubesatsim.org/telem

Instructions:

Copy your raw telemetry data into the Data Input tab.  If you don't have any data, copy data from the Sample Data tab.  

The odd rows can be a callsign or time timestamp (which is ignored) while the even rows should contain frames of telemetry data (beginning with "hi hi" then a series of 3 digit numbers).  If this isn't the case, you will need to edit until even lines starting at row 2 contain frames of telemetry.

The telemetry starting in Row 2 will be automatically calculated and plotted.

See plots in Voltage Graphs, Current Graphs, and Temperature Graph tabs.

To see raw data or create a new plot, go to Graphs tab.
