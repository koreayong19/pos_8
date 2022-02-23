import pyaudio
import wave
import os
import pygame
import speech_recognition as sr   

f_list = os.listdir()
print(f_list)

if "voice" in f_list:
    pass
else:
    os.mkdir("voice")

form_1 = pyaudio.paInt16
chans = 1
samp_rate = 44100
chunk = 4096
record_secs = 3
dev_index = 2

wav_output_filename = "voice/unknown.wav"

audio = pyaudio.PyAudio()

stream = audio.open(format = form_1, rate = samp_rate, channels = chans, \
                    input_device_index = dev_index, input = True, \
                    frames_per_buffer = chunk)

print("recording")
frames = []

for ii in range(0,int((samp_rate/chunk)*record_secs)):
    data = stream.read(chunk, exception_on_overflow = False)
    frames.append(data)
    
print("finished recording")

stream.stop_stream()
stream.close()
audio.terminate()

wavefile = wave.open(wav_output_filename,'wb')
wavefile.setnchannels(chans)
wavefile.setsampwidth(audio.get_sample_size(form_1))
wavefile.setframerate(samp_rate)
wavefile.writeframes(b''.join(frames))
wavefile.close()
'''
pygame.init()
pygame.mixer.init()
answer = pygame.mixer.Sound("voice/answer.wav")

answer.play()
'''