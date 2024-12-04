UPDATE Author 
SET Period_of_Activity = '1900-1930' 
WHERE Nationality = 'Austrian-Czech' AND Name LIKE 'Franz%';

UPDATE Author 
SET Nationality = 'Italian'
WHERE AID IN (100, 102);


UPDATE Publisher 
SET Country = 'UK' 
WHERE Name = 'Penguin' AND Country = 'England';

UPDATE Publisher 
SET Name = 'Penguin Classics' 
WHERE PID = 103 AND Country IS NOT NULL;


UPDATE Book 
SET Price = Price - 5 
WHERE Price > 20 AND Category = 'Historical Fiction';

UPDATE Book 
SET Feel = 'Hardcover' 
WHERE ISBN IN (9786066094276, 9786066094238);


UPDATE Client 
SET Phone_Number = '0723456789'
WHERE Name = 'George Enescu' OR Name = 'Nicolae Dumitrescu';

UPDATE Client 
SET Name = 'Lucian Mihai'
WHERE CID = 103 AND Phone_Number LIKE '0712345681';


UPDATE Sale 
SET Sale_Date = '2022-01-01'
WHERE SaleID = 100 OR SaleID = 101;

UPDATE Sale 
SET ISBN = 9786066094238
WHERE ISBN = 9781631498190 AND CID IN (100, 104);


UPDATE Review 
SET Rating = Rating - 1 
WHERE ReviewID = 100 OR ReviewID = 101;

UPDATE Review 
SET Review_Text = 'Amazing!' 
WHERE Rating >= 4 AND ISBN = 9786066094276;


UPDATE Book_Store 
SET Number_of_Employees = 12
WHERE Store_Size = 'Large' AND BSID = 101;

UPDATE Book_Store 
SET Address = 'New Address, Bucharest'
WHERE BSID = 100 AND Store_Size IS NOT NULL;


UPDATE Employee 
SET Salary = Salary + 200
WHERE DepartmentID = 100 AND Contracted_Period IS NOT NULL;

UPDATE Employee 
SET Name = 'Ana Maria Popescu' 
WHERE CNP = 100 OR Name LIKE 'Ana%';


UPDATE Department 
SET DepartmentName = 'HR' 
WHERE DepartmentName = 'Human Resources' AND BSID = 100;

UPDATE Department 
SET DepartmentName = 'Sales & Marketing' 
WHERE DepartmentID = 100 AND DepartmentName IS NOT NULL;


UPDATE Subscription 
SET Nr_of_Fidelity_Points = 1000
WHERE Nr_of_Fidelity_Points BETWEEN 500 AND 900;

UPDATE Subscription 
SET Expiration_Date = '2026-01-01' 
WHERE SID = 100 OR BSID = 102;

SELECT * FROM Subscription

DELETE FROM Author 
WHERE AID = 102;

DELETE FROM Author 
WHERE Nationality = 'Austrian-Czech';


DELETE FROM Publisher 
WHERE PID = 101;

DELETE FROM Publisher 
WHERE Name = 'Penguin Classics';


DELETE FROM Book 
WHERE ISBN = 9786066094238;

DELETE FROM Book 
WHERE Price < 20 AND Category = 'Mystical Fiction';


DELETE FROM Client 
WHERE CID = 103;

DELETE FROM Client 
WHERE Phone_Number LIKE '0712345681';


DELETE FROM Sale 
WHERE SaleID = 103;

DELETE FROM Sale 
WHERE Sale_Date BETWEEN '2000-01-01' AND '2010-01-01';


DELETE FROM Review 
WHERE ReviewID = 100;

DELETE FROM Review 
WHERE Rating < 4 AND ISBN = 9786066094276;


DELETE FROM Book_Store 
WHERE BSID = 104;

DELETE FROM Book_Store 
WHERE Store_Size = 'Small';


DELETE FROM Employee 
WHERE CNP = 108;

DELETE FROM Employee 
WHERE Salary < 3000;


