#!/usr/bin/python3
import iot
import requests
import picamera
import serial
import io
import os
import time

ser = serial.Serial('/dev/ttyACM0', timeout=1, baudrate=115200)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

BOT_API_TOKEN ='Insert your Telegram token'
GROUP_ID = 'Insert your Telegram groupID'
TG_URL = 'https://api.telegram.org/bot'+ BOT_API_TOKEN


# Kuuntele sanaa "hello", ja vastaa "Hello, world!" 
@iot.listen("start counting")
def start_command():
    print('1\n')
    ser.write(b'1\n')
    url = TG_URL + '/sendMessage'
    telegram_message = 'OK, I will count!'
    res = requests.post(url, params={'chat_id': GROUP_ID}, data ={'text': telegram_message})

@iot.listen("stop counting")
def stop_command():
    ser.write(b'0\n')
    answered = False
    BPM = '180'
    trials = 10
    while not answered:
        try:
            serial_in = int(sio.readline())
            trials += 1
            message = str(serial_in)
            print('Found in Serial: ' +message)
            if serial_in != -1:
                BPM = str(serial_in)
            if serial_in == -1:
                answered = True
            if trials > 10:
                BMP = '220'
                answered = True

        except ValueError:
            print('Could not get the Value from Arduino')
            pass
    url = TG_URL + '/sendMessage'
    telegram_message = 'You jumped {} times! Well Done!'.format(BPM)
    res = requests.post(url, params={'chat_id': GROUP_ID}, data ={'text': telegram_message})
            
        
    ser.write(b'0\n')
    url = TG_URL + '/sendMessage'
    telegram_message = 'stop jumps'
    res = requests.post(url, params={'chat_id': GROUP_ID}, data ={'text': telegram_message})

@iot.listen("photo")
def take_photo():
    print('2\n')
    ser.write(b'2\n')
    camera_capture('image.jpg', 'photo')
    send_to_telegram('image.jpg', 'photo')

@iot.listen("record a video")
def take_video():
    print('3\n')
    ser.write(b'3\n')
    camera_capture('video.h264', 'video', length=4)
    convert_h264_to_mp4()
    send_to_telegram('video.mp4', 'video')
    

def camera_capture(filename, type, length=3):
    with picamera.PiCamera() as camera:
        with open(filename, 'wb') as capture_file:
            """ Lisaa kamera-asetuksia voi muokata tahan.
                https://picamera.readthedocs.io/en/release-1.12/api_camera.html """
            camera.hflip = True # kaanna vaaka-akselin suhteen
            camera.vflip = True # peilikuva
            if type == 'photo':
                camera.resolution = (1296, 972)
                camera.capture(capture_file)
            elif type == 'video':
                camera.resolution = (640, 480)
                camera.framerate = 24
                camera.start_recording(capture_file)
                camera.wait_recording(length)
                camera.stop_recording()

def send_to_telegram(filename, type):
    """ Funktio lahettaa telegramiin tyypista riippuen 
        kuvan tai videon. """
    with open(filename, 'rb') as capture_file:
        if type == 'photo':
            url = TG_URL + '/sendPhoto'
            files = {"photo": capture_file}
        elif type == 'video':
            url = TG_URL + '/sendVideo'
            files = {"video": capture_file}
        res = requests.post(url, files=files, params={'chat_id': GROUP_ID})

def handle_video():
    camera_capture('video.h264', 'video', length=4)
    convert_h264_to_mp4()
    send_to_telegram('video.mp4', 'video')
    return '{"success":"true"}'

def convert_h264_to_mp4():
    """ Muuntaa raa'an .h264 videon mp4:ksi MP4Box:n avulla.
        Huom! Vaatii koneelta GPAC-binaarit.
        >> sudo apt-get update
        >> sudo apt-get install gpac """
    os.system('rm video.mp4')
    os.system('MP4Box -add video.h264:fps=24 video.mp4')
    os.system('rm video.h264')
    
@iot.listen("sparkle")
def said_forward():
    iot.publish("messages","sparkle")
    iot.say("OK, sparkle")

    


@iot.listen("How are you")
def said_How_are_you():
    iot.say("I'm good. Thank you for asking ")

iot.run("voice1","shiftr-key","shiftr-secret")
 
