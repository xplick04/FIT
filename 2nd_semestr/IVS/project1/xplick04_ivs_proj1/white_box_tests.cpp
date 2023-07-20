//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     MAXIM PLIČKA <xplick04@stud.fit.vutbr.cz>
// $Date:       $2021-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Maxim Plička
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"
#include <vector>

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/

class MatrixTest :public ::testing::Test {};

TEST_F(MatrixTest, Constructor){

    //VYTVOŘENÍ ČTVERCOVÉ MATICE
    ASSERT_NO_THROW(Matrix());
    EXPECT_NO_THROW(Matrix(1,1));
    EXPECT_NO_THROW(Matrix(5,5));
    EXPECT_ANY_THROW(Matrix(0,0));
    EXPECT_ANY_THROW(Matrix(-1,-1));

    //VYTVOŘENÍ NEČTVERCOVÉ MATICE(KLADNÉ)
    EXPECT_ANY_THROW(Matrix(0,1));
    EXPECT_ANY_THROW(Matrix(1,0));
    EXPECT_NO_THROW(Matrix(1,2));
    EXPECT_NO_THROW(Matrix(2,1));

    //VYTVOŘENÍ NEČTVERCOVÉ MATICE(ZÁPORNÉ)
    EXPECT_ANY_THROW(Matrix(0,-1));
    EXPECT_ANY_THROW(Matrix(-1,0));
    //|ROW|<|COLUMN|
    EXPECT_ANY_THROW(Matrix(-1,2));
    EXPECT_ANY_THROW(Matrix(1,-2));
    //|ROW|>|COLUMN|
    EXPECT_ANY_THROW(Matrix(2,-1));
    EXPECT_ANY_THROW(Matrix(-2,1));
    //OBĚ ZÁPORNÉ
    EXPECT_ANY_THROW(Matrix(-2,-1));
    EXPECT_ANY_THROW(Matrix(-1,-2));
}

TEST_F(MatrixTest, SetValue){

    //VYTVOŘENÍ ČTVERCOVÉ MATICE+STANDARTNÍ VLOŽENÍ
    Matrix Matice = Matrix(1,1);
    ASSERT_TRUE(Matice.set(0, 0, 1));

    //VKLÁDÁNÍ ZÁPORNÉ HODNOTY, NEBO NULY
    ASSERT_TRUE(Matice.set(0, 0, 0));
    ASSERT_TRUE(Matice.set(0, 0, -1));

    //VLOŽENÍ NA STEJNOU POZICI(STEJNÉHO A RŮZNÉHO ČÍSLA)
    EXPECT_TRUE(Matice.set(0, 0, 1));
    EXPECT_TRUE(Matice.set(0, 0, 1));
    EXPECT_TRUE(Matice.set(0, 0, 2));

    //VKLÁDÁNÍ NA ZÁPORNOU POZICI ČTVERCOVÉ MATICE
    EXPECT_FALSE(Matice.set(0, -1, 1));
    EXPECT_FALSE(Matice.set(-1, 0, 1));
    EXPECT_FALSE(Matice.set(-1, -1, 1));

    //VKLÁDÁNÍ NA POZICI ČTVERCOVÉ MATICE, KTERÁ NEEXISTUJE
    EXPECT_FALSE(Matice.set(0, 2, 1));
    EXPECT_FALSE(Matice.set(2, 0, 1));
    EXPECT_FALSE(Matice.set(2, 2, 1));


    //VYTVOŘENÍ NEČTVERCOVÉ MATICE+STANDARTNÍ VLOŽENÍ
    Matice = Matrix(2,3);
    ASSERT_TRUE(Matice.set(1, 0, 1));
    EXPECT_TRUE(Matice.set(0, 2, 1));
    EXPECT_TRUE(Matice.set(1, 1, 1));

    //VLOŽENÍ NA STEJNOU POZICI U NEČTVERCOVÉ MATICE
    EXPECT_TRUE(Matice.set(1, 2, 1));
    EXPECT_TRUE(Matice.set(1, 2, 1));

    //VKLÁDÁNÍ NA ZÁPORNOU POZICI NEČTVERCOVÉ MATICE
    EXPECT_FALSE(Matice.set(1, -1, 1));
    EXPECT_FALSE(Matice.set(-1, 2, 1));
    EXPECT_FALSE(Matice.set(-1, -1, 1));

    //VKLÁDÁNÍ NA POZICI NEČTVERCOVÉ MATICE, KTERÁ NEEXISTUJE
    EXPECT_FALSE(Matice.set(0, 3, 1));
    EXPECT_FALSE(Matice.set(3, 0, 1));
    EXPECT_FALSE(Matice.set(3, 3, 1));
}

