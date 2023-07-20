-- Projekt 2. èást - SQL skript pro vytvoøení základních objektù schématu
-- zadání èíslo 28 - Nemocnice
-- autoøi: Maxim Plièka(xplick04), Jan Zdenìk(xzdene01)

DROP TABLE PacientsHistory;
DROP TABLE PacientUsesMedicine;
DROP TABLE Examinations;
DROP TABLE Hospitalizations;
DROP TABLE Medicines;
DROP TABLE Rooms;
DROP TABLE Pacients;
DROP TABLE Nurses;
DROP TABLE Doctors;
DROP TABLE Departments;
DROP MATERIALIZED VIEW ViewPacientsMedsCount;


CREATE TABLE Departments (
    DepartmentID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    Name VARCHAR(255) NOT NULL
        CHECK(REGEXP_LIKE(Name, '^[A-Z][a-z]*$')),
    Specialization VARCHAR(255) NOT NULL
        CHECK(REGEXP_LIKE(Specialization, '^[a-z]+( [a-z]+)*$'))
);

CREATE TABLE Doctors (
    DoctorID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    FirstName VARCHAR(50) NOT NULL
            CHECK(REGEXP_LIKE(Firstname, '^[A-Z][a-z]+$')),
    LastName VARCHAR(50) NOT NULL
        CHECK(REGEXP_LIKE(Lastname, '^[A-Z][a-z]+$')),
    BirthDate DATE NOT NULL,

    DepartmentID INT NOT NULL,
    CONSTRAINT FK_DoctorDepartment --pracuje na
        FOREIGN KEY (DepartmentID)
        REFERENCES Departments(DepartmentID)
);

CREATE TABLE Nurses (
    NurseID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    FirstName VARCHAR(50) NOT NULL
            CHECK(REGEXP_LIKE(Firstname, '^[A-Z][a-z]+$')),
    LastName VARCHAR(50) NOT NULL
        CHECK(REGEXP_LIKE(Lastname, '^[A-Z][a-z]+$')),
    BirthDate DATE NOT NULL,

    DepartmentID INT NOT NULL,
    CONSTRAINT FK_NurseDepartment --pracuje na
        FOREIGN KEY (DepartmentID)
        REFERENCES Departments(DepartmentID)
);

CREATE TABLE Pacients (
    PacientID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    FirstName VARCHAR(50) NOT NULL
            CHECK(REGEXP_LIKE(Firstname, '^[A-Z][a-z]+$')),
    LastName VARCHAR(50) NOT NULL
        CHECK(REGEXP_LIKE(Lastname, '^[A-Z][a-z]+$')),
    Gender VARCHAR(10) NOT NULL
        CHECK(REGEXP_LIKE(Gender, '^[a-z]+$')),
    Street VARCHAR(255) NOT NULL
        CHECK(REGEXP_LIKE(Street, '^[A-Z][a-z]+ [0-9]+$')),
    City VARCHAR(50) NOT NULL
        CHECK(REGEXP_LIKE(City, '^[A-Z][a-z]+$')),
    ZipCode VARCHAR(20) NOT NULL
        CHECK(REGEXP_LIKE(ZipCode, '^[0-9]+$')),
    BirthDate DATE NOT NULL,

    NurseID INT NOT NULL,
    CONSTRAINT FK_PacientNurse  --objedna
        FOREIGN KEY (NurseID)
        REFERENCES Nurses(NurseID)
);

CREATE TABLE Rooms (
    RoomID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    Equipment VARCHAR(255) NOT NULL --set(values)
        CHECK(REGEXP_LIKE(Equipment, '^[a-z]+(,[a-z]+)*$')),
    NumberOfBeds SMALLINT,

    DepartmentID INT NOT NULL,
    CONSTRAINT FK_RoomDepartment --se nachazi na
        FOREIGN KEY (DepartmentID)
        REFERENCES Departments(DepartmentID)
);

CREATE TABLE Medicines (
    MedicineID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    Brand VARCHAR(50) NOT NULL
        CHECK(REGEXP_LIKE(Brand, '^[A-Z][a-z]+$')),
    Dosage VARCHAR(20) NOT NULL --number/time interval(H/D/W)
        CHECK(REGEXP_LIKE(Dosage, '^[0-9]+\/(H|D|W)$')),
    Composition VARCHAR(255) NOT NULL
        CHECK(REGEXP_LIKE(Composition, '^[a-z]+(,[a-z]+)*$')),
    Usage VARCHAR(255) NOT NULL
);

