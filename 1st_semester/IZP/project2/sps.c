#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
	char *text;
	int delka;
	int sloupec;
	int radek;
	int print;
}bunka_t;


void load_function(char buffer[],bunka_t bunka[],int bunka_id, int minule_pismeno, int sloupec,char delim[100]){
	bunka[bunka_id].text = malloc(bunka[bunka_id].delka);
	bunka[bunka_id].print = 1;
	int j = minule_pismeno + 1;
	if(sloupec == 1){
		j = minule_pismeno;
	}
	int i = 0;
	if(buffer[j]=='\n'){
		j++;
	}
	while(j < bunka[bunka_id].delka + minule_pismeno){
		if(buffer[j] == delim[0]){
			j++;
		}
		bunka[bunka_id].text[i] = buffer[j];
		i++;
		j++;
	}
}

void print_function(bunka_t bunka[], int bunka_id,int nejdelsi_sloupec, char delim[100], int nejdelsi_radek){
	int pocet_print = 1;
	int bunka_print = 1;
	int radek = 1;
	int sloupec = 1;
			while(bunka_print < bunka_id){//vypisuje podle radku a sloupce
				if(bunka[bunka_print].radek == radek){
					if(bunka[bunka_print].sloupec == sloupec){
						if(bunka[bunka_print].print==1){
							printf("%s",bunka[bunka_print].text);
						}
						sloupec++;
						pocet_print++;
						if(bunka[bunka_print].sloupec < nejdelsi_sloupec && bunka[bunka_print].print==1){
							printf("%c",delim[0]);//pokud nejsem na konci radku vypise delim
						}
					}
				}
				if(bunka[bunka_print].sloupec == nejdelsi_sloupec && radek != nejdelsi_radek){
					if(bunka[bunka_print].print == 1){
						printf("\n");//vypise \n pokud jsem na konci radku
					}
					sloupec = 1;
					radek++;
				}
				if(bunka_print == bunka_id && pocet_print != bunka_id){
					bunka_print = 1;//pro dcol a drow
				}
				bunka_print++;
			}
}
void min_function(bunka_t bunka[],int bunka_id,int *selekce_radek,int *selekce_sloupec,int *selekce_radek_from,int *selekce_radek_to,int *selekce_sloupec_from,int *selekce_sloupec_to){
	int i = 1;
	int j = 0;
	int cislo_kontrola = 0;
	char nejmensi_cislo_tmp[1000];//uklada string z bunky
	int nejmensi_cislo_potencional = 0;
	int nejmensi_cislo_porovnani;
	bool flag = true;
	int delka = 0;
	while(i < bunka_id){
		if(bunka[i].radek == *selekce_radek || (bunka[i].radek >= *selekce_radek_from && bunka[i].radek <= *selekce_radek_to)){
			if(bunka[i].sloupec == *selekce_sloupec || (bunka[i].sloupec >= *selekce_sloupec_from && bunka[i].sloupec <= *selekce_sloupec_to)){
				strcpy(nejmensi_cislo_tmp, bunka[i].text);
				delka = strlen(nejmensi_cislo_tmp);
				while(isdigit(nejmensi_cislo_tmp[j])){
					cislo_kontrola++;
					j++;
				}
				if(cislo_kontrola == delka){
					nejmensi_cislo_porovnani = atoi(nejmensi_cislo_tmp);
					if(flag == true){//ulozil cislo poprve,co nejake najde
						nejmensi_cislo_potencional = nejmensi_cislo_porovnani;
						*selekce_sloupec = bunka[i].sloupec;
						*selekce_radek = bunka[i].radek;
						flag = false;
					}
					if(nejmensi_cislo_porovnani < nejmensi_cislo_potencional){//srovnava s prvnim cislem
						nejmensi_cislo_potencional = nejmensi_cislo_porovnani;
						*selekce_sloupec = bunka[i].sloupec;
						*selekce_radek = bunka[i].radek;
					}
				}
			}
		}
			i++;
			memset(nejmensi_cislo_tmp, 0, 1000);
	}
	if(*selekce_radek != 0 && *selekce_sloupec != 0){//pokud najde nejake nejmensi cislo vymaze selekci
		*selekce_radek_from = 0;
		*selekce_radek_to = 0;
		*selekce_sloupec_from = 0;
		*selekce_sloupec_to = 0;
	}
}

