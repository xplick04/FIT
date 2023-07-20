# Implementační dokumentace k 1. úloze do IPP 2021/2022
* Jméno a příjmení: Maxim Plička
* Login: xplick04

### Použítí
* Program si bere vstupní data ze STDIN a vypisuje je na STDOUT.
* Program se spouští buď bez argumentu nebo s argumentem --help/--stats="file.txt".
* Pokud uživatel zvolí argument --stats má možnost za něj přidat následující argumenty
* --comments, --loc, --labels, --jumps, --fwjumps, --backjumps, --badjumps

### Zpracovávání argumentů
Argumenty jsou zpracovávány funkcí argumentCheck(), která kontroluje zda se ve
spouštecích argumentech vyskytuje help, stats nebo nic. Při zadání help se v argumentech nesmí vyskytovat nic jiného. 
V případě stats se za ním mohou ještě navíc nacházet argumenty vypsané v použití. Pokud nenastane ani jedna z výše uvedených
situací tak funkce vrací chybu 10.

### Zpracovávání dat z INPUTU
Data se berou po řádcích ze STDIN. Po ořezání případných řádků s komentářem a bílých znaků se zkontroluje, zda
se před první operací nachází povinná hlavička ".IPPcode22". V připadě, že tomu tak není se na výstup
vypíše chyba 21. Dále se zpracovávají operace tak, že se jejich operační kód převede do velkých písmen
a dále spadne do konkrétního případu. Tyto případy jsou rozděleny podle parametrů jednotlivých operací.
Pokud se operace nebude shodovat s žádným případem vypíše se chyba 22. V každém dalším případě se zkontroluje, 
zda má operace správný počet parametrů. U parametrů typu var, label, symb a type se navíc kontrolují argumenty pomocí funkcí isVar(), 
isSymb(), isLabel(), isType(). Každá z těchto funkcí obsahuje regulární výrazy, které 
kontrolují správnost formátu argumentu. V případě špatného formátu funkce vracejí chybu 23. U parametrů typu symb se
navíc ještě pomocí funkcí getSymbVal() a getSymbType() získá hodnota a typ symbolu.

### Výpis dat na OUTPUT
Po úspěšné kontrole hlavičky se na výstup vypíše hlavička v XML reprezentaci. Následně se stejným způsobem na výstup 
vypisují i ostatní operace. Pro lepší rozlišení dat jsou u jednotlivých řádků s instrukcemi použity 4 mezery. U 
argumentů operací je použito mezer 8. 

### Rozšíření STATP
Po výpisu dat na STDOUT se při zadání argumentu --stats spustí výpis zvolených dat do souboru
uvedeného za --stats. Následně se ověří, zda už tento soubor nebyl v tomto běhu programu otevřen,
pokud byl tak nastane chyba 12. V opačném případě se postupně zpracovávají zadané argumenty. Při výpočtu
operací je využito počítadlo $opCounter, které se inkrementuje při každé vypsané instrukci. Pro počítání komentářů
je před jejich vymazáním použito počítadlo $commentCount. Při počítání návěští se všechny postupně načtou do pole
$labelArray, kde se po přečtení celého vstupu spočítají všechny unikátní názvy návěští. Pro skoky je použit
stejný způsob přidávání do pole $jumpArray. Rozdíl oproti počítání návěští je v tom, že se počítají i neunikátní skoky.
Tyto skoky počítá počítadlo $jumpCount. Pro počítání zpětných skoků je v jednotlivých případech skoku udělána kontrola,
zda skok neskáče na již vytvořené návěští(je přidáno v $labelArray). V tomto případě se inkrementuje počítadlo $backJumpCount.
Pro výpočet špatných skoků se po zpracování vstupního souboru porovnají pole $labelArray a $jumpArray. V případě, že 
se návěští skoku nachází v poli $labelArray, inkrementuje se počítadlo $goodJumpCount. Po srovnání těchto dvou polí se jen odečtou
$goodJumpCount od $jumpCount, tím vznikne $badJumpCount. Při výpočtu dopředných skoků se už jen od $jumpCount odečte 
$badJumpCount a $backJumpcount.