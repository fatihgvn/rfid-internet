#!/usr/bin/env python

"""main.py: Kartlı geçiş sistemleri, kontrol ve denetim yazılımı"""

__author__ = "GlobalMedia"
__license__ = "Apache 2"
__version__ = "0.1"
__email__ = "info@globalmedia.com.tr"
__status__ = "Development"

import sqlite3, sys, re, socket

import requests

# import mysql.connector

# mydb = mysql.connector.connect(
#     host="localhost",
#     user="root",
#     password="indeho",
#     database="turnique"
# )


# # cihaz bilgilerini getir
# def getDevice(id):
#     mycursor = mydb.cursor(dictionary=True)
#     mycursor.execute("SELECT * FROM Devices WHERE deviceId='"+str(id)+"'")
#     myresult = mycursor.fetchone()

#     return myresult

# # Öğrenci bilgilerini getir
# def getStudent(cardId):
#     mycursor = mydb.cursor(dictionary=True)
#     mycursor.execute("SELECT * FROM Students WHERE cardId='"+str(cardId)+"'")
#     myresult = mycursor.fetchone()

#     return myresult

cardIdregex = re.compile("^[A-F0-9]+$")
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('', 48578)

print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

sock.listen(5)

try:
    while True:
        # Wait for a connection
        print('waiting for a connection')
        connection, client_address = sock.accept()
        try:
            print('connection from', client_address)

            # Receive the data in small chunks and retransmit it
            while True:
                data = connection.recv(18)
                if data:
                    print("http://192.168.1.199/WebHook/readCard/"+str(data.decode("utf-8")))
                    r = requests.get("http://192.168.1.199/WebHook/readCard/"+str(data.decode("utf-8")))
                    print(r.text)
                    userState = str(r.text)
                    connection.send("{0}".format(userState).encode())
                else:
                    break
        except Exception as ex:
            print(ex)

finally:
    # Clean up the connection
    connection.close()

    conn.close()

