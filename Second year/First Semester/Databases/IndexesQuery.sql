USE IndexLab
GO

CREATE TABLE Ta (
    aid INT PRIMARY KEY,
    a2 INT UNIQUE
);

CREATE TABLE Tb (
    bid INT PRIMARY KEY,
    b2 INT
);

CREATE TABLE Tc (
    cid INT PRIMARY KEY,
    aid INT,
    bid INT,
    FOREIGN KEY (aid) REFERENCES Ta(aid),
    FOREIGN KEY (bid) REFERENCES Tb(bid)
);

CREATE OR ALTER PROCEDURE populateTableTa(@rows INT)
AS
	WHILE @rows > 0 
	BEGIN
		INSERT INTO Ta VALUES(@rows, @rows - 5)
		SET @rows = @rows - 1
	END
GO

CREATE OR ALTER PROCEDURE populateTableTb(@rows INT)
AS
	WHILE @rows > 0 BEGIN
		INSERT INTO Tb VALUES(@rows, @rows - 10)
		SET @rows = @rows - 1
	END
GO

CREATE OR ALTER PROCEDURE populateTableTc(@rows INT) 
AS
	IF @rows > (SELECT COUNT(*) FROM Ta) * (SELECT COUNT(*) FROM Tb) 
	BEGIN
		RAISERROR ('Too many entities requested', 10, 1)
	END

	DECLARE valuesCursor CURSOR LOCAL FOR (
		SELECT a.aid, b.bid
		FROM Ta a CROSS JOIN Tb b
	)

	DECLARE @aid INT
	DECLARE @bid INT
	OPEN valuesCursor
	FETCH valuesCursor INTO @aid, @bid
	WHILE @@FETCH_STATUS = 0 AND @rows > 0
	BEGIN
		INSERT INTO Tc VALUES(@rows, @aid, @bid)
		FETCH valuesCursor INTO @aid, @bid	
		SET @rows = @rows - 1
	END
	CLOSE valuesCursor 
	DEALLOCATE valuesCursor;
GO

EXEC populateTableTa 1000;
EXEC populateTableTb 2000;
EXEC populateTableTc 3000;

-- a.

-- Clustered index scan
SELECT * FROM Ta -- 0.0038

-- Clustered index seek
SELECT * FROM Ta WHERE aid = 5; -- 0.0031

-- Non-clustered index scan
CREATE NONCLUSTERED INDEX idx_a2 ON Ta(a2);
SELECT * FROM Ta; -- 0.0038

-- Non-clustered index seek & key lookup
SELECT * FROM Ta WHERE a2 = 200; -- 0.0031

-- b.

SELECT * FROM Tb WHERE b2 = 120; -- 0.0060

SELECT b2 FROM Tb WHERE b2 = 120; -- 0.0060

CREATE NONCLUSTERED INDEX idx_b2 ON Tb(b2);

SELECT * FROM Tb WHERE b2 = 120; -- 0.0031

SELECT b2 FROM Tb WHERE b2 = 120; -- 0.0031

-- c.

CREATE OR ALTER VIEW v_Ta_Tb AS
SELECT Ta.aid, Tb.bid, Ta.a2, Tb.b2
FROM Ta
JOIN Tc ON Ta.aid = Tc.aid
JOIN Tb ON Tb.bid = Tc.bid;

SELECT * FROM v_Ta_Tb; -- 0.0083

CREATE NONCLUSTERED INDEX idx_Tc_aid_bid ON Tc(aid, bid);

SELECT * FROM v_Ta_Tb; -- 0.0075

DROP TABLE Ta
DROP TABLE Tb
DROP TABLE Tc
DROP PROCEDURE populateTableTa
DROP PROCEDURE populateTableTb
DROP PROCEDURE populateTableTc