-- ModifyColumnType
CREATE PROCEDURE ModifyColumnType
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @NewType NVARCHAR(50),
    @Version INT
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ALTER COLUMN ' + QUOTENAME(@ColumnName) + 
               N' ' + @NewType + N';';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion(Version) VALUES (@Version);
END
GO

-- RevertModifyColumnType
CREATE PROCEDURE RevertModifyColumnType(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @OldType NVARCHAR(50),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ALTER COLUMN ' + QUOTENAME(@ColumnName) + 
               N' ' + @OldType + N';';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion(Version) VALUES (@Version);
END
GO

-- AddColumn
CREATE PROCEDURE AddColumn(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @ColumnType NVARCHAR(50),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ADD ' + QUOTENAME(@ColumnName) + N' ' + @ColumnType + N';';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- RemoveColumn
CREATE PROCEDURE RemoveColumn(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' DROP COLUMN ' + QUOTENAME(@ColumnName) + N';';
    EXEC sp_executesql @SQL;
    DELETE FROM DatabaseVersion WHERE Version = @Version;
END
GO

-- AddDefaultConstraint
CREATE PROCEDURE AddDefaultConstraint(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @DefaultValue SQL_VARIANT,
    @ConstraintName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ADD CONSTRAINT ' + QUOTENAME(@ConstraintName) + 
               N' DEFAULT (' + CAST(@DefaultValue AS NVARCHAR) + N') FOR ' + QUOTENAME(@ColumnName) + N';';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- RemoveDefaultConstraint
CREATE PROCEDURE RemoveDefaultConstraint(
    @TableName NVARCHAR(128),
    @ConstraintName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) +
               N' DROP CONSTRAINT ' + QUOTENAME(@ConstraintName) + N';';
    EXEC sp_executesql @SQL;
    DELETE FROM DatabaseVersion WHERE Version = @Version;
END
GO

-- AddPrimaryKey
CREATE PROCEDURE AddPrimaryKey(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @PKName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ADD CONSTRAINT ' + QUOTENAME(@PKName) + 
               N' PRIMARY KEY (' + QUOTENAME(@ColumnName) + N');';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- RemovePrimaryKey
CREATE PROCEDURE RemovePrimaryKey(
    @TableName NVARCHAR(128),
    @PKName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' DROP CONSTRAINT ' + QUOTENAME(@PKName) + N';';
    EXEC sp_executesql @SQL;
    DELETE FROM DatabaseVersion WHERE Version = @Version;
END
GO

-- AddCandidateKey
CREATE PROCEDURE AddCandidateKey
    @TableName NVARCHAR(128),
    @ConstraintName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @Version INT
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) +
               N' ADD CONSTRAINT ' + QUOTENAME(@ConstraintName) +
               N' UNIQUE (' + QUOTENAME(@ColumnName) + N');';

    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion(Version) VALUES (@Version);
END
GO

-- RemoveCandidateKey
CREATE PROCEDURE RemoveCandidateKey
    @TableName NVARCHAR(128),
    @ConstraintName NVARCHAR(128),
    @Version INT
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) +
               N' DROP CONSTRAINT ' + QUOTENAME(@ConstraintName) + N';';
    EXEC sp_executesql @SQL;
    DELETE FROM DatabaseVersion WHERE Version = @Version;
END
GO

-- AddForeignKey
CREATE PROCEDURE AddForeignKey(
    @TableName NVARCHAR(128),
    @ColumnName NVARCHAR(128),
    @RefTable NVARCHAR(128),
    @RefColumn NVARCHAR(128),
    @FKName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' ADD CONSTRAINT ' + QUOTENAME(@FKName) + 
               N' FOREIGN KEY (' + QUOTENAME(@ColumnName) + N') REFERENCES ' + 
               QUOTENAME(@RefTable) + N'(' + QUOTENAME(@RefColumn) + N');';
    EXEC sp_executesql @SQL;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- RemoveForeignKey
CREATE PROCEDURE RemoveForeignKey(
    @TableName NVARCHAR(128),
    @FKName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
    DECLARE @SQL NVARCHAR(MAX);
    SET @SQL = N'ALTER TABLE ' + QUOTENAME(@TableName) + 
               N' DROP CONSTRAINT ' + QUOTENAME(@FKName) + N';';
    EXEC sp_executesql @SQL;
    DELETE FROM DatabaseVersion WHERE Version = @Version;
END
GO

-- CreateTable
CREATE PROCEDURE CreateTable(
    @TableScript NVARCHAR(MAX),
    @Version INT
)
AS
BEGIN
	EXEC sp_executesql @TableScript;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- DropTableIfExists
CREATE PROCEDURE DropTableIfExists(
    @TableName NVARCHAR(128),
    @Version INT
)
AS
BEGIN
	DECLARE @DropTableScript NVARCHAR(MAX);
    SET @DropTableScript = 'IF EXISTS (SELECT 1 FROM sys.objects WHERE OBJECT_ID = OBJECT_ID(N''' + @TableName + ''') AND type = (N''U'')) DROP TABLE ' + QUOTENAME(@TableName) + ';';
    EXEC sp_executesql @DropTableScript;
    INSERT INTO DatabaseVersion (Version) VALUES (@Version);
END
GO

-- SetDatabaseVersion
CREATE PROCEDURE SetDatabaseVersion(@TargetVersion INT)
AS
BEGIN
    DECLARE @CurrentVersion INT;
    SET @CurrentVersion = (SELECT ISNULL(MAX(Version), 0) FROM DatabaseVersion);

    IF @TargetVersion > @CurrentVersion
    BEGIN
        WHILE @CurrentVersion < @TargetVersion
        BEGIN
            SET @CurrentVersion = @CurrentVersion + 1;

            IF @CurrentVersion = 1
            BEGIN
                EXEC AddColumn @TableName = 'TestTable', @ColumnName = 'Age', @ColumnType = 'INT', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 2
            BEGIN
                EXEC AddDefaultConstraint @TableName = 'TestTable', @ColumnName = 'Age', @DefaultValue = 30, 
                    @ConstraintName = 'DF_TestTable_Age', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 3
            BEGIN
                EXEC AddPrimaryKey @TableName = 'TestTable', @ColumnName = 'ID', @PKName = 'PK_TestTable_ID', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 4
            BEGIN
                EXEC AddCandidateKey @TableName = 'TestTable', @ConstraintName = 'UQ_TestTable_Name', 
                    @ColumnName = 'Name', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 5
            BEGIN
                EXEC AddForeignKey @TableName = 'TestTable', @ColumnName = 'ID', @RefTable = 'RefTable', 
                    @RefColumn = 'RefID', @FKName = 'FK_TestTable_RefTable', @Version = @CurrentVersion;
            END
        END
    END
    ELSE IF @TargetVersion < @CurrentVersion
    BEGIN
        WHILE @CurrentVersion > @TargetVersion
        BEGIN
            IF @CurrentVersion = 5
            BEGIN
                EXEC RemoveForeignKey @TableName = 'TestTable', @FKName = 'FK_TestTable_RefTable', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 4
            BEGIN
                EXEC RemoveCandidateKey @TableName = 'TestTable', @ConstraintName = 'UQ_TestTable_Name', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 3
            BEGIN
                EXEC RemovePrimaryKey @TableName = 'TestTable', @PKName = 'PK_TestTable_ID', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 2
            BEGIN
                EXEC RemoveDefaultConstraint @TableName = 'TestTable', @ConstraintName = 'DF_TestTable_Age', @Version = @CurrentVersion;
            END
            ELSE IF @CurrentVersion = 1
            BEGIN
                EXEC RemoveColumn @TableName = 'TestTable', @ColumnName = 'Age', @Version = @CurrentVersion;
            END
            SET @CurrentVersion = @CurrentVersion - 1;
        END
    END
END
GO



-- Table for version tracking
CREATE TABLE DatabaseVersion (
    Version INT PRIMARY KEY,
    AppliedDate DATETIME DEFAULT GETDATE()
);
GO

-- Testing Procedures
CREATE TABLE TestTable (
    ID INT NOT NULL,
    Name NVARCHAR(100)
);
CREATE TABLE RefTable (RefID INT PRIMARY KEY);


-- Test runs
-- Add a column
EXEC AddColumn @TableName = 'TestTable', @ColumnName = 'Age', @ColumnType = 'INT', @Version = 1;

-- Modify column type
EXEC ModifyColumnType @TableName = 'TestTable', @ColumnName = 'Name', @NewType = 'NVARCHAR(200)', @Version = 2;

-- Add a default constraint
EXEC AddDefaultConstraint @TableName = 'TestTable', @ColumnName = 'Age', @DefaultValue = 2, 
    @ConstraintName = 'DF_TestTable_Age', @Version = 3;

-- Add a primary key
EXEC AddPrimaryKey @TableName = 'TestTable', @ColumnName = 'ID', @PKName = 'PK_TestTable_ID', @Version = 4;

-- Add a foreign key
EXEC AddForeignKey @TableName = 'TestTable', @ColumnName = 'ID', @RefTable = 'RefTable', @RefColumn = 'RefID', 
    @FKName = 'FK_TestTable_RefTable', @Version = 5;

EXEC SetDatabaseVersion @TargetVersion = 1;
SELECT * FROM DatabaseVersion

EXEC SetDatabaseVersion @TargetVersion = 3;
SELECT * FROM DatabaseVersion

-- Cleanup
DROP TABLE TestTable;
DROP TABLE RefTable;
DROP TABLE DatabaseVersion;
