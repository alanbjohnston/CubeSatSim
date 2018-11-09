These spreadsheets are for analyzing the CubeSat Simulator telemetry.

There is one VisualBasic Macro called "cleanup" which cleans up the raw telemetry copied from AFSK 1200 Decoder by removing the timestamps by deleting every other line.

You can either open the spreadsheet with the macros (CubeSatSim TLM Analysis.xls) and select Enable Macros when you open it.  Or, you could open the spreadsheet without the macros (CubeSatSim TLM Analysis No Macros.xls) and then import the VB macro cleanup.bas.  If you choose not the use macros, you can just manually delete rows to get the telemetry data in the right format.

The instructions for using the spreadsheets are as follows:

Instructions:

Copy your raw telemetry data into the tab/sheet "Paste Raw Data Here".

If it is CW telemetry, you will need to edit so that it is one frame of telemetry per row

If it is AFSK telemetry from AFSK 1200 Decoder, you will need to delete every other row which has the timestamp.  You can use a macro to do this.  Select a row at the top that has a timestamp, and type Control-t and every other line will be deleted.

Once you have one frame of telemetry per line and no extra lines, copy and paste into the tab/sheet, "Data Input"

The telemetry starting in Row 2 will be calculated and plotted.

See plots in "Voltage Graphs", "Current Graphs", and "Temperature Graphs"

To see raw data or create a new plot, go to "Graphs"
