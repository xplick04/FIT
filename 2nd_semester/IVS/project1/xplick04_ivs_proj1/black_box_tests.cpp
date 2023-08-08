//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     MAXIM PLIČKA <xplick04@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Maxim Plička
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

/*** Konec souboru black_box_tests.cpp ***/
class NonEmptyTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        int values[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };
        int count = 14;
        for(int i = 0; i < count; i++)  //vkladani hodnot do stromu
        {
            Tree.InsertNode(values[i]);
        }
    }

    BinaryTree Tree;
};

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree Tree;    //zlaozeni prazdneho stromu
};


TEST_F(EmptyTree, Insert)
{
    //TESTY NA PRÁDNÝ STROM
    ASSERT_TRUE(Tree.GetRoot() == NULL);

    //TESTY NA PŘIDÁNÍ 1 HODNOTY
    std::pair<bool, BinaryTree::Node_t *> Elem = Tree.InsertNode(1);
    ASSERT_FALSE(Tree.GetRoot() == NULL);
    EXPECT_TRUE(Elem.first);
    EXPECT_EQ(Elem.second->key, 1);
    EXPECT_TRUE(Elem.second->pParent == NULL);
    EXPECT_TRUE(Elem.second->color == BLACK);
    EXPECT_TRUE(Elem.second->pRight != NULL);
    EXPECT_TRUE(Elem.second->pLeft != NULL);

    //TESTY NA LEVÉHO POTOMKA
    BinaryTree::Node_t *left = Elem.second->pLeft;
    EXPECT_EQ(left->key, NULL);
    EXPECT_TRUE(left->pParent->key == 1);
    EXPECT_TRUE(left->pLeft == NULL);
    EXPECT_TRUE(left->pRight == NULL);
    EXPECT_TRUE(left->color == BLACK);

    //TESTY NA PRAVEHO POTOMKA
    BinaryTree::Node_t *right = Elem.second->pRight;
    EXPECT_EQ(right->key, NULL);
    EXPECT_TRUE(right->pParent->key == 1);
    EXPECT_TRUE(right->pLeft == NULL);
    EXPECT_TRUE(right->pRight == NULL);
    EXPECT_TRUE(right->color == BLACK);

    //VLOŽENÍ DRUHÉ HODNOTY, KTERÁ JE STEJNÁ, JAKO TA PRVNÍ
    std::pair<bool, BinaryTree::Node_t *> ElemEqual = Tree.InsertNode(1);
    EXPECT_FALSE(ElemEqual.first);

    //DELETE PRO TEST NOVÉHO PRVKU
    Tree.DeleteNode(1);
    ASSERT_TRUE(Tree.GetRoot() == NULL);

    //TESTY NA PŘIDÁNÍ NULOVÉ HODNOTY
    std::pair<bool, BinaryTree::Node_t *> Elem0 = Tree.InsertNode(0);
    ASSERT_FALSE(Tree.GetRoot() == NULL);
    EXPECT_TRUE(Elem0.first);
    EXPECT_EQ(Elem0.second->key, 0);
    EXPECT_TRUE(Elem0.second->pParent == NULL);
    EXPECT_TRUE(Elem0.second->color == BLACK);
    EXPECT_TRUE(Elem0.second->pRight != NULL);
    EXPECT_TRUE(Elem0.second->pLeft != NULL);

    //DELETE PRO TEST NOVÉHO PRVKU
    Tree.DeleteNode(0);
    ASSERT_TRUE(Tree.GetRoot() == NULL);

    //TESTY NA PŘIDÁNÍ ZÁPORNÉ HODNOTY
    std::pair<bool, BinaryTree::Node_t *> Elem_1 = Tree.InsertNode(-1);
    ASSERT_FALSE(Tree.GetRoot() == NULL);
    EXPECT_TRUE(Elem_1.first);
    EXPECT_EQ(Elem_1.second->key, -1);
    EXPECT_TRUE(Elem_1.second->pParent == NULL);
    EXPECT_TRUE(Elem_1.second->color == BLACK);
    EXPECT_TRUE(Elem_1.second->pRight != NULL);
    EXPECT_TRUE(Elem_1.second->pLeft != NULL);
}

