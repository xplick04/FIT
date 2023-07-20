#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGHT 10240
char delimetr[100];
int delka_delim = 0;
int prvni_radek_sloupec = 0;

int dcols_function(int dcols_from, int dcols_to, char *tabulka);
int icol_function(int icol, char *tabulka);
int dcol_function(int dcol, char *tabulka);
int acol_function(char *tabulka, int acol);
int drow_function(char *tabulka, int drow, int radek);
int drows_function(char *tabulka,int radek,int drows_from,int drows_to);
int first_column_counter(char *tabulka);
int irow_function(int prvni_radek_sloupec, int radek, int irow);
int arow(char *tabulka);
int tolower_function(char *tabulka,int tolower_sloupec);
int toupper_function(char *tabulka,int toupper_sloupec);
int cset_function(char *tabulka, char *cset_nazev, int cset_sloupec);
int contains_function(char *tabulka, char *contains_nazev, int contains_sloupec);
int beginswith_function(char *tabulka, char *beginswith_nazev, int beginswith_sloupec);
int round_function(char *tabulka, int round_sloupec);
int int_function(char *tabulka, int int_sloupec);
int copy_function(char *tabulka, int copy_sloupec1, int copy_sloupec2);
int swap_function(char *tabulka, int swap_sloupec1, int swap_sloupec2);
int move_function(char *tabulka, int move_sloupec1, int move_sloupec2);

int first_column_counter(char *tabulka){//funkce pro spocitani sloupcu v prvnim radku
    int j = 0;
    int i = 0;
    int a = 0;
        while(tabulka[i] != '\0'){//cyklus, ktery projizdi radek z inputu
            while(j<=delka_delim){
                  if(tabulka[i] == delimetr[j]){//srovnava kazde pismeno v delimetru s aktualnim pismenem
                      a++;
                    }
                    else{
                          break;
                      }
            j++;
            }
            j = 0;
            i++;
        }
        prvni_radek_sloupec = a;
    return 0;
}

int icol_function(int icol,char *tabulka){// char* prepisuje mi promenou aniz bych musel vracet
    int i = 0;
    int j = 0;
    int k = 0;
    int sloupec = 1;
    bool flag = false;
    char tabulka2[MAX_LENGHT];

    while(tabulka[i] != '\0'){
        if(icol == 1 && flag == false){
            tabulka2[k] = delimetr[0];
            k++;
            flag = true;
        }
        while(j <= delka_delim){//cykluje dokud nenarazi na znak z delimetru
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  if(sloupec == icol){
                      tabulka2[k] = delimetr[0];
                      k++;
                      tabulka2[k] = delimetr[0];
                  }
                  else{
                      break;
                  }
              }
        j++;
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    if(flag == false && sloupec + 1 == icol){
        tabulka2[k] = delimetr[0];
        tabulka2[k+1] = '\0';
    }
    else if(icol > sloupec+1){//pokud bude zadan vesti sloupec nez je pocet sloupcu
        fprintf(stderr,"byl zadan sloupec, ktery neni");
        return 1;
    }
    else{
        tabulka2[k] = '\0';
    }
    strcpy(tabulka,tabulka2);//zkopiruje upravenou tabulku2 zpet do vstupni tabulky
    tabulka2[0] = '\0';//vymaze tabulku2
    return 0;

}

int dcol_function(int dcol,char *tabulka){//dcol 1 nechava delimetr
    int i = 0;
    int k = 0;
    int j = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;
    bool flag = false;

        while(tabulka[i] != '\0'){//radek cykluje dokud nenarazi na konec radku
            while(j <= delka_delim){//cyklus pro porovnavani charu s chary c retezci delim
                  if(tabulka[i] == delimetr[j]){
                      tabulka[i] = delimetr[0];
                      sloupec++;
                  }
                  else{
                      break;
                  }
            j++;
            }
            j = 0;
            if(dcol == 1 && tabulka[i] == delimetr[0] && flag == false){
                flag = true;
                i++;
            }
            if(sloupec == dcol){
                i++;
            }else{
            tabulka2[k] = tabulka[i];
            k++;
            i++;
            }
        }
        tabulka2[k] = '\0';
        if(dcol > sloupec+1 && tabulka[i] == '\0'){
            fprintf(stderr,"byl zadan sloupec, ktery neni\n");
            return 1;
        }
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    return 0;
}

int dcols_function(int dcols_from,int dcols_to,char *tabulka){
    int i = 0;
    int k = 0;
    int j = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;
    bool flag = false;


    while(tabulka[i] != '\0'){//radek cykluje dokud nenarazi na konec radku
        while(j <= delka_delim){//cyklus pro porovnavani charu s chary c retezci delim
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
              }
              else{
                  break;
              }
        j++;
        }
        j = 0;
        if(sloupec >= dcols_from && sloupec <= dcols_to){
            i++;
        }else{
            if(tabulka[i] == delimetr[0] && dcols_from == 1 && flag == false){
                flag = true;
                i++;
            }
        tabulka2[k] = tabulka[i];
        k++;
        i++;
        }
    }
    if(dcols_to > sloupec+1 && tabulka[i] == '\0'){
        fprintf(stderr,"byl zadan sloupec, ktery neni\n");
        return 1;
    }else{
        tabulka2[k] = '\0';
        strcpy(tabulka,tabulka2);
        tabulka2[0] = '\0';
        return 0;
    }
}

