# code based on https://developer.here.com/blog/understanding-geocoding-with-python
#
# copy or rename to loc-foxtelem.py and add api key

import requests
from os import system
import re

latitude = 0
longitude = 0

print("\nIn order to upload your satellite telemetry data to the AMSAT servers, we need to know your location")
print("\nYou can enter:\n")
print("1. Location name (e.g. country or city, state, etc)")
print("2. Grid square (e.g. FM29)")
print("3. Latitude and longitude")
print("4. No location\n")

choice = input ("Enter your choice (1 -4): ")

if (choice == '2'):    
  regex = '[A-Z]+[A-Z]+[0-9]+[0-9]'

  grid = input("\nType your 4 character grid square (i.e. FM29): ")

  if re.search(regex, grid):
    print("grid is valid!")
  else:
    print("grid is not valid!")
    
elif (choice == '3'):
  
  lat = input("\nType your latitude: ")
  try:
          float(lat)
          print("Valid number!")
  except ValueError:
          print("Not a number!")
  long = input("\nType your longitude: ")
  try:
          float(long)
          print("Valid number!")
  except ValueError:
          print("Not a number!")
      
elif (choice == '1'):          
  URL = "https://geocode.search.hereapi.com/v1/geocode"

  print("\nEnter your location  including country. \n\n")
  location = input("Type your location: ") #taking user input
  api_key = '' # Acquire from developer.here.com
  PARAMS = {'apikey':api_key,'q':location} 

  try:
    # sending get request and saving the response as response object 
    r = requests.get(url = URL, params = PARAMS) 
    data = r.json()

    #print(data)

    latitude = data['items'][0]['position']['lat']
    longitude = data['items'][0]['position']['lng']

    print(latitude)
    print(longitude)
  
  except:
    print("There is a problem with the location API.  Please try again")

else:
  print("\nYou have chosen not to enter your location.")
  print("To track satellites and upload telemetry data,") 
  print("you can set your location in Settings in FoxTelem.")
  
#file = open(r"/home/pi/CubeSatSim/groundstation/.profile","w+")

if ((latitude != 0) and (longitude != 0)):

  latSedStr = 'sed -i "s/latitude=.*/latitude=' + str(latitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
  #print (latSedStr)
  system(latSedStr)

  longSedStr = 'sed -i "s/longitude=.*/longitude=' + str(longitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
  #print (longSedStr)
  system(longSedStr)

  print("\nFoxTelem configuration updated with your latitude and longitude")

if grid:

  gridSedStr = 'sed -i "s/maidenhead=.*/maidenhead=' + grid + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
  #print (gridSedStr)
  system(gridSedStr)

  print("\nFoxTelem configuration updated with your Maidenhead grid")



