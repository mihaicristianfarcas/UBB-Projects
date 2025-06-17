use hotelroomreservation
go

create table User (
    id int auto_increment primary key,
    username varchar(50) unique not null,
    password int not null
);

create table HotelRoom (
    id int auto_increment primary key,
    roomNumber varchar(50) not null,
    capacity int not null,
    basePrice int not null
);

create table Reservation (
    id int auto_increment primary key,
    userId int,
    foreign key (userId) references User(id),
    roomId int,
    foreign key (roomId) references HotelRoom(id),
    checkInDate date not null,
    checkOutDate date not null,
    numberOfGuests int not null,
    totalPrice int not null
);

show tables;

describe Reservation;

-- Insert sample hotel rooms
INSERT INTO HotelRoom (roomNumber, capacity, basePrice) VALUES
('101', 2, 100),
('102', 2, 100),
('103', 4, 150),
('201', 2, 120),
('202', 2, 120),
('203', 4, 180),
('301', 1, 80),
('302', 1, 80),
('303', 2, 110),
('304', 3, 140);

-- Insert sample users (password is plain text for demo)
INSERT INTO User (username, password) VALUES
('john_doe', 123456),
('jane_smith', 123456),
('mike_wilson', 123456);

-- View sample data
SELECT * FROM HotelRoom;
SELECT * FROM User;