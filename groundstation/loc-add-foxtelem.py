# code based on https://developer.here.com/blog/understanding-geocoding-with-python
#
# copy or rename to loc-foxtelem.py and add api key

import requests
from os import system

URL = "https://geocode.search.hereapi.com/v1/geocode"
print("\nIn order to upload your satellite telemetry data to the AMSAT servers, we need to know your location\n")
print("Enter your location  including country. \n\n")
location = input("Type your location: ") #taking user input
api_key = '' # Acquire from developer.here.com
PARAMS = {'apikey':api_key,'q':location} 

# sending get request and saving the response as response object 
r = requests.get(url = URL, params = PARAMS) 
data = r.json()

#print(data)

latitude = data['items'][0]['position']['lat']
longitude = data['items'][0]['position']['lng']

print(latitude)
print(longitude)

#file = open(r"/home/pi/CubeSatSim/groundstation/.profile","w+")

latSedStr = 'sed -i "s/latitude=0.0/latitude=' + str(latitude) + '/g" /home/pi/home/pi/FoxTelemetryData/FoxTelem.properties'
#print (latSedStr)
system(latSedStr)

longSedStr = 'sed -i "s/longitude=0.0/longitude=' + str(longitude) + '/g" /home/pi/home/pi/FoxTelemetryData/FoxTelem.properties'
#print (longSedStr)
system(longSedStr)

print("\nFoxTelem configuration updated with your latitude and longitude")

