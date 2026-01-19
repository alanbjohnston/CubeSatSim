#! /usr/bin/env python3
"""
example  Python gpsd client
run this way: python3 example1.py.txt
"""

# from https://gpsd.gitlab.io/gpsd/gpsd-client-example-code.html

# this client is for the CubeSatSim Lite which does not have a Pico microcontroller

import gps               # the gpsd interface module
import time
import signal
import time

# Don't turn these signal into exceptions, just die. 
# https://stackoverflow.com/questions/26692284/how-to-prevent-brokenpipeerror-when-doing-a-flush-in-python
signal.signal(signal.SIGINT, signal.SIG_DFL)
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

while True:
    try:
    
        mode = -1    
        lat = 0    
        lon = 0    
        alt = 0
    
        session = gps.gps(mode=gps.WATCH_ENABLE)
        
        start = time.perf_counter()
    
        try:
    #        while (session.read() == 0) and ((time.perf_counter() - start) < 2) and (mode < 2):
            while ((time.perf_counter() - start) < 2) and (mode < 2):
                if session.waiting(2.0):
                    session.read()
                    
        #        print(gps.MODE_SET)
         #       print(session.valid)
                if (session.valid):
                    # not useful, probably not a TPV message
        #            continue
        
        #        print('Mode: %s(%d) Time: ' %
        #              (("Invalid", "NO_FIX", "2D", "3D")[session.fix.mode],
        #               session.fix.mode), end="")
                # print time, if we have it
    #                print("%d " % session.fix.mode, end="")
                    if (session.fix.mode > mode):
                        mode = session.fix.mode
            #        if gps.TIME_SET & session.valid:
            #            print(session.fix.time, end="")
            #        else:
            #            print('n/a', end="")
                
                    if gps.isfinite(session.fix.latitude): 
                        lat = session.fix.latitude
                    if gps.isfinite(session.fix.longitude):         
                        lon = session.fix.longitude 
                    if gps.isfinite(session.fix.altitude):        
                        alt = session.fix.altitude     
    #                print("%.6f %.6f %.6f" % (session.fix.latitude, session.fix.longitude, session.fix.altitude))
                   
        #            else:
        #                print(" 0 0 0")
        
        except KeyboardInterrupt:
            # got a ^C.  Say bye, bye
            print('')
        
        # Got ^C, or fell out of the loop.  Cleanup, and leave.
        session.close()
#        print("%d %.6f %.6f %.1f" % (mode, lat, lon, alt))
    #    exit(0)
        
    except:    
#        print("-1 0 0 0")
        mode = -1    
        lat = 0    
        lon = 0    
        alt = 0
        time.sleep(2)
#        exit(0)
    try:
        inp = input()
        print("%d %.6f %.6f %.1f" % (mode, lat, lon, alt))
#        print(inp)
    except:
        print("Python error getting input!")        
    