void max_function(bunka_t bunka[],int bunka_id,int *selekce_radek,int *selekce_sloupec,int *selekce_radek_from,int *selekce_radek_to,int *selekce_sloupec_from,int *selekce_sloupec_to){
	int i = 1;
	int j = 0;
	int cislo_kontrola = 0;//kotroluje, jestli jsem nacetl cislo
	char nejvetsi_cislo_tmp[1000];
	int nejvetsi_cislo_potencional = 0;
	int nejvetsi_cislo_porovnani;
	bool flag = true;
	int delka = 0;
	while(i < bunka_id){
		if(bunka[i].radek == *selekce_radek || (bunka[i].radek >= *selekce_radek_from && bunka[i].radek <= *selekce_radek_to)){
			if(bunka[i].sloupec == *selekce_sloupec || (bunka[i].sloupec >= *selekce_sloupec_from && bunka[i].sloupec <= *selekce_sloupec_to)){
				strcpy(nejvetsi_cislo_tmp, bunka[i].text);
				delka = strlen(nejvetsi_cislo_tmp);
				while(isdigit(nejvetsi_cislo_tmp[j])){
					cislo_kontrola++;
					j++;
				}
				if(cislo_kontrola == delka){
					nejvetsi_cislo_porovnani = atoi(nejvetsi_cislo_tmp);
					if(flag == true){//nacte prvni int v tabulce
						nejvetsi_cislo_potencional = nejvetsi_cislo_porovnani;
						*selekce_sloupec = bunka[i].sloupec;
						*selekce_radek = bunka[i].radek;
						flag = false;
					}
					if(nejvetsi_cislo_porovnani > nejvetsi_cislo_potencional){//srovnava s prvnim cislem
						nejvetsi_cislo_potencional = nejvetsi_cislo_porovnani;
						*selekce_sloupec = bunka[i].sloupec;
						*selekce_radek = bunka[i].radek;
					}
				}
			}
		}
			i++;
			memset(nejvetsi_cislo_tmp, 0, 1000);
	}
	if(*selekce_radek != 0 && *selekce_sloupec != 0){//pokud najde nejake nejmensi cislo vymaze selekci
		*selekce_radek_from = 0;
		*selekce_radek_to = 0;
		*selekce_sloupec_from = 0;
		*selekce_sloupec_to = 0;
	}
}

void drow_function(bunka_t bunka[], int bunka_id, int selekce_radek, int selekce_radek_from, int selekce_radek_to, int *nejdelsi_radek){
		if(selekce_radek_from == 0 && selekce_radek_to == 0){
		for(int i = 0;i <= bunka_id ; i++){//
			if(bunka[i].radek == selekce_radek){
				bunka[i].print = 0;
			}
		}
	}
	else{
		for(int i = 0;i <= bunka_id; i++){
			if(bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to){
				bunka[i].print = 0;
			}
		}
	}
	if(selekce_radek == *nejdelsi_radek || selekce_radek_to == *nejdelsi_radek){
		*nejdelsi_radek -= 1;//zmeni hodnotu nejdelsiho radku, pokud vymazu posledni radek
	}
}

void dcol_function(bunka_t bunka[], int bunka_id, int selekce_sloupec, int selekce_sloupec_from, int selekce_sloupec_to,int *nejdelsi_sloupec){
		if(selekce_sloupec_from == 0 && selekce_sloupec_to == 0){
		for(int i = 1;i <= bunka_id; i++){
			if(bunka[i].sloupec == selekce_sloupec){
				bunka[i].print = 0;
			}
		}
	}
	else{
		for(int i = 1;i <= bunka_id; i++){
			if(bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to){
				bunka[i].print = 0;
			}
		}
	}
	if(selekce_sloupec == *nejdelsi_sloupec || selekce_sloupec_to == *nejdelsi_sloupec){
		*nejdelsi_sloupec -= 1;
	}
}
void set_function(char set_text[],bunka_t bunka[],int bunka_id,int selekce_sloupec,int selekce_sloupec_from,int selekce_sloupec_to,int selekce_radek,int selekce_radek_from,int selekce_radek_to){
int i = 1;
int j = 0;
int set_lenght = strlen(set_text);
	while(i < bunka_id){
		if(bunka[i].radek == selekce_radek || (bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to)){
			if(bunka[i].sloupec == selekce_sloupec || (bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to)){
				bunka[i].delka = set_lenght;
				free(bunka[i].text);
				bunka[i].text = malloc(set_lenght);
				while(j < bunka[i].delka){
					bunka[i].text[j] = set_text[j];
					j++;
				}
				j = 0;
			}
		}
		i++;
	}
}
void find_function(char find_text[],bunka_t bunka[],int bunka_id,int *selekce_sloupec,int *selekce_sloupec_from,int *selekce_sloupec_to,int *selekce_radek,int *selekce_radek_from,int *selekce_radek_to){
int i = 1;
bool flag = false;
	while(i < bunka_id){
		if(bunka[i].radek == *selekce_radek || (bunka[i].radek >= *selekce_radek_from && bunka[i].radek <= *selekce_radek_to)){
			if(bunka[i].sloupec == *selekce_sloupec || (bunka[i].sloupec >= *selekce_sloupec_from && bunka[i].sloupec <= *selekce_sloupec_to)){
				if(!strcmp(bunka[i].text,find_text) && flag == false){//pokud najde slovo, stane se jen jednou
					*selekce_sloupec = bunka[i].sloupec;
					*selekce_radek = bunka[i].radek;
					flag = true;
				}
			}
		}
		i++;
	}
	if(flag == true && *selekce_radek != 0){//vymaze selekci, pokud naslo
		*selekce_radek_from = 0;
		*selekce_radek_to = 0;
		*selekce_sloupec_from = 0;
		*selekce_sloupec_to = 0;
	}
}
void clear_function(bunka_t bunka[],int bunka_id,int selekce_sloupec,int selekce_sloupec_from,int selekce_sloupec_to,int selekce_radek,int selekce_radek_from,int selekce_radek_to){
int i = 1;

	while(i < bunka_id){//vypisuje podle radku a sloupce
		if(bunka[i].radek == selekce_radek || (bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to)){
			if(bunka[i].sloupec == selekce_sloupec || (bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to)){
				bunka[i].delka = 1;
				free(bunka[i].text);
				bunka[i].text = malloc(1);
			}
		}
		i++;
	}
}


