/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
   if(x >= 0 && x < width && y >= 0 && y < height){//hlida frame buffer
     return frame_buffer[y*width + x];
   }
   else{
     return COLOR_BLACK; //vraci barvu (0, 0, 0)
   }
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
  if(x >= 0 && x < width && y >= 0 && y < height){//hlida frame buffer
    frame_buffer[y*width + x] = color;
  }
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
    S_RGBA tmp;
    int intenzita;
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
          tmp = getPixel(x,y);
          intenzita = ROUND(tmp.red*0.299 + tmp.blue*0.587 + tmp.green*0.114);
          tmp.red = intenzita;
          tmp.green = intenzita;
          tmp.blue = intenzita;
          putPixel(x,y,tmp);
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
      grayScale();
      S_RGBA tmp;
      int a, b;  //pro matici
      for(int x = 0; x < width; x++){
          for(int y = 0; y < height; y++){
              tmp = getPixel(x,y);
              a = x % M_SIDE;
              b = y % M_SIDE;
              if(tmp.blue > M[a + b*M_SIDE]){
                tmp = COLOR_WHITE;
                putPixel(x,y,tmp);
              }else{
                tmp = COLOR_BLACK;
                putPixel(x,y,tmp);
              }
          }
      }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution()
{

}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni.
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/
