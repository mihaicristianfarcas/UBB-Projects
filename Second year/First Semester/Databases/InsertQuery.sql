USE Lab1
GO 

insert into Author (AID, Name, Period_of_Activity, Nationality) values (100, 'Fyodor Dostoevsky', '1844–1880', 'Russian')
insert into Author (AID, Name, Period_of_Activity, Nationality) values (101, 'Franz Kafka', '1912-1924', 'Austrian-Czech')
insert into Author (AID, Name, Period_of_Activity, Nationality) values (102, 'Hermann Hesse', '1904-1962', 'German')
insert into Author (AID, Name, Period_of_Activity, Nationality) values (103, 'Umberto Eco', '1961-2016', 'Italian')

select *
from Author

insert into Publisher (PID, Name, Country) values (100, 'Polirom','Romania')
insert into Publisher (PID, Name, Country) values (101, 'Rao','Romania')
insert into Publisher (PID, Name, Country) values (102, 'Wordsworth Classics','England')
insert into Publisher (PID, Name, Country) values (103, 'Penguin','England')

select *
from Publisher

alter table Book
add Name varchar(20)

alter table Book
alter column Name varchar(50)

select * from Book

insert into Book (ISBN, AID, Category, Feel, Price, PID, Name) 
values 
(9789734678266, 101, 'Absurdist Fiction', 'Paperback', 9.99, 100, 'Metamorphosis'),
(9781631498190, 100, 'Classic Fiction', 'Hardcover', 49.99, 102, 'The Brothers Karamazov'),
(9786066094276, 102, 'Historical Fiction', 'Hardcover', 13.99, 103, 'Siddhartha'),
(9786066094238, 102, 'Mystical Fiction', 'Hardcover', 39.99, 101, 'The Glass Bead Game'),
(9789734628896, 103, 'Historical Fiction', 'Paperback', 29.99, 100, 'The Name of the Rose')


insert into Client (CID, Name, Phone_Number) 
values
(100, 'Mihai-Cristian Farcas', '0712345678'),
(101, 'George Enescu', '0712345679'),
(102, 'Nicolae Dumitrescu', '0712345680'),
(103, 'Lucian Maier', '0712345681'),
(104, 'Diana Maria', '0712345682')

select * from Client

insert into Sale(SaleID, CID, ISBN, Sale_Date)
values
(100, 100, 9789734678266, '2004-04-05'),
(101, 100, 9781631498190, '2012-08-08'),
(102, 104, 9786066094276, '2024-07-18'),
(103, 104, 9781631498190, '2008-11-06'),
(104, 101, 9789734628896, '2000-12-23')

select * from Sale

insert into Review(ReviewID, CID, ISBN, Review_Text, Rating, Review_Date)
values
(100, 100, 9789734628896, 'Marvelous!', 4, '2004-05-05'),
(101, 100, 9781631498190, 'Best book I have ever read.', 5, '2013-08-08'),
(102, 104, 9786066094276, 'A journey to your inner self.', 4, '2024-08-05'),
(103, 104, 9781631498190, 'Too many characters for my taste...', 3, '2009-01-01'),
(104, 101, 9789734628896, 'Symbolism at its finest!', 5, '2001-02-04')

select * from Review

alter table Book_Store
alter column Store_Size varchar(20)

Delete from Book_Store

insert into Book_Store (BSID, Address, Store_Size, Number_of_Employees) 
values 
(100, 'Main St 123, Bucharest', 'Large', 10),
(101, 'Iulius Mall 456, Cluj-Napoca', 'Large', 11),
(102, 'Central 789, Timisoara', 'Medium', 5),
(103, 'Carolina Mall 101, Alba Iulia', 'Small', 3),
(104, 'Dealul Florilor 112, Baia Mare', 'Medium', 6)


select * from Book_Store

insert into Employee (CNP, DepartmentID, Name, Salary, Contracted_Period) 
values 
(100, 100, 'Ana Popescu', 3500, '2023-04 -> 2024-04'),
(101, 100, 'Ioan Ionescu', 2800, '2023-05 -> 2024-05'),
(102, 102, 'Elena Marinescu', 4000, '2023-06 -> 2023-09'),
(103, 101, 'Mihai Vasilescu', 3200, '2022-04 -> 2024-08'),
(104, 102, 'Maria Dumitru', 3000, '2021-05 -> 2023-09'),
(105, 103, 'Ion Grigorescu', 3100, '2019-03 -> 2022-07'),
(106, 101, 'Andrei Gheorghe', 3800, '2020-11 -> 2023-05'),
(107, 103, 'Sorina Pop', 2900, '2022-01 -> 2024-06'),
(108, 102, 'Adrian Vasile', 3200, '2021-07 -> 2024-01'),
(109, 101, 'Eliza Matei', 3500, '2020-04 -> 2023-10'),
(110, 103, 'Cristina Ionescu', 3700, '2018-09 -> 2021-12');

Delete from Employee
select * from Employee

delete from Department

insert into Department (DepartmentID, BSID, DepartmentName) 
values 
(100, 100, 'Sales'),
(101, 100, 'Human Resources'),
(102, 101, 'Marketing'),
(103, 102, 'Management');

select * from Department

insert into Subscription (SID, CID, BSID, Nr_of_Fidelity_Points, Expiration_Date) 
values 
(100, 100, 100, 500, '2025-05-05'),
(101, 101, 101, 300, '2025-06-06'),
(102, 102, 101, 450, '2025-07-07'),
(103, 103, 103, 600, '2025-08-08'),
(104, 104, 102, 900, '2025-09-09');

delete from Subscription
select * from Subscription