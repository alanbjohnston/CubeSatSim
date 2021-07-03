# code based on https://developer.here.com/blog/understanding-geocoding-with-python
#
# copy or rename to loc-foxtelem.py and add api key

import requests
from os import system
import re

latitude = 0
longitude = 0

print("\nIn order to upload your satellite telemetry data to the AMSAT servers, we need to know your location\n")
print("\nYou can enter:")
print("1. Location name (e.g. country or city, state, etc)")
print("2. Grid square (e.g. FM29)")
print("3. Latitude and longitude")
print("4. No location")

choice = input ("Enter your choice (1 -4): ")

if (choice == '2'):    
  regex = '[A-Z]+[A-Z]+[0-9]+[0-9]'

  grid = input("Type your 4 character grid square (i.e. FM29): ")

  if re.search(regex, grid):
    print("grid is valid!")
  else:
    print("grid is not valid!")
    
elif (choice == '3'):
  
  lat = input("Type your latitude: ")
  try:
          float(lat)
          print("Valid number!")
  except ValueError:
          print("Not a number!")
  long = input("Type your longitude: ")
  try:
          float(long)
          print("Valid number!")
  except ValueError:
          print("Not a number!")
      
elif (choice == '1'):          
  URL = "https://geocode.search.hereapi.com/v1/geocode"

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

else:
  print("You have chosen not to enter your location.")
  print("To track satellites and upload telemetry data,") 
  print("you can set your location in Settings in FoxTelem.")
  
#file = open(r"/home/pi/CubeSatSim/groundstation/.profile","w+")

latSedStr = 'sed -i "s/latitude=0.0/latitude=' + str(latitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
#print (latSedStr)
system(latSedStr)

longSedStr = 'sed -i "s/longitude=0.0/longitude=' + str(longitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
#print (longSedStr)
system(longSedStr)

print("\nFoxTelem configuration updated with your latitude and longitude")

