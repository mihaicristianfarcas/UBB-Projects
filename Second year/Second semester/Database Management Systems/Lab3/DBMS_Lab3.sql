USE DBMS_Lab3
GO

-- Schema
CREATE OR ALTER TABLE Worker (
    id INT PRIMARY KEY IDENTITY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL
);

CREATE OR ALTER TABLE Department (
    id INT PRIMARY KEY IDENTITY,
    name VARCHAR(100) NOT NULL,
    location VARCHAR(100) NOT NULL
);

CREATE OR ALTER TABLE WorkerDepartment (
    worker_id INT,
    department_id INT,
    PRIMARY KEY(worker_id, department_id),
    FOREIGN KEY(worker_id) REFERENCES Worker(id),
    FOREIGN KEY(department_id) REFERENCES Department(id)
);

-- Validation functions
CREATE OR ALTER FUNCTION uf_ValidateEmail (@email VARCHAR(100)) RETURNS INT AS
BEGIN
    RETURN CASE 
        WHEN @email LIKE '%@%.%' THEN 1 
        ELSE 0 
    END
END

CREATE OR ALTER FUNCTION uf_ValidateName (@name VARCHAR(100)) RETURNS INT AS
BEGIN
    RETURN CASE 
        WHEN ASCII(LEFT(@name, 1)) BETWEEN 65 AND 90 THEN 1 
        ELSE 0 
    END
END

-- Log table
CREATE OR ALTER TABLE LogTable (
    Lid INT IDENTITY PRIMARY KEY,
    TypeOperation VARCHAR(50),
    TableOperation VARCHAR(50),
    ExecutionDate DATETIME DEFAULT GETDATE()
);

-- Stored procedure
CREATE OR ALTER PROCEDURE AddWorkerDepartment
    @workerName VARCHAR(100),
    @workerEmail VARCHAR(100),
    @departmentName VARCHAR(100),
    @departmentLocation VARCHAR(100)
AS
BEGIN
    BEGIN TRANSACTION

    BEGIN TRY
        -- Validation
        IF dbo.uf_ValidateEmail(@workerEmail) = 0
            THROW 50001, 'Invalid email format.', 1;
        IF dbo.uf_ValidateName(@workerName) = 0
            THROW 50002, 'Worker name must start with an uppercase letter.', 1;

        -- Insert Worker
        INSERT INTO Worker (name, email)
        VALUES (@workerName, @workerEmail);

        DECLARE @workerId INT = SCOPE_IDENTITY();

        -- Insert Department
        INSERT INTO Department (name, location)
        VALUES (@departmentName, @departmentLocation);

        DECLARE @departmentId INT = SCOPE_IDENTITY();

        -- Insert WorkerDepartment
        INSERT INTO WorkerDepartment (worker_id, department_id)
        VALUES (@workerId, @departmentId);

        -- Log entries
        INSERT INTO LogTable (TypeOperation, TableOperation) VALUES 
        ('INSERT', 'Worker'),
        ('INSERT', 'Department'),
        ('INSERT', 'WorkerDepartment');

        COMMIT TRANSACTION;
        PRINT 'Transaction committed.';
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Transaction rolled back: ' + ERROR_MESSAGE();
    END CATCH
END

-- Successful insert
EXEC AddWorkerDepartment 
    @workerName = 'Alice',
    @workerEmail = 'alice@example.com',
    @departmentName = 'IT',
    @departmentLocation = 'Building A';

-- Unsuccessful insert
EXEC AddWorkerDepartment 
    @workerName = 'Bob',
    @workerEmail = 'not-an-email',
    @departmentName = 'HR',
    @departmentLocation = 'Building B';

-- Procedure with partial recovery
CREATE OR ALTER PROCEDURE AddWorkerDepartment_PartialRecovery
    @workerName VARCHAR(100),
    @workerEmail VARCHAR(100),
    @departmentName VARCHAR(100),
    @departmentLocation VARCHAR(100)
AS
BEGIN
    DECLARE @workerId INT = NULL;
    DECLARE @departmentId INT = NULL;

    -- INSERT WORKER
    BEGIN TRY
        BEGIN TRANSACTION
        IF dbo.uf_ValidateEmail(@workerEmail) = 0
            THROW 50001, 'Invalid email format.', 1;
        IF dbo.uf_ValidateName(@workerName) = 0
            THROW 50002, 'Worker name must start with an uppercase letter.', 1;

        INSERT INTO Worker (name, email)
        VALUES (@workerName, @workerEmail);

        SET @workerId = SCOPE_IDENTITY();

        INSERT INTO LogTable (TypeOperation, TableOperation) 
        VALUES ('INSERT', 'Worker');

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Worker insert failed: ' + ERROR_MESSAGE();
        RETURN;
    END CATCH

    -- INSERT DEPARTMENT
    BEGIN TRY
        BEGIN TRANSACTION

        INSERT INTO Department (name, location)
        VALUES (@departmentName, @departmentLocation);

        SET @departmentId = SCOPE_IDENTITY();

        INSERT INTO LogTable (TypeOperation, TableOperation) 
        VALUES ('INSERT', 'Department');

        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Department insert failed: ' + ERROR_MESSAGE();
        RETURN;
    END CATCH

    -- INSERT INTO WorkerDepartment (link table)
    BEGIN TRY
        BEGIN TRANSACTION

        INSERT INTO WorkerDepartment (worker_id, department_id)
        VALUES (@workerId, @departmentId);

        INSERT INTO LogTable (TypeOperation, TableOperation) 
        VALUES ('INSERT', 'WorkerDepartment');

        COMMIT TRANSACTION;
        PRINT 'All inserts completed successfully.';
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        PRINT 'Link insert failed: ' + ERROR_MESSAGE();
        RETURN;
    END CATCH
END

-- Successful insert
EXEC AddWorkerDepartment_PartialRecovery 
    @workerName = 'Maria',
    @workerEmail = 'maria@company.com',
    @departmentName = 'Finance',
    @departmentLocation = 'HQ';

-- Partially successful (worker ok, department fail)
EXEC AddWorkerDepartment_PartialRecovery 
    @workerName = 'George',
    @workerEmail = 'george@company.com',
    @departmentName = 'Security',
    @departmentLocation = NULL;