int acol_function(char *tabulka,int acol){// char* prepisuje mi promenou aniz bych musel vracet
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];

    if(acol!=0){
    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    if(tabulka[i] == '\0'){
        tabulka2[k] = delimetr[0];
        k++;
    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    }
    return 0;
}

int drow_function(char *tabulka,int drow, int radek){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];

    if(drow != radek){
        while(tabulka[i] != '\0'){
            while(j <= delka_delim){
                  if(tabulka[i] == delimetr[j]){
                      tabulka[i] = delimetr[0];
                  }
            j++;
            }
            j = 0;
            tabulka2[k] = tabulka[i];
            k++;
            i++;
        }
        tabulka2[k] = '\0';
        strcpy(tabulka,tabulka2);
        tabulka2[0] = '\0';
    }
return 0;
}

int drows_function(char *tabulka,int radek,int drows_from,int drows_to){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    if(drows_from > radek && drows_to < radek){
        while(tabulka[i] != '\0'){
            while(j <= delka_delim){
                  if(tabulka[i] == delimetr[j]){
                      tabulka[i] = delimetr[0];
                  }
            j++;
            }
            j = 0;
            tabulka2[k] = tabulka[i];
            k++;
            i++;
        }
        tabulka2[k] = '\0';
        strcpy(tabulka,tabulka2);
        tabulka2[0] = '\0';
    }
return 0;
}

int irow_function(int prvni_radek_sloupec, int radek, int irow){

    if(irow == radek){
        for(int i = 0; i < prvni_radek_sloupec; i++){
        printf("%c",delimetr[0]);
    }
    printf("\n");
    }
return 0;
}
int arow_function(char *tabulka){
    int j = 0;
    int i = 0;

    while(tabulka[i] != '\0'){
            while(j <= delka_delim){
                  if(tabulka[i] == delimetr[j]){
                      tabulka[i] = delimetr[0];
                  }
            j++;
            }
        j = 0;
        i++;
        }
return 0;
}

