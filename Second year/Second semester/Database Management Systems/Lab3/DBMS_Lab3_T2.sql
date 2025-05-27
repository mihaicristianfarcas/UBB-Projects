USE DBMS_Lab3
GO

-- T2: Use READ UNCOMMITED to prevent dirty reads (second SELECT returns only committed data)
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN TRAN
SELECT * FROM Worker WHERE id = 1
WAITFOR DELAY '00:00:10'
SELECT * FROM Worker WHERE id = 1
COMMIT TRAN

-- T2: Use REPEATABLE READ to ensure the row can’t change while the transaction is open
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
BEGIN TRAN
SELECT email FROM Worker WHERE id = 2
WAITFOR DELAY '00:00:10'
SELECT email FROM Worker WHERE id = 2
COMMIT TRAN

-- T2: Use SERIALIZALBE to lock range of rows, preventing new ones from being inserted during the transaction
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRAN
SELECT * FROM Worker
WAITFOR DELAY '00:00:10'
SELECT * FROM Worker
COMMIT TRAN

-- T2: Use DEADLOCK_PRIORITY HIGH to keep this transaction alive and compute results
SET DEADLOCK_PRIORITY HIGH
BEGIN TRAN
UPDATE Department SET name = 'Deadlock T2' WHERE id = 1
WAITFOR DELAY '00:00:10'
UPDATE Worker SET name = 'Blocked' WHERE id = 1
COMMIT TRAN
