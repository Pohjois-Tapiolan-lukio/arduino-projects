#!/bin/python3
"""
IoT-apuri on kirjasto jolla saa helposti tehtyä ohjelmia,
jotka hyödyntävät Googlen Assistantia ääniohjaukseen,
ja Shiftr.io:ta tiedonsiirtoon.

Lisää README.md:ssä.
"""

import time
import sys
import multiprocessing

import paho.mqtt.client as mqtt

import requests
import serial
import io

ser = serial.Serial('/dev/ttyACM0', timeout=1, baudrate=115200)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

BOT_API_TOKEN ='Insert Your Telegram Token here'
GROUP_ID = 'Insert your Telegram groupId'
TG_URL = 'https://api.telegram.org/bot'+ BOT_API_TOKEN

from google.assistant.library.event import EventType
import aiy.assistant.auth_helpers
from aiy.assistant.library import Assistant
import aiy.audio
import aiy.voicehat

DATA = {
    "subs": {},
    "phrases": {},
    "initialized": False  # default False
}

DEBUG = False

# Shiftr.io-stuff

def run(shiftr_name=0, shiftr_key=0, shiftr_pass=0):
    """
    Yhdistää shiftr.io palvelimelle ja käynnistää Google Assistantin.
    """

    if shiftr_name != 0 and shiftr_key != 0 and shiftr_pass != 0:
        def handle_connect(_client, _userdata, _flags, _code):
            """
            Kutsutaan kun client yhdistää shiftr.io:n palvelimelle.
            """
            print("Shiftr.io connected!")
            DATA["initialized"] = True

        def handle_message(_client, _userdata, msg):
            """
            Kutsutaan kun client saa viestin shiftr.io:n palvelimilta.
            """
            call_subscriptions(msg.topic, msg.payload.decode("utf-8"))

        if shiftr_key == "shiftr-key":
            print("HUOM!  Muista korvata 'shiftr-key' omalla shiftr keyllasi!")
        if shiftr_pass == "shiftr-secret":
            print("HUOM!  Muista korvata 'shiftr-secret' omalla shiftr secretilasi")
        client = mqtt.Client(client_id=shiftr_name)
        client.on_connect = handle_connect
        client.on_message = handle_message
        client.username_pw_set(shiftr_key, password=shiftr_pass)
        client.connect("broker.shiftr.io", 1883, 60)
        client.loop_start()
        DATA["client"] = client
        while not DATA["initialized"]:
            time.sleep(0.1)
        # Shiftr connected, subscribe
        for topic in DATA["subs"]:
            DATA["client"].subscribe(topic)

    aiy_main()

def call_subscriptions(topic, message):
    """
    Kutsuu iot.subscribe-funktiolla alustettuja funktiota, riippuen topic:sta.
    """
    if DEBUG:
        print("New message: '" + message + "' (in topic: '" + topic + "')")
    if topic in DATA["subs"]:
        for func in DATA["subs"][topic]:
            func(message)


def publish(topic, message):
    """
    Julkaise uusi viesti (message) annetulla aiheella (topic),
    jota muut laitteet voivat kuunnella subscribe-funktiolla.
    """
    if DEBUG:
        print("Publish: '" + message + "' (topic: '" + topic + "')")
    DATA["client"].publish(topic, message)


def subscribe(topic):
    """
    Kuuntele viestejä annetusta aiheesta (topic), ja
    kutsu annettu funktio kun shiftr.io:sta saadaan uusi viesti.
    """
    
    def handler(func):
        """
        Lisää annettu funktio subscriptioneihin.
        """
        if DEBUG:
            print("Subscribe: " + topic)
        if topic not in DATA["subs"]:
            DATA["subs"][topic] = []
        DATA["subs"][topic].append(func)
    return handler

# AIY-stuff

def listen(phrase):
    """
    Kuuntele äänikomentoa (phrase) ja kutsu annettu funktio,
    kun käyttäjä sanoo äänikomennon.
    """
    phrase = phrase.lower()

    def handler(func):
        """
        Lisää annettu funktio äänikomentoihin.
        """
        if phrase not in DATA["phrases"]:
            DATA["phrases"][phrase] = []
        DATA["phrases"][phrase].append(func)
    return handler


def say(message):
    """
    Google Assistant sanoo tekstin message-parametrissä
    """
    aiy.audio.say(message)


# Code from https://github.com/google/aiyprojects-raspbian
# Modified for this library in various places.

def aiy_process_event(assistant, event):
    """
    Prosessoi AIY-eventtejä.
    """
    status_ui = aiy.voicehat.get_status_ui()
    #print(status_ui)
    if event.type == EventType.ON_START_FINISHED:
        status_ui.status('ready')
        if sys.stdout.isatty():
            print('Say "OK, Google" then speak, or press Ctrl+C to quit...')
    elif event.type == EventType.ON_CONVERSATION_TURN_STARTED:
        status_ui.status('listening')
    elif event.type == EventType.ON_RECOGNIZING_SPEECH_FINISHED and event.args:
        print('You said:', event.args['text'])
        url = TG_URL + '/sendMessage'
        telegram_message = 'You said: '+event.args['text']
        res = requests.post(url, params={'chat_id': GROUP_ID}, data ={'text': telegram_message})
        text = event.args['text'].lower()
        if text in DATA["phrases"]:
            assistant.stop_conversation()
            for func in DATA["phrases"][text]:
                func()
    elif event.type == EventType.ON_END_OF_UTTERANCE:
        status_ui.status('thinking')
    elif (event.type == EventType.ON_CONVERSATION_TURN_FINISHED
          or event.type == EventType.ON_CONVERSATION_TURN_TIMEOUT
          or event.type == EventType.ON_NO_RESPONSE):
        status_ui.status('ready')
    elif event.type == EventType.ON_ASSISTANT_ERROR and event.args and event.args['is_fatal']:
        sys.exit(1)


def aiy_main():
    """
    Yhdistää Google Assistant API:n.
    """
    credentials = aiy.assistant.auth_helpers.get_assistant_credentials()
    with Assistant(credentials) as assistant:
        for event in assistant.start():
            aiy_process_event(assistant, event)
