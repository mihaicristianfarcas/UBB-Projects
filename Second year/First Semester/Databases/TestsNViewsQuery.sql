USE Lab
GO

CREATE OR ALTER PROCEDURE populateAuthorTable
    @NumberOfAuthors INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @i INT = 1;
    DECLARE @AID INT;
    DECLARE @Name NVARCHAR(100);
    DECLARE @Period_of_Activity NVARCHAR(50);
    DECLARE @Nationality NVARCHAR(50);

    DECLARE @NamePool TABLE (Name NVARCHAR(100));
    INSERT INTO @NamePool (Name)
    VALUES 
        ('Franz Kafka'), 
        ('Leo Tolstoy'), 
        ('Gabriel Garcia Marquez'), 
        ('Virginia Woolf'),
        ('George Orwell'),
        ('Fyodor Dostoevsky'),
        ('Jane Austen'),
        ('Mark Twain'),
        ('Homer'),
        ('Charles Dickens');

    DECLARE @PeriodPool TABLE (Period NVARCHAR(50));
    INSERT INTO @PeriodPool (Period)
    VALUES 
        ('1800-1850'),
        ('1851-1900'),
        ('1900-1950'),
        ('1951-2000'),
        ('2001-Present');

    DECLARE @NationalityPool TABLE (Nationality NVARCHAR(50));
    INSERT INTO @NationalityPool (Nationality)
    VALUES 
        ('Russian'),
        ('British'),
        ('American'),
        ('Austrian-Czech'),
        ('Colombian'),
        ('Greek'),
        ('French'),
        ('Italian'),
        ('Japanese'),
        ('Indian');

    WHILE @i <= @NumberOfAuthors
    BEGIN
        SET @AID = ABS(CHECKSUM(NEWID())) % 1000 + 1; -- Random ID (1 to 1000)

        SELECT TOP 1 @Name = Name FROM @NamePool ORDER BY NEWID();
        SELECT TOP 1 @Period_of_Activity = Period FROM @PeriodPool ORDER BY NEWID();
        SELECT TOP 1 @Nationality = Nationality FROM @NationalityPool ORDER BY NEWID();

        INSERT INTO Author (AID, Name, Period_of_Activity, Nationality)
        VALUES (@AID, @Name, @Period_of_Activity, @Nationality);

        SET @i = @i + 1;
    END
END;
GO


CREATE OR ALTER PROCEDURE populateBookTable
    @NumberOfBooks INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @i INT = 1;
    DECLARE @ISBN NVARCHAR(13);
	DECLARE @AID INT;
	DECLARE @Category NVARCHAR(30);
	DECLARE @Feel NVARCHAR(20);
    DECLARE @Price INT;
    DECLARE @PID INT;
    DECLARE @Name NVARCHAR(200);

    DECLARE @CategoryPool TABLE (Category NVARCHAR(30));
    INSERT INTO @CategoryPool (Category)
    VALUES 
        ('Fiction'),
        ('Science Fiction'),
        ('Non-Fiction'),
        ('Biography'),
        ('Fantasy'),
        ('Mystery'),
        ('Romance'),
        ('Historical'),
        ('Thriller'),
        ('Self-Help');

    DECLARE @FeelPool TABLE (Feel NVARCHAR(20));
    INSERT INTO @FeelPool (Feel)
    VALUES 
        ('Hardcover'),
        ('Paperback'),
        ('EBook');

    DECLARE @TitlePool TABLE (Name NVARCHAR(200));
    INSERT INTO @TitlePool (Name)
    VALUES 
        ('The Great Adventure'),
        ('Journey to the Unknown'),
        ('Shadows in the Night'),
        ('Breaking the Chains'),
        ('The Last Horizon'),
        ('Echoes of Eternity'),
        ('Beneath the Surface'),
        ('A Love Forgotten'),
        ('The Call of Destiny'),
        ('Whispers of the Wind');

    WHILE @i <= @NumberOfBooks
    BEGIN
        SET @ISBN = CAST(ABS(CHECKSUM(NEWID())) % 10000000000000 AS NVARCHAR(13));

        SELECT TOP 1 @Category = Category FROM @CategoryPool ORDER BY NEWID();
        SELECT TOP 1 @Feel = Feel FROM @FeelPool ORDER BY NEWID();
        SELECT TOP 1 @Name = Name FROM @TitlePool ORDER BY NEWID();

        SET @Price = ABS(CHECKSUM(NEWID())) % (100 - 10 + 1) + 10;

        SELECT TOP 1 @PID = PID FROM Publisher ORDER BY NEWID();

        SELECT TOP 1 @AID = AID FROM Author ORDER BY NEWID();

        INSERT INTO Book (ISBN, AID, Category, Feel, Price, PID, Name)
        VALUES (@ISBN, @AID, @Category, @Feel, @Price, @PID, @Name);

        SET @i = @i + 1;
    END
END;
GO


