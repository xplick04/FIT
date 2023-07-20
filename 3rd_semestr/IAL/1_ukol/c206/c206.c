
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
		//inicializace seznamu
		list->firstElement = NULL;
		list->activeElement = NULL;
		list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

		DLLElementPtr tmp = list->firstElement; //nastaveni pomocne promene na prvni prvek seznamu
		DLLElementPtr tmpDel;

		while(tmp != NULL) //projede seznam a postupne rusi prvky
		{
			tmpDel = tmp;
			tmp = tmp->nextElement;
			free(tmpDel);
		}
		//reinicializace seznamu
		list->firstElement = NULL;
		list->activeElement = NULL;
		list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
		DLLElementPtr tmp = list->firstElement;
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if(new==NULL) //kontorla mallocu
		{
				DLL_Error();
				return;
		}
		//nastaveni hodnot noveho prvku
		new->data = data;
		new->previousElement = NULL;
		new->nextElement = tmp;
		//nastaveni prvniho prvku seznamu
		list->firstElement = new;

		if(tmp != NULL) //seznam neni prazdny
		{
			tmp->previousElement = new;
		}
		else	//seznam je prazdny
		{
			list->lastElement = new;
		}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
		DLLElementPtr tmp = list->lastElement;
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if(new == NULL) //kontorla mallocu
		{
				DLL_Error();
				return;
		}
		//nastaveni hodnot noveho prvku
		new->data = data;
		new->previousElement = tmp;
		new->nextElement = NULL;
		//nastaveni prvniho prvku seznamu
		list->lastElement = new;

		if(tmp != NULL) //seznam neni prazdny
		{
			tmp->nextElement = new;
		}
		else //seznam je prazdny
		{
			list->firstElement = new;
		}
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement; //nastaveni aktivity na prvni prvek
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
		list->activeElement = list->lastElement; //nastaveni aktivity na posledni prvek
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if(list->firstElement == NULL)	//seznam je prazdny
		{
			DLL_Error();
			return;
		}
		else
		{
			*dataPtr = list->firstElement->data; //hodnota z prvniho prvku do dataPtr
		}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

		if(list->lastElement == NULL) //seznam je prazdny
		{
			DLL_Error();
			return;
		}
		else
		{
			*dataPtr = list->lastElement->data; //hodnota z posledniho prvku do dataPtr
		}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
		if(list->firstElement == NULL)return; //kontrola prazdneho seznamu
		DLLElementPtr tmpDel = list->firstElement;

			if(list->firstElement == list->activeElement) //prvni je aktivni
			{
				list->activeElement = NULL; //prvek ztraci aktivitu
			}
			if(list->firstElement->nextElement == NULL) //prvni prvek uz nema nasledujici prvek
			{
				list->firstElement = NULL; //nastavi ukazatel prvniho prvku na NULL
				list->lastElement = NULL; //nastavi ukazatel posledniho prvku na NULL
				free(tmpDel); //uvolneni mazane promenne
				return;
			}
			else //prvni prvek ma nasleduji prvek
			{
				list->firstElement->nextElement->previousElement = NULL; //nastavi ukazatel druheho prvku z prvniho(mazany) na NULL
			}
			list->firstElement = list->firstElement->nextElement; //prehodi ukazatel prvniho prvku na nasledujici
			free(tmpDel); //uvolneni mazane promenne
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
		if(list->lastElement == NULL)return; //kontrola prazdneho seznamu
		DLLElementPtr tmpDel = list->lastElement;

			if(list->lastElement == list->activeElement) //posledni prvek je aktivni
			{
				list->activeElement = NULL;
			}
			if(list->lastElement->previousElement==NULL) //posledni prvek uz nema predchozi prvek
			{
				list->lastElement = NULL; //nastavi ukazatel posledniho prvku na NULL
				list->firstElement = NULL; //nastavi ukazatel prvniho prvku na NULL
				free(tmpDel);	//uvolneni mazane promenne
				return;
			}
			else //posledni prvek ma predchozi prvek
			{
				//prenastaveni ukazatelu v listu
				list->lastElement->previousElement->nextElement = NULL;
			}
			list->lastElement = list->lastElement->previousElement;
			free(tmpDel); //uvolneni mazane promenne
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
		if(list->firstElement == NULL)return; //kontrola prazdneho seznamu

		DLLElementPtr tmpDel;

		if((list->activeElement != NULL) && (list->activeElement != list->lastElement)) //pokud je seznam aktivni a aktivni prvek neni poslednim prvkem seznamu
		{
			tmpDel = list->activeElement->nextElement; //nastavi tmpDel na mazany prvek
			if(tmpDel == list->lastElement) //mazany prvek je posledni prvek seznamu
			{
				//prenastaveni ukazatelu v listu
				list->lastElement = list->activeElement;
				list->activeElement->nextElement = NULL;
			}
			else //mazany prvek neni na konci
			{
				//prenastaveni ukazatelu v listu
				tmpDel->nextElement->previousElement = list->activeElement;
				list->activeElement->nextElement = tmpDel->nextElement;
			}
			free(tmpDel); //uvolneni prvku
		}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
		if(list->firstElement == NULL)return; //kontrola prazdneho seznamu

		DLLElementPtr tmpDel;

		if((list->activeElement != NULL) && (list->activeElement != list->firstElement)) //seznam aktivni a aktivni prvek neni prvnim prvkem seznamu
		{
			tmpDel = list->activeElement->previousElement; //nastavi tmpDel na mazany prvek
			if(tmpDel == list->firstElement) //mazany prvek je prvni prvek seznamu
			{
				//prenastaveni ukazatelu v listu
				list->firstElement = list->activeElement;
				list->activeElement->previousElement = NULL;
			}
			else //mazany prvek neni na zacatku
			{
				//prenastaveni ukazatelu v listu
				tmpDel->previousElement->nextElement = list->activeElement;
				list->activeElement->previousElement = tmpDel->previousElement;
			}
			free(tmpDel); //uvolneni prvku
		}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
		if(list->activeElement == NULL)return; //zadny prvek neni aktivni
		if(list->firstElement == NULL)return;	//kontrola prazdneho seznamu

		DLLElementPtr tmp;
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if(new == NULL) //kontorla mallocu
		{
				DLL_Error();
				return;
		}

		new->data = data;
		new->previousElement = list->activeElement;
		if(list->activeElement->nextElement == NULL) //vkladany prvek je posledni
		{
			//prenastaveni ukazatelu v listu
			list->activeElement->nextElement = new;
			list->lastElement = new;
		}
		else
		{
			//prenastaveni ukazatelu v listu
			tmp = list->activeElement->nextElement;
			tmp->previousElement = new;
			list->activeElement->nextElement = new;
		}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
		if(list->activeElement == NULL)return; //kontrola aktivity
		if(list->firstElement == NULL)return;	//kontrola prazdneho seznamu

		DLLElementPtr tmp;
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if(new == NULL) //kontorla mallocu
		{
				DLL_Error();
				return;
		}

		new->data = data;
		new->nextElement = list->activeElement;

		if(list->activeElement == list->firstElement)	// aktivni prvek je prvnim prvkem seznamu
		{
			//prenastaveni ukazatelu v listu
			list->activeElement->previousElement = new;
			list->firstElement = new;
		}
		else //aktivni prvek neni na zacatku
		{
			//prenastaveni ukazatelu v listu
			tmp = list->activeElement->previousElement;
			tmp->nextElement = new;
			list->activeElement->previousElement = new;
		}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
		if(list->activeElement == NULL) //zadny prvek neni aktivni
		{
				DLL_Error();
		}
		else //nejaky prvek je aktivni
		{
				*dataPtr = list->activeElement->data;
		}

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
		if(list->activeElement != NULL) //nejaky prvek je aktivni
		{
				list->activeElement->data = data;
		}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
		if(list->activeElement != NULL) //nejaky prvek je aktivni
		{
			list->activeElement = list->activeElement->nextElement;
		}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
		if(list->activeElement != NULL) //nejaky prvek je aktivni
		{
			list->activeElement = list->activeElement->previousElement;
		}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    return (list->activeElement != NULL); //je-li aktivni prvek, vraci true
}

/* Konec c206.c */
