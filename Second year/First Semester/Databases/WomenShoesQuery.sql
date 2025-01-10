use WomenShoes
go

-- Table Definitions
create table PresentationShop (
    id int primary key identity(1,1),
    name varchar(50),
    city varchar(50)
);

create table Woman (
    id int primary key identity(1,1),
    name varchar(50),
    amount_to_spend int
);

create table ShoeModel (
    id int primary key identity(1,1),
    name varchar(50),
    season varchar(50)
);

create table Shoe (
    id int primary key identity(1,1),
    price int not null,
    shoe_model_id int not null,
    foreign key (shoe_model_id) references ShoeModel(id)
);

create table Stock (
    shoe_id int not null,
    shop_id int not null,
    nr_available_shoes int not null,
    foreign key (shoe_id) references Shoe(id),
    foreign key (shop_id) references PresentationShop(id),
    primary key (shoe_id, shop_id)
);

create table Sale (
    id int primary key identity(1,1),
    woman_id int not null,
    shoe_id int not null,
    nr_pairs int not null,
    amount_spent int not null,
    foreign key (woman_id) references Woman(id),
    foreign key (shoe_id) references Shoe(id)
);
go

-- Stored Procedure (Add shoe to shop)
create or alter procedure AddShoetoShop(@ShoeID int, @PresentationShopID int, @NrShoes int)
as
begin
    -- Validate if shoe and shop exist
    if exists (select 1 from Shoe where id = @ShoeID) and
       exists (select 1 from PresentationShop where id = @PresentationShopID)
    begin
        insert into Stock(shoe_id, shop_id, nr_available_shoes)
        values (@ShoeID, @PresentationShopID, @NrShoes);
    end
    else
    begin
        throw 50000, 'Invalid ShoeID or PresentationShopID.', 1;
    end
end;
go

-- View (Women who bought at least 2 shoes from a given shoe model)
create or alter view Shopaholics
as
select w.name
from Woman w
join Sale s on w.id = s.woman_id
join Shoe sh on s.shoe_id = sh.id
group by w.name
having count(s.id) >= 2;
go

-- Function (Shoes available in at least T presentation shops)
create or alter function ShoesEverywhere(@NrShops int)
returns table
as
return (
    select s.id, s.shoe_model_id
    from Shoe s
    where @NrShops <= (
        select count(*)
        from Stock st
        where s.id = st.shoe_id
    )
);
go