CREATE OR ALTER PROCEDURE populateSaleTable
    @NumberOfSales INT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @i INT = 1;
    DECLARE @SaleID INT;
    DECLARE @CID INT;
    DECLARE @ISBN NVARCHAR(13);
    DECLARE @SaleDate DATE;

    WHILE @i <= @NumberOfSales
    BEGIN
        SET @SaleID = ABS(CHECKSUM(NEWID())) % 1000 + 1; -- Random ID (1 to 1000)

        SELECT TOP 1 @CID = CID FROM Client ORDER BY NEWID();

        SELECT TOP 1 @ISBN = ISBN FROM Book ORDER BY NEWID();

        SET @SaleDate = DATEADD(DAY, -ABS(CHECKSUM(NEWID())) % 365, GETDATE());

        INSERT INTO Sale (SaleID, CID, ISBN, Sale_Date)
        VALUES (@SaleID, @CID, @ISBN, @SaleDate);

        SET @i = @i + 1;
    END
END;
GO

CREATE OR ALTER VIEW vw_Books AS
SELECT 
    ISBN, 
    Name, 
    Category, 
    Feel, 
    Price
FROM 
    Book;
GO

CREATE OR ALTER VIEW vw_SalesWithBooks AS
SELECT 
    s.SaleID, 
    s.Sale_Date, 
    b.Name AS BookName, 
    b.Price, 
    c.Name AS ClientName
FROM 
    Sale s
JOIN 
    Book b ON s.ISBN = b.ISBN
JOIN 
    Client c ON s.CID = c.CID;
GO

CREATE OR ALTER VIEW vw_SalesSummaryByCategory AS
SELECT 
    b.Category, 
    COUNT(s.SaleID) AS TotalSales, 
    SUM(b.Price) AS TotalRevenue
FROM 
    Sale s
JOIN 
    Book b ON s.ISBN = b.ISBN
GROUP BY 
    b.Category;
GO


CREATE OR ALTER PROCEDURE addToTables (@TableName VARCHAR(50)) AS
    IF @TableName IN (SELECT Name FROM Tables) BEGIN
        PRINT 'Table already in Tables'
        RETURN
    END
    IF @TableName NOT IN (SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES) BEGIN
        PRINT 'Table not in database'
        RETURN
    END
    INSERT INTO Tables (Name) VALUES (@TableName)
GO

CREATE OR ALTER PROCEDURE addToViews (@ViewName VARCHAR(50)) AS
    IF @ViewName IN (SELECT Name FROM Views) BEGIN
        PRINT 'View already in views'
        RETURN
    END
    IF @ViewName NOT IN (SELECT TABLE_NAME FROM INFORMATION_SCHEMA.VIEWS) BEGIN
        PRINT 'View not in database'
        RETURN
    END
    INSERT INTO Views (Name) VALUES (@ViewName)
GO

CREATE OR ALTER PROCEDURE addToTests (@TestName VARCHAR(50)) AS
    IF @TestName IN (SELECT Name FROM Tests) BEGIN
        PRINT 'Test already in Tests'
        RETURN
    END
    INSERT INTO Tests (Name) VALUES (@TestName)
GO

CREATE OR ALTER PROCEDURE connectTableToTest (@TableName VARCHAR(50), @TestName VARCHAR(50), @Rows INT, @Pos INT) AS
    IF @TableName NOT IN (SELECT Name FROM Tables) BEGIN
        PRINT 'Table not in Tables'
        RETURN
    END
    IF @TestName NOT IN (SELECT Name FROM TESTS) BEGIN
        PRINT 'Test not in Tests'
        RETURN
    END
    IF EXISTS(
        SELECT *
        FROM TestTables T1 JOIN Tests T2 ON T1.TestID = T2.TestID
        WHERE T2.Name = @TestName AND Position = @Pos
        ) BEGIN
        PRINT 'Position conflicts with prev. positions'
        RETURN
    END
    INSERT INTO TestTables (TestID, TableID, NoOfRows, Position) VALUES (
        (SELECT Tests.TestID FROM Tests WHERE Name = @TestName),
        (SELECT Tables.TableID FROM Tables WHERE Name = @TableName),
        @Rows,
        @Pos
    )
GO

CREATE OR ALTER PROCEDURE connectViewToTest (@ViewName VARCHAR(50), @TestName VARCHAR(50)) AS
    IF @ViewName NOT IN (SELECT Name FROM Views) BEGIN
        PRINT 'View not in Views'
        RETURN
    END
    IF @TestName NOT IN (SELECT Name FROM Tests) BEGIN
        PRINT 'Test not in Tests'
        RETURN
    END
    INSERT INTO TestViews(TestID, ViewID) VALUES (
        (SELECT Tests.TestID FROM Tests WHERE Name = @TestName),
        (SELECT Views.ViewID FROM Views WHERE Name = @ViewName)
    )
GO


