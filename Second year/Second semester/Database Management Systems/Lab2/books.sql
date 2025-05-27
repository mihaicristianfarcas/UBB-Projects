use DBMS
go

create table Book (
	id int primary key identity(1, 1),
	title varchar(50),
	author varchar(50)
)

create table Review (
	id int primary key identity(1, 1),
	bid int foreign key references Book(id),
	content varchar(500)
)

insert into Book values ('Divine Comedy', 'Dante'), ('The Idiot', 'Dostoevsky'), ('Metamorphosis', 'Franz Kafka'), ('Behave', 'Robert Sapolsky'), ('Onix Storm', 'Rebecca Yarros')
insert into Review values (1, 'greap'), (2, 'good soup'), (3, 'made me weak'), (4, 'im amazing'), (5, 'dragons'), (1, 'this guy evil'), (2, 'the idiot + me = love'), (2, 'Aglaia > Nastasia'), (3, 'apples are bad for you!')

select * from Book
select * from Review