DELETE FROM Department 
WHERE DepartmentID = 102;

DELETE FROM Department 
WHERE BSID = 100 AND DepartmentName = 'HR';


DELETE FROM Subscription 
WHERE SID = 102;

DELETE FROM Subscription 
WHERE Nr_of_Fidelity_Points < 500;


-- a.
SELECT DISTINCT TOP 5 Name FROM Author WHERE Nationality = 'Russian'
UNION
SELECT DISTINCT Name FROM Publisher WHERE Country = 'Romania'
ORDER BY Name ASC;

SELECT DISTINCT TOP 5 Name FROM Book WHERE Price > 30 OR Category = 'Mystical Fiction'
UNION ALL
SELECT DISTINCT Address FROM Book_Store WHERE Store_Size = 'Large'
ORDER BY Name DESC;

-- b.
SELECT Name FROM Client WHERE CID IN (100, 101, 102)
INTERSECT
SELECT Name FROM Employee WHERE DepartmentID = 100;

SELECT Name FROM Author WHERE AID IN (100, 101, 102)
INTERSECT
SELECT Name FROM Publisher WHERE PID IN (100, 102);

-- c.
SELECT Name FROM Book WHERE Price > 20
EXCEPT
SELECT Name FROM Book WHERE Category = 'Historical Fiction';

SELECT Name FROM Employee WHERE DepartmentID = 100
EXCEPT
SELECT Name FROM Employee WHERE CNP NOT IN (100, 101);

SELECT * FROM Employee

-- d.
SELECT Client.Name, Sale.SaleID, Book.Name 
FROM Client
INNER JOIN Sale ON Client.CID = Sale.CID
INNER JOIN Book ON Sale.ISBN = Book.ISBN;

SELECT Employee.Name, Department.DepartmentName 
FROM Employee
LEFT JOIN Department ON Employee.DepartmentID = Department.DepartmentID;

SELECT * FROM Employee
SELECT * FROM Department

SELECT Book_Store.Address, Subscription.Nr_of_Fidelity_Points 
FROM Subscription
RIGHT JOIN Book_Store ON Subscription.BSID = Book_Store.BSID;

SELECT Author.Name, Book.Name, Publisher.Name 
FROM Author
FULL JOIN Book ON Author.AID = Book.AID
FULL JOIN Publisher ON Book.PID = Publisher.PID;

-- e.
SELECT Name 
FROM Book 
WHERE ISBN IN (SELECT ISBN 
               FROM Sale 
               WHERE Sale_Date > '2010-01-01');

SELECT Name 
FROM Client 
WHERE CID IN (SELECT CID 
              FROM Subscription 
              WHERE BSID IN (SELECT BSID 
                             FROM Book_Store 
                             WHERE Store_Size = 'Large'));

SELECT * FROM Client
SELECT * FROM Subscription
SELECT * FROM Book_Store

-- f.
SELECT Name 
FROM Author a 
WHERE EXISTS (SELECT * 
              FROM Book b 
              WHERE b.AID = a.AID AND b.Price > 30);

SELECT Name 
FROM Client c 
WHERE EXISTS (SELECT * 
              FROM Sale s 
              WHERE s.CID = c.CID AND s.Sale_Date BETWEEN '2000-01-01' AND '2020-12-31');

-- g.
SELECT Name, Avg_Price 
FROM (SELECT AID, AVG(Price) AS Avg_Price 
      FROM Book 
      GROUP BY AID) AS AvgBookPrices
INNER JOIN Author ON Author.AID = AvgBookPrices.AID;

SELECT Store_Size, Total_Employees 
FROM (SELECT BSID, SUM(Number_of_Employees) AS Total_Employees 
      FROM Book_Store 
      GROUP BY BSID) AS StoreEmployeeCount
INNER JOIN Book_Store ON Book_Store.BSID = StoreEmployeeCount.BSID;

-- h.
SELECT AID, COUNT(ISBN) AS Total_Books 
FROM Book 
GROUP BY AID;