void swap_function(char swap_radek[],char swap_sloupec[],bunka_t bunka[],int bunka_id,int selekce_sloupec,int selekce_radek){
	int swap_r = atoi(swap_radek);
	int swap_s = atoi(swap_sloupec);
	int i = 1;
	char bunka_swap_1[1000];
	char bunka_swap_2[1000];
	int swap_1_lenght;
	int swap_2_lenght;
	while(i < bunka_id){
		if(bunka[i].radek == selekce_radek && bunka[i].sloupec == selekce_sloupec){
			strcpy(bunka_swap_1,bunka[i].text);
		}
		i++;
	}
	i = 1;
	swap_1_lenght = strlen(bunka_swap_1);
	while(i <= bunka_id){
		if(bunka[i].radek == swap_r && bunka[i].sloupec == swap_s){
			swap_2_lenght = strlen(bunka[i].text);
			strcpy(bunka_swap_2,bunka[i].text);
			free(bunka[i].text);
			bunka[i].text = malloc(swap_1_lenght);
			strcpy(bunka[i].text,bunka_swap_1);
		}
		i++;
	}
	i = 1;
	while(i < bunka_id){
		if(bunka[i].radek == selekce_radek && bunka[i].sloupec == selekce_sloupec){
			free(bunka[i].text);
			bunka[i].text = malloc(swap_2_lenght);
			strcpy(bunka[i].text,bunka_swap_2);
		}
		i++;
	}
}

