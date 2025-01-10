use Books1
go

-- 1.
create table MetroType (
	id int primary key identity(1,1),
	description char(50)
)

create table Metro (
	id int primary key identity(1,1),
	prod_year int,
	max_speed int,
	type int foreign key references MetroType(id)
)

create table Driver (
	id int primary key identity(1,1),
	name char(50),
	birth_year int,
	email nvarchar(100),
	phone_nr varchar(20)
)

create table Station (
	id int primary key identity(1,1),
	name char(50) not null unique,
)

create table Route (
	id int primary key identity(1,1),
	metro_id int foreign key references Metro(id),
)

create table Stops (
	route_id int foreign key references Route(id),
	station_id int foreign key references Station(id),
	arrival_time datetime,
	departure_time datetime,
	constraint pk_Stops primary key (route_id, station_id)
)
go

-- 2.
create or alter procedure AddStation(@RouteID int, @StationID int, @arr_time time, @dep_time time)
AS
	DECLARE @nr int;
	SET @nr = 0;
	SELECT @nr = COUNT(*) FROM Stops WHERE route_id=@RouteID and station_id=@StationID
	IF(@nr<>0) 
	BEGIN
		UPDATE Stops
		SET arrival_time=@arr_time, departure_time=@dep_time
		WHERE route_id=@RouteID and station_id=@StationID
	END
	ELSE BEGIN
	INSERT INTO Stops VALUES (@RouteID, @StationID, @arr_time, @dep_time)
END
go

-- 3.
create or alter view AllStarStations
as
	select top 2 s.name
	from Station s
	inner join Stops st on st.station_id = s.id
	group by s.name, st.station_id
	order by st.station_id desc
go

-- 4.
create or alter function NRoadsLeadToStation(@NrRoutes int)
returns table
as
	return
		select s.id, s.name
		from Station s
		inner join Stops st on st.station_id = s.id
		group by s.id, s.name
		having count(s.name) > @NrRoutes
		
go

-- inserts
INSERT INTO MetroType VALUES('description 1'), ('description 2'), ('description 3'), ('description 4'), ('description 5')
INSERT INTO Metro values (2003, 100, 1), (2004, 150, 2), (2012, 200, 3), (2019, 200, 3), (2024, 350, 4)
INSERT INTO Station values ('Cluj-Napoca'), ('Brasov'), ('Bucuresti'), ('City1'), ('City2')
Insert into Route values (1), (2), (1), (3), (4)
INSERT Stops VALUES(1,1,'12:00:00', '18:00:00'), (1,2,'15:30:00', '22:42:00'),
(2,2,'08:05:00', '21:48:00'), (2,3,'08:00:00', '12:00:00'), (3,3,'12:00:00', '13:00:00')
insert into Driver values('Name1', 1990, 'a@b.c', '123'), ('Name2', 1991, 'a@b.d', '124'), ('Name3', 1992, 'a@b.e', '125'), ('Name4', 1994, 'a@b.f', '126'), ('Name5', 1998, 'a@b.g', '128')

select * from MetroType
select * from Metro
select * from Station
select * from Route
select * from Stops

-- proc. check
-- add station to route
select * from Stops
exec AddStation 1,3,'12:00:00', '13:00:00'
select * from Stops

-- view check
select *
from AllStarStations

-- func. check
SELECT * FROM NRoadsLeadToStation(1)
SELECT * FROM NRoadsLeadToStation(2)
SELECT * FROM NRoadsLeadToStation(3)