TEST_F(MatrixTest, SetValues){
    //VYTVOŘENÍ ČTVERCOVÉ MATICE, POLE HODNOT+STANDARTNÍ VLOŽENÍ
    Matrix Matice = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,2},
                                                  {3,4}};
    ASSERT_TRUE(Matice.set(values));

    //VKLÁDÁNÍ MÉNĚ HODNOT, NEŽ JE POZIC V MATICI
    values = {{1,2}};
    EXPECT_FALSE(Matice.set(values));

    //VKLÁDÁNÍ VÍCE HODNOT, NEŽ JE POZIC V MATICI
    values = {{1,2},
              {3,4},
              {3,4}};
    EXPECT_FALSE(Matice.set(values));

    //VYTVOŘENÍ+VKLÁDÁNÍ DO NEČTVERCOVÉ MATICE

    //ROWS>COLUMNS
    Matice = Matrix(3,2);
    values = {{1,2},
              {3,4},
              {3,4}};
    EXPECT_TRUE(Matice.set(values));

    //ROWS<COLUMNS
    Matice = Matrix(2,3);
    values = {{1,2,3},
              {4,5,6}};
    EXPECT_TRUE(Matice.set(values));
}

TEST_F(MatrixTest, GetValue){
    //VYTVOŘENÍ MATICE+ KONTROLA NULOVÉ HODNOTY
    Matrix Matice = Matrix();
    ASSERT_TRUE(Matice.get(0,0) == 0);

    //HLEDÁNÍ HODNOTY MIMO MATICI
    ASSERT_ANY_THROW(Matice.get(1,1));
    EXPECT_ANY_THROW(Matice.get(0,1));
    EXPECT_ANY_THROW(Matice.get(1,0));

    //HLEDÁNÍ NA ZÁPORNÉ POZICI
    ASSERT_ANY_THROW(Matice.get(-1,-1));
    EXPECT_ANY_THROW(Matice.get(1,-1));
    EXPECT_ANY_THROW(Matice.get(-1,1));

    //VYTVOŘENÍ+VKLÁDÁNÍ A HLEDÁNÍ VE ČTVERCOVÉ MATICI
    Matice = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,2},
                                                  {3,4}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_TRUE(Matice.get(0,0) == 1);
    EXPECT_TRUE(Matice.get(0,1) == 2);
    EXPECT_TRUE(Matice.get(1,0) == 3);
    EXPECT_TRUE(Matice.get(1,1) == 4);

    //VYTVOŘENÍ+VKLÁDÁNÍ A HLEDÁNÍ VE NEČTVERCOVÉ MATICI
    Matice = Matrix(3,2);
    values = {{1, 2},
               {3,4},
               {5,6}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_TRUE(Matice.get(0,0) == 1);
    EXPECT_TRUE(Matice.get(0,1) == 2);
    EXPECT_TRUE(Matice.get(1,0) == 3);
    EXPECT_TRUE(Matice.get(1,1) == 4);
    EXPECT_TRUE(Matice.get(2,0) == 5);
    EXPECT_TRUE(Matice.get(2,1) == 6);
}

