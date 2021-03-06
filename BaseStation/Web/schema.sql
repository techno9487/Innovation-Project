CREATE TABLE Device (
    Id int NOT NULL primary key AUTO_INCREMENT,
    DeviceName varchar(255),
    DeviceKey varchar(255),
    DeviceIV varchar(255)
);

CREATE TABLE Log (
    Id int NOT NULL primary key AUTO_INCREMENT,
    LogData TEXT,
    LogStamp DATETIME,
    DeviceId int,
    FOREIGN KEY (DeviceId) REFERENCES Devices(Id)
);

CREATE TABLE Session (
    Id int not null primary key AUTO_INCREMENT,
    Uid VARCHAR(255) NOT NULL,
    Data BLOB
)

CREATE TABLE User (
    Id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    FirstName VARCHAR(255),
    LastName VARCHAR(255),
    Password VARCHAR(255),
    Email VARCHAR(255),
    Permissions TEXT,
    UserMetadata BLOB
)