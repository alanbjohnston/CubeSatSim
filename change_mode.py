#!/usr/bin/env python

import time
from time import sleep
#import os
import sys

if __name__ == "__main__":
  if (len(sys.argv)) > 1:
#        	print("There are arguments!")
		if (('d' == sys.argv[1]) or ('-d' in sys.argv[1])):
			debug_mode = 1
		elif (('x' == sys.argv[1]) or ('-x' in sys.argv[1])):	
			mode = "x"
			print("Forcing APRS mode")
		elif (('s' == sys.argv[1]) or ('-s' in sys.argv[1])):
			skip = True
			print("Skipping delay and CW ID")