TEST_F(EmptyTree, DeleteNode){
    //TEST NA PRÁDNÝ STROM
    ASSERT_TRUE(Tree.GetRoot() == NULL);
    //ZÁKLADNÍ DELETE OPERACE
    EXPECT_FALSE(Tree.DeleteNode(1));
    EXPECT_FALSE(Tree.DeleteNode(0));
    EXPECT_FALSE(Tree.DeleteNode(-1));

    //PŘIDÁNÍ HODNOTY A PAK KONTROLA SMAZÁNÍ HODNOTY
    std::pair<bool, BinaryTree::Node_t *> Elem = Tree.InsertNode(1);
    ASSERT_FALSE(Tree.GetRoot() == NULL);
    ASSERT_TRUE(Tree.FindNode(1));
    Tree.DeleteNode(1);
    EXPECT_FALSE(Tree.FindNode(1));
}

TEST_F(EmptyTree, FindNode){
    //KONTROLA PRÁZDNÉHO STROMU
    ASSERT_TRUE(Tree.GetRoot() == NULL);

    //ZÁKLADNÍ VYHLEDÁVÁNÍ HODNOT
    EXPECT_FALSE(Tree.FindNode(1));
    EXPECT_FALSE(Tree.FindNode(0));
    EXPECT_FALSE(Tree.FindNode(-1));


    //PŘIDÁNÍ KLADNÉ HODNOTY A JEJÍ NÁSLEDOVNÉ HLEDÁNÍ
    std::pair<bool, BinaryTree::Node_t *> Elem = Tree.InsertNode(4);
    ASSERT_TRUE(Elem.first);
    EXPECT_TRUE(Tree.FindNode(4));

    //PŘIDÁNÍ A VYHLEDÁVÁNÍ ZÁPORNÉ HODNOTY
    std::pair<bool, BinaryTree::Node_t *> ElemNeg = Tree.InsertNode(-1);
    ASSERT_TRUE(ElemNeg.first);
    EXPECT_TRUE(Tree.FindNode(-1));

    //PŘIDÁNÍ A VYHLEDÁVÁNÍ NULY
    std::pair<bool, BinaryTree::Node_t *> ElemNul = Tree.InsertNode(0);
    ASSERT_TRUE(ElemNul.first);
    EXPECT_TRUE(Tree.FindNode(0));

}
//NON EMPTY TREE-----------------------------------------------------------------------------------------------------------------------------------
TEST_F(NonEmptyTree, Insert){
    //TESTY NA NEPRÁDNÝ STROM
    ASSERT_FALSE(Tree.GetRoot() == NULL);
    EXPECT_EQ(Tree.GetRoot()->key, 5);
    EXPECT_TRUE(Tree.GetRoot()->color == BLACK);

    //VLOŽENÍ HODNOTY, KTERÁ UŽ JE V STROMU
    std::pair<bool, BinaryTree::Node_t *> ElemJe = Tree.InsertNode(3);
    EXPECT_FALSE(ElemJe.first);

    //VLOŽENÍ ZÁPORNÉ HODNOTY
    std::pair<bool, BinaryTree::Node_t *> ElemNeg = Tree.InsertNode(-1);
    EXPECT_TRUE(ElemNeg.first);
    //EXPECT_EQ(ElemNeg.second->color, BLACK);

    //VLOŽENÍ NULY
    std::pair<bool, BinaryTree::Node_t *> ElemNul = Tree.InsertNode(0);
    EXPECT_TRUE(ElemNul.first);

    //VLOŽENÍ HODNOTY, KTERÁ ZATÍM NENÍ VE STROMU
    std::pair<bool, BinaryTree::Node_t *> Elem = Tree.InsertNode(4);
    EXPECT_TRUE(Elem.first);
    EXPECT_TRUE(Elem.second->key == 4);
    EXPECT_EQ(Elem.second->color, RED);

    //TESTY NA LEVEHO POTOMKA
    BinaryTree::Node_t *left = Elem.second->pLeft;
    EXPECT_TRUE(left->pParent != NULL);;
    EXPECT_EQ(left->color, BLACK);

    //TESTY NA PRAVÉHO POTOMKA
    BinaryTree::Node_t *right = Elem.second->pRight;
    EXPECT_TRUE(right->pParent != NULL);
    EXPECT_EQ(right->color, BLACK);

    //VLOŽENÍ DRUHÉ HODNOTY, KTERÁ JE STEJNÁ, JAKO TA PRVNÍ
    std::pair<bool, BinaryTree::Node_t *> ElemEqual = Tree.InsertNode(4);
    EXPECT_FALSE(ElemEqual.first);
}