CREATE TABLE Hospitalizations (
    HospitalizationID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    StartTime TIMESTAMP,

    DoctorID INT NOT NULL,
    CONSTRAINT FK_HospitalizationDoctor --je pridelen k
        FOREIGN KEY (DoctorID)
        REFERENCES Doctors(DoctorID),

    PacientID INT NOT NULL,
    CONSTRAINT FK_HospitalizationPacient --je prijat k
        FOREIGN KEY (PacientID)
        REFERENCES Pacients(PacientID) ON DELETE CASCADE,

    RoomID INT NOT NULL,
    CONSTRAINT FK_HospitalizationRoom --probiha v
        FOREIGN KEY (RoomID)
        REFERENCES Rooms(RoomID)
);

CREATE TABLE Examinations (
    ExaminationID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    Name VARCHAR(255) NOT NULL
        CHECK(REGEXP_LIKE(Name, '^[A-Z][a-z]+$')),
    Description VARCHAR(255) NOT NULL,
    StartTime TIMESTAMP,

    DoctorID INT NOT NULL,
    CONSTRAINT FK_ExaminationDoctor --provadi
        FOREIGN KEY (DoctorID)
        REFERENCES Doctors(DoctorID),

    PacientID INT NOT NULL,
    CONSTRAINT FK_ExaminationPacient --podstupuje
        FOREIGN KEY (PacientID)
        REFERENCES Pacients(PacientID) ON DELETE CASCADE,

    HospitalizationID INT DEFAULT NULL, --0..1
    CONSTRAINT FK_ExaminationHospitalization --probiha behem
        FOREIGN KEY (HospitalizationID)
        REFERENCES Hospitalizations(HospitalizationID) ON DELETE CASCADE,

    RoomID INT NOT NULL,
    CONSTRAINT FK_ExaminationRoom --probiha v
        FOREIGN KEY (RoomID)
        REFERENCES Rooms(RoomID)
);

CREATE TABLE PacientUsesMedicine (  --m:n relace, pacient pouziva leky
    PacientUsesMedicineID INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    PacientID INT NOT NULL,
    CONSTRAINT FK_PacientUsesMedicine_p --probiha v
        FOREIGN KEY (PacientID)
        REFERENCES Pacients(PacientID) ON DELETE CASCADE,

    MedicineID INT NOT NULL,
    CONSTRAINT FK_PacientUsesMedicine_m --probiha v
        FOREIGN KEY (MedicineID)
        REFERENCES Medicines(MedicineID) ON DELETE CASCADE
);


/*---------------------------Triggery-------------------------------------*/
CREATE TABLE PacientsHistory (
    PacientID INT,
    FirstName VARCHAR(50),
    LastName VARCHAR(50),
    Gender VARCHAR(10) ,
    Street VARCHAR(255),
    City VARCHAR(50) ,
    ZipCode VARCHAR(20),
    BirthDate DATE,
    DoctorID INT,
    DeleteDate DATE
);

SET serveroutput on;

CREATE OR REPLACE TRIGGER DeleteHistory AFTER DELETE ON Pacients FOR EACH ROW
    BEGIN
        INSERT INTO PacientsHistory(PacientID, FirstName, LastName, Gender,
                 Street, City, ZipCode, BirthDate, DeleteDate)
        VALUES (:OLD.PacientID, :OLD.Firstname, :OLD.Lastname, :OLD.Gender,
         :OLD.Street, :OLD.City, :OLD.ZipCode, :OLD.BirthDate, CURRENT_DATE);
    END;
/

CREATE OR REPLACE TRIGGER CheckRoomCapacity BEFORE INSERT OR UPDATE ON Hospitalizations FOR EACH ROW
    DECLARE
        CountCheck int;
        RoomCapacity int;
    BEGIN
        SELECT COUNT(*) INTO CountCheck FROM Hospitalizations WHERE RoomID = :NEW.RoomID;
        /*DBMS_OUTPUT.PUT_LINE(CountCheck);*/
        SELECT NumberOfBeds INTO RoomCapacity FROM Rooms WHERE Rooms.RoomID = :NEW.RoomID;
        /*DBMS_OUTPUT.PUT_LINE(RoomCapacity);*/
        IF RoomCapacity <= CountCheck THEN
            raise_application_error(-20000, 'Room is already full.');
        END IF;
    END;
/

/*---------------------------Procedury-------------------------------------*/


CREATE OR REPLACE PROCEDURE GetPacientHeader(PersonID IN INT) 
IS
    FirstNameOUT Pacients.FirstName%TYPE;
    LastNameOUT Pacients.LastName%TYPE;
    StreetOUT Pacients.Street%TYPE;
    CityOUT Pacients.City%TYPE;
    ZipCodeOUT Pacients.ZipCode%TYPE;
