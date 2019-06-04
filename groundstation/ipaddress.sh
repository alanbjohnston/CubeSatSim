echo -e "IP Address of this Pi is: "

hostname -I|cut -f1 -d ' '
