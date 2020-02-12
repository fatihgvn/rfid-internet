-- SQLite

DROP TABLE IF EXISTS logs;
DROP TABLE IF EXISTS cards;

CREATE TABLE IF NOT EXISTS cards (
    id INTEGER PRIMARY KEY,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    cardId TEXT NOT NULL UNIQUE,
    state BOOLEAN NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS logs (
    id INTEGER PRIMARY KEY,
    logText TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

DELETE FROM cards;
DELETE FROM logs;

INSERT INTO cards (first_name, last_name, cardId, state) 
VALUES ("Fatih","Güven", "DBFAC022", 0);
INSERT INTO cards (first_name, last_name, cardId, state) 
VALUES ("Furkan","Keleşoğlu", "2A643A15", 1);

INSERT INTO logs (logText) 
VALUES ("Builded system");