SELECT AID, AVG(Price) AS Avg_Price 
FROM Book 
GROUP BY AID 
HAVING AVG(Price) > (SELECT AVG(Price) 
                     FROM Book 
                     WHERE Category = 'Historical Fiction');

SELECT PID, SUM(Price) AS Total_Sales 
FROM Book 
GROUP BY PID 
HAVING SUM(Price) > (SELECT SUM(Price) 
                     FROM Book 
                     WHERE Category = 'Absurdist Fiction');

SELECT CID, COUNT(SaleID) AS Total_Purchases 
FROM Sale 
GROUP BY CID 
HAVING COUNT(SaleID) > 2;

-- i.
SELECT Name 
FROM Book 
WHERE Price > ANY (SELECT Price 
                   FROM Book 
                   WHERE Category = 'Historical Fiction');

SELECT Name 
FROM Book 
WHERE Price > (SELECT MIN(Price) 
               FROM Book 
               WHERE Category = 'Historical Fiction');

SELECT Name 
FROM Book 
WHERE Price < ALL (SELECT Price 
                   FROM Book 
                   WHERE Category = 'Absurdist Fiction');

SELECT Name 
FROM Book 
WHERE Price < (SELECT MAX(Price) 
               FROM Book 
               WHERE Category = 'Absurdist Fiction');

SELECT Name 
FROM Author 
WHERE AID IN (SELECT AID 
              FROM Book 
              WHERE Price > 40);

SELECT Name 
FROM Client 
WHERE CID NOT IN (SELECT CID 
                  FROM Sale 
                  WHERE Sale_Date BETWEEN '2020-01-01' AND '2022-12-31');

-- Update statements with arithmetic expressions
UPDATE Book 
SET Price = Price - 5 
WHERE Price > 20 AND Category = 'Historical Fiction';

UPDATE Employee 
SET Salary = Salary + 200 
WHERE DepartmentID = 100 AND Contracted_Period IS NOT NULL;

UPDATE Subscription 
SET Nr_of_Fidelity_Points = Nr_of_Fidelity_Points + 100 
WHERE Nr_of_Fidelity_Points BETWEEN 500 AND 900;

-- Select statements with arithmetic expressions
SELECT ISBN, Name, Price - 5 AS Discounted_Price 
FROM Book 
WHERE Price > 20 AND Category = 'Historical Fiction';

SELECT CNP, Name, Salary + 200 AS Updated_Salary 
FROM Employee 
WHERE DepartmentID = 100 AND Contracted_Period IS NOT NULL;

SELECT SID, Nr_of_Fidelity_Points, Nr_of_Fidelity_Points + 100 AS New_Fidelity_Points 
FROM Subscription 
WHERE Nr_of_Fidelity_Points BETWEEN 500 AND 900;

-- Conditions using AND, OR, NOT, and parentheses
UPDATE Author 
SET Period_of_Activity = '1900-1930' 
WHERE Nationality = 'Austrian-Czech' AND (Name LIKE 'Franz%' OR Name LIKE 'Karl%');

DELETE FROM Book 
WHERE Price < 20 AND (Category = 'Mystical Fiction' OR Category = 'Historical Fiction') AND NOT Category = 'Absurdist Fiction';

SELECT Name, Phone_Number 
FROM Client 
WHERE (Name = 'George Enescu' OR Name = 'Nicolae Dumitrescu') AND NOT Phone_Number LIKE '074%';

-- DISTINCT, ORDER BY, and TOP usage
SELECT DISTINCT TOP 5 Name 
FROM Author 
WHERE Nationality = 'Russian' 
ORDER BY Name ASC;

SELECT * FROM Author

SELECT DISTINCT TOP 5 Name 
FROM Publisher 
WHERE Country = 'Romania' 
ORDER BY Name DESC;

SELECT DISTINCT TOP 10 Price 
FROM Book 
WHERE Category = 'Mystical Fiction' 
ORDER BY Price DESC;
