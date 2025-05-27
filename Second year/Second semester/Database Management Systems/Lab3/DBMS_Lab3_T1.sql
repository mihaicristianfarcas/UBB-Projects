USE DBMS_Lab3
GO

-- T1: Updates but then rolls back (is in uncommited state while T2 works)
BEGIN TRAN
UPDATE Worker SET name = 'DirtyName' WHERE id = 1
WAITFOR DELAY '00:00:10'
ROLLBACK TRAN

-- T1: Changes row while T2 reads
BEGIN TRAN
WAITFOR DELAY '00:00:05'
UPDATE Worker SET email = 'new@mail.com' WHERE id = 2
COMMIT TRAN

-- T1: Inserts the phantom while T2 works
BEGIN TRAN
WAITFOR DELAY '00:00:04'
INSERT INTO Worker(name, email) VALUES ('Phantom', 'phantom@mail.com')
COMMIT TRAN

-- T1: Deadlock victim
BEGIN TRAN
UPDATE Worker SET name = 'Deadlock T1' WHERE id = 1
WAITFOR DELAY '00:00:10'
UPDATE Department SET name = 'Blocked' WHERE id = 1
COMMIT TRAN