CREATE OR ALTER PROCEDURE runTest (@TestName VARCHAR(50)) AS
    IF @TestName NOT IN (SELECT Name FROM Tests) BEGIN
        PRINT 'Test not in Tests'
        RETURN
    END

    DECLARE @Command VARCHAR(100)
    DECLARE @TestStartTime DATETIME2
    DECLARE @StartTime DATETIME2
    DECLARE @EndTime DATETIME2
    DECLARE @Table VARCHAR(50)
    DECLARE @Rows INT
    DECLARE @Pos INT
    DECLARE @View VARCHAR(50)
    DECLARE @TestID INT
    DECLARE @TestRunID INT

    SELECT @TestID = TestID FROM Tests WHERE Name = @TestName
    SET @TestRunID = ISNULL((SELECT MAX(TestRunID) + 1 FROM TestRuns), 0)

    DECLARE TableCursor CURSOR SCROLL FOR
        SELECT T1.Name, T2.NoOfRows, T2.Position
        FROM Tables T1 
        JOIN TestTables T2 ON T1.TableID = T2.TableID
        WHERE T2.TestID = @TestID
        ORDER BY T2.Position

    DECLARE ViewCursor CURSOR FOR
        SELECT V.Name
        FROM Views V 
        JOIN TestViews TV ON V.ViewID = TV.ViewID
        WHERE TV.TestID = @TestID

    SET @TestStartTime = SYSDATETIME()
    OPEN TableCursor
    FETCH LAST FROM TableCursor INTO @Table, @Rows, @Pos

    WHILE @@FETCH_STATUS = 0 BEGIN
        EXEC ('DELETE FROM ' + @Table)
        FETCH PRIOR FROM TableCursor INTO @Table, @Rows, @Pos
    END
	CLOSE TableCursor;
	
	OPEN TableCursor;
    SET IDENTITY_INSERT TestRuns ON
    INSERT INTO TestRuns(TestRunID, Description, StartAt)
    VALUES (@TestRunID, 'TESTS RESULTS FOR: ' + @TestName, @TestStartTime)
    SET IDENTITY_INSERT TestRuns OFF

    FETCH NEXT FROM TableCursor INTO @Table, @Rows, @Pos

    WHILE @@FETCH_STATUS = 0 BEGIN
        SET @Command = 'populate' + @TABLE + 'Table';

        IF @Command NOT IN (SELECT ROUTINE_NAME FROM INFORMATION_SCHEMA.ROUTINES) BEGIN
            PRINT @Command + ' DOES NOT EXIST'
            RETURN
        END

        SET @StartTime = SYSDATETIME()
        EXEC @Command @ROWS
        SET @EndTime = SYSDATETIME()

        INSERT INTO TestRunTables(TestRunID, TableID, StartAt, EndAt)
        VALUES (@TestRunID, (SELECT TableID FROM Tables WHERE Name = @Table), @StartTime, @EndTime)

        FETCH NEXT FROM TableCursor INTO @Table, @Rows, @Pos
    END

    CLOSE TableCursor
    DEALLOCATE TableCursor

    OPEN ViewCursor
    FETCH NEXT FROM ViewCursor INTO @View

    WHILE @@FETCH_STATUS = 0 BEGIN
        SET @Command = 'SELECT * FROM ' + @View
        SET @StartTime = SYSDATETIME()
        EXEC (@Command)
        SET @EndTime = SYSDATETIME()

        INSERT INTO TestRunViews(TestRunID, ViewID, StartAt, EndAt)
        VALUES (@TestRunID, (SELECT ViewID FROM Views WHERE Name = @View), @StartTime, @EndTime)

        FETCH NEXT FROM ViewCursor INTO @View
    END

    CLOSE ViewCursor
    DEALLOCATE ViewCursor

    UPDATE TestRuns
    SET EndAt = SYSDATETIME()
    WHERE TestRunID = @TestRunID
GO


EXEC populateBooksTable @NumberOfBooks = 2000;

EXEC addToTables Book;
EXEC addToTables Sale;

EXEC addToViews vw_Books;
EXEC addToViews vw_SalesWithBooks;
EXEC addToViews vw_SalesSummaryByCategory;

EXEC addToTests BookTest1;
EXEC connectTableToTest Book, BookTest1, 1000, 5;
EXEC connectViewToTest vw_Books, BookTest1;

EXEC addToTests SaleTest1;
EXEC connectTableToTest Sale, SaleTest1, 1000, 5;
EXEC connectViewToTest vw_SalesWithBooks, SaleTest1;

EXEC runTest SaleTest1;

EXEC runTest BookTest1;

SELECT * FROM TestRuns;
SELECT * FROM TestTables;
SELECT * FROM TestRunTables;
SELECT * FROM TestRunViews;

DROP PROCEDURE runTest;


SELECT * FROM Book;
SELECT * FROM vw_Books;
SELECT * FROM vw_SalesWithBooks;
SELECT * FROM vw_SalesSummaryByCategory;
SELECT * FROM TestRuns



