from gtts import gTTS
import pygame
import os
from pydub import AudioSegment



#text = "Hello, I'm from Korea. Nice to meet you."
#tts = gTTS(text,lang = 'en')

#with open('voice/unknown.mp3', 'wb') as f:
#    tts.write_to_fp(f)
path_input = input() #"voice/music"
f_path = path_input

f_list = os.listdir(f_path)

for i in f_list:
    src = f_path + "/" + i #mp3 file
    dst = f_path + "/" + i[:-3] #wave file                                            
    sound = AudioSegment.from_mp3(src)
    sound.export(dst, format="wav")
    os.remove(src)

print(os.listdir(f_path))