TEST_F(MatrixTest, Equal){
    //POROVNÁNÍ DVOU PRÁZDNÝCH MATIC
    Matrix Matice = Matrix();
    Matrix Matice2 = Matrix();
    ASSERT_TRUE(Matice == Matice2);

    //POROVNÁNÍ DVOU PRÁDNÝCH MATIC S RŮZNOU VELIKOSTÍ
    Matice = Matrix(2,2);
    EXPECT_ANY_THROW(Matice == Matice2);

    //POROVNÁNÍ PRÁDNÉ S NEPRÁZDNOU MATICÍ
    Matice2 = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,2},
                                                  {3,4}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_FALSE(Matice == Matice2);

    //POROVNÁNÍ DVOU ČTVERCOVÝCH MATIC

    //STEJNÉ HODNOTY
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_TRUE(Matice == Matice2);

    //RŮZNÉ HODNOTY
    values = {{1,2},
              {3,5}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_FALSE(Matice == Matice2);

    //POROVNÁNÍ DVOU NEČTVERCOVÝCH MATIC
    Matice = Matrix(3,2);
    Matice2 = Matrix(3,2);
    values = {{1,2},
              {3,4},
              {5,6}};
    //STEJNÉ HODNOTY
    ASSERT_TRUE(Matice.set(values));
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_TRUE(Matice == Matice2);

    //RŮZNÉ HODNOTY
    values = {{1,2},
              {3,4},
              {5,7}};
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_FALSE(Matice == Matice2);
}

TEST_F(MatrixTest, Plus){

    //ZALOŽENÍ DVOU STEJNÝCH MATIC
    Matrix Matice = Matrix(2,2);
    Matrix Matice2 = Matrix(2,2);
    Matrix MaticeFinal = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,1},
                                                  {1,1}};
    ASSERT_TRUE(Matice.set(values));
    ASSERT_TRUE(Matice2.set(values));

    //SČÍTÁNÍ DVOU ČTVERCOVÝCH MATIC O STEJNÉ VELIKOSTI
    ASSERT_NO_THROW(Matice+Matice2);
    MaticeFinal = Matice+Matice2;
    EXPECT_TRUE(MaticeFinal.get(0,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(0,1) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,1) == 2);

    //SČÍTÁNÍ DVOU ČTVERCOVÝCH MATIC O RŮZNÉ VELIKOSTI
    Matice2 = Matrix(3,3);
    ASSERT_ANY_THROW(Matice+Matice2);

    //SČÍTÁNÍ DVU NEČTVERCOVÝCH MATIC O RŮZNÉ VELIKOSTI
    Matice = Matrix(4,2);
    Matice2 = Matrix(3,2);
    ASSERT_ANY_THROW(Matice+Matice2);

    //SČÍTÁNÍ DVOU NEČTVERCOVÝCH MATIC O STEJNÉ VELIKOSTI
    Matice = Matrix(3,2);
    MaticeFinal = Matrix(3,2);
    values = {{1,2},
              {3,4},
              {5,6}};
    ASSERT_TRUE(Matice.set(values));
    ASSERT_TRUE(Matice2.set(values));
    ASSERT_NO_THROW(Matice+Matice2);
    MaticeFinal = Matice+Matice2;
    EXPECT_TRUE(MaticeFinal.get(0,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(0,1) == 4);
    EXPECT_TRUE(MaticeFinal.get(1,0) == 6);
    EXPECT_TRUE(MaticeFinal.get(1,1) == 8);
    EXPECT_TRUE(MaticeFinal.get(2,0) == 10);
    EXPECT_TRUE(MaticeFinal.get(2,1) == 12);
}

