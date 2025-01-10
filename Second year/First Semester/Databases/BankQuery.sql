use Bank
go

DROP TABLE IF EXISTS Transactions
DROP TABLE IF EXISTS Cards
DROP TABLE IF EXISTS BankAccounts
DROP TABLE if EXISTS Customers
DROP TABLE IF EXISTS ATM

create table Customer (
 id int primary key identity(1,1),
 name varchar(50),
 date_of_birth date
)

create table BankAccount (
 id int primary key identity(1,1),
 IBAN char(50) not null unique,
 balance int,
 holder_id int,
 foreign key (holder_id) references Customer(id),
)

create table Card (
 id int primary key identity(1,1),
 number int,
 cvv int,
 bank_account_id int,
 foreign key (bank_account_id) references BankAccount(id)
)

create table ATM (
 id int primary key identity(1,1),
 address varchar(100)
)

create table Transactions (
 id int primary key identity(1,1),
 atm_id int,
 card_id int,
 amount int,
 date_time datetime,
 foreign key (atm_id) references ATM(id),
 foreign key (card_id) references Card(id)
)
GO


create or alter procedure DeleteTransactions(@CardID int)
as
begin
	delete
	from Transactions
	where card_id = @CardID
end
go


create or alter view ShowCardsAllATMs
as
 select number
 from Card
 where id = (
	select dist.card_id
	from (
		select distinct card_id, atm_id
		from Transactions
	) as dist
	group by dist.card_id
	having count(*) = (select count(*) from ATM)
 )
go

create or alter function HeavyCards()
returns table
as
 return
  select c.number, c.cvv
  from Card c
  where 2000 < (
   select sum(amount)
   from Transactions
   where card_id = c.id
  )
go

