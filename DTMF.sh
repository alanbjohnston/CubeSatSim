#!/bin/sh

# Shell script for reading DTMF signals on stdin. 
# Checks for consecutive DTMF signals to create shutdown code "BAD99" 
# Each character must come within a specific time threshold of one another. (Currently 200 ms, for 100 ms tones and 100 ms spaces)
# This threshold can be changed by altering the TIME_THRESH variable

B="DTMF: B"
D="DTMF: D"
NINE="DTMF: 9"
SHUTDOWN_CODE="BAD99"
CUR_CODE="NA"
TIME_THRESH=210000000 #time threshold for receiving DTMF codes in nanoseconds
LAST_CODE="NA" #previous input from stdin
STOPPED=0

while IFS='$\n' read -r line; do # read from stdin
	DT=$(echo $line | cut -d':' -f 1) # cut first part of text
	if [ "$DT" = "DTMF" ]; then # if input is DTMF signal
		if [ "$line" = "$B" ]; then # No need to check time threshold if B, just start over
			CUR_CODE="B"
		else
			T=$(date +"%s%N")  #get current time
			DELTA_T="$((T-LAST_TIME))" #time change
			THRESH="$((TIME_THRESH-DELTA_T))" 
			if [ "$THRESH" -ge 0 ]; then # check threshold
				case "$line" in
					$A)
						if [ "$CUR_CODE" = "B" ]; then
							CUR_CODE=$CUR_CODE"A"
						else
							CUR_CODE="NA"
						fi
						;;
					$B)
						CUR_CODE="B"
						;;
					$D) 
						if [ "$CUR_CODE" = "BA" ]; then
							CUR_CODE=$CUR_CODE"D"
						else
							CUR_CODE="NA"
						fi
						;;
					$NINE) 
						if [ "$CUR_CODE" = "BAD" ] || [ "$CUR_CODE" = "BAD9" ]; then
							CUR_CODE=$CUR_CODE"9"
						else
							CUR_CODE="NA"
						fi
						;;
					*) 
						CUR_CODE="NA"
						;;
				esac	
			else # Time expired, reset code
				CUR_CODE="NA"
			fi
		fi
		LAST_TIME=$(date +"%s%N") # Update previous time
		
	fi
	echo $CUR_CODE
	if [ "$CUR_CODE" = "BAD99" ]; then # Check for complete code
		CUR_CODE="NA"
	#	echo "Termination code received" # Replace this with actual termination command
		ID = $(pgrep radioafsk) 
		if [ "$STOPPED" -eq 0]; then
			kill -TSTP $ID 
			STOPPED=1
		else
			kill -CONT $ID
			STOPPED=0
		fi 

	fi
done

# while IFS='$\n' read -r line; do #read from stdin
# 	if [ "$line" = "$SHUTDOWN_CODE" ]; then #check for code
# 		if [ "$line" = "$LAST_CODE" ]; then #check if second in a row
# 			T=$(date +"%s") #current time
# 			DELTA_T="$((T - LAST_TIME))"
# 			THRESH="$((TIME_THRESH-DELTA_T))"
# 			if [ "$THRESH" -ge  0 ]; then
# 			fi
# 		fi
# 	fi
# 	LAST_CODE=$line #update previous input and timestamp
# 	LAST_TIME=$(date +"%s")
# done 