void len_function(char len_radek[],char len_sloupec[],bunka_t bunka[],int bunka_id,int selekce_sloupec,int selekce_radek){
	int len_r = atoi(len_radek);
	int len_s = atoi(len_sloupec);
	int i = 1;
	int len_lenght;
	int pozice_cisla = 0;
	while(i < bunka_id){
		if(bunka[i].radek == selekce_radek && bunka[i].sloupec == selekce_sloupec){
			len_lenght = strlen(bunka[i].text);//ulozi mi delku retezce
		}
		i++;
	}
	int n = len_lenght;
	int rem = 0;
	while(n != 0){//zmeri mi kolik bude potreba pozic
		n /= 10;
		pozice_cisla++;
	}
	char len_text[pozice_cisla];
	n = len_lenght;

	for(int y = 0; y < pozice_cisla; y++){//prevod cisla na array
		rem = n%10;
		n /= 10;
		len_text[pozice_cisla - (y + 1)] = rem + '0';
	}
	len_text[pozice_cisla] = '\0';
	i = 1;
	if(len_lenght==0){
		len_text[0] = '0';
	}
	while(i <= bunka_id){
		if(bunka[i].radek == len_r && bunka[i].sloupec == len_s){
			free(bunka[i].text);
			bunka[i].delka = pozice_cisla;
			bunka[i].text = malloc(pozice_cisla);
			strcpy(bunka[i].text, len_text);
		}
		i++;
	}
}
void sum_function(char sum_radek[], char sum_sloupec[], bunka_t bunka[], int bunka_id, int selekce_sloupec, int selekce_radek,int selekce_radek_from,int selekce_radek_to,int selekce_sloupec_from,int selekce_sloupec_to){
	int sum_num;
	int sum = 0;
	int delka = 0;
	int cislo_kontrola = 0;
	int i = 1;
	int sum_r = atoi(sum_radek);
	int sum_s = atoi(sum_sloupec);

	while(i < bunka_id){//vypisuje podle radku a sloupce
		if(bunka[i].radek == selekce_radek || (bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to)){
			if(bunka[i].sloupec == selekce_sloupec || (bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to)){
				delka = strlen(bunka[i].text);//kontrola, jesli je v bunce cislo
				for(int k = 0; k <= delka;){
					if(isdigit(bunka[i].text[k])){
						cislo_kontrola++;
					}
					k++;
				}
				if(cislo_kontrola == delka){
					sum_num = atoi(bunka[i].text);
					sum += sum_num;
				}
				cislo_kontrola = 0;
			}
		}
		i++;
	}
	int pozice_cisla = 0;
	int n = sum;
	int rem = 0;
	while(n != 0){//zmeri mi kolik bude potreba pozic
		n /= 10;
		pozice_cisla++;
	}
	char sum_text[pozice_cisla];
	n = sum;

	for(int y = 0; y < pozice_cisla; y++){//prevod cisla na array
		rem = n%10;
		n /= 10;
		sum_text[pozice_cisla - (y + 1)] = rem + '0';
	}
	sum_text[pozice_cisla] = '\0';
	if(sum == 0){
		sum_text[0] = '0';
	}
	i = 1;
	while(i <= bunka_id){
		if(bunka[i].radek == sum_r && bunka[i].sloupec == sum_s){
			free(bunka[i].text);
			bunka[i].delka = pozice_cisla;
			bunka[i].text = malloc(pozice_cisla);
			strcpy(bunka[i].text, sum_text);
		}
		i++;
	}
}
void avg_function(char avg_radek[], char avg_sloupec[], bunka_t bunka[], int bunka_id, int selekce_sloupec, int selekce_radek,int selekce_radek_from,int selekce_radek_to,int selekce_sloupec_from,int selekce_sloupec_to){
	int sum_num;
	int sum = 0;
	int delka = 0;
	int cislo_kontrola = 0;
	int i = 1;
	int avg_r = atoi(avg_radek);
	int avg_s = atoi(avg_sloupec);
	int avg_delitel = 0;
	int avg;
	while(i < bunka_id){//vypisuje podle radku a sloupce
		if(bunka[i].radek == selekce_radek || (bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to)){
			if(bunka[i].sloupec == selekce_sloupec || (bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to)){
				delka = strlen(bunka[i].text);//kontrola, jesli je v bunce cislo
				for(int k = 0; k <= delka;){
					if(isdigit(bunka[i].text[k])){
						cislo_kontrola++;
					}
					k++;
				}
				if(cislo_kontrola == delka){
					sum_num = atoi(bunka[i].text);
					sum += sum_num;
					if(sum_num != 0){
						avg_delitel++;
					}
				}
				cislo_kontrola = 0;
			}
		}
		i++;
	}
	avg = sum/avg_delitel;
	int pozice_cisla = 0;
	int n = avg;
	int rem = 0;
	while(n != 0){//zmeri mi kolik bude potreba pozic
		n /= 10;
		pozice_cisla++;
	}
	char avg_text[pozice_cisla];
	n = avg;

	for(int y = 0; y < pozice_cisla; y++){//prevod cisla na array
		rem = n%10;
		n /= 10;
		avg_text[pozice_cisla - (y + 1)] = rem + '0';
	}
	avg_text[pozice_cisla] = '\0';
	i = 1;
	while(i < bunka_id){
		if(bunka[i].radek == avg_r && bunka[i].sloupec == avg_s){
			free(bunka[i].text);
			bunka[i].delka = pozice_cisla;
			bunka[i].text = malloc(pozice_cisla);
			strcpy(bunka[i].text, avg_text);
		}
		i++;
	}
}