TEST_F(NonEmptyTree, DeleteNode){
    //KONTROLA NEPRÁZDNÉHO STROMU
    ASSERT_TRUE(Tree.GetRoot() != NULL);

    //ZÁKLADNÍ MAZÁNÍ HODNOT
    EXPECT_TRUE(Tree.DeleteNode(3));
    EXPECT_FALSE(Tree.DeleteNode(0));
    EXPECT_FALSE(Tree.DeleteNode(-1));

    //KONTROLA SMAZÁNÍ HODNOTY
    ASSERT_TRUE(Tree.FindNode(1));
    Tree.DeleteNode(1);
    EXPECT_FALSE(Tree.FindNode(1));

    //SMAZÁNÍ VŠECH HODNOT
    int values[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };
    int count = 14;
    for(int i = 0; i < count; i++){
        Tree.DeleteNode(values[i]);
    }
    ASSERT_TRUE(Tree.GetRoot() == NULL);
}

TEST_F(NonEmptyTree, FindNode){
    //KONTROLA NEPRÁZDNÉHO STROMU
    ASSERT_TRUE(Tree.GetRoot() != NULL);

    //ZÁKLADNÍ VYHLEDÁVÁNÍ HODNOT
    EXPECT_TRUE(Tree.FindNode(1));
    EXPECT_FALSE(Tree.FindNode(0));
    EXPECT_FALSE(Tree.FindNode(-1));

    //SMAZÁNÍ HODNOTY A JEJÍ HLEDÁNÍ
    Tree.DeleteNode(1);
    EXPECT_FALSE(Tree.FindNode(1));
}

TEST_F(NonEmptyTree, Axiom1){
    std::vector<BinaryTree::Node_t *> Elem;
    Tree.GetLeafNodes(Elem);

    for(int i = 0; i < Elem.size(); i++){   //listuje polem ukazatelu a kontorluje, jestli maji prvky cernou barvu
        ASSERT_TRUE(Elem[i] != NULL);
        EXPECT_EQ(Elem[i]->color, BLACK);
    }
}

TEST_F(NonEmptyTree, Axiom2){
    std::vector<BinaryTree::Node_t *> Elem;
    Tree.GetAllNodes(Elem);

    for(int i = 0; i < Elem.size(); i++){   //listuje polem ukazatelu
        ASSERT_TRUE(Elem[i] != NULL);
        if(Elem[i]->color == RED){
            //TESTY PRO PRAVÉHO POTOMKA
            ASSERT_TRUE(Elem[i]->pRight != NULL);
            EXPECT_EQ(Elem[i]->pRight->color, BLACK);

            //TESTY PRO LEVÉHO POTOMKA
            ASSERT_TRUE(Elem[i]->pLeft != NULL);
            EXPECT_EQ(Elem[i]->pLeft->color, BLACK);
        }
    }
}

TEST_F(NonEmptyTree, Axiom3){
    std::vector<BinaryTree::Node_t *> Elem;
    BinaryTree::Node_t *tmp;
    Tree.GetAllNodes(Elem);
    int counter, counter2;
    bool flag = false;    //zajistuje, aby se pri prvnim pruchodu nesrovnavaly hodnoty

    for(int i = 0; i < Elem.size(); i++){   //listuje polem ukazatelu, dokud nenarazi na posledni prvek
        ASSERT_TRUE(Elem[i] != NULL);
        if(Elem[i]->pLeft == NULL || Elem[i]->pRight == NULL){      //pokud je prvek list
            tmp = Elem[i];
            ASSERT_TRUE(tmp != NULL);
            counter = 0;
            while(tmp->pParent != NULL){        //cyklus, ktery se dostava ke korenu
                if(tmp->color == BLACK){
                    counter++;
                }
                tmp = tmp->pParent;
            }
            if(flag == true) {
                EXPECT_EQ(counter, counter2);
            }
            counter2 = counter;
            flag = true;    //po prvnim pruchodu se nastavi na true
        }
    }
}