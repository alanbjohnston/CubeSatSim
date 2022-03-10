import tago
 
print("Input telemetry string")
 
telem_string = input()
 
chunks = telem_string.split(' ')
 
#printchunks = str.split(' ')
 
print(chunks)

temp = 0
pressure = 0
altitude = 0
humidity = 0

timestamp = chunks[0] + " " + chunks[1]
print(timestamp)

lat1 = chunks[3].split("=")
lat2_dm = lat1[1].split("N")
lat_float = float(lat2_dm[0])

lon1 = lat2_dm[1].split("/")
lon2_dm = lon1[1].split("W")
lon_float = float(lon2_dm[0])

print(lat_float)
print(lon_float)

lat_int = int(lat_float/100)
lat_mm = int(lat_float - lat_int * 100)
lat_mm2 = lat_float - int(lat_float) * 100

lat = lat_int + (lat_mm / 60.0) + (lat_mm2 / (60.0 * 60.0))

lon_int = int(lon_float/100)
lon_mm = int(lon_float - lon_int * 100)
lon_mm2 = lon_float - int(lon_float) * 100

lon = -1.0 * (lon_int + (lon_mm / 60.0) + (lon_mm2 / (60.0 * 60.0)))

print(lat)
print(lon)

lat = lat_float / 100.0
lon = lon_float / (-100.0)

for i in range(len(chunks)):
    if (chunks[i] == "BME280"):
        print("Found BME280")
        temp = chunks[i+1]
        pressure = chunks[i+2]
        altitude = chunks[i+3]
        humidity = chunks[i+4]
        print(temp)
        print(humidity)
       
 
#print(telem_string)
 
 
my_device = tago.Device('a824cdc6-dc87-4c54-a848-41dabb8873ad')
 
"""
The following code defines the set of data to be sent to TagoIO
data fields:
- variable name
- variable unit
- variable value
- Optional: desired data timestamp
- Optional: lat/long location (associated to your data)
"""
data = {
            'variable': 'temperature',               
            'unit'    : 'C',                                   
            'value'   : temp,                                                   
            'time'    : timestamp,                          
            'location': {'lat': lat, 'lng': lon}   
}
 
result = my_device.insert(data)
print(result)

#print(data)

data = {
            'variable': 'pressure',               
            'unit'    : 'kPa',                                   
            'value'   : pressure,                                                   
            'time'    : timestamp,                          
            'location': {'lat': lat, 'lng': lon}   
} 

result = my_device.insert(data)
print(result)

data = {
            'variable': 'altitude',               
            'unit'    : 'm',                                   
            'value'   : altitude,                                                   
            'time'    : timestamp,                          
            'location': {'lat': lat, 'lng': lon}   
} 

result = my_device.insert(data)
print(result)
 
data = {
            'variable': 'humidity',               
            'unit'    : '%',                                   
            'value'   : humidity,                                                   
            'time'    : timestamp,                          
            'location': {'lat': lat, 'lng': lon}   
} 

result = my_device.insert(data)
print(result)
 
data = {
  "variable": "location",
  "value": "Villanova University HAB-2",
  "location": {
    "lat": lat,
    "lng": lon 
  }
}
result = my_device.insert(data)
print(result)
