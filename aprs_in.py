import sys
from os import system

for line in sys.stdin:
	if '^c' == line.rstrip():
		break
#	print(line)
	if (line.find("MODE=a")) > 0:
		system("echo 'APRS Mode!!'")
		mode = 'a'
		change_mode = True
	if (line.find("MODE=f")) > 0:
		system("echo 'FSK Mode!!'")
	if (line.find("MODE=b")) > 0:
		system("echo 'BPSK Mode!!'")
	if (line.find("MODE=s")) > 0:
		system("echo 'SSTV Mode!!'")
	if (line.find("MODE=m")) > 0:
		system("echo 'CW Mode!!'")
print("Done")
