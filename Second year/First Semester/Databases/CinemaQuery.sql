use Cinema
go

-- Tables
create table Company (
 id int primary key identity(1,1),
 name char(50)
);

create table StageDirector (
 id int primary key identity(1,1),
 name char(50),
 nr_awards int
);

create table Actor (
 id int primary key identity(1,1),
 name char(50),
 ranking int
);

create table Movie (
 id int primary key identity(1,1),
 name char(50),
 release_date date,
 company_id int foreign key references Company(id),
 stage_director_id int foreign key references StageDirector(id)
);

create table CinemaProduction (
 id int primary key identity(1,1),
 title char(50),
 movie_id int foreign key references Movie(id)
);

create table Scene (
 production_id int foreign key references CinemaProduction(id),
 actor_id int foreign key references Actor(id),
 entry_time datetime,
 primary key (production_id, actor_id)
);
go

-- Stored Procedure
create or alter procedure AddActorToProduction (
    @ActorID int,
    @EntryMoment datetime,
    @CinemaProductionID int
)
as
begin
    insert into Scene (production_id, actor_id, entry_time) 
    values (@CinemaProductionID, @ActorID, @EntryMoment);
end;
go

-- View
create or alter view MrOrMsWorldwide
as
 select a.name
 from Actor a
 inner join Scene s on a.id = s.actor_id
 group by a.id, a.name
 having count(distinct s.production_id) = (select count(*) from CinemaProduction);
go

-- Function
create or alter function AAAMovies (@Productions int)
returns table
as
 return
  select m.id, m.name, m.release_date
  from Movie m
  inner join CinemaProduction cp on cp.movie_id = m.id
  group by m.id, m.name, m.release_date
  having count(cp.id) >= @Productions and m.release_date >= '2018-01-01';
go
