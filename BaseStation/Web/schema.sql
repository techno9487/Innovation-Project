CREATE TABLE 'Room' (
    id int NOT NULL primary key AUTO_INCREMENT,
    name VARCHAR(255)
);

CREATE TABLE 'Devices' (
    id int NOT NULL primary key AUTO_INCREMENT,
    name varchar(255),
    key BLOB,
    metadata BLOB,
    FOREIGN KEY (room_id) REFERENCES Room(id)
);

CREATE TABLE 'Logs' (
    id int NOT NULL primary key AUTO_INCREMENT,
    FOREIGN KEY (device_id) REFERENCES Devices(id),
    data TEXT,
    timestamp DATETIME
);