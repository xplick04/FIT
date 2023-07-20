# Implementační dokumentace k 2. úloze do IPP 2021/2022
* Jméno a příjmení: Maxim Plička
* Login: xplick04

### Použití
* Program si bere vstupní data ze zvolených souborů a vypisuje je na STDOUT.
* Program se spouští buď bez argumentu nebo s argumentem --help/--input="file.txt"/--source="file.txt".
* Uživatel musí zadat alespoN jeden z argumentů --input="file.txt"/--source="file.txt".

### Zpracovávání dat z INPUTU
XML data ze source jsou převedena do stromové struktury. Následně jsou jednotlivé prvky stromu postupně převáděny na
objekty instruction. Tyto obejkty jsou v případě jejich validity postupně vkládány do listu instructions.
Pokud má instrukce atribut opcode nastaven na label, tak se tato instrukce přidá do slovníku objektů labels.
Následně jsou instrukce v poli instructions seřazeny podle jejich atributu order, toto seřazení se provádí, kvůli
funkčnosti hlavního cyklu. Poté se inicializuje objekt frames třídy Frames, během toho se automaticky inicializuje 
prázdný globální rámec a prázdný zásobník lokálních rámců. Navíc se inicializuje instance třídy Frame, která v sobě má list vars určený 
pro proměnné v danném rámci. Následně hlavní cyklus postupně prochází již seřazené pole instructions a intepretuje jednotlivé instrukce.

### Interpretace dat na OUTPUT
K inteprataci instrukcí, které mohou pracovat s proměnnou se využívá objektu frames, kde se v případě proměnné dohledá 
její rámec, typ a hodnota. V případě práce s konstantou se pouze vrátí její typ a hodnota. Následně se s 
těmito hodnotami provede patřičná operace a výsledek se vratí do hlavního cyklu, kde se výsledek přiřadí dané proměnné.
Pokud instrukce nepracuje s žádnou proměnnou tak se intepretace provede přimo v hlavním cyklu(např. JUMP, CALL, atd...).
U instrukcí JUMPIFEQ a JUMPIFNEQ interpret prvně vyhodnotí výsledek operace a až následně provede skok na dané návěští.
Všechny skoky jsou prováděny nastavením proměnné instruction_counter na hodnotu podle názvu labelu. 

### Rozšíření NVI
Všechny třídy jsou použity hlavně za účelem lepší abstrakce a dekompozice problémů. 
#### Třída Program
Je využita pro kontrolu argumentů a její atributy obsahují vstupy zvolené uživatelem.
#### Třída Argument
Reprezentuje argumenty instrukce. Má atributy type a value. Metoda get_var_name() slouží k získání typu a názvu proměnné.
Metoda get_var_frame() vrací specifický rámec, v kterém se proměnná vyskytuje.
#### Třída Variable
Reprezentuje proměnné. Mezi její atributy patří name, value a type. V třídě je reimplementována metoda pro porovnání, která
porovná dvě jména proměnných a vrátí hodnotu true nebo false. Metoda update() slouží k aktualizaci typu a hodnoty proměnné. 
#### Třída Instruction
Reprezentuje instrukce z xml souboru. Při inicializaci se u instrukce zkontroluje správný formát a přiradí se
ji argumenty arg1, arg2 a arg3. Dále se z kontrolních důvodů přiřadí počet argumentů arg_count. Třída má matody check_for_duplicates
a check_num_of_args(). Tyto metody jsou pouze pomocné pro ověření správnosti instrukce.
#### Třída Labels
Reprezentuje slovník labelů, na které se může skákat.
Metoda add() se používá k přidání labelu do slovníku a zároveň ověřuje, zda se tam název labelu už
nevyskytuje. Metoda get_position(), která se využíva pro skoky, zjistí zda název labelu existuje a vrátí hodnotu indexu 
label_name v poli instrukcí.
#### Třída Frame
Reprezentuje rámce a má atribut vars, který obashuje proměnné v daném rámci. Metoda add() zkontroluje existenci
názvu proměnné v daném rámci a následně ji do rámce přidá. Metoda get_value() a get_type() vrací hodnotu a typ proměnné 
podle jejího jména. Metoda update_var() kontroluje validitu typu proměnné a následně aktualizuje její typ a hodnotu.
#### Třída Frames
Reprezentuje všechny rámce a operace nad nimi. Třida má atribut GF, LF a TF. GF reprezentuje globální rámec, LF zásobník
lokálních rámců a TF dočasný rámec. V této třídě je implementován výpočet většiny instrukcí. Metoda defvar() přidá var_name
do daného rámce. Metoda create_frame() vytvoří dočasný rámec. Metoda push_frame() vloží TF na zásobník LF a nastaví TF na None.
Metoda pop_frame() vytáhne z vrcholu zásobníku LF rámec a vloží jej do TF. Metody  get_symb_value() a get_symb_type() 
vrací hodnotu a typ proměnné podle rámce, v kterém se nachází. Metoda update_var() aktualizuje hodnotu a typ opět podle rámce.
Metoda get_result() si vezme argumenty podle atributu arg_count a následně vrací výsledky jednotlivých operací.


