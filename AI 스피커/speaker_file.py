import speech_recognition as sr     
import pyaudio
import wave
import os
import pygame
import time
import os
import sys
import random
from urllib.request import urlopen, Request
from thingspeak_command import thingspeak_upload,thingspeak_read
import urllib
import bs4

WRITE_API_KEY = "D4NQ354W2XRCURI2"
READ_API_KEY = "MHMG4UXL6R4M3UGT"
CHANNEL_ID = 931761

def change_song(f_list,music_path,genre,mus_num,command):
    f_len = len(f_list)
    
    if command == "next":
        pygame.mixer.music.stop()
        next_music = pygame.mixer.Sound("voice/nextmusic.wav")
        time.sleep(3)
        if mus_num == (f_len-1):
            mus_num = 0
        else:
            mus_num += 1
        mus_name = f_list[mus_num]
        pygame.mixer.music.load(music_path + "/" + genre + "/" + str(mus_name))
        pygame.mixer.music.set_volume(0.5)
        pygame.mixer.music.play()
        
    elif command == "rear":
        pygame.mixer.music.stop()
        next_music = pygame.mixer.Sound("voice/rearmusic.wav")
        time.sleep(3)
        if mus_num == 0:
            mus_num = (f_len-1)
        else:
            mus_num -= 1
        mus_name = f_list[mus_num]
        pygame.mixer.music.load(music_path + "/" + genre + "/"+str(mus_name))
        pygame.mixer.music.set_volume(0.5)
        pygame.mixer.music.play()
    else:
        pass
    return mus_num,mus_name
    
def change_volume(command):
    volume = pygame.mixer.music.get_volume()
    if command == "up":
        if (volume+0.2) >= 1:
            pygame.mixer.music.set_volume(1)
        else:
            pygame.mixer.music.set_volume((volume+0.2))
    elif command == "down":
        if (volume-0.2) <= 0:
            pygame.mixer.music.set_volume(0)
        else:
            pygame.mixer.music.set_volume((volume-0.2))
    else:
        pass

def genre_change(music_path,change_genre,genre):
    f_list = os.listdir(music_path + "/" + genre)
    random.shuffle(f_list)
    mus_num = random.randint(0,len(f_list)-1)
    mus_name = f_list[mus_num]
    change_genre = pygame.mixer.Sound("voice/changegenre.wav")
    change_genre.play()
    time.sleep(2)
    pygame.mixer.music.load(music_path + "/" + genre + "/" +str(mus_name))
    pygame.mixer.music.set_volume(0.5)
    pygame.mixer.music.play()
    change_genre = False
    return f_list, mus_num, mus_name, change_genre

