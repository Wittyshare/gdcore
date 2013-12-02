 /*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
 */

#include <stdio.h>

#include "gdBarcode.h"

// Les deux fonctions suivantes sont adaptées de :
// V 2.0.0

// Copyright (C) 2006 (Grandzebu)

// Cette bibliothèque est  libre, vous pouvez la redistribuer et/ou la modifier
// selon les termes de la Licence Publique Générale Amoindrie GNU (GNU LGPL)
// Veuillez charger une copie de la license à l'adresse : http://www.gnu.org/licenses/lgpl.html
// Une traduction non officielle se trouve à l'adresse : http://www.linux-france.org/article/these/licence/lgpl/lgpl_monoblock.html

// This library is free, you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License (GNU LGPL)
// Please download a license copy at : http://www.gnu.org/licenses/lgpl.html


/*
 * Formule : 
 *   x = somme des chiffres pairs   (on compte a partir de 0)
 *   y = somme des chiffres impairs (on compte a partir de 0)
 *   z = x - (3 * y)
 *   m = Multiple de 10 au dessus de z - z exemple si z = 73 (80 - 73 = 7)
 *   on travaille par modulo 2x si le nombre est divisible par 10
*/
int gdBarcode_doEan13Checksum(const char* BufIn, char* BufOut) 
 {
  int i;
  char x;
  long checksum;
  //Vérifier qu'il y a 12 caractères
  //Check for 12 characters
  //Et que ce sont bien des chiffres
  //And they are really digits
  for (i = 0; i < 12; i++)
   {
    x = *(BufIn + i);
    if (x < 48 || x > 57) return 0;
   }
//fprintf(stderr, "bufIn = %s\n", BufIn);
  checksum = 0;
  for (i = 11; i >= 0; i = i - 2) checksum = checksum + *(BufIn + i) - 48;
//fprintf(stderr, "checksum = %ld\n", checksum);
  checksum = checksum * 3;
//fprintf(stderr, "checksum = %ld\n", checksum);
  for (i = 10; i >= 0; i = i - 2) checksum = checksum + *(BufIn + i) - 48;
//fprintf(stderr, "checksum = %ld\n", checksum);
  checksum = (10 - checksum % 10) % 10;
//fprintf(stderr, "checksum = %ld\n", checksum);
  sprintf(BufOut, "%1ld", checksum);
  return 1;
 }

int gdBarcode_toEan13Font(const char* BufIn, unsigned char* BufOut) 
 {
  int i;
  int  tableA;
  long checksum;

  //Calcul de la clé de contrôle
  //Calculation of the checksum
  checksum = 0;
  for (i = 11; i >= 0; i = i - 2) checksum = checksum + *(BufIn + i) - 48;
  checksum = checksum * 3;
  for (i = 10; i >= 0; i = i - 2) checksum = checksum + *(BufIn + i) - 48;
  checksum = (10 - checksum % 10) % 10;
 
  //Le premier chiffre est pris tel quel, le deuxième vient de la table A
  //The first digit is taken just as it is, the second one come from table A
  *BufOut = *BufIn;
  *(BufOut + 1) = *(BufIn + 1) + 17;
  for (i = 2; i <= 6; i++) {
    tableA = 0;
    switch (i) {
    case 2:
      if (*BufIn > 47 && *BufIn < 52) tableA = 1;
      break;
    case 3:
      if (*BufIn == 48 || *BufIn == 52 || *BufIn == 55 || *BufIn == 56) tableA = 1;
      break;
    case 4:
      if (*BufIn == 48 || *BufIn == 49 || *BufIn == 52 || *BufIn == 53 || *BufIn == 57) tableA = 1;
      break;
    case 5:
      if (*BufIn == 48 || *BufIn == 50 || *BufIn == 53 || *BufIn == 54 || *BufIn == 55) tableA = 1;
      break;
    case 6:
      if (*BufIn == 48 || *BufIn == 51 || *BufIn == 54 || *BufIn == 56 || *BufIn == 57) tableA = 1;
    }
    if (tableA) {
      *(BufOut + i) = *(BufIn + i) + 17;
    } else {
      *(BufOut + i) = *(BufIn + i) + 27;
    }
  }
  *(BufOut + 7) = (char) '*'; //Ajout séparateur central / Add middle separator
  for (i = 7; i <= 11; i++) *(BufOut + i + 1) = *(BufIn + i) + 49;
  *(BufOut + 13) = checksum + 97;
  *(BufOut + 14) = (char) '+'; //Ajout de la marque de fin / Add end mark
  return 1;
 }