TEST_F(MatrixTest, Multi){
    //ZALOŽENÍ DVOU STEJNÝCH MATIC
    Matrix Matice = Matrix(2,2);
    Matrix Matice2 = Matrix(2,2);
    Matrix MaticeFinal = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,1},
                                                  {1,1}};
    ASSERT_TRUE(Matice.set(values));
    ASSERT_TRUE(Matice2.set(values));

    //NÁSOBENÍ DVOU ČTVERCCOVÝCH MATIC O STEJNÉ VELIKOSTI
    ASSERT_NO_THROW(Matice*Matice2);
    MaticeFinal = Matice*Matice2;
    EXPECT_TRUE(MaticeFinal.get(0,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(0,1) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,1) == 2);

    //NÁSOBENÍ DVOU ČTVERCOVÝCH MATIC O RŮZNÉ VELIKOSTI
    Matice2 = Matrix(3,3);
    ASSERT_ANY_THROW(Matice*Matice2);

    //NÁSOBENÍ DVU NEČTVERCOVÝCH MATIC NEVYHOVUJÍCÍ VELIKOSTI
    Matice = Matrix(4,2);
    Matice2 = Matrix(3,2);
    ASSERT_ANY_THROW(Matice*Matice2);
    Matice = Matrix(2,4);
    Matice2 = Matrix(3,2);
    EXPECT_ANY_THROW(Matice*Matice2);

    //NÁSOBENÍ DVU NEČTVERCOVÝCH MATIC VYHOVUJÍCÍ VELIKOSTI
    Matice = Matrix(2,3);
    Matice2 = Matrix(3,2);
    MaticeFinal = Matrix(2,2);
    values = {{1,1,1},
              {1,1,1}};
    ASSERT_TRUE(Matice.set(values));
    values = {{1,1},
              {1,1},
              {1,1}};
    ASSERT_TRUE(Matice2.set(values));
    ASSERT_NO_THROW(Matice*Matice2);
    MaticeFinal = Matice*Matice2;
    EXPECT_TRUE(MaticeFinal.get(0,0) == 3);
    EXPECT_TRUE(MaticeFinal.get(0,1) == 3);
    EXPECT_TRUE(MaticeFinal.get(1,0) == 3);
    EXPECT_TRUE(MaticeFinal.get(1,1) == 3);
}

TEST_F(MatrixTest, MultiConst){
    //ZALOŽENÍ MATICE
    int Const = 2;
    Matrix Matice = Matrix(2,2);
    Matrix MaticeFinal = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,1},
                                                  {1,1}};
    ASSERT_TRUE(Matice.set(values));
    //NÁSOBENÍ MATICE KONSTANTOU
    ASSERT_NO_THROW(Matice*Const);
    MaticeFinal = Matice*Const;
    EXPECT_TRUE(MaticeFinal.get(0,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(0,1) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,0) == 2);
    EXPECT_TRUE(MaticeFinal.get(1,1) == 2);
}

TEST_F(MatrixTest, SolveEquation){
    std::vector<double> Solving;
    Matrix Matice = Matrix(2,2);
    std::vector<std::vector< double > > values;
    std::vector<double> RightSide;

    //DOAZENÍ PARAMETRU S VÝSLEDKEM DO ROVNICE S JINÝMI ROZMĚRY
    RightSide = {{1,1,1}};
    values = {{1,1},
              {1,0}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));
    RightSide = {{1}};
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));

    //NEČTVERCOVÁ MATICE
    Matice = Matrix(3,2);
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));
    Matice = Matrix(2,3);
    RightSide = {{0,0,0}};
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));

    //SINGULÁRNÍ MATICE
    Matice = Matrix(2,2);
    values = {{1,2},
              {2,4}};
    RightSide = {3,6};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));

    //ŘEŠENÍ STANDARTNÍ MATICE
    Matice = Matrix(2,2);
    values = {{1,2},
              {4,3}};
    RightSide = {5,15};
    Solving = {3,1};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_NO_THROW(Matice.solveEquation(RightSide));
    EXPECT_TRUE(Matice.solveEquation(RightSide) == Solving);

    //MATICE 1X1
    Matice = Matrix();
    values = {{1}};
    ASSERT_TRUE(Matice.set(values));
    RightSide = {1};
    Solving = {1};
    EXPECT_TRUE(Matice.solveEquation(RightSide)==Solving);

    //MATICE 5X5
    Matice = Matrix(5,5);
    RightSide = {0,0,0,0,0};
    EXPECT_ANY_THROW(Matice.solveEquation(RightSide));
}

