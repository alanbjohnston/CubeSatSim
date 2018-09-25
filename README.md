CubeSatSim

RPi code using the Brandenburg Tech Digital Transceiver 
     https://brandenburgtech.wordpress.com/- 
     
 - cw - Code that sends telemetry in CW (Morse code) using AO-7 format
 - afsk - Code that sends a simple text message in 1k2 AFSK X.25 format

# based on DigitalTxRxRP

- RemoteSystemsTempFiles - used by Eclipse.
- afsktx - A sample 'C' program that transmits an AFSK modulated signal. It transmits at UHF frequencies because that is the matching network installed on my Digital Transceiver for Raspberry Pi board (even though FSK is typically used at UHF frequencies).
- ax5043 - Source for a library of functions to communicate with the AX5043 and configure the AX5043.
- chat - A simple text chat application using the Digital Transceiver for Raspberry Pi.
- init - A test program that checks to see if the AX5043 can be successfully initialized.
- libs - Source for third party libraries used by these samples.
- piglatin - a sample demonstration application that receives some text, converts that text using simple rules of pig Latin, then transmits that text. It demonstrates an extremely trivial "server" or "satellite" responding to commands.
- receive - a sample application that receives text transmissions and prints them to the screen.
- sandbox - a folder containing various test code to understand the operation of the AX5043. This folder can safely be ignored.
- transmit - a sample application that transmits text strings.
- transmit2freq - A sample application that transmits text strings with the ability to specify the transmission frequency.