def speaker_main():
    pygame.init()
    pygame.mixer.init()

    stress = ""
    emotion = ""
    music_path = ""
    genre = "DEFAULT"
    change_genre = False
    music_pause = False
    data = thingspeak_read(CHANNEL_ID,READ_API_KEY)
    stress_val, emo_val, light_val = int(data['field1']),int(data['field2']),int(data['field3'])
    if light_val == -1:
        light_on = False
    else:
        light_on = True
    
    print(stress_val,emo_val, light_val)
    
    if stress_val == 1:
        stress = "O"
    elif stress_val == 0:
        stress = "X"
    else:
        raise ValueError("Wrong value!")

    if emo_val == 1:
        emotion = "P"
    elif emo_val == 0:
        emotion = "NE"
    elif emo_val == -1:
        emotion = "N"
    else:
        raise ValueError("Wrong value!")
   

    if (stress == "O"):
        o_num = random.randint(1,3)
        o1 = pygame.mixer.Sound("voice/O1-"+str(o_num)+".wav")
        o1.play()
        time.sleep(3)
        o2 = pygame.mixer.Sound("voice/O2.wav")
        o2.play()
        music_path = "voice/music_p"
    elif (stress == "X") and (emotion == "P"):
        xp1 = pygame.mixer.Sound("voice/XP1.wav")
        xp1.play()
        time.sleep(3)
        xp2 = pygame.mixer.Sound("voice/XP2.wav")
        xp2.play()
        music_path = "voice/music_n"
    elif (stress == "X") and ((emotion == "NE") or (emotion == "N")):
        xn1 = pygame.mixer.Sound("voice/XN1.wav")
        xn1.play()
        time.sleep(3)
        xn2 = pygame.mixer.Sound("voice/XN2.wav")
        xn2.play()
        music_path = "voice/music_n"

    while True:
        time.sleep(2)
        r = sr.Recognizer()
        r.dynamic_energy_threshold = False
        try:
            with sr.Microphone() as source:
                print("Say something!")
                audio = r.listen(source, timeout = 1.5)
            music_on = False
            command_raw = r.recognize_google(audio, language = "ko-KR")
            command = command_raw.split()
            print(command)

            if ("스피커" in command) and ("꺼" in command):
                end_ment = pygame.mixer.Sound("voice/endspeaker.wav")
                end_ment.play()
                time.sleep(2)
                break
            elif (("전등" in command) or ("등" in command) or ("무드등" in command)) and ("켜" in command):
                data = thingspeak_read(CHANNEL_ID,READ_API_KEY)
                light_val = int(data['field3'])
                if light_val == -1:
                    light_on = False
                else:
                    light_on = True
                if light_on == False:
                    start_light = pygame.mixer.Sound("voice/startlight.wav")
                    start_light.play()
                    light_on = True
                    if stress == "O":
                        if emotion == "P": #Green
                            light_color = 4
                        else: #Blue
                            light_color = 2
                    elif stress == "X":
                        if emotion == "P": #Red
                            light_color = 3
                        elif emotion == "NE": #Brown
                            light_color = 5
                        elif emotion == "N": #Orange
                            light_color = 1
                    thingspeak_upload({'field3': light_color},CHANNEL_ID,READ_API_KEY,WRITE_API_KEY)
                    time.sleep(2)
                elif light_on == True:
                    already_on_light = pygame.mixer.Sound("voice/already_on_light.wav")
                    already_on_light.play()
                    time.sleep(2)
            elif (("전등" in command) or ("등" in command) or ("무드등" in command)) and ("꺼" in command):
                data = thingspeak_read(CHANNEL_ID,READ_API_KEY)
                light_val = int(data['field3'])
                if light_val == -1:
                    light_on = False
                else:
                    light_on = True
                if light_on == True:
                    end_light = pygame.mixer.Sound("voice/endlight.wav")
                    end_light.play()
                    light_on = False
                    thingspeak_upload({'field3': -1},CHANNEL_ID,READ_API_KEY,WRITE_API_KEY)
                    time.sleep(2)
                elif light_on == False:
                    already_off_light = pygame.mixer.Sound("voice/already_off_light.wav")
                    already_off_light.play()
                    time.sleep(2)
            elif (("노래" in command) or ("음악" in command)) and (("켜" in command) or ("틀어" in command)):
                f_list = os.listdir(music_path + "/" + genre)
                random.shuffle(f_list)
                mus_num = random.randint(0,len(f_list)-1)
                mus_name = f_list[mus_num]
                play_music = pygame.mixer.Sound("voice/startmusic.wav")
                play_music.play()
                music_on = True
                time.sleep(2)
                pygame.mixer.music.load(music_path + "/" + genre + "/" + str(mus_name))
                pygame.mixer.music.set_volume(0.5)
                pygame.mixer.music.play()
            elif (("안녕" in command) or ("안녕하세요" in command)) and (("엘리스" in command) or ("앨리스" in command)):
                hello = pygame.mixer.Sound("voice/hello.wav")
                hello.play()
                time.sleep(2)
            elif (("반가워" in command) or ("반갑습니다" in command)) and (("엘리스" in command) or ("앨리스" in command)):
                nicetomeet = pygame.mixer.Sound("voice/nicetomeet.wav")
                nicetomeet.play()
                time.sleep(2)
                
            elif (("오늘" in command) or ("포항" in command)) and ("날씨" in command):
                location = '포항'
                enc_location = urllib.parse.quote(location + '+날씨')

                url = 'https://search.naver.com/search.naver?ie=utf8&query='+ enc_location

                req = Request(url)
                page = urlopen(req)
                html = page.read()
                soup = bs4.BeautifulSoup(html,'html5lib')
                today_morning_temp = soup.find('div', class_ = "today_area _mainTabContent").findAll('p', class_='info_temperature')[0].find('span', class_='todaytemp').text
                today_morning_weather = soup.find('div', class_ = "today_area _mainTabContent").find('p', class_='cast_txt').text.split(',')[0]
                temperature = int(today_morning_temp)
                weather = today_morning_weather
                if weather == "맑음":
                    weather_name = "good_weather"
                elif (weather == "흐림") or (weather == "구름많음"):
                    weather_name = "bad_weather"
                elif (weather == "비") or (weather == "흐리고 비"):
                    weather_name = "rain_weather"
                wea1 = pygame.mixer.Sound("voice/today_pohang.wav")
                wea1.play()
                time.sleep(1)
                if temperature >=0:
                    if temperature<=10:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    elif temperature<20:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)[0]+"0.wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/"+str(temperature)[1]+".wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/do.wav")
                        wea5.play()
                        time.sleep(0.5)
                        wea6 = pygame.mixer.Sound("voice/nida.wav")
                        wea6.play()
                        time.sleep(2)
                    elif temperature == 20:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    
                elif temperature <0:
                    if temperature>=(-10):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    elif temperature>(-20):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)[0]+"0.wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/"+str(temperature)[1]+".wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/do.wav")
                        wea5.play()
                        time.sleep(0.5)
                        wea6 = pygame.mixer.Sound("voice/nida.wav")
                        wea6.play()
                        time.sleep(2)
                    elif temperature == (-20):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
            
            elif (("오늘" in command) or ("서울" in command)) and ("날씨" in command):
                location = '서울'
                enc_location = urllib.parse.quote(location + '+날씨')

                url = 'https://search.naver.com/search.naver?ie=utf8&query='+ enc_location

                req = Request(url)
                page = urlopen(req)
                html = page.read()
                soup = bs4.BeautifulSoup(html,'html5lib')
                today_morning_temp = soup.find('div', class_ = "today_area _mainTabContent").findAll('p', class_='info_temperature')[0].find('span', class_='todaytemp').text
                today_morning_weather = soup.find('div', class_ = "today_area _mainTabContent").find('p', class_='cast_txt').text.split(',')[0]
                temperature = int(today_morning_temp)
                weather = today_morning_weather
                if weather == "맑음":
                    weather_name = "good_weather"
                elif (weather == "흐림") or (weather == "구름많음"):
                    weather_name = "bad_weather"
                elif (weather == "비") or (weather == "흐리고 비"):
                    weather_name = "rain_weather"
                wea1 = pygame.mixer.Sound("voice/today_seoul.wav")
                wea1.play()
                time.sleep(1)
                if temperature >=0:
                    if temperature<=10:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    elif temperature<20:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)[0]+"0.wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/"+str(temperature)[1]+".wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/do.wav")
                        wea5.play()
                        time.sleep(0.5)
                        wea6 = pygame.mixer.Sound("voice/nida.wav")
                        wea6.play()
                        time.sleep(2)
                    elif temperature == 20:
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    
                elif temperature <0:
                    if temperature>=(-10):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                    elif temperature>(-20):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)[0]+"0.wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/"+str(temperature)[1]+".wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/do.wav")
                        wea5.play()
                        time.sleep(0.5)
                        wea6 = pygame.mixer.Sound("voice/nida.wav")
                        wea6.play()
                        time.sleep(2)
                    elif temperature == (-20):
                        wea2 = pygame.mixer.Sound("voice/"+weather_name+".wav")
                        wea2.play()
                        time.sleep(1.5)
                        minus = pygame.mixer.Sound("voice/minus.wav")
                        minus.play()
                        time.sleep(0.5)
                        wea3 = pygame.mixer.Sound("voice/"+str(temperature)+".wav")
                        wea3.play()
                        time.sleep(0.5)
                        wea4 = pygame.mixer.Sound("voice/do.wav")
                        wea4.play()
                        time.sleep(0.5)
                        wea5 = pygame.mixer.Sound("voice/nida.wav")
                        wea5.play()
                        time.sleep(2)
                
            elif (("너" in command) or ("네" in command) or ("니" in command)) and (("이름" in command) or ("이름이" in command)) and (("뭐야" in command) or ("뭐니" in command)):
                name1 = pygame.mixer.Sound("voice/name1.wav")
                name1.play()
                time.sleep(2)
                name2 = pygame.mixer.Sound("voice/name2.wav")
                name2.play()
                time.sleep(3)
            elif len(command) == 0:
                un_num = random.randint(1,2)
                un_answer = pygame.mixer.Sound("voice/unknown"+ str(un_num) +".wav")
                un_answer.play()
                time.sleep(3)
                continue
            else:
                un_num = random.randint(1,2)
                un_answer = pygame.mixer.Sound("voice/unknown"+ str(un_num) +".wav")
                un_answer.play()
                time.sleep(3)
                continue
            while music_on:
               time.sleep(3)
               while True:
                   try:
                       with sr.Microphone() as source:
                           print("Say something!")
                           audio = r.listen(source, timeout = 3)
                           command_raw = r.recognize_google(audio, language = "ko-KR")
                           command = command_raw.split()
                           print(command)
                           
                       if len(command) == 0:
                           continue
                       elif (("노래" in command) or ("음악" in command)) and ("꺼" in command):
                           answer = pygame.mixer.Sound("voice/endmusic.wav")
                           answer.play()
                           time.sleep(2)
                           pygame.mixer.music.stop()
                           music_on = False
                           break
                       elif (("다른" in command) and (("노래" in command) or ("장르" in command))):
                           if change_genre == False:
                               pygame.mixer.music.stop()
                               time.sleep(1)
                               stop_genre = pygame.mixer.Sound("voice/whichgenre.wav")
                               stop_genre.play()
                               time.sleep(2)
                               change_genre = True
                       elif (("팝" in command) or ("팝음악" in command)):
                           if change_genre == True:
                               genre = "POP"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif (("락" in command) or ("락음악" in command)):
                           if change_genre == True:
                               genre = "ROCK"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif ("재즈" in command):
                           if change_genre == True:
                               genre = "JAZZ"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif ("신나는" in command):
                           if change_genre == True:
                               genre = "ACOUSTIC"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif ("잔잔한" in command):
                           if change_genre == True:
                               genre = "URBAN"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif (("기본" in command) or ("원래" in command)):
                           if change_genre == True:
                               genre = "DEFAULT"
                               f_list, mus_num, mus_name, change_genre = genre_change(music_path,change_genre,genre)
                       elif (("정지" in command) or ("일시" in command)) and (music_pause == False):
                           pygame.mixer.music.pause()
                           music_pause = True
                       elif (("재생" in command) or ("다시" not in command)) and (music_pause == True):
                           pygame.mixer.music.unpause()
                           music_pause = False
                       elif (("재생" in command) and ("다시" in command)) or ("처음부터" in command):
                           pygame.mixer.music.rewind()
                       elif ("이전" in command) and ("노래" in command):
                           mus_num, mus_name = change_song(f_list,music_path,genre,mus_num,"rear")
                       elif ("다음" in command) and ("노래" in command):
                           mus_num, mus_name = change_song(f_list,music_path,genre,mus_num,"next")
                       elif (("볼륨" in command) or ("소리" in command)) and (("높여" in command) or ("올려" in command)):
                           change_volume("up")
                       elif (("볼륨" in command) or ("소리" in command)) and (("낮춰" in command) or ("내려" in command)):
                           change_volume("down")
                       else:
                           continue
                   except:
                       continue
        except:
            continue
if __name__ == "__main__":
    speaker_main()