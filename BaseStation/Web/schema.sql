CREATE TABLE Room (
    Id int NOT NULL primary key AUTO_INCREMENT,
    RoomName VARCHAR(255)
);

CREATE TABLE Device (
    Id int NOT NULL primary key AUTO_INCREMENT,
    DeviceName varchar(255),
    DeviceKey BLOB,
    DeviceMeta BLOB,
    RoomId int,
    FOREIGN KEY (RoomId) REFERENCES Room(Id)
);

CREATE TABLE Log (
    Id int NOT NULL primary key AUTO_INCREMENT,
    LogData TEXT,
    LogStamp DATETIME,
    DeviceId int,
    FOREIGN KEY (DeviceId) REFERENCES Devices(Id)
);