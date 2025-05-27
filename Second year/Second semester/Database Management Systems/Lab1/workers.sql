use DBMS
go

create table Worker (
	id int primary key identity(1, 1),
	name varchar(50),
	age int,
	position varchar(100)
)

insert into Worker values ('Mihai', 21, 'pavator')
insert into Worker values ('Dorel', 31, 'pavator')
insert into Worker values ('Marcel', 33, 'pavator')
insert into Worker values ('Mirel', 42, 'pavator')


create table Bonus (
	id int primary key identity(1, 1),
	wid int foreign key references Worker(id),
	sum int
)

insert into Bonus values (1, 100)
insert into Bonus values (1, 200)
insert into Bonus values (5, 400)
insert into Bonus values (3, 500)
insert into Bonus values (3, 1000)

select * from Worker
select * from Bonus