void count_function(char count_radek[],char count_sloupec[],bunka_t bunka[],int bunka_id,int selekce_sloupec,int selekce_radek, int selekce_radek_from, int selekce_radek_to, int selekce_sloupec_from, int selekce_sloupec_to){
	int count_r = atoi(count_radek);
	int count_s = atoi(count_sloupec);
	int i = 1;
	int delka;
	int pocet_nepraznych_bunek = 0;
	while(i < bunka_id){//vypisuje podle radku a sloupce
		if(bunka[i].radek == selekce_radek || (bunka[i].radek >= selekce_radek_from && bunka[i].radek <= selekce_radek_to)){
			if(bunka[i].sloupec == selekce_sloupec || (bunka[i].sloupec >= selekce_sloupec_from && bunka[i].sloupec <= selekce_sloupec_to)){
				delka = strlen(bunka[i].text);
				if(delka != 0){//kontrola jestli je bunka neprazdna
					pocet_nepraznych_bunek++;
				}
			}
		}
		i++;
	}
	int pozice_cisla = 0;
	int n = pocet_nepraznych_bunek;
	int rem = 0;
	while(n != 0){//zmeri mi kolik bude potreba pozic
		n /= 10;
		pozice_cisla++;
	}
	char count_text[pozice_cisla];
	n = pocet_nepraznych_bunek;
	for(int y = 0; y < pozice_cisla; y++){
		rem = n%10;
		n /= 10;
		count_text[pozice_cisla - (y + 1)] = rem + '0';
	}
	count_text[pozice_cisla] = '\0';
	if(pocet_nepraznych_bunek == 0){
		count_text[0] = '0';
	}
	i = 1;
	while(i < bunka_id){
		if(bunka[i].radek == count_r && bunka[i].sloupec == count_s){
			free(bunka[i].text);
			bunka[i].delka = pozice_cisla;
			bunka[i].text = malloc(pozice_cisla);
			strcpy(bunka[i].text, count_text);
		}
		i++;
	}
}

int isdelim(char pismeno,char delim[100], int delka_delim){//funkce pro kontrolu delimetru
	int x = 0;
	int i = 0;
	while(i <= delka_delim){
		if(pismeno == delim[i]){
			x = 1;
		}
		i++;
	}
return x;//pokud je delim, x = 1
}

int get_input_lenght_function(FILE *fp){//funkce pro zjisteni delku vstupniho souboru
	int lenght;
	fseek(fp, 0L, SEEK_END);//najde posledni char v retezci
	lenght = ftell(fp);
	fseek(fp, 0L, SEEK_SET);//najde prvni char v retezci
	return lenght;
}