int tolower_function(char *tabulka,int tolower_sloupec){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;


    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka2[k] = delimetr[0];
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        if(sloupec == tolower_sloupec && tabulka[i] != delimetr[0]){
            if(tabulka[i] >= 'A' && tabulka[i] <= 'Z'){
                tabulka[i] = tabulka[i] +32;//prevede velke pismeno na male
            }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(sloupec < tolower_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}

int toupper_function(char *tabulka,int toupper_sloupec){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;


    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka[i] = delimetr[0];
                  tabulka2[k] = delimetr[0];
              }
        j++;
        }
        if(sloupec == toupper_sloupec && tabulka[i] != delimetr[0]){
            if(tabulka[i] >= 'a' && tabulka[i] <= 'z'){
                tabulka[i] = tabulka[i] -32;
            }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(sloupec < toupper_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}
int cset_function(char *tabulka,char *cset_nazev,int cset_sloupec){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;
    int delka_cset = strlen(cset_nazev);
    int a = 0;

    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka2[k] = delimetr[0];
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        if(sloupec == cset_sloupec && tabulka[i] != delimetr[0]){
            for(; a < delka_cset; a++){
                tabulka2[k] = cset_nazev[a];
                k++;
                while(tabulka[i] != delimetr[0]){
                    i++;
                }
            }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(sloupec < cset_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;

}

int round_function(char *tabulka,int round_sloupec){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;
    int round = 0;
    int round2 = 0;
    char round3[1];
    bool flag = false;


    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka2[k] = delimetr[0];
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        if(sloupec == round_sloupec){
                if(tabulka[i] == '.' && isdigit(tabulka[i-1])){
                    round = tabulka[i-1] - '0';
                    round2 = tabulka[i+1] - '0';
                    if(round2 >= 5){
                        round++;
                        round3[0] = '0' + round;
                        tabulka2[k-1] = round3[0];
                        flag = true;
                        k--;
                    }
                    while(tabulka[i+1] != delimetr[0]){
                        i++;
                    }
                }

        }
        j = 0;
        if(flag == false){
            tabulka2[k] = tabulka[i];
        }
        k++;
        i++;
        flag = false;

    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    if(sloupec < round_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}

int int_function(char *tabulka,int int_sloupec){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    int sloupec = 1;

    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka2[k] = delimetr[0];
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        if(sloupec == int_sloupec){
                if(tabulka[i] == '.' && isdigit(tabulka[i-1]))
                    while(tabulka[i] != delimetr[0]){
                        i++;
                    }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(sloupec < int_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}

int contains_function(char *tabulka,char *contains_nazev,int contains_sloupec){
    int i = 0;
    int j = 0;
    int sloupec = 1;
    int delka_contains = strlen(contains_nazev);
    int a = 0;
    int pomocna_contains = 0;


    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        if(sloupec == contains_sloupec && tabulka[i] != delimetr[0]){
            while(tabulka[i] == contains_nazev[a]){
                pomocna_contains ++;
                i++;
                a++;
            }
            if(pomocna_contains == delka_contains){//pokud sloupec obsahuje vsechny pozadovane znaky, tak se sloupe vybere
                goto stop;
            }
            else{
                pomocna_contains = 0;
            }
        }
        j = 0;
        i++;
    }
    if(0){
        stop:
        return 2;
    }
    if(sloupec < contains_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else{
    return 0;
    }
}

int beginswith_function(char *tabulka,char *beginswith_nazev,int beginswith_sloupec){
    int i = 0;
    int j = 0;
    int sloupec = 1;
    int delka_beginswith = strlen(beginswith_nazev);
    int a = 0;
    int pomocna_beginswith = 0;

    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  tabulka[i] = delimetr[0];
                  sloupec++;
              }
        j++;
        }
        if(sloupec == beginswith_sloupec  && tabulka[i-1] == delimetr[0] && sloupec != 1){
            while(tabulka[i] == beginswith_nazev[a]){//zkousi, jestli je v sloupci slovo z parametru
                pomocna_beginswith++;
                i++;
                a++;
            }
            if(pomocna_beginswith == delka_beginswith){
                goto stop;
            }
            else{
                pomocna_beginswith = 0;
            }
        }
        else if(sloupec == beginswith_sloupec && beginswith_sloupec == 1 && i == 0){//cyklus pro prvni radek, protoze pred sebou nema delimetr
                while(tabulka[i] == beginswith_nazev[a]){
                    pomocna_beginswith++;
                    i++;
                    a++;
                }
                if(pomocna_beginswith < delka_beginswith){
                    pomocna_beginswith = 0;
                }
                if(pomocna_beginswith == delka_beginswith){
                    goto stop;
                }
        }
        j = 0;
        i++;
    }
    if(0){
        stop:
        return 2;
    }
    if(sloupec < beginswith_sloupec){
        fprintf(stderr, "tento sloupec neexistuje\n\n");
        return 1;
    }
    else{
    return 0;
    }
}

int copy_function(char *tabulka,int copy_sloupec1,int copy_sloupec2){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    char buffer_copy[MAX_LENGHT];
    buffer_copy[0] = '\0';
    int sloupec = 1;
    int a = 0;
    int delka_copy;
    int posledni_sloupec;

        while(tabulka[i] != '\0'){
            while(j <= delka_delim){
                  if(tabulka[i] == delimetr[j]){
                      sloupec++;
                      tabulka[i] = delimetr[0];
                  }
            j++;
            }
            j = 0;
            if(sloupec == copy_sloupec1 && tabulka[i] != delimetr[0]){
                    buffer_copy[a] = tabulka[i];
                    a++;
            }
                buffer_copy[a] = '\0';
            i++;
    }
    posledni_sloupec = sloupec;

    sloupec = 1;
    i = 0;
    a = 0;
    delka_copy = strlen(buffer_copy);

    while(tabulka[i] != '\0'){
          if(tabulka[i] == delimetr[0]){
              sloupec++;
              tabulka2[k] = delimetr[0];
          }
        if(sloupec == copy_sloupec2 && tabulka[i]!=delimetr[0]){
            for(; a < delka_copy; a++){
                    tabulka2[k] = buffer_copy[a];
                    k++;
                while(tabulka[i]!=delimetr[0]){
                    i++;
                }
            }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    if(posledni_sloupec == copy_sloupec2){
        if(tabulka2[k-1] == delimetr[0]){
            tabulka2[k-1] = '\0';
        }
    }
    buffer_copy[0] = 0;
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(posledni_sloupec < copy_sloupec1 || posledni_sloupec < copy_sloupec2){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}

int swap_function(char *tabulka,int swap_sloupec1,int swap_sloupec2){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    char buffer_swap[MAX_LENGHT];
    char buffer_swap2[MAX_LENGHT];
    buffer_swap[0] = '\0';
    buffer_swap2[0] = '\0';
    int sloupec = 1;
    int a = 0;
    int b = 0;
    int delka_swap;
    int delka_swap2;
    int posledni_sloupec;

    while(tabulka[i] != '\0'){
        while(j <= delka_delim){
              if(tabulka[i] == delimetr[j]){
                  sloupec++;
                  tabulka[i] = delimetr[0];
              }
        j++;
        }
        j = 0;
        if(sloupec == swap_sloupec1 && tabulka[i] != delimetr[0]){
                buffer_swap[a] = tabulka[i];
                a++;
        }
            buffer_swap[a] = '\0';

        if(sloupec == swap_sloupec2 && tabulka[i] != delimetr[0]){
                buffer_swap2[b] = tabulka[i];
                b++;
        }
            buffer_swap2[b] = '\0';
        i++;
}

posledni_sloupec = sloupec;

sloupec = 1;
i = 0;
a = 0;
b = 0;

delka_swap = strlen(buffer_swap);
delka_swap2 = strlen(buffer_swap2);

while(tabulka[i] != '\0'){
      if(tabulka[i] == delimetr[0]){
          sloupec++;
          tabulka2[k] = delimetr[0];
      }
      if(sloupec == swap_sloupec1 && tabulka[i] != delimetr[0]){
          for(; b < delka_swap2; b++){
              tabulka2[k] = buffer_swap2[b];
              k++;
              while(tabulka[i] != delimetr[0]){
                  i++;
              }
          }
          sloupec++;
      }

    if(sloupec == swap_sloupec2 && tabulka[i] != delimetr[0]){
        for(; a < delka_swap; a++){
                tabulka2[k] = buffer_swap[a];
                k++;
            while(tabulka[i] != delimetr[0]){
                i++;
            }
        }
        sloupec++;
    }

    tabulka2[k] = tabulka[i];
    k++;
    i++;
}
    if(posledni_sloupec == swap_sloupec1 || posledni_sloupec == swap_sloupec2){
        if(tabulka2[k-1] == delimetr[0]){
            tabulka2[k-1] = '\0';
        }
    }
    buffer_swap[0] = '\0';
    buffer_swap2[0] = '\0';
    tabulka2[k] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(posledni_sloupec < swap_sloupec1 || posledni_sloupec < swap_sloupec2){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;
}

int move_function(char *tabulka, int move_sloupec1, int move_sloupec2){
    int i = 0;
    int j = 0;
    int k = 0;
    char tabulka2[MAX_LENGHT];
    char buffer_move[MAX_LENGHT];
    buffer_move[0] = '\0';
    int sloupec = 1;
    int a = 0;
    int delka_move;
    int posledni_sloupec;

        while(tabulka[i] != '\0'){
            while(j <= delka_delim){
                  if(tabulka[i] == delimetr[j]){
                      sloupec++;
                      tabulka[i] = delimetr[0];
                  }
            j++;
            }
            j = 0;
            if(sloupec == move_sloupec1 && tabulka[i] != delimetr[0]){
                    buffer_move[a] = tabulka[i];
                    a++;
            }
            buffer_move[a] = delimetr[0];
            buffer_move[a+1] = '\0';
            i++;
    }
    posledni_sloupec = sloupec;
    sloupec = 1;
    i = 0;
    a = 0;
    delka_move = strlen(buffer_move);

    while(tabulka[i] != '\0'){
          if(tabulka[i] == delimetr[0]){
              sloupec++;
              tabulka2[k] = delimetr[0];
          }
        if(sloupec == move_sloupec1){
            i++;
            while(tabulka[i-1]!=delimetr[0]){
                i++;
            }
            sloupec++;
        }
        if(sloupec == move_sloupec2 && tabulka[i] != delimetr[0]){
            for(; a < delka_move; a++){
                    tabulka2[k] = buffer_move[a];
                    k++;
            }
        }
        j = 0;
        tabulka2[k] = tabulka[i];
        k++;
        i++;
    }
    if(posledni_sloupec == move_sloupec2){
        if(tabulka2[k-1] == delimetr[0]){
            tabulka2[k-1] = '\0';
        }
    }
    tabulka2[k] = '\0';
    buffer_move[0] = '\0';
    strcpy(tabulka,tabulka2);
    tabulka2[0] = '\0';
    if(posledni_sloupec < move_sloupec1 || posledni_sloupec < move_sloupec2){
        fprintf(stderr, "tento sloupec neexistuje\n");
        return 1;
    }
    else
    return 0;

}

int main(int argc, char *argv[]){

    for(int i = 0; i < argc; i++ ){//delka argumentu < 100
        if(strlen(argv[i]) > 100){
            fprintf(stderr, "zadal jste moc dlouhy argument\n");
            return 1;
        }
    }
  int q=1;
  strcpy(delimetr," ");//zakladne nastaveny na mezeru
  if(argc >= 2){ //zkousi, jestli je dostatecny pocet argumentu pro spusteni funkce
        if(!strcmp(argv[1],"-d")){
          if(!strcmp(argv[2],"irow")||!strcmp(argv[2],"arow")||//kontroluje kdyby bylo zadano -d a zatim nebyl delimetr, ale funkce
           !strcmp(argv[2],"drow")||!strcmp(argv[2],"drows")||!strcmp(argv[2],"icol")||
           !strcmp(argv[2],"acol")||!strcmp(argv[2],"dcol")||!strcmp(argv[2],"dcols")){
           printf("nezadany parametr delimetru\n");
           return 1;
          }
          strcpy(delimetr,argv[2]); //pokud se prvni arg = -d pak druhy je delimetr
          q = 3; // argumenty se budou projizdet po delimetru
        }
        else{
            q = 1;// argumenty se budou projizdet od 1. pozice
        }
  }
  else{
    printf("spatne zadane parametry\n");
    return 1;
  }

 // prvni_delim = delimetr[0];// vystup je prvni delim a delimettr

  delka_delim = strlen(delimetr)-1;//pocita mi delku delimetru
  char pismeno = getchar();
  if(pismeno == EOF){//kontorluje, jestli je vstup pradny
      printf("chyba, prazny vstupni soubor\n");
      return 1;
  }

  int prvni_funkce = 0; //prepne se na 1, pokud narazi na sadu prvnich
  int druha_funkce = 0;
  int treti_funkce = 0;
//inicializace promenych
  int arow = 0;
  int acol = 0;

  int irow = 0;
  int drow = 0;
  int icol = 0;
  int dcol = 0;

  int drows_from = 0;
  int drows_to = 0;
  int dcols_from = 0;
  int dcols_to = 0;

  int rows_from = 0;
  int rows_to = 0;

  char *beginswith_nazev;
  int beginswith_sloupec = 0;

  char *contains_nazev;
  int contains_sloupec = 0;

  char *cset_nazev;
  int cset_sloupec = 0;

  int tolower_sloupec = 0;
  int toupper_sloupec = 0;

  int round_sloupec = 0;

  int int_sloupec = 0;

  int copy_sloupec1 = 0;
  int copy_sloupec2 = 0;

  int swap_sloupec1 = 0;
  int swap_sloupec2 = 0;

  int move_sloupec1 = 0;
  int move_sloupec2 = 0;

 while(q < argc)//cyklus pro zjisteni parametru v argumentu
{
  //AROW
  if(!strcmp(argv[q],"arow")){ //vypsat chybu pokud bude arow>1/funkci provest pouze jednou
    if(arow != 0){ //pokud byl uz predtim zadan tento parametr vyskoci to z programu
      fprintf(stderr,"zadal jste vícekrát funkci arow\n");
      return 1;
    }
    else{
      arow++;//detekuje funkci a prida ji hodnotu
    }
     prvni_funkce = 1;//je tu, aby nesly kombinovat funkce ze selekce a upravy radku
  }
  //ACOL
  if(!strcmp(argv[q],"acol")){
    if(acol != 0){
        fprintf(stderr,"zadal jste vícekrát funkci acol\n");
        return 1;
    }
    else{
      acol++;
    }
     prvni_funkce = 1;
  }
  //IROW
    if(!strcmp(argv[q],"irow")){
      if(irow != 0){
          fprintf(stderr,"zadal jste vícekrát funkci irow\n");
          return 1;
      }
      if(argc >= q+2){//zkousi, jestli se za danym argumentem nachazi aspon 2 dalsi
          q++;
          irow = atoi(argv[q]);
      }
      if(irow <= 0){
          fprintf(stderr,"spatne zadany parametr pro irow\n");
          return 1;
      }
       prvni_funkce = 1;
    }
  //DROW
    if(!strcmp(argv[q],"drow")){
      if(drow != 0){
          fprintf(stderr,"zadal jste vícekrát funkci drow\n");
          return 1;
      }
      if(argc >= q+2){
          q++;
          drow = atoi(argv[q]);
      }
      if(drow <= 0){
          fprintf(stderr,"spatne zadany parametr pro drow\n");
          return 1;
      }
       prvni_funkce = 1;
    }
//ICOL
    if(!strcmp(argv[q],"icol")){
          if(icol != 0){
              fprintf(stderr,"zadal jste vícekrát funkci icol\n");
              return 1;
          }
          else{
              if(argc >= q+2){
                  q++;
                  icol = atoi(argv[q]);
              }
              if(icol <= 0){
                  fprintf(stderr,"spatne zadany parametr pro icol\n");
                  return 1;
              }

          }
           prvni_funkce = 1;
    }
 //DCOL
    if(!strcmp(argv[q],"dcol")){
          if(dcol != 0){
              fprintf(stderr,"zadal jste vicekrat funkci dcol\n");
              return 1;
          }
          if(argc >= q+2){
              q++;
              dcol = atoi(argv[q]);
          }
          if(dcol <= 0){
              fprintf(stderr,"spatne zadany parametr pro dcol\n");
              return 1;
          }
           prvni_funkce = 1;
    }
//DROWS
  if(!strcmp(argv[q],"drows")){
      if(argc >= q+3){//zkousi, jestli se za danym argumentem nachazi aspon 3 dalsi
          q++;
          drows_from = atoi(argv[q]);
          q++;
          drows_to = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci drows\n");
          return 1;
      }
      if(drows_from>drows_to){
          fprintf(stderr,"chybne zadane parametry drows\n");
          return 1;
      }
      if(drows_from <= 0 || drows_to <= 0){
          fprintf(stderr,"spatne zadany parametr pro drows\n");
          return 1;
      }
      if(drows_from == drows_to){
          if(drow != 0){
              fprintf(stderr,"zadal jste vícekrát funkci drow(drow N N=drow)\n");
              return 1;
          }
          else{
              drow = drows_from;
          }
        drows_from = 0;
        drows_to = 0;
      }
       prvni_funkce = 1;
}
//DCOLS
  if(!strcmp(argv[q],"dcols")){
      if(argc >= q+3){
          q++;
          dcols_from = atoi(argv[q]);
          q++;
          dcols_to = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci dcols\n");
          return 1;
      }
      if(dcols_from>dcols_to){
          fprintf(stderr,"chybne zadane parametry dcols\n");
          return 1;
      }
      if(dcols_from <= 0 || dcols_to <= 0){
          fprintf(stderr,"spatne zadany parametr pro dcols\n");
          return 1;
      }
      if(dcols_from == dcols_to){
          if(dcol != 0){
              fprintf(stderr,"zadal jste vícekrát funkci dcol(drow N N=dcol)\n");
              return 1;
          }
          else{
              dcol = dcols_from;
          }
        dcols_from = 0;
        dcols_to = 0;
      }
       prvni_funkce = 1;
  }
//druhe funkce

  if(!strcmp(argv[q],"rows")){
      if(argc >= q+3){
          q++;
          rows_from = atoi(argv[q]);
          q++;
          rows_to = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci rows\n");
          return 1;
      }
      if(rows_from > rows_to){
          fprintf(stderr,"chybne zadane parametry rows\n");
          return 1;
      }
      if(rows_from <= 0 || rows_to <= 0){
          fprintf(stderr,"spatne zadany parametr pro rows\n");
          return 1;
      }
      druha_funkce ++;
  }

  if(!strcmp(argv[q],"beginswith")){
      if(argc >= q+3){
          q++;
          beginswith_sloupec = atoi(argv[q]);
          q++;
          beginswith_nazev = argv[q];
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci beginswith\n");
          return 1;
      }
      if( beginswith_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro beginswith\n");
          return 1;
      }
      druha_funkce ++;
  }

  if(!strcmp(argv[q],"contains")){
      if(argc >= q+3){
          q++;
          contains_sloupec = atoi(argv[q]);
          q++;
          contains_nazev = argv[q];
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci contains\n");
          return 1;
      }
      if( contains_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro contains\n");
          return 1;
      }
      druha_funkce ++;
  }

  if(!strcmp(argv[q],"cset")){
      if(argc >= q+3){
          q++;
          cset_sloupec = atoi(argv[q]);
          q++;
          cset_nazev = argv[q];
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci cset\n");
          return 1;
      }
      if( cset_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro cset\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"tolower")){
      if(argc >= q+2){
          q++;
          tolower_sloupec = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci tolower\n");
          return 1;
      }
      if( tolower_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro tolower\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"toupper")){
      if(argc >= q+2){
          q++;
          toupper_sloupec = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci toupper\n");
          return 1;
      }
      if( toupper_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro toupper\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"round")){
      if(argc >= q+2){
          q++;
          round_sloupec = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci round\n");
          return 1;
      }
      if( round_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro round\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"int")){
      if(argc >= q+2){
          q++;
          int_sloupec = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci int\n");
          return 1;
      }
      if( int_sloupec <= 0){
          fprintf(stderr,"spatne zadany parametr pro int\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"copy")){
      if(argc >= q+3){
          q++;
          copy_sloupec1 = atoi(argv[q]);
          q++;
          copy_sloupec2 = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci copy\n");
          return 1;
      }
      if( copy_sloupec1 <= 0 || copy_sloupec2 <= 0){
          fprintf(stderr,"spatne zadany parametr pro copy\n");
          return 1;
      }
      treti_funkce++;//selekce radku, selekce radku je povolona jen jedna na jeden prikaz
  }

  if(!strcmp(argv[q],"swap")){
      if(argc >= q+3){
          q++;
          swap_sloupec1 = atoi(argv[q]);
          q++;
          swap_sloupec2 = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci swap\n");
          return 1;
      }
      if( swap_sloupec1 <= 0 || swap_sloupec2 <= 0){
          fprintf(stderr,"spatne zadany parametr pro swap\n");
          return 1;
      }
      treti_funkce++;
  }

  if(!strcmp(argv[q],"move")){
      if(argc >= q+3){
          q++;
          move_sloupec1 = atoi(argv[q]);
          q++;
          move_sloupec2 = atoi(argv[q]);
      }
      else{
          fprintf(stderr,"nezadany parametr pro funkci move\n");
          return 1;
      }
      if( move_sloupec1 <= 0 || move_sloupec2 <= 0){
          fprintf(stderr,"spatne zadany parametr pro move\n");
          return 1;
      }
      treti_funkce++;
  }
  q++;
}
if((prvni_funkce > 0 && druha_funkce > 0 ) || (prvni_funkce != 0 && treti_funkce != 0)){//zakazuje kombinaci selekce radku a upravy tabulky
    fprintf(stderr, "nemuzete kombinovat tyto dve funkce\n");
    return 1;
}
if(druha_funkce > 1){//zakauje vice prikazu v uprave tabulky
    fprintf(stderr, "nemuzete kombinovat vice funkci pro selekci radku\n");
    return 1;
}
if(treti_funkce > 1){//zakazuje vice prikazu zpracovavani dat
    fprintf(stderr, "nemuzete kombinovat vice funkci pro zpracovavani dat\n");
    return 1;
}




char tabulka[MAX_LENGHT];
int j = 0;
int radek = 0;
int pomocna_promena = 0;

//cyklus projizdi vstupni tabulku po radcich a nasledne na nich provadi zmeny
if(prvni_funkce == 1){//spusti cyklus, pokud se v argumentu nachazi jedna z prvnich funkci(uprava tabulky)
   while(pismeno != EOF){//cyklus projizdi po radku a upresnuje poradi provedeni funkci
        tabulka[j] = pismeno;
            if(pismeno == '\n' || tabulka[j] == EOF){
                radek++;
                tabulka[j] = '\0';//vlozi nakonec radku prazdny znak

                if(dcols_from <= icol && dcols_to>=icol && dcols_from != 0 && dcols_to != 0){
                    fprintf(stderr,"icol je kolizni s dcols\n");
                    return 1;
                }
                if(dcols_from <= dcol && dcols_to>=dcol && dcols_from != 0 && dcols_to != 0){
                    fprintf(stderr,"dcol je kolizni s dcols\n");
                    return 1;
                }
                if(icol == dcol && dcol != 0){
                    fprintf(stderr,"dcol je kolizni s icol\n");
                    return 1;
                }

                    if(dcols_from > icol && dcols_to > dcol){//upresni poradni provadeni funkci,provadi se od zadu, od nejvetsiho cisla
                        if(dcol > icol){
                            if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                                return 1;
                            }
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                        if(dcol < icol){
                            if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                                return 1;
                            }
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                        else{
                            if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                    }
                    if(dcols_from < icol && dcols_to < dcol){
                        if(dcol > icol){
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                            if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                        if(dcol < icol){
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                    }
                    if(dcols_from > icol && dcols_to < dcol){
                        if(dcol_function(dcol,tabulka) == 1){
                            return 1;
                        }
                        if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                            return 1;
                        }
                        if(icol_function(icol,tabulka) == 1){
                            return 1;
                        }
                        acol_function(tabulka, acol);
                    }
                    if(dcols_from < icol && dcols_to > dcol){
                        if(icol_function(icol,tabulka) == 1){
                            return 1;
                        }
                        if(dcols_function(dcols_from,dcols_to,tabulka) == 1){
                            return 1;
                        }
                        if(dcol_function(dcol,tabulka) == 1){
                            return 1;
                        }
                        acol_function(tabulka, acol);
                    }
                    if(dcols_from == 0 && dcols_to == 0){
                        if(dcol > icol){
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                        acol_function(tabulka, acol);
                        }
                        else if(dcol < icol){
                            if(icol_function(icol,tabulka) == 1){
                                return 1;
                            }
                            if(dcol_function(dcol,tabulka) == 1){
                                return 1;
                            }
                            acol_function(tabulka, acol);
                        }
                    }
                    if(dcols_from == 0 && dcols_to == 0 && dcol == 0 && icol == 0){
                        acol_function(tabulka, acol);
                    }

                    //mozne kolize
                    if(drows_from <= irow && drows_to >= irow && drows_from != 0 && drows_to != 0){
                        fprintf(stderr,"irow je kolizni s drows\n");
                        return 1;
                    }
                    if(drows_from <= drow && drows_to >= drow && drows_from != 0 && drows_to != 0){
                        fprintf(stderr,"drow je kolizni s drows\n");
                        return 1;
                    }
                    if(irow == drow && drow != 0){
                        fprintf(stderr,"drow je kolizni s irow\n");
                        return 1;
                    }
                    //rows
                    if(pomocna_promena == 0){
                    first_column_counter(tabulka);
                    pomocna_promena++;
                    }
                    if(drow < drows_from && drows_to < irow && drows_from != 0 && drows_to != 0){
                    irow_function(prvni_radek_sloupec, radek, irow);
                    drows_function(tabulka, radek, drows_from, drows_to);
                    drow_function(tabulka,radek, drow);
                    }
                    if(drow > drows_from && drows_to > irow && drows_from != 0 && drows_to != 0){
                    drow_function(tabulka,radek, drow);
                    drows_function(tabulka, radek, drows_from, drows_to);
                    irow_function(prvni_radek_sloupec, radek, irow);
                    }
                    if(drow > drows_from && drows_to < irow && drows_from != 0 && drows_to != 0){
                        if(drow > irow){
                            drow_function(tabulka,radek, drow);
                            irow_function(prvni_radek_sloupec, radek, irow);
                            drows_function(tabulka, radek, drows_from, drows_to);
                        }
                        if(drow < irow){
                            irow_function(prvni_radek_sloupec, radek, irow);
                            drow_function(tabulka,radek, drow);
                            drows_function(tabulka, radek, drows_from, drows_to);
                        }
                    }
                    if(drow < drows_from && drows_to > irow && drows_from != 0 && drows_to != 0){
                        drows_function(tabulka, radek, drows_from, drows_to);
                        if(drow > irow){
                            drow_function(tabulka,radek, drow);
                            irow_function(prvni_radek_sloupec, radek, irow);
                        }
                        if(irow > drow){
                            irow_function(prvni_radek_sloupec, radek, irow);
                            drow_function(tabulka,radek, drow);
                        }
                    }
                    if(drows_from == 0 && drows_to == 0){
                        if(drow > irow){
                            drow_function(tabulka,radek, drow);
                            irow_function(prvni_radek_sloupec, radek, irow);
                        }
                        if(irow > drow){
                            irow_function(prvni_radek_sloupec, radek, irow);
                            drow_function(tabulka,radek, drow);
                        }
                    }
                    if(radek == drow || (radek >= drows_from && radek <= drows_to)){//funkce drow a drows
                    j = 0;
                }
                else{
                    j = 0;
                    printf("%s\n",tabulka);
                }
            }//konec radku 1303(if)
            else{
                j++;
            }
            pismeno = getchar();
    }
    if(arow == 1){//arow funkce
    for(int i = 0; i < prvni_radek_sloupec; i++){
    printf("%c",delimetr[0]);
    }
    printf("\n");
    }
}

    if(druha_funkce !=0 || treti_funkce != 0){
        while(pismeno != EOF){//cyklus projizdi po radku
        tabulka[j] = pismeno;
            if(pismeno == '\n'){
            radek++;
            tabulka[j] = '\0';//vlozi nakonec radku prazdny znak
            j = 0;
            }
            else{
            j++;
            }
            if(pismeno=='\n'){
                if(beginswith_sloupec != 0){
                    if(beginswith_function(tabulka,beginswith_nazev,beginswith_sloupec) == 1){
                        return 1;
                    }
                    if(beginswith_function(tabulka,beginswith_nazev,beginswith_sloupec) == 2){
                        if(tolower_sloupec != 0){
                            if(tolower_function(tabulka, tolower_sloupec)==1){
                                return 1;
                            }
                        }
                        if(toupper_sloupec != 0){
                            if(toupper_function(tabulka, toupper_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(cset_sloupec != 0){//nepusti to do funkce pokud nebdue zapnuta
                            if(cset_function(tabulka, cset_nazev, cset_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(round_sloupec != 0){
                            if(round_function(tabulka,round_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(int_sloupec != 0){
                            if(int_function(tabulka, int_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(copy_sloupec1 != 0){
                            if(copy_function(tabulka,copy_sloupec1,copy_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(swap_sloupec1 != 0){
                            if(swap_function(tabulka, swap_sloupec1, swap_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(move_sloupec1 != 0){
                            if(move_function(tabulka, move_sloupec1, move_sloupec2) == 1){
                                return 1;
                            }
                        }
                        printf("%s\n",tabulka);
                    }
                    else{
                        printf("%s\n",tabulka);
                    }
                }
                if(contains_sloupec != 0){
                    if(contains_function(tabulka, contains_nazev, contains_sloupec) == 1){
                        return 1;
                    }
                    if(contains_function(tabulka, contains_nazev, contains_sloupec) == 2){
                        if(tolower_sloupec != 0){
                            if(tolower_function(tabulka, tolower_sloupec)==1){
                                return 1;
                            }
                        }
                        if(toupper_sloupec != 0){
                            if(toupper_function(tabulka, toupper_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(cset_sloupec != 0){//nepusti to do funkce pokud nebdue zapnuta
                            if(cset_function(tabulka, cset_nazev, cset_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(round_sloupec != 0){
                            if(round_function(tabulka,round_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(int_sloupec != 0){
                            if(int_function(tabulka, int_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(copy_sloupec1 != 0){
                            if(copy_function(tabulka,copy_sloupec1,copy_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(swap_sloupec1 != 0){
                            if(swap_function(tabulka, swap_sloupec1, swap_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(move_sloupec1 != 0){
                            if(move_function(tabulka, move_sloupec1, move_sloupec2) == 1){
                                return 1;
                            }
                        }
                        printf("%s\n",tabulka);
                    }
                    else{
                         printf("%s\n",tabulka);
                    }
                }
                if(rows_from!=0){
                    if(radek >= rows_from && radek <= rows_to){
                        if(tolower_sloupec != 0){
                            if(tolower_function(tabulka, tolower_sloupec)==1){//spusti funkci pouze pokud ma nejakou hodnotu
                                return 1;
                            }
                        }
                        if(toupper_sloupec != 0){
                            if(toupper_function(tabulka, toupper_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(cset_sloupec != 0){//nepusti to do funkce pokud nebdue zapnuta
                            if(cset_function(tabulka, cset_nazev, cset_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(round_sloupec != 0){
                            if(round_function(tabulka,round_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(int_sloupec != 0){
                            if(int_function(tabulka, int_sloupec) == 1){
                                return 1;
                            }
                        }
                        if(copy_sloupec1 != 0){
                            if(copy_function(tabulka,copy_sloupec1,copy_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(swap_sloupec1 != 0){
                            if(swap_function(tabulka, swap_sloupec1, swap_sloupec2) == 1){
                                return 1;
                            }
                        }
                        if(move_sloupec1 != 0){
                            if(move_function(tabulka, move_sloupec1, move_sloupec2) == 1){
                                return 1;
                            }
                        }
                        printf("%s\n",tabulka);
                    }
                    else{
                        printf("%s\n",tabulka);
                    }
                }
                if(rows_from == 0 && contains_sloupec == 0 && beginswith_sloupec == 0){
                    if(tolower_sloupec != 0){
                        if(tolower_function(tabulka, tolower_sloupec)==1){
                            return 1;
                        }
                    }
                    if(toupper_sloupec != 0){
                        if(toupper_function(tabulka, toupper_sloupec) == 1){
                            return 1;
                        }
                    }
                    if(cset_sloupec != 0){//nepusti to do funkce pokud nebdue zapnuta
                        if(cset_function(tabulka, cset_nazev, cset_sloupec) == 1){
                            return 1;
                        }
                    }
                    if(round_sloupec != 0){
                        if(round_function(tabulka,round_sloupec) == 1){
                            return 1;
                        }
                    }
                    if(int_sloupec != 0){
                        if(int_function(tabulka, int_sloupec) == 1){
                            return 1;
                        }
                    }
                    if(copy_sloupec1 != 0){
                        if(copy_function(tabulka,copy_sloupec1,copy_sloupec2) == 1){
                            return 1;
                        }
                    }
                    if(swap_sloupec1 != 0){
                        if(swap_function(tabulka, swap_sloupec1, swap_sloupec2) == 1){
                            return 1;
                        }
                    }
                    if(move_sloupec1 != 0){
                        if(move_function(tabulka, move_sloupec1, move_sloupec2) == 1){
                            return 1;
                        }
                    }
                    printf("%s\n",tabulka);//tiskne upraveny radek
                }
            }
        pismeno = getchar();
    }
}
  return 0;
}
