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

try:
    while session.read() == 0 and ((time.perf_counter(0 - start)) < 1):
        print(gps.MODE_SET)
        print(session.valid)
        if not (gps.MODE_SET & session.valid):
            # not useful, probably not a TPV message
            continue

#        print('Mode: %s(%d) Time: ' %
#              (("Invalid", "NO_FIX", "2D", "3D")[session.fix.mode],
#               session.fix.mode), end="")
        # print time, if we have it
        print("%d " % session.fix.mode, end="")
#        if gps.TIME_SET & session.valid:
#            print(session.fix.time, end="")
#        else:
#            print('n/a', end="")

        if ((gps.isfinite(session.fix.latitude) and
             gps.isfinite(session.fix.longitude))):
            print("%.6f %.6f" %
                  (session.fix.latitude, session.fix.longitude))
        else:
            print(" 0 0")

except KeyboardInterrupt:
    # got a ^C.  Say bye, bye
    print('')

# Got ^C, or fell out of the loop.  Cleanup, and leave.
session.close()
exit(0)
