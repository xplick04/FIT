/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if(table)
  {
    for(int i = 0; i < HT_SIZE; i++)
    {
      (*table)[i] = NULL;
    }
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  ht_item_t *tmp = (*table)[get_hash(key)]; //najdu si polozku v tabulce

  while(tmp != NULL) //kontrolovani hodnot linked-listu v polozce tabulky
  {
    if(!strcmp(tmp->key, key)) return tmp; //hodnota nalezena
    tmp = tmp->next;
  }
  return NULL;  //hodnota nenalezena, nebo polozka tabulky je prazdna
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if(table == NULL) return;
  ht_item_t *tmp = ht_search(table, key);
  if(tmp == NULL) //bunka je pradzna
  {
      ht_item_t *new = malloc(sizeof(ht_item_t));
      if(new == NULL) return; //chyba mallocu
      new->key = key;
      new->value = value;
      new->next = (*table)[get_hash(key)];  //navazani prvku v linked listu
      (*table)[get_hash(key)] = new;  //navazani prvku v linked listu
  }
  else
  {
    tmp->value = value;
  }

}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *tmp = ht_search(table, key);

  if(tmp != NULL) return &(tmp)->value;
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *tmp = (*table)[get_hash(key)];
  ht_item_t *previous;
  while(tmp != NULL)
  {
    if(!strcmp(tmp->key, key))
    {
      if(previous == NULL) //maze se prvni prvek
      {
        (*table)[get_hash(key)] = tmp->next;
      }
      else
      {
        previous->next = tmp->next;
      }
      free(tmp);
      return;
    }
    previous = tmp;
    tmp = tmp->next;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t *tmp, *tmpDel;
  for(int i = 0; i < HT_SIZE; i++)
  {
      tmp = (*table)[i];
      while(tmp != NULL) //projizdi kazdy prvek linked listu
      {
        tmpDel = tmp;
        tmp = tmp->next;
        free(tmpDel);
      }
      (*table)[i] = NULL;
  }
}
