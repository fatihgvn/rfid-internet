#!/usr/bin/env python

"""main.py: Kartlı geçiş sistemleri, kontrol ve denetim yazılımı"""

__author__ = "GlobalMedia"
__license__ = "Apache 2"
__version__ = "0.1"
__email__ = "info@globalmedia.com.tr"
__status__ = "Development"

import sqlite3, sys, re, socket


cardIdregex = re.compile("^[A-F0-9]+$")
conn = sqlite3.connect('system.db')
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('', 48578)

print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

sock.listen(1)

def runSql(sql):
    c = conn.cursor()
    c.execute(sql)
    conn.commit()

def addLogSql(log):
    c = conn.cursor()
    c.execute("INSERT INTO logs (logText) VALUES (?);", (log,))
    conn.commit()

def getCards(id):
    cur = conn.cursor()
    cur.execute("SELECT * FROM cards WHERE cardId=?", (id,))
 
    rows = cur.fetchone()
    conn.commit()

    return rows

# runSql('''
# INSERT INTO cards (first_name, last_name, cardId, state) 
# VALUES ("Furkan","Keleşoğlu", "22AF71CD", 1);
# ''')
try:
    while True:
        # Wait for a connection
        print('waiting for a connection')
        connection, client_address = sock.accept()
        try:
            print('connection from', client_address)

            # Receive the data in small chunks and retransmit it
            while True:
                data = connection.recv(16)
                if data:
                    userState = 0
                    cardId = data.decode("utf-8").rstrip()
                    print('received {!r}'.format(cardId))
                    if(cardIdregex.match(cardId)):
                        user = getCards(cardId)
                        if(user):
                            userState = user[4]
                            proc = "geçiş izni verilmedi" if(user[4] == 0) else "geçiş izni verildi"
                            addLogSql("[{0}] {1} {2} isimli kart sahibine {3}".format(cardId,user[1],user[2],proc))
                        else:
                            userState = 0
                            addLogSql("[{0}] kar id'sine sahip kullanıcı bulunamadı".format(cardId))
                    else:
                        addLogSql("[{0}] kart id'si tanımlamanın dışarısında kaldığı için kabul edilmedi".format(cardId))
                        
                    connection.sendall("{0}".format(userState).encode())
                else:
                    break
        except Exception as ex:
            print(ex)

finally:
    # Clean up the connection
    connection.close()

    conn.close()

