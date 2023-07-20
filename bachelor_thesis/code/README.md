# Bakalářská práce
### Identifikace zvěře na základě biometrických informací
### Autor: Maxim Plička <xplick04@fit.vutbr.cz>

### Popis programu
Program ze vstupní fotografie pomocí navržené sekvence operací extrahuje markanty, 
které následně použije pro výpis na standardní výstup, nebo v případě zadání cesty
ke složce, která obsahuje fotografie nosů včetně korespondujících šablon, porovnává
šablony a zobrazí fotografii, která je nejpodobnější té vstupní. V popisku fotografie
se dále nachází její název a procentuální shodnost se vstupní fotografií.

Kromě samotného programu se ve složce s kódem nachází složka s moduly, které byly
použity pro tvorbu databáze šablon.

### Požadavky
'''python 3.10.11'''
'''opencv-python 4.7.0'''
'''numpy 1.24.1'''

### Použití programu
Program se spouští následovně:

.\python.exe main.py -i/--input "cesta" [-c/--cmpFile "cesta"]

kde

-i/--input je povinný argument, za který se dává cesta ke vstupní fotografii nosu
-c/--cmpFile je nepovinný argument, za který se dává cesta k databázi šablon

Následně se uživateli zobrazí okno s požadovanou fotografií. Na této fotografií je 
nutné anotovat šest bodů ohraničující nosní dírky. Pozice těchto 
bodů je definována následovně:

první bod: je nejvrchnějším bodem na LEVÉ nosní dírce
druhý bod: je nejspodnějším bodem na LEVÉ nosní dírce
třetí bod: je nejpravějším bodem na LEVÉ nosní dírce
čtvrtý bod: je nejvrchnějším bodem na PRAVÉ nosní dírce
pátý bod: je nejspodnějším bodem na PRAVÉ nosní dírce
šestý bod: je nejlevějším bodem na PRAVÉ nosní dírce

Označení bodu se provádí dvojklikem na pozici v obraze. Pokud je bod značený, 
vykreslí se kolem něj kružnice. V případě, že uživatel není s výběrem spokojen
stačí označit pomocí dvojkliku označit novou pozici. Poté je možné provést 
následující operace:

pomocí stisku klávesy "a" je možné označený bod přidat
pomocí stisku klávesy "d" je možné potvrdit označení všech bodů a pokračovat
pomocí stisku klávesy "r" je možné restartovat proces označování bodů
pomocí stisku klávesy "ESC" je možné ukončit program

Následně se provede extrakce markantů. V závislosti na zadaných argumentech se
provede jedna z následujících věcí. První možností je výpis nalezených markantů
ve formě šablony. Druhou možností je zobrazení fotografie, s kterou si byla vstupní
nejvíce podobná. Toto porovnávání se dělá na základě podobnosti šablon. V popisku 
fotografie se dále nachází její název a procentuální shodnost se vstupní fotografií.