TEST_F(MatrixTest, Transponse){
    //VYTVOŘENÍ ČTVERCOVÉ MATICE+TEST
    Matrix Matice = Matrix(2,2);
    Matrix Matice2 = Matrix(2,2);
    std::vector<std::vector< double > > values;
    values = {{1,0},
              {1,0}};
    ASSERT_TRUE(Matice.set(values));
    values = {{1,1},
              {0,0}};
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_TRUE(Matice.transpose()==Matice2);


    //VYTVOŘENÍ NEČTVERCOVÉ MATICE

    //ROWS>COLUMNS
    Matice = Matrix(3,2);
    Matice2 = Matrix(2,3);
    values = {{1,0},
              {1,0},
              {1,0}};
    ASSERT_TRUE(Matice.set(values));
    values = {{1,1,1},
              {0,0,0}};
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_TRUE(Matice.transpose()==Matice2);

    //ROWS<COLUMNS
    Matice = Matrix(2,3);
    Matice2 = Matrix(3,2);
    values = {{2,1,0},
              {2,1,0}};
    ASSERT_TRUE(Matice.set(values));
    values = {{2,2},
              {1,1},
              {0,0}};
    ASSERT_TRUE(Matice2.set(values));
    EXPECT_TRUE(Matice.transpose()==Matice2);
}

TEST_F(MatrixTest, Inverse){
    //TESTY PRO MATICI 1X1
    Matrix Matice = Matrix();
    EXPECT_ANY_THROW(Matice.inverse());

    //TESTY PRO MATICI 2X2
    Matice = Matrix(2,2);
    Matrix MaticeInversed = Matrix(2,2);
    std::vector<std::vector< double > > values = {{1,0},
                                                  {2,1}};
    ASSERT_TRUE(Matice.set(values));
    ASSERT_NO_THROW(MaticeInversed=Matice.inverse());
    EXPECT_TRUE(MaticeInversed.get(0,0)==1);
    EXPECT_TRUE(MaticeInversed.get(0,1)==0);
    EXPECT_TRUE(MaticeInversed.get(1,0)==-2);
    EXPECT_TRUE(MaticeInversed.get(1,1)==1);

    //TESTY PRO MATICI 3X3
    Matice = Matrix(3,3);
    MaticeInversed = Matrix(3,3);
    values = {{1,0,0},
              {2,1,0},
              {0,2,1}};
    ASSERT_TRUE(Matice.set(values));
    ASSERT_NO_THROW(MaticeInversed=Matice.inverse());
    EXPECT_TRUE(MaticeInversed.get(0,0)==1);
    EXPECT_TRUE(MaticeInversed.get(0,1)==0);
    EXPECT_TRUE(MaticeInversed.get(0,2)==0);
    EXPECT_TRUE(MaticeInversed.get(1,0)==-2);
    EXPECT_TRUE(MaticeInversed.get(1,1)==1);
    EXPECT_TRUE(MaticeInversed.get(1,2)==0);
    EXPECT_TRUE(MaticeInversed.get(2,0)==4);
    EXPECT_TRUE(MaticeInversed.get(2,1)==-2);
    EXPECT_TRUE(MaticeInversed.get(2,2)==1);

    //TESTY PRO NEČTVERCOVOU MATICI
    //COLUMNS>ROW
    Matice = Matrix(2,3);
    EXPECT_ANY_THROW(Matice.inverse());
    //COLUMN<ROW
    Matice = Matrix(3,2);
    EXPECT_ANY_THROW(Matice.inverse());

    //TESTY PRO ČTVERCOVOU MATICI VĚTŠÍ NEŽ 3x3
    Matice = Matrix(4,4);
    EXPECT_ANY_THROW(Matice.inverse());

    //TESTY NA SINGULÁRNÍ MATICI
    Matice = Matrix(2,2);
    values = {{1,2},
              {2,4}};
    ASSERT_TRUE(Matice.set(values));
    EXPECT_ANY_THROW(Matice.inverse());

}




