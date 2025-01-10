use Zoo
go

-- Tables
create table Zoo (
 id int primary key identity(1,1),
 name char(50),
 administrator char(50)
);

create table Animal (
 id int primary key identity(1,1),
 name char(50),
 birthday date
);

create table AnimalsAtZoo (
 animal_id int foreign key references Animal(id),
 zoo_id int foreign key references Zoo(id),
 primary key (zoo_id, animal_id)
);

create table Food (
 id int primary key identity(1,1),
 name char(50)
);

create table Yummy (
 animal_id int foreign key references Animal(id),
 food_id int foreign key references Food(id),
 daily_quota int not null,
 constraint pk_Yummy primary key (animal_id, food_id) 
);

create table Visitor (
 id int primary key identity(1,1),
 name char(50),
 age int
);

create table Visit (
 id int primary key identity(1,1),
 zoo_id int,
 visitor_id int,
 date date,
 paid_price int,
 foreign key (zoo_id) references Zoo(id),
 foreign key (visitor_id) references Visitor(id)
);
go

-- Stored Procedure
create or alter procedure DeleteFoodQuotas(@AnimalID int)
as
begin
 delete
 from Yummy
 where animal_id = @AnimalID;
end;
go

-- View: Underrated Zoos
create or alter view UnderratedZoos
as
 select top 5 zoo_id
 from Visit
 group by zoo_id
 order by count(zoo_id) asc;
go

-- Function: WannaSeeThemAll
create or alter function WannaSeeThemAll(@HowManyAnimals int)
returns table
as
 return
  select v.id
  from Visitor v
  inner join Visit vis on v.id = vis.visitor_id
  inner join AnimalsAtZoo az on vis.zoo_id = az.zoo_id
  group by v.id
  having count(distinct az.animal_id) >= @HowManyAnimals;
go
