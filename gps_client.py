#! /usr/bin/env python3
"""
example  Python gpsd client
run this way: python3 example1.py.txt
"""

# from https://gpsd.gitlab.io/gpsd/gpsd-client-example-code.html

# this client is for the CubeSatSim Lite which does not have a Pico microcontroller

import gps               # the gpsd interface module
import time

session = gps.gps(mode=gps.WATCH_ENABLE)

start = time.perf_counter()

mode = -1

lat = 0

lon = 0

alt = 0

try:
    while (session.read() == 0) and ((time.perf_counter() - start) < 2) and (mode < 2):
#        print(gps.MODE_SET)
 #       print(session.valid)
        if (session.valid):
            # not useful, probably not a TPV message
#            continue

#        print('Mode: %s(%d) Time: ' %
#              (("Invalid", "NO_FIX", "2D", "3D")[session.fix.mode],
#               session.fix.mode), end="")
        # print time, if we have it
            print("%d " % session.fix.mode, end="")
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
            print("%.6f %.6f %.6f" %
                      (session.fix.latitude, session.fix.longitude, session.fix.altitude))
           
#            else:
#                print(" 0 0 0")

except KeyboardInterrupt:
    # got a ^C.  Say bye, bye
    print('')

# Got ^C, or fell out of the loop.  Cleanup, and leave.
session.close()
print("%d %.6f %.6f %.1f" % (mode, lat, lon, alt))
exit(0)
