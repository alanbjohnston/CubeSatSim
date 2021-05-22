# code based on https://developer.here.com/blog/understanding-geocoding-with-python

import requests

URL = "https://geocode.search.hereapi.com/v1/geocode"
print("In order to track satellites, we need to know your location\n")
print("You can enter your state or country or region\n")
print("as your exact location is not needed. \n\n")
location = input("Type your location: ") #taking user input
api_key = 'EabUlpQK8GcVVEMs2OLI0Bdb6A9Wj_yeli6d4Wwe8u0' # Acquire from developer.here.com
PARAMS = {'apikey':api_key,'q':location} 

# sending get request and saving the response as response object 
r = requests.get(url = URL, params = PARAMS) 
data = r.json()

print(data)

latitude = data['items'][0]['position']['lat']
longitude = data['items'][0]['position']['lng']

print(latitude)
print(longitude)

file = open(r"/home/pi/.config/Gpredict/MyLocation.qth","w+")

str1 = "[QTH]\n"
str2 = "LOCATION=" + location + "\n"
str3 = "LAT=" + str(latitude) + "\n"
str4 = "LON=" + str(longitude) + "\n"
str5 = "ALT=0\n"
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