int main(int argc, char *argv[]){

	int bunka_id = 1;//identifikace bunek
	int sloupec = 1;
	int radek = 1;
	int nejdelsi_sloupec = 1;
	int nejdelsi_radek = 1;
	char delim[100];
	int arg_pozice;
	char *token;
	char vstup[50];
	char selekce_sloupec_tmp[100];
	char selekce_sloupec_tmp2[100];
	char selekce_radek_tmp[100];
	char selekce_radek_tmp2[100];
	int selekce_radek = 1;
	int selekce_sloupec = 1;
	int selekce_radek_from = 0;
	int selekce_radek_to = 0;
	int selekce_sloupec_from = 0;
	int selekce_sloupec_to = 0;
	int input_lenght;


	if(argc >= 3){//zjisteni delimteru
		if(!strcmp(argv[1],"-d")){
			strcpy(delim,argv[2]);
			arg_pozice = 3;
		}
		else{
			strcpy(delim," ");
			arg_pozice = 1;
		}
	}
	else{
		fprintf(stderr, "malo argumentu\n");
		return 1;
	}
	strcpy(vstup,argv[arg_pozice+1]);//zjisti vstupni tabulku
	int delka_delim = strlen(delim);


 	FILE *fp;//nacitani souboru do bufferu
 	fp = fopen(vstup,"r");
	if(fp == NULL){
		fprintf(stderr, "spatne nactena tabulka\n");
		return 1;
	}
	input_lenght = get_input_lenght_function(fp);
	if(input_lenght == 0){//pokud se pocitani zastavi na prvnim pismenu, tak je tabulk
		fprintf(stderr, "prazdna tabulka\n");
		return 1;
	}
	char buffer[input_lenght];
	int i = 0;
	buffer[i] = fgetc(fp);
	//funkce pro nacteni tabulky do arraye
	while(buffer[i] != EOF){
		if(isdelim(buffer[i], delim, delka_delim)){
		sloupec++;
			if(sloupec > nejdelsi_sloupec){
				nejdelsi_sloupec = sloupec;
			}
		}
		if(buffer[i] == '\n'){
			radek++;
			if(radek > nejdelsi_radek){
				nejdelsi_radek = radek - 1;
			}
			sloupec = 1;
		}
		i++;
		buffer[i] = fgetc(fp);
	}
	buffer[i-1] = '\n';//nahradi posledni \n
	buffer[i] = '\0';//nahradi posledni \n
	int k = 0;
	i = 0;
	sloupec = 1;
	radek = 1;
	int doplneni_velikosti = input_lenght + ((radek-1)*nejdelsi_sloupec);//velikost tabulky kdyby byla neuplna
	char buffer2[doplneni_velikosti];

	buffer2[i] = buffer[k];//funcke pro doplneni tabulky na zakladni tvar
	while(buffer2[i]!='\0'){
		if(isdelim(buffer2[i], delim, delka_delim)){
			buffer2[i] = delim[0];
			sloupec++;
		}
		if(buffer2[i] == '\n'){
			while(sloupec < nejdelsi_sloupec){
					buffer2[i] = delim[0];
					i++;
					sloupec++;
			}
			buffer2[i] = '\n';
			radek++;
			sloupec = 1;
		}
		i++;
		k++;
		buffer2[i] = buffer[k];
	}
	buffer2[i] = '\0';
	memset(buffer, 0, input_lenght);
	int pocet_bunek;//pro pocet bunek
	pocet_bunek = radek * nejdelsi_sloupec;

	bunka_t bunka[pocet_bunek];//bunek je tolik kolik je ve vstupu radku*sloupcu
	int j = 0;
	sloupec = 1;
	radek = 1;
	i = 0;
	while(buffer2[i] != '\0'){
			if(isdelim(buffer2[i], delim, delka_delim) || buffer2[i] == '\n'){
				bunka[bunka_id].sloupec = sloupec;
				bunka[bunka_id].radek = radek;
				bunka[bunka_id].delka = i - j;
				load_function(buffer2, bunka, bunka_id, j, sloupec, delim);
				j = i;
				bunka_id++;
				sloupec++;
			}
			if(buffer2[i] == '\n'){
				sloupec = 1;
				radek++;
			}
		i++;
	}

	int token_i = 0;
	int funkce_i = 0;
	int pocet_prikazu = 1;
	char find_check[5];
	int token_size;

	token = strtok(argv[arg_pozice],";");//zpracovavani argumentu s prikazy
	while(token != NULL ){
		int j = 0;
		int i = 0;
		if(token[i]=='['){//selekce
			i++;
			while(token[i] != ','){
				if(token[i] == '-'){
					fprintf(stderr, "nelze zadat zaporne cislo\n");
					return 1;
				}
				if(isdigit(token[i])){
					selekce_radek_tmp[j] = token[i];
				}
				if(token[i] == '_'){
					selekce_radek_from = 1;
					selekce_radek_to = nejdelsi_radek;//posledni radek(maximalni radek)
					selekce_radek = 0;
					i++;
					break;
				}
				else if(token[i] != '_' && !isdigit(token[i])){//break v pripade ze prijde min nebo max
					break;
				}
				i++;
				j++;
			}
			i++;//pozice po carce
			selekce_radek_tmp[j] = '\0';
			j = 0;
			while(token[i] != ']'){
				if(token[i] == '-'){
					fprintf(stderr, "nelze zadat zaporne cislo\n");
					return 1;
				}
				if(isdigit(token[i])){
					selekce_sloupec_tmp[j] = token[i];
				}
				if(token[i] == '_'){
					selekce_sloupec_from = 1;
					selekce_sloupec_to = nejdelsi_sloupec;//posledni radek(maximalni radek)
					selekce_sloupec = 0;
					i++;
					break;
				}
				i++;
				j++;
				if(token[i] == ','){
					selekce_sloupec_tmp[j] = '\0';
					j = 0;
					i++;
					while(token[i] != ','){
						if(isdigit(token[i])){
							selekce_radek_tmp2[j] = token[i];
						}
						if(token[i] == '-'){
							selekce_radek_tmp2[j] = token[i];
							if(isdigit(token[i+1])){
								fprintf(stderr, "nelze zadat zaporne cislo\n");
								return 1;
							}
						}
						j++;
						i++;
					}
					j = 0;
					i++;
					while(token[i] != ']'){
						if(isdigit(token[i])){
							selekce_sloupec_tmp2[j] = token[i];
						}
						if(token[i] == '-'){
							selekce_sloupec_tmp2[j] = token[i];
							if(isdigit(token[i])){
								fprintf(stderr, "nelze zadat zaporne cislo\n");
								return 1;
							}
						}
						j++;
						i++;
					}
				}
			}
			if(isdigit(selekce_radek_tmp[0])){//pokud je zadana selekce pro radek
				selekce_radek = atoi(selekce_radek_tmp);
				if(isdigit(selekce_radek_tmp2[0])){//pokud je zadan selekce pro okno(radek_to)
					selekce_radek_from = selekce_radek;
					selekce_radek_to = atoi(selekce_radek_tmp2);
					selekce_radek = 0;
					if(selekce_radek_to < selekce_radek_from){
						fprintf(stderr,"spatne zadany parametr pro selekci\n");
						return 1;
					}
					memset(selekce_radek_tmp2, 0, 100);
				}
				if(selekce_radek_tmp2[0] == '-'){
					selekce_radek_from = selekce_radek;
					selekce_radek_to = nejdelsi_radek;
					selekce_radek = 0;
					if(selekce_radek_to < selekce_radek_from){
						fprintf(stderr,"spatne zadany parametr pro selekci\n");
						return 1;
					}
					memset(selekce_radek_tmp2, 0, 100);
				}
			memset(selekce_radek_tmp, 0, 100);
			}
			if(isdigit(selekce_sloupec_tmp[0])){//pokud je zadana selekce pro sloupec
				selekce_sloupec = atoi(selekce_sloupec_tmp);
				if(isdigit(selekce_sloupec_tmp2[0])){//pokud je zadana selekce pro okno(sloupec_to)
					selekce_sloupec_from = selekce_sloupec;
					selekce_sloupec_to = atoi(selekce_sloupec_tmp2);
					selekce_sloupec = 0;
					if(selekce_sloupec_to < selekce_sloupec_from){
						fprintf(stderr,"spatne zadany parametr pro selekci\n");
						return 1;
					}
					memset(selekce_sloupec_tmp2, 0, 100);
				}
				if(selekce_sloupec_tmp2[0] == '-'){
					selekce_sloupec_from = selekce_sloupec;
					selekce_sloupec_to = nejdelsi_sloupec;
					selekce_sloupec = 0;
					if(selekce_sloupec_to < selekce_sloupec_from){
						fprintf(stderr,"spatne zadany parametr pro selekci\n");
						return 1;
					}
					memset(selekce_sloupec_tmp2, 0, 100);
				}
			memset(selekce_sloupec_tmp, 0, 100);
			}
			if(selekce_radek != 0 && selekce_sloupec != 0){//kdyz je selekce na bunku smaze selekci na interval
				selekce_radek_from = 0;
				selekce_radek_to = 0;
				selekce_sloupec_from = 0;
				selekce_sloupec_to = 0;
			}
		}
		if(!strcmp(token,"[min]")){
			min_function(bunka, bunka_id, &selekce_radek, &selekce_sloupec, &selekce_radek_from, &selekce_radek_to, &selekce_sloupec_from, &selekce_sloupec_to);
		}
		if(!strcmp(token,"[max]")){
			max_function(bunka, bunka_id, &selekce_radek, &selekce_sloupec, &selekce_radek_from, &selekce_radek_to, &selekce_sloupec_from, &selekce_sloupec_to);
		}
		if(!strcmp(token,"drow")){
			drow_function(bunka, bunka_id, selekce_radek, selekce_radek_from, selekce_radek_to, &nejdelsi_radek);
		}
		if(!strcmp(token,"arow")){
			printf("arow\n");
		}
		if(!strcmp(token,"irow")){
			printf("irow\n");
		}
		if(!strcmp(token,"dcol")){
			dcol_function(bunka, bunka_id, selekce_sloupec, selekce_sloupec_from, selekce_sloupec_to, &nejdelsi_sloupec);
		}
		if(!strcmp(token,"acol")){
			printf("acol\n");
		}
		if(!strcmp(token,"icol")){
			printf("icol\n");
		}
		if(token[0] == 's' && token[1] == 'e' && token[2] == 't') {//set str
					token_i = 4;
					char set_text[strlen(token)-3];
					while(token[token_i-1] != '\0'){
						set_text[funkce_i] = token[token_i];
						token_i++;
						funkce_i++;
					}
					set_function(set_text, bunka, bunka_id, selekce_sloupec, selekce_sloupec_from, selekce_sloupec_to,selekce_radek, selekce_radek_from, selekce_radek_to);
					token_i = 0;
					funkce_i = 0;
		}
		if(token[0] == 's' && token[1] == 'w' && token[2] == 'a' && token[3] == 'p'){
					token_i = 6;
					char swap_radek[strlen(token)-4];
					char swap_sloupec[strlen(token)-4];
					while(token[token_i] != ','){
						swap_radek[funkce_i] = token[token_i];
						token_i++;
						funkce_i++;
					}
					token_i++;//carka
					funkce_i = 0;
					while(token[token_i] != ']'){
						swap_sloupec[funkce_i] = token[token_i];
						token_i++;
						funkce_i++;
					}
					swap_sloupec[funkce_i] = '\0';
					token_i = 0;
					funkce_i = 0;
					if(selekce_radek_to != 0 || selekce_sloupec_to != 0){
						fprintf(stderr, "u swapu muze byt vybrana pouze jedna bunka\n");
						return 1;
					}
					swap_function(swap_radek,swap_sloupec,bunka, bunka_id, selekce_sloupec,selekce_radek);
		}
		if(token[0] == 'l' && token[1] == 'e' && token[2] == 'n'){
			token_i = 5;
			char len_radek[strlen(token)-3];
			char len_sloupec[strlen(token)-3];
			while(token[token_i] != ','){
				len_radek[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			token_i++;//carka
			funkce_i = 0;
			while(token[token_i] != ']'){
				len_sloupec[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			len_sloupec[funkce_i] = '\0';
			token_i = 0;
			funkce_i = 0;
			if(selekce_radek_to != 0 || selekce_sloupec_to != 0){
				fprintf(stderr, "u lenu muze byt vybrana pouze jedna bunka\n");
				return 1;
			}
			len_function(len_radek,len_sloupec,bunka, bunka_id, selekce_sloupec,selekce_radek);
		}
		if(token[0] == 's' && token[1] == 'u' && token[2] == 'm'){
			token_i = 5;
			char sum_radek[strlen(token)-3];
			char sum_sloupec[strlen(token)-3];
			while(token[token_i] != ','){
				sum_radek[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			token_i++;//carka
			funkce_i = 0;
			while(token[token_i] != ']'){
				sum_sloupec[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			sum_sloupec[funkce_i] = '\0';
			token_i = 0;
			funkce_i = 0;
			sum_function(sum_radek, sum_sloupec, bunka, bunka_id, selekce_sloupec, selekce_radek, selekce_radek_from, selekce_radek_to, selekce_sloupec_from, selekce_sloupec_to);
		}
		if(token[0] == 'a' && token[1] == 'v' && token[2] == 'g'){
			token_i = 5;
			char avg_radek[strlen(token)-3];
			char avg_sloupec[strlen(token)-3];
			while(token[token_i] != ','){
				avg_radek[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			token_i++;//carka
			funkce_i = 0;
			while(token[token_i] != ']'){
				avg_sloupec[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			avg_sloupec[funkce_i] = '\0';
			token_i = 0;
			funkce_i = 0;
			avg_function(avg_radek, avg_sloupec, bunka, bunka_id, selekce_sloupec, selekce_radek, selekce_radek_from, selekce_radek_to, selekce_sloupec_from, selekce_sloupec_to);
		}
		if(token[0] == 'c' && token[1] == 'o' && token[2] == 'u' && token[3] == 'n' && token[4] == 't'){
			token_i = 7;
			char count_radek[strlen(token)-7];
			char count_sloupec[strlen(token)-7];
			while(token[token_i] != ','){
				count_radek[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			token_i++;//carka
			funkce_i = 0;
			while(token[token_i] != ']'){
				count_sloupec[funkce_i] = token[token_i];
				token_i++;
				funkce_i++;
			}
			count_sloupec[funkce_i] = '\0';
			token_i = 0;
			funkce_i = 0;
			count_function(count_radek, count_sloupec, bunka, bunka_id, selekce_sloupec, selekce_radek, selekce_radek_from, selekce_radek_to, selekce_sloupec_from, selekce_sloupec_to);
		}
		if(token[token_i] == 'f'){//find str
			while(token[token_i-1] != 'd'){
				find_check[token_i] = token[token_i];
				token_i++;
			}
			if(!strcmp(find_check,"find")){
				token_i++;
				char find_text[strlen(token)-4];//slovo find ma 4 pismena
				while(token[token_i-1] != '\0'){
					find_text[funkce_i] = token[token_i];
					token_i++;
					funkce_i++;
				}
				find_function(find_text, bunka, bunka_id, &selekce_sloupec, &selekce_sloupec_from, &selekce_sloupec_to,&selekce_radek, &selekce_radek_from, &selekce_radek_to);
				token_i = 0;
				funkce_i = 0;
			}
		}
		if(!strcmp(token,"clear")){
			clear_function(bunka, bunka_id, selekce_sloupec, selekce_sloupec_from, selekce_sloupec_to,selekce_radek, selekce_radek_from, selekce_radek_to);
		}
		token_size = strlen(token);
		if(token_size > 1000){
			fprintf(stderr, "prilis dlouhy argument\n");
			return 1;
		}
		pocet_prikazu++;
		token = strtok(NULL,";");//vezme si dalsi prikaz
	}

	if(pocet_prikazu >= 1002){//kontrola jestli neni zadano prilis mnoho prikazu
		fprintf(stderr, "bylo zadano prilis mnoho prikazu\n");
		return 1;
	}
	print_function(bunka, bunka_id,nejdelsi_sloupec,delim, nejdelsi_radek);//funkce pro print
	printf("\n");
	for(int i = 1; i <= bunka_id - 1;){//cyklus pro uvolneni vsech mallocu
		free(bunka[i].text);
		i++;
	}
	fclose(fp);
return 0;
}
