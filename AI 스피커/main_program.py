from speaker_file import speaker_main
from thingspeak_command import thingspeak_upload,thingspeak_read
import time

WRITE_API_KEY = "D4NQ354W2XRCURI2"
READ_API_KEY = "MHMG4UXL6R4M3UGT"
CHANNEL_ID = 931761

while True:
    print("시작!")
    while True:
        time.sleep(15)
        data = thingspeak_read(CHANNEL_ID,READ_API_KEY)
        stress_info, face_info = int(data['field5']), int(data['field6'])
        print("받은 값:",stress_info,face_info)
        if (stress_info == 1) and (face_info == 1):
            break
        else:
            continue
    print("스피커를 실행합니다.")
    speaker_main()
    thingspeak_upload({'field5':-1,'field6':-1},CHANNEL_ID,READ_API_KEY,WRITE_API_KEY)