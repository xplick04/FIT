-- Projekt 2. část - SQL skript pro vytvoření základních objektů schématu
-- zadání číslo 28 - Nemocnice
-- autoři: Maxim Plička(xplick04), Jan Zdeněk(xzdene01)

DROP TABLE PacientUsesMedicine;
DROP TABLE Examinations;
DROP TABLE Hospitalizations;
DROP TABLE Medicines;
DROP TABLE Rooms;
DROP TABLE Pacients;
DROP TABLE Nurses;
DROP TABLE Doctors;
DROP TABLE Departments;

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
        REFERENCES Pacients(PacientID),

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
        REFERENCES Pacients(PacientID),

    HospitalizationID INT DEFAULT NULL, --0..1
    CONSTRAINT FK_ExaminationHospitalization --probiha behem
        FOREIGN KEY (HospitalizationID)
        REFERENCES Hospitalizations(HospitalizationID),

    RoomID INT NOT NULL,
    CONSTRAINT FK_ExaminationRoom --probiha v
        FOREIGN KEY (RoomID)
        REFERENCES Rooms(RoomID)
);

CREATE TABLE PacientUsesMedicine (  --m:n relace, pacient pouziva leky
    PacientID INT NOT NULL,
    CONSTRAINT FK_PacientUsesMedicine_p --probiha v
        FOREIGN KEY (PacientID)
        REFERENCES Pacients(PacientID),

    MedicineID INT NOT NULL,
    CONSTRAINT FK_PacientUsesMedicine_m --probiha v
        FOREIGN KEY (MedicineID)
        REFERENCES Medicines(MedicineID)
);

--------------------------------naplneni tabulky hodnotami-----------------------------------------------

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
VALUES ('Janis','Pokora',TO_DATE('2001-03-30', 'yyyy/mm/dd'),2);

INSERT INTO Pacients (FirstName, LastName, Gender, Street, City, ZipCode, BirthDate, NurseID)
VALUES ('Jeans','Denim','nezname','Credit 2','England','11900',CURRENT_DATE,1);
INSERT INTO Pacients (FirstName, LastName, Gender, Street, City, ZipCode, BirthDate, NurseID)
VALUES ('Kalhoty','Latka','nezname','Kredit 1','Ingland','11100',CURRENT_DATE,2);

INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',2,1);
INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',3,1);
INSERT INTO Rooms (Equipment, NumberOfBeds, DepartmentID)
VALUES ('zidle',4,2);

INSERT INTO Medicines (Brand, Dosage, Composition, Usage)
VALUES ('Arginmax','2/H','maso','oralni podani');
INSERT INTO Medicines (Brand, Dosage, Composition, Usage)
VALUES ('Brufen','2/W','zelenina','injekce');

INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (CURRENT_TIMESTAMP, 1, 1, 1);
INSERT INTO Hospitalizations (StartTime, DoctorID, PacientID, RoomID)
VALUES (CURRENT_TIMESTAMP, 1, 2, 2);

INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola prostaty',CURRENT_TIMESTAMP,1,1,1,1);
INSERT INTO Examinations(Name, Description, StartTime, DoctorID, PacientID, HospitalizationID, RoomID)
VALUES ('Prostata','kontrola prostaty',CURRENT_TIMESTAMP,1,2,2,2);

INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (1,1);
INSERT INTO PacientUsesMedicine(PacientID, MedicineID)
VALUES (2,2);
