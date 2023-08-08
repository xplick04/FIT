/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while(tree != NULL)
  {
    if(tree->key == key)
    {
      *value = tree->value;
      return true;
    }
    else if(tree->key > key) tree = tree->left;
    else if(tree->key < key) tree = tree->right;
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  bst_node_t *insertAfter = *tree;
  bool left;
  while(insertAfter != NULL) //vyhledavani prvku, za ktery chceme doplnit
  {

    if(insertAfter->key > key) //hodnota hledaneho klice je mensi -> jde se doleva
    {
        if(insertAfter->left == NULL) //hodnota ve stromu jeste neni
        {
          left = true; //budu vkladat doleva
          break;
        }
        else insertAfter = insertAfter->left; //dalsi prvek
    }

    else if(insertAfter->key < key) //hodnota hledaneho klice je vetsi -> jde se doprava
    {
        if(insertAfter->right == NULL) //hodnota ve stromu jeste neni
        {
          left = false; //budu vkladat doprava
          break;
        }
        else insertAfter = insertAfter->right; //dalsi prvek
    }

    else if(insertAfter->key == key) //hodnota jiz existuje -> prepise a ukonci
    {
      insertAfter->value = value;
      return;
    }
  }

  bst_node_t *new = malloc(sizeof(bst_node_t));
  if(!new) //chyba mallocu
  {
    return;
  }
  new->key = key;
  new->value = value;
  new->right = NULL;
  new->left = NULL;

  if(insertAfter == NULL) //strom je prazdny
  {
    *tree = new;
    return;
  }
  if(left) //vkladam nalevo
  {
    insertAfter->left = new;
    return;
  }
  else  //vkladam napravo
  {
    insertAfter->right = new;
    return;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
 void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
   bst_node_t *parent = NULL;
   bst_node_t *current = *tree;
   target->left = *tree;

   while(current->right != NULL) //hledani nejpravejsiho prvku
   {
     parent = current;
     current = current->right;
   }

   //nejpravejsi prvek nalezen
   target->key = current->key;
   target->value = current->value;

   if(parent != NULL)
   {
     parent->right = current->left;
   }
   free(current);
 }

 /*
  * Odstránenie uzlu v strome.
  *
  * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
  * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
  * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
  * ľavého podstromu. Najpravejší uzol nemusí byť listom!
  * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
  *
  * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
  * použitia vlastných pomocných funkcií.
  */
 void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *deleteNode = *tree;
  bst_node_t *deleteNodeParent = NULL;

  while(deleteNode != NULL) //vyhledavani prvku, za ktery chceme doplnit
  {
    if(deleteNode->key == key) //hodnota nalezena, break a jde dal
    {
      break;
    }
    else if(deleteNode->key > key) //hodnota hledaneho klice je mensi -> jde se doleva
    {
        deleteNodeParent = deleteNode;
        deleteNode = deleteNode->left; //dalsi prvek
    }
    else if(deleteNode->key < key) //hodnota hledaneho klice je vetsi -> jde se doprava
    {
        deleteNodeParent = deleteNode;
        deleteNode = deleteNode->right; //dalsi prvek
    }
  }

  if(deleteNode == NULL) return; //strom je prazdny, nenasel se mazany prvek

  //mazany prvek nema podstrom
  if(deleteNode->left == NULL && deleteNode->right == NULL)
  {
    if(deleteNodeParent != NULL) //pokud prvek neni koren
    {
    if(deleteNodeParent->key < key) deleteNodeParent->right = NULL; //uprava ukazatelu z otce
    else deleteNodeParent->left = NULL; //uprava ukazatelu z otce
    }
    free(deleteNode);
    deleteNode = NULL;
    return;
  }
  //mazany prvek ma jeden podstrom
  else if(deleteNode->left == NULL && deleteNode->right != NULL) //ma pravy podstrom
  {
    bst_node_t *tmp = deleteNode->right;
    if(deleteNodeParent != NULL) //pokud prvek neni koren
    {
      if(deleteNodeParent->key < key) deleteNodeParent->right = tmp; //uprava ukazatelu z otce
      else deleteNodeParent->left = tmp; //uprava ukazatelu z otce
    }
    free(deleteNode);
    deleteNode = NULL;
    return;
  }
  else if(deleteNode->left != NULL && deleteNode->right == NULL) //ma levy podstrom
  {
    bst_node_t *tmp = deleteNode->left;
    if(deleteNodeParent != NULL) //pokud prvek neni koren
    {
      if(deleteNodeParent->key < key) deleteNodeParent->right = tmp; //uprava ukazatelu z otce
      else deleteNodeParent->left = tmp; //uprava ukazatelu z otce
    }
    free(deleteNode);
    deleteNode = NULL;
    return;
  }
  //mazany prvek ma oba podstromy
  else if(deleteNode->left != NULL && deleteNode->right != NULL)
  {
    bst_replace_by_rightmost(deleteNode, &(deleteNode)->left);
  }
  return;
 }

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  //kod z prednasky
  do
  {
    if (*tree == NULL)
    {
      if(!stack_bst_empty(&stack))
      {
        *tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    }
    else
    {
      if((*tree)->right != NULL)
      {
        stack_bst_push(&stack, (*tree)->right);
      }
      bst_node_t *auxPtr = *tree;
      *tree = (*tree)->left;
      free(auxPtr);
    }
  } while( *tree != NULL || !(stack_bst_empty(&stack)) );

}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  //kod z prednasky
  while(tree)
  {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    //kod z prednasky
    stack_bst_t stack;
    stack_bst_init(&stack);

    bst_leftmost_preorder(tree,&stack);
    while(!stack_bst_empty(&stack))
    {
      tree = stack_bst_top(&stack);
      stack_bst_pop(&stack);
      bst_leftmost_preorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  //kod z prednasky
  while(tree)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  //kod z prednasky
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree, &stack);
  while(!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  //kod z prednasky
  while(tree)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit,true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  //kod z prednasky
  stack_bst_t stack;
  stack_bool_t bStack;
  bool fromLeft;
  stack_bst_init(&stack);
  stack_bool_init(&bStack);

  bst_leftmost_postorder(tree, &stack, &bStack);
  while(!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);
    fromLeft = stack_bool_top(&bStack);
    stack_bool_pop(&bStack);
    if(fromLeft)
    {
      stack_bool_push(&bStack, false);
      bst_leftmost_postorder(tree->right, &stack, &bStack);
    }
    else
    {
      stack_bst_pop(&stack);
      bst_print_node(tree);
    }
  }
}
