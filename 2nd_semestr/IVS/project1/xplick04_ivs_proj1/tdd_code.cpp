//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     MAXIM PLIČKA <xplick04@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Maxim Plička
 *
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"



//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue() //vytvori prazdny seznam
{
    m_pHead = nullptr;
}

PriorityQueue::~PriorityQueue() //desctructor
{
    Element_t *tmp = GetHead(); //nastavi do pomocne promene hlavu
    while(tmp != nullptr){  //listuje seznamem
        Element_t *current = tmp;   //musi se pokazde znovu zalozit, protoze se pri kazde iteraci smaze
        tmp = tmp->pNext;
        delete current;
    }
}

void PriorityQueue::Insert(int value)
{
    if(GetHead() == nullptr){   //pokud v seznamu neni jeste zadny prvek
        m_pHead = new Element_t
                {
                        .pNext = nullptr,
                        .value = value
                };
    }
    else{
        Element_t *tmp = GetHead();
        while(tmp != nullptr){  //klasicke vkladani
            if(GetHead()->value < value){ //pokud dojde vetsi hodnota nez hlava
                Element_t *newElement = new Element_t
                        {
                                .pNext = GetHead(),
                                .value = value
                        };
                m_pHead = newElement;
                break;
            }
            else if(tmp->pNext == nullptr){//pokud bude nejmensi hodnota v seznamu, vlozi se hodnota na konec
                tmp->pNext = new Element_t
                        {
                                .pNext = nullptr,
                                .value = value
                        };
                break;
            }
            else if(value >= tmp->pNext->value && tmp->pNext != nullptr){//pokud je hodnota vetsi nez dalsi hodnota, vlozi se pred ni
                Element_t *newElement = new Element_t
                        {
                                .pNext = tmp->pNext,
                                .value = value
                        };
                tmp->pNext = newElement;
                break;
            }
            tmp = tmp->pNext;
        }
    }
}

bool PriorityQueue::Remove(int value)
{
    Element_t *tmp = GetHead();
    Element_t *prev = GetHead();
    while(tmp != nullptr)   //listuje seznamem
    {
        if(GetHead()->value == value){  //pokud je hledany prvek hlava
            m_pHead = tmp->pNext;   //prenastaveni hlavy na dalsi prvek
            delete prev;
            return true;
        }
        if(value == tmp->value){    //pokud hledany prvek neni hlava
            prev->pNext = tmp->pNext;
            delete tmp;
            return true;
        }
        prev = tmp;
        tmp = tmp->pNext;
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *tmp = GetHead();
    while(tmp != nullptr){
        if(tmp->value == value){    //returnuje hledany prvek, pokud se najde
            return tmp;
        }
        tmp = tmp->pNext;
    }
    return nullptr;
}

size_t PriorityQueue::Length()
{
    Element_t *tmp = GetHead();
    size_t count = 0;
    while(tmp != nullptr){  //listuje seznamem a pripocitava ke countu
        count++;
        tmp = tmp->pNext;
    }
    return count;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
