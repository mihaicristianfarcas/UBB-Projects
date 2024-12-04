CREATE TABLE Author (
    AID INT PRIMARY KEY,
    Name VARCHAR(100),
    Period_of_Activity VARCHAR(50),
    Nationality VARCHAR(50)
);

CREATE TABLE Book (
    ISBN VARCHAR(20) PRIMARY KEY,
    AID INT,
    Category VARCHAR(100),
    Feel VARCHAR(30),
    Price DECIMAL(10, 2),
    FOREIGN KEY (AID) REFERENCES Author(AID)
);

CREATE TABLE Book_Store (
    BSID INT PRIMARY KEY,
    Address VARCHAR(255),
    Store_Size INT,
    Number_of_Employees INT
);

CREATE TABLE Employee (
    CNP VARCHAR(20) PRIMARY KEY,
    BSID INT,
    Name VARCHAR(100),
    Salary DECIMAL(10, 2),
    Contracted_Period VARCHAR(50),
    FOREIGN KEY (BSID) REFERENCES Book_Store(BSID)
);

CREATE TABLE Client (
    CID VARCHAR(20) PRIMARY KEY,
    Name VARCHAR(100),
    Phone_Number VARCHAR(10)
);

CREATE TABLE Subscription (
    SID INT PRIMARY KEY,
    CID VARCHAR(20),
    BSID INT,
    Nr_of_Fidelity_Points INT,
    Expiration_Date DATE,
    FOREIGN KEY (CID) REFERENCES Client(CID),
    FOREIGN KEY (BSID) REFERENCES Book_Store(BSID)
);

ALTER TABLE Client
ALTER COLUMN CID VARCHAR(13);

ALTER TABLE Subscription
DROP FK_Subscription_CID;


CREATE TABLE Publisher (
    PID INT PRIMARY KEY,
    Name VARCHAR(100),
    Country VARCHAR(50)
);

ALTER TABLE Book
ADD PID INT;

ALTER TABLE Book
ADD CONSTRAINT FK_Book_Publisher
FOREIGN KEY (PID) REFERENCES Publisher(PID);



CREATE TABLE Book_Sale (
    SaleID INT PRIMARY KEY,
    BSID INT,
    ISBN VARCHAR(20),
    Sale_Date DATE,
    Quantity INT,
    FOREIGN KEY (BSID) REFERENCES Book_Store(BSID),
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
);

CREATE TABLE Review (
    ReviewID INT PRIMARY KEY,
    CID VARCHAR(20),
    ISBN VARCHAR(20),
    Review_Text TEXT,
    Rating INT CHECK (Rating BETWEEN 1 AND 5),
    Review_Date DATE,
    FOREIGN KEY (CID) REFERENCES Client(CID),
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
);

CREATE TABLE Loan (
    LoanID INT PRIMARY KEY,
    CID VARCHAR(20),
    ISBN VARCHAR(20),
    Loan_Date DATE,
    Quantity INT,
    FOREIGN KEY (CID) REFERENCES Client(CID),
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
);

CREATE TABLE Department (
    DepartmentID INT PRIMARY KEY,
    BSID INT,
    DepartmentName VARCHAR(100),
    FOREIGN KEY (BSID) REFERENCES Book_Store(BSID)
);

ALTER TABLE Employee
ADD DepartmentID INT;

ALTER TABLE Employee
ADD CONSTRAINT FK_Employee_Department
FOREIGN KEY (DepartmentID) REFERENCES Department(DepartmentID);

DROP TABLE Loan

CREATE TABLE Sale (
    SaleID INT PRIMARY KEY,
    CID VARCHAR(20),
    ISBN VARCHAR(20),
    Sale_Date DATE,
    FOREIGN KEY (CID) REFERENCES Client(CID),
    FOREIGN KEY (ISBN) REFERENCES Book(ISBN)
);



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

