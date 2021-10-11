# code based on https://developer.here.com/blog/understanding-geocoding-with-python
#
# copy or rename to loc-foxtelem.py and add api key

import requests
from os import system
import re
import sys
import maidenhead as mh

latitude = 0
longitude = 0
location = "My Location"
grid = ""
return_value = 0
altitude = 0
dquote = '\\"'      

print("\nIn order to track the satellites as they fly over your location and upload your satellite telemetry data to the AMSAT servers, you need to set your location.")
print("\nYou can enter:\n")
print("1. Location name (e.g. country or city, state, etc)")
print("2. Grid square (e.g. FM29ha)")
print("3. Latitude and longitude")
print("4. Leave location unchanged\n")

choice = input ("Enter your choice (1 - 4): ")

if (choice == '2'):  
  
  regex = '[A-Za-z]+[A-Za-z]+[0-9]+[0-9]+[A-Za-z]+[A-Za-z]'

  for i in list(range(3)):
  
    grid = input("\nEnter your 6 character character grid square (i.e. FM29ha): ")

    if re.search(regex, grid):
      print("grid is valid!")
      print (mh.to_location(grid)[0])
      print (mh.to_location(grid)[1])
      latitude = mh.to_location(grid)[0]
      longitude = mh.to_location(grid)[1]
      location = input("\nEnter the name of your location (or hit Return to leave blank): ")
      break
    else:
      print("grid is not valid!")
      grid = ""

elif (choice == '3'):

  for i in list(range(3)):
    lat = input("\nType your latitude (South is negative): ")
    try:
          latitude = float(lat)
          print(latitude)
          break
    except ValueError:
          print("Not a number!")
        
  for i in list(range(3)):        
    long = input("\nType your longitude (West is negative): ")
    try:
          longitude = float(long)
          print(longitude)
          break
    except ValueError:
          print("Not a number!")
    location = input("\nEnter the name of your location (or hit Return to leave blank): ")
    
elif (choice == '1'):          
  URL = "https://geocode.search.hereapi.com/v1/geocode"

  #print("\nEnter your location  including country. \n\n")
  location = input("Enter your location including country: ") #taking user input
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
    print("There is a problem with the location API.  Try setting your location by latitude and longitude or grid square.")

else:
  print("\nYour location is unchanged.")
  print("To track satellites and upload telemetry data,") 
  print("you can set your location in Settings in FoxTelem.")
  
#file = open(r"/home/pi/CubeSatSim/groundstation/.profile","w+")

if (latitude != 0) and (longitude != 0):

  latSedStr = 'sed -i "s/latitude=.*/latitude=' + str(latitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
  #print (latSedStr)
  system(latSedStr)

  longSedStr = 'sed -i "s/longitude=.*/longitude=' + str(longitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
  #print (longSedStr)
  system(longSedStr)

  print("\nFoxTelem configuration updated with your latitude and longitude")
      
  latSedStr = 'sed -i "s/latitude=.*/latitude=' + str(latitude) + '/g" /home/pi/KLATrack/klatracker.properties'
  #print (latSedStr)
  system(latSedStr)

  longSedStr = 'sed -i "s/longitude=.*/longitude=' + str(longitude) + '/g" /home/pi/KLATrack/klatracker.properties'
  #print (longSedStr)
  system(longSedStr)

  print("\nKLAtracker configuration updated with your latitude and longitude")  
      
  receiver_gpsSedStr = 'sudo sed -i "s/        ' + dquote + 'lat' + dquote  + ': .*/        ' + dquote + 'lat' + dquote +  ': ' + str(latitude) + ',/g" /var/lib/openwebrx/settings.json'
  #print (receiver_gpsSedStr)
  system(receiver_gpsSedStr)
      
  receiver_gpsSedStr = 'sudo sed -i "s/        ' + dquote + 'lon' + dquote  + ': .*/        ' + dquote + 'lon' + dquote +  ': ' + str(longitude) + '/g" /var/lib/openwebrx/settings.json'
  #print (receiver_gpsSedStr)
  system(receiver_gpsSedStr)

  nameSedStr = 'sudo sed -i "s/    ' + dquote + 'receiver_location' + dquote  + ': .*/    ' + dquote + 'receiver_location' + dquote +  ': ' + dquote + location + dquote + ',/g" /var/lib/openwebrx/settings.json'
  #print (nameSedStr)
  system(nameSedStr)
        
  print("\nOpenWebRX configuration updated with your latitude and longitude")
         
  return_value = 1
   
  for i in list(range(3)):
    alt = input("\nType your altitude in integer meters: ")
    try:
      altitude = int(alt)
      if (altitude >= 0):
        #print(altitude)
        altSedStr = 'sed -i "s/altitude=.*/altitude=' + str(altitude) + '/g" /home/pi/FoxTelemetryData/FoxTelem.properties'
        #print (altSedStr)
        system(altSedStr)
    
        print("\nFoxTelem configuration updated with your alitude")  
      
        altSedStr = 'sed -i "s/altitude=.*/altitude=' + str(altitude) + '/g" /home/pi/KLATrack/klatracker.properties'
        #print (altSedStr)
        system(altSedStr)
    
        print("\nKLAtracker configuration updated with your alitude")    
      
        aslSedStr = 'sudo sed -i "s/    ' + dquote + 'receiver_asl' + dquote  + ': .*/    ' + dquote + 'receiver_asl' + dquote +  ': ' + str(altitude) + ',/g" /var/lib/openwebrx/settings.json'
        #print (receiver_gpsSedStr)
        system(aslSedStr)
        
        print("\nOpenWebRX configuration updated with your alitude")    
      
        break
      else:
        print("\nAltitude cannot be negative!")
        altitude = 0
    except ValueError:
      print("\nAltitude must be an integer!")
    
  if ((latitude != 0) and (longitude != 0)):
    
      file = open(r"/home/pi/.config/Gpredict/MyLocation.qth","w+")

      str1 = "[QTH]\n"
      str2 = "LOCATION=" + location + "\n"
      str3 = "LAT=" + str(latitude) + "\n"
      str4 = "LON=" + str(longitude) + "\n"
      str5 = "ALT=" + str(altitude) + "\n"
      str6 = "WX=\n"
      str7 = "QTH_TYPE=0\n"

      file.write(str1)
      file.write(str2)
      file.write(str3)
      file.write(str4)
      file.write(str5)
      file.write(str6)
      file.write(str7)

      file.close()
   
      print("\nGpredict configuration updated with your latitude and longitude and altitude\n")
     
sys.exit(return_value)