BEGIN
    SELECT FirstName, LastName, Street, City, ZipCode INTO 
    FirstNameOUT, LastNameOUT, StreetOUT, CityOUT, ZipCodeOUT FROM Pacients 
    WHERE PacientID = PersonID;
    DBMS_OUTPUT.PUT_LINE(FirstNameOUT || ' ' || LastNameOUT || CHR(10)|| StreetOUT 
    || CHR(10)|| CityOUT || CHR(10)|| ZipCodeOUT);
    EXCEPTION WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Cannot find Pacient by his ID');
END;
/    

CREATE OR REPLACE PROCEDURE DoctorStats AS
        CURSOR DoctorTable IS SELECT * FROM Doctors;
        singleRow Doctors%ROWTYPE;
        HospiCount INT;
        ExamiCount INT;
    BEGIN
        DBMS_OUTPUT.PUT_LINE('Doctors statistics:');
        OPEN DoctorTable;
        LOOP
            FETCH DoctorTable INTO singleRow;
            EXIT WHEN DoctorTable%NOTFOUND;
            SELECT Count(*) INTO HospiCount FROM Doctors NATURAL JOIN Examinations WHERE DoctorID = singleRow.DoctorID;
            SELECT Count(*) INTO ExamiCount FROM Doctors NATURAL JOIN Hospitalizations WHERE DoctorID = singleRow.DoctorID;
            IF HospiCount = 0 AND ExamiCount = 0 THEN
                DBMS_OUTPUT.PUT_LINE('Doctor ' || singleRow.FirstName || ' ' || singleRow.LastName || ' has done nothing.');
            ELSE
                DBMS_OUTPUT.PUT_LINE('Doctor ' || singleRow.FirstName || ' ' || singleRow.LastName || ' has done ' ||
                TO_CHAR(HospiCount) || ' hospitalizations and ' || TO_CHAR(ExamiCount)|| ' examinations.');
            END IF;
        END LOOP;
    EXCEPTION WHEN OTHERS THEN
         DBMS_OUTPUT.PUT_LINE('Error in DoctorStats');
    END;
/
--------------------------------naplnìní tabulky hodnotami-----------------------------------------------

INSERT INTO Departments (Name, Specialization)
VALUES ('A','endoskopicke centrum');
INSERT INTO Departments (Name, Specialization)
VALUES ('B','chirurgicka klinika');
INSERT INTO Departments (Name, Specialization)
VALUES ('C','klinika decke orl');
INSERT INTO Departments (Name, Specialization)
VALUES ('D','ocni klinika');
INSERT INTO Departments (Name, Specialization)
VALUES ('E','usni klinika');

