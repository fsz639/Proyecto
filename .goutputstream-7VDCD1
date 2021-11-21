DROP DATABASE IF EXISTS M8_bd;
CREATE DATABASE M8_bd;

USE M8_bd;

CREATE TABLE jugadores (
    Id INT NOT NULL PRIMARY KEY,
    nombre VARCHAR(60),
    contrasenya VARCHAR(20)
)ENGINE=InnoDB;

CREATE TABLE partidas (
    Id INT NOT NULL PRIMARY KEY,
    Fecha VARCHAR(20),
    Ganador VARCHAR(20)
)ENGINE=InnoDB;

CREATE TABLE relacion (
    Idpartida INT NOT NULL,
    Idjugador1 INT NOT NULL,
    Idjugador2 INT NOT NULL,
    Puntos1 INT,
    Puntos2 INT,
    FOREIGN KEY (Idpartida) REFERENCES partidas(Id),
    FOREIGN KEY (Idjugador1)REFERENCES jugadores(Id),
    FOREIGN KEY (Idjugador2)REFERENCES jugadores(Id)
    ON DELETE CASCADE
    ON UPDATE CASCADE
)ENGINE=InnoDB;
INSERT INTO jugadores VALUES (1,'David','123abc');
INSERT INTO jugadores VALUES (2,'Alvaro','456abc');
INSERT INTO jugadores VALUES (3,'Ferran','789abc');
INSERT INTO partidas VALUES (1,'10/10/2021','David');
INSERT INTO partidas VALUES (2,'11/10/2021','Alvaro');
INSERT INTO relacion VALUES (1,1,2,20,10);
INSERT INTO relacion VALUES (2,2,3,40,10);

