import requests
import json
import time
from Adafruit_IO import Client, Feed

#data keyss
ADAFRUIT_IO_USERNAME = 'mikewholey'
ADAFRUIT_IO_KEY = 'c0fda808a03545139cde2e4bba2c4bc3'
channel = 693074
readkey = "3J7Y9ZHRCO37E9KF"

#Create our client then get all the feeds
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
dust_feed = aio.feeds('dust')
heartbeat_feed = aio.feeds('heartbeat')
light_feed = aio.feeds('light')
pressure_feed = aio.feeds('pressure')
altitude_feed = aio.feeds('altitude')
uv_feed = aio.feeds('uv')
temperature_feed = aio.feeds('temperature')
humidity_feed = aio.feeds('humidity')


#Mapping of thingspeak fields to adafruit feeds
mapping = {1 : humidity_feed,
			2 : temperature_feed,
			3 : pressure_feed,
			4 : altitude_feed,
			5 : light_feed,
			6 : dust_feed,
			7 : uv_feed}


while True:
	#Build our thingspeak url
	rurl = "http://api.thingspeak.com/channels/" + str(channel) + "/feeds/last.json?api_key=" + readkey

	#get the data
	r = requests.get(rurl)
	ourData = json.loads(r.text)

	#For each field we create the field name, print the data
	#to the console and then send it to adafruit
	for x in range(1,8):
		field = 'field'+str(x)
		print(str(x) + " : " + str(ourData[field]))
		if (ourData[field] == None):
			aio.send(mapping[x].key, 0)
		else:
			aio.send(mapping[x].key, ourData[field])
	#Sleep for 20 seconds to avoid going over AdaFruit API rate limit
	time.sleep(20)

		
	# for x in [1, 5, 6]:
	# 	field = 'field'+str(x)
	# 	#aio.send(temperature_feed.key, str(temperature))
	# 	print(str(x) + " : " + str(ourData[field]))
	# 	if (ourData[field] == None):
	# 		#print("No Data, field: " + str(x))
	# 		aio.send(mapping[x].key, 0)
	# 	else:
	# 		aio.send(mapping[x].key, ourData[field])
	# time.sleep(6)