INSERT INTO Nurses (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('Jana','Pokora',TO_DATE('2001-03-29', 'yyyy/mm/dd'),1);
INSERT INTO Nurses (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('Janka','Kralovna',TO_DATE('2008-03-30', 'yyyy/mm/dd'),1);
INSERT INTO Nurses (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('Januse','Princezna',TO_DATE('2009-03-31', 'yyyy/mm/dd'),2);

INSERT INTO Doctors (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('Jan','Pokora',TO_DATE('2001-03-29', 'yyyy/mm/dd'),1);
INSERT INTO Doctors (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('Janis','Pokora',TO_DATE('1999-03-30', 'yyyy/mm/dd'),2);
INSERT INTO Doctors (FirstName, LastName, BirthDate, DepartmentID)
VALUES ('John','Petrohrad',TO_DATE('2000-03-28', 'yyyy/mm/dd'),3);

INSERT INTO Pacients (FirstName, LastName, Gender, Street, City, ZipCode, BirthDate, NurseID)
VALUES ('Andrew','Denim','nezname','Oxford 2','England','11900',TO_DATE('2001-03-30', 'yyyy/mm/dd'),1);
INSERT INTO Pacients (FirstName, LastName, Gender, Street, City, ZipCode, BirthDate, NurseID)
VALUES ('Kalhoty','Latka','nezname','Kredit 1','Ingland','11100',TO_DATE('2001-05-12', 'yyyy/mm/dd'),1);
INSERT INTO Pacients (FirstName, LastName, Gender, Street, City, ZipCode, BirthDate, NurseID)
VALUES ('Jan','Latka','muz','Kredit 2','Brno','60200',TO_DATE('2002-08-12', 'yyyy/mm/dd'),3);

INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',2,2);
INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',1,1);
INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',4,2);

INSERT INTO Medicines (Brand, Dosage, Composition, Usage)
VALUES ('Arginmax','2/H','maso','oralni podani');
INSERT INTO Medicines (Brand, Dosage, Composition, Usage)
VALUES ('Brufen','2/W','zelenina','injekce');
INSERT INTO Medicines (Brand, Dosage, Composition, Usage)
VALUES ('Paralen','1/W','citrusy','oralni podani');

INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (TO_DATE('2020-03-30', 'yyyy/mm/dd'), 1, 1, 1);
INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (TO_DATE('2020-04-28', 'yyyy/mm/dd'), 1, 2, 3);
INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (TO_DATE('2021-03-30', 'yyyy/mm/dd'), 2, 3, 1);


INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola prostaty',TO_DATE('2021-08-05', 'yyyy/mm/dd'),1,1,1,1);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola prostaty',TO_DATE('2021-03-30', 'yyyy/mm/dd'),1,2,2,1);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola oci',TO_DATE('2020-03-30', 'yyyy/mm/dd'),1,2,2,2);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola chodidel',TO_DATE('2020-03-30', 'yyyy/mm/dd'),2,1,1,1);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola chodidel',TO_DATE('2020-03-31', 'yyyy/mm/dd'),2,1,1,2);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola chodidel',TO_DATE('2020-03-30', 'yyyy/mm/dd'),2,3,3,1);

INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (1,1);
INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (1,2);
INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (2,1);
INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (2,3);
INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (3,3);


-- DeleteHistory trigger demonstrace
DELETE FROM PACIENTS WHERE PACIENTID = 1;

-- CheckRoomCapacity trigger demonstrace
-- Správnì:
INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (TO_DATE('2021-03-31', 'yyyy/mm/dd'), 1, 3, 2);
-- Vyjímka:
/*INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (TO_DATE('2021-03-31', 'yyyy/mm/dd'), 2, 1, 1);*/


-- Tisk hlavièky pacienta
-- Správnì:
CALL GetPacientHeader(1);
-- Vyjímka: (špatné PatientID):
/*CALL GetPacientHeader(10);*/

--statistiky hospitalizací a prohlídek doktorù
CALL DoctorStats();


/*-------------------definice práv--------------------------------------------*/
GRANT ALL ON Departments TO xzdene01;
GRANT ALL ON Nurses TO xzdene01;
GRANT ALL ON Doctors TO xzdene01;
GRANT ALL ON Pacients TO xzdene01;
GRANT ALL ON Rooms TO xzdene01;
GRANT ALL ON Medicines TO xzdene01;
GRANT ALL ON Hospitalizations TO xzdene01;
GRANT ALL ON Examinations TO xzdene01;
GRANT ALL ON PacientUsesMedicine TO xzdene01;
GRANT ALL ON PacientsHistory TO xzdene01;

/*-------------------materializovany pohled-----------------------------------*/
CREATE MATERIALIZED VIEW ViewPacientsMedsCount REFRESH ON COMMIT AS
    SELECT FirstName, LastName, COUNT(*) PocetLeku FROM xplick04.PACIENTS
    NATURAL JOIN xplick04.PacientUsesMedicine NATURAL JOIN xplick04.Medicines
    GROUP BY FirstName, LastName;
    
--operace nad materializovaným pohledem
SELECT * FROM ViewPacientsMedsCount;

INSERT INTO xplick04.PacientUsesMedicine(PacientID, MedicineID) VALUES (1,3);
COMMIT;

SELECT * FROM ViewPacientsMedsCount;


/*-------------------explain plan---------------------------------------------*/
--kolikrát byl pacient jménem jeans hospitalozvan v jednotlivých letech
EXPLAIN PLAN FOR 
SELECT  TO_CHAR(StartTime, 'yyyy'), COUNT(*) FROM Pacients 
    NATURAL JOIN Hospitalizations WHERE FirstName = 'Jeans' 
    GROUP BY TO_CHAR(StartTime, 'yyyy');
SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());

CREATE INDEX PacientsIDX ON Pacients(PacientID, FirstName);
CREATE INDEX HospitalizationsIDX ON Hospitalizations(PacientID, StartTime);

EXPLAIN PLAN FOR 
SELECT  TO_CHAR(StartTime, 'yyyy'), COUNT(*) FROM Pacients 
    NATURAL JOIN Hospitalizations WHERE FirstName = 'Jeans' 
    GROUP BY TO_CHAR(StartTime, 'yyyy');
SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());

