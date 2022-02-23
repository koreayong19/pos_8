import thingspeak
import json

READ_API_KEY='MHMG4UXL6R4M3UGT'
WRITE_API_KEY = 'D4NQ354W2XRCURI2'
CHANNEL_ID=931761

def thingspeak_upload(input_data,CHANNEL_ID,READ_API_KEY,WRITE_API_KEY):
    temp_dict = {}
    field_list = ['field1','field2','field3','field4','field5','field6']
    read_channel = thingspeak.Channel(CHANNEL_ID, READ_API_KEY)
    write_channel = thingspeak.Channel(CHANNEL_ID, WRITE_API_KEY)
    origin_data = json.loads(read_channel.get())['feeds'][-1]
    for i in field_list:
        temp_dict[i] = origin_data[i]
    for j in input_data.keys():
        temp_dict[j] = input_data[j]
    write_channel.update(temp_dict)

#input_data = {'field1':5,'field4':5}
#thingspeak_upload(input_data,CHANNEL_ID,READ_API_KEY,WRITE_API_KEY)

def thingspeak_read(CHANNEL_ID,READ_API_KEY):
    read_channel = thingspeak.Channel(CHANNEL_ID, READ_API_KEY)
    origin_data = json.loads(read_channel.get())['feeds'][-1]
    return origin_data

#data = thingspeak_read(CHANNEL_ID,READ_API_KEY)
