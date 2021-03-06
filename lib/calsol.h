#ifndef CALSOL_H
#define CALSOL_H
/*
 * Trouvé ici et exporté en CSV puis forgé par notepad++
 * http://ptaff.ca/soleil/?l1pays=France&l1etat=Haute-Vienne&l1ville=Limoges&l2pays=&l2etat=&l1cityname=Limoges%2C+Haute-Vienne%2C+France&l1ltd=45&l1ltm=50&l1lts=07&l1ltx=N&l1lgd=1&l1lgm=15&l1lgs=45&l1lgx=E&l1tz=0.0&l1dst=&l2cityname=&l2ltd=&l2ltm=&l2lts=&l2ltx=N&l2lgd=&l2lgm=&l2lgs=&l2lgx=E&l2tz=0&l2dst=&year=19&month=02&day=15&lang=fr_CA&go=Voir+le+graphe%21
 * */
const uint8_t DateSol_t[][9] ={
  {20, 1, 24, 7, 26, 16, 48, 9, 22},
  {20, 1, 25, 7, 25, 16, 49, 9, 25},
  {20, 1, 26, 7, 24, 16, 51, 9, 27},
  {20, 1, 27, 7, 23, 16, 52, 9, 30},
  {20, 1, 28, 7, 22, 16, 54, 9, 32},
  {20, 1, 29, 7, 21, 16, 55, 9, 35},
  {20, 1, 30, 7, 20, 16, 57, 9, 37},
  {20, 1, 31, 7, 18, 16, 58, 9, 40},
  {20, 2, 1, 7, 17, 17, 0, 9, 42},
  {20, 2, 2, 7, 16, 17, 1, 9, 45},
  {20, 2, 3, 7, 15, 17, 3, 9, 48},
  {20, 2, 4, 7, 14, 17, 4, 9, 51},
  {20, 2, 5, 7, 12, 17, 6, 9, 53},
  {20, 2, 6, 7, 11, 17, 7, 9, 56},
  {20, 2, 7, 7, 10, 17, 8, 9, 59},
  {20, 2, 8, 7, 8, 17, 10, 10, 2},
  {20, 2, 9, 7, 7, 17, 11, 10, 5},
  {20, 2, 10, 7, 5, 17, 13, 10, 8},
  {20, 2, 11, 7, 4, 17, 14, 10, 10},
  {20, 2, 12, 7, 2, 17, 16, 10, 13},
  {20, 2, 13, 7, 1, 17, 17, 10, 16},
  {20, 2, 14, 6, 59, 17, 19, 10, 19},
  {20, 2, 15, 6, 58, 17, 20, 10, 22},
  {20, 2, 16, 6, 56, 17, 22, 10, 25},
  {20, 2, 17, 6, 55, 17, 23, 10, 28},
  {20, 2, 18, 6, 53, 17, 25, 10, 32},
  {20, 2, 19, 6, 51, 17, 26, 10, 35},
  {20, 2, 20, 6, 50, 17, 28, 10, 38},
  {20, 2, 21, 6, 48, 17, 29, 10, 41},
  {20, 2, 22, 6, 46, 17, 30, 10, 44},
  {20, 2, 23, 6, 45, 17, 32, 10, 47},
  {20, 2, 24, 6, 43, 17, 33, 10, 50},
  {20, 2, 25, 6, 41, 17, 35, 10, 53},
  {20, 2, 26, 6, 40, 17, 36, 10, 57},
  {20, 2, 27, 6, 38, 17, 38, 11, 0},
  {20, 2, 28, 6, 36, 17, 39, 11, 3},
  {20, 2, 29, 6, 34, 17, 40, 11, 6},
  {20, 3, 1, 6, 33, 17, 42, 11, 9},
  {20, 3, 2, 6, 31, 17, 43, 11, 12},
  {20, 3, 3, 6, 29, 17, 45, 11, 16},
  {20, 3, 4, 6, 27, 17, 46, 11, 19},
  {20, 3, 5, 6, 25, 17, 47, 11, 22},
  {20, 3, 6, 6, 23, 17, 49, 11, 25},
  {20, 3, 7, 6, 22, 17, 50, 11, 29},
  {20, 3, 8, 6, 20, 17, 52, 11, 32},
  {20, 3, 9, 6, 18, 17, 53, 11, 35},
  {20, 3, 10, 6, 16, 17, 54, 11, 38},
  {20, 3, 11, 6, 14, 17, 56, 11, 42},
  {20, 3, 12, 6, 12, 17, 57, 11, 45},
  {20, 3, 13, 6, 10, 17, 58, 11, 48},
  {20, 3, 14, 6, 8, 18, 0, 11, 51},
  {20, 3, 15, 6, 6, 18, 1, 11, 55},
  {20, 3, 16, 6, 4, 18, 2, 11, 58},
  {20, 3, 17, 6, 3, 18, 4, 12, 1},
  {20, 3, 18, 6, 1, 18, 5, 12, 4},
  {20, 3, 19, 5, 59, 18, 6, 12, 8},
  {20, 3, 20, 5, 57, 18, 8, 12, 11},
  {20, 3, 21, 5, 55, 18, 9, 12, 14},
  {20, 3, 22, 5, 53, 18, 10, 12, 17},
  {20, 3, 23, 5, 51, 18, 12, 12, 21},
  {20, 3, 24, 5, 49, 18, 13, 12, 24},
  {20, 3, 25, 5, 47, 18, 14, 12, 27},
  {20, 3, 26, 5, 45, 18, 16, 12, 30},
  {20, 3, 27, 5, 43, 18, 17, 12, 34},
  {20, 3, 28, 5, 41, 18, 18, 12, 37},
  {20, 3, 29, 5, 40, 18, 20, 12, 40},
  {20, 3, 30, 5, 38, 18, 21, 12, 43},
  {20, 3, 31, 5, 36, 18, 22, 12, 47},
  {20, 4, 1, 5, 34, 18, 24, 12, 50},
  {20, 4, 2, 5, 32, 18, 25, 12, 53},
  {20, 4, 3, 5, 30, 18, 26, 12, 56},
  {20, 4, 4, 5, 28, 18, 27, 12, 59},
  {20, 4, 5, 5, 26, 18, 29, 13, 3},
  {20, 4, 6, 5, 24, 18, 30, 13, 6},
  {20, 4, 7, 5, 22, 18, 31, 13, 9},
  {20, 4, 8, 5, 21, 18, 33, 13, 12},
  {20, 4, 9, 5, 19, 18, 34, 13, 15},
  {20, 4, 10, 5, 17, 18, 35, 13, 18},
  {20, 4, 11, 5, 15, 18, 37, 13, 22},
  {20, 4, 12, 5, 13, 18, 38, 13, 25},
  {20, 4, 13, 5, 11, 18, 39, 13, 28},
  {20, 4, 14, 5, 10, 18, 41, 13, 31},
  {20, 4, 15, 5, 8, 18, 42, 13, 34},
  {20, 4, 16, 5, 6, 18, 43, 13, 37},
  {20, 4, 17, 5, 4, 18, 45, 13, 40},
  {20, 4, 18, 5, 3, 18, 46, 13, 43},
  {20, 4, 19, 5, 1, 18, 47, 13, 46},
  {20, 4, 20, 4, 59, 18, 48, 13, 49},
  {20, 4, 21, 4, 57, 18, 50, 13, 52},
  {20, 4, 22, 4, 56, 18, 51, 13, 55},
  {20, 4, 23, 4, 54, 18, 52, 13, 58},
  {20, 4, 24, 4, 52, 18, 54, 14, 1},
  {20, 4, 25, 4, 51, 18, 55, 14, 4},
  {20, 4, 26, 4, 49, 18, 56, 14, 7},
  {20, 4, 27, 4, 47, 18, 58, 14, 10},
  {20, 4, 28, 4, 46, 18, 59, 14, 13},
  {20, 4, 29, 4, 44, 19, 0, 14, 16},
  {20, 4, 30, 4, 43, 19, 1, 14, 19},
  {20, 5, 1, 4, 41, 19, 3, 14, 21},
  {20, 5, 2, 4, 40, 19, 4, 14, 24},
  {20, 5, 3, 4, 38, 19, 5, 14, 27},
  {20, 5, 4, 4, 37, 19, 7, 14, 30},
  {20, 5, 5, 4, 35, 19, 8, 14, 32},
  {20, 5, 6, 4, 34, 19, 9, 14, 35},
  {20, 5, 7, 4, 33, 19, 10, 14, 38},
  {20, 5, 8, 4, 31, 19, 12, 14, 40},
  {20, 5, 9, 4, 30, 19, 13, 14, 43},
  {20, 5, 10, 4, 29, 19, 14, 14, 45},
  {20, 5, 11, 4, 27, 19, 15, 14, 48},
  {20, 5, 12, 4, 26, 19, 17, 14, 50},
  {20, 5, 13, 4, 25, 19, 18, 14, 53},
  {20, 5, 14, 4, 24, 19, 19, 14, 55},
  {20, 5, 15, 4, 23, 19, 20, 14, 58},
  {20, 5, 16, 4, 21, 19, 21, 15, 0},
  {20, 5, 17, 4, 20, 19, 22, 15, 2},
  {20, 5, 18, 4, 19, 19, 24, 15, 4},
  {20, 5, 19, 4, 18, 19, 25, 15, 7},
  {20, 5, 20, 4, 17, 19, 26, 15, 9},
  {20, 5, 21, 4, 16, 19, 27, 15, 11},
  {20, 5, 22, 4, 15, 19, 28, 15, 13},
  {20, 5, 23, 4, 14, 19, 29, 15, 15},
  {20, 5, 24, 4, 14, 19, 30, 15, 17},
  {20, 5, 25, 4, 13, 19, 31, 15, 18},
  {20, 5, 26, 4, 12, 19, 32, 15, 20},
  {20, 5, 27, 4, 11, 19, 33, 15, 22},
  {20, 5, 28, 4, 10, 19, 34, 15, 24},
  {20, 5, 29, 4, 10, 19, 35, 15, 25},
  {20, 5, 30, 4, 9, 19, 36, 15, 27},
  {20, 5, 31, 4, 8, 19, 37, 15, 28},
  {20, 6, 1, 4, 8, 19, 38, 15, 30},
  {20, 6, 2, 4, 7, 19, 39, 15, 31},
  {20, 6, 3, 4, 7, 19, 39, 15, 33},
  {20, 6, 4, 4, 6, 19, 40, 15, 34},
  {20, 6, 5, 4, 6, 19, 41, 15, 35},
  {20, 6, 6, 4, 6, 19, 42, 15, 36},
  {20, 6, 7, 4, 5, 19, 42, 15, 37},
  {20, 6, 8, 4, 5, 19, 43, 15, 38},
  {20, 6, 9, 4, 5, 19, 44, 15, 39},
  {20, 6, 10, 4, 5, 19, 44, 15, 40},
  {20, 6, 11, 4, 4, 19, 45, 15, 41},
  {20, 6, 12, 4, 4, 19, 46, 15, 41},
  {20, 6, 13, 4, 4, 19, 46, 15, 42},
  {20, 6, 14, 4, 4, 19, 47, 15, 42},
  {20, 6, 15, 4, 4, 19, 47, 15, 43},
  {20, 6, 16, 4, 4, 19, 47, 15, 43},
  {20, 6, 17, 4, 4, 19, 48, 15, 44},
  {20, 6, 18, 4, 4, 19, 48, 15, 44},
  {20, 6, 19, 4, 4, 19, 48, 15, 44},
  {20, 6, 20, 4, 5, 19, 49, 15, 44},
  {20, 6, 21, 4, 5, 19, 49, 15, 44},
  {20, 6, 22, 4, 5, 19, 49, 15, 44},
  {20, 6, 23, 4, 5, 19, 49, 15, 44},
  {20, 6, 24, 4, 6, 19, 49, 15, 43},
  {20, 6, 25, 4, 6, 19, 49, 15, 43},
  {20, 6, 26, 4, 7, 19, 49, 15, 43},
  {20, 6, 27, 4, 7, 19, 49, 15, 42},
  {20, 6, 28, 4, 7, 19, 49, 15, 42},
  {20, 6, 29, 4, 8, 19, 49, 15, 41},
  {20, 6, 30, 4, 8, 19, 49, 15, 40},
  {20, 7, 1, 4, 9, 19, 49, 15, 40},
  {20, 7, 2, 4, 10, 19, 49, 15, 39},
  {20, 7, 3, 4, 10, 19, 48, 15, 38},
  {20, 7, 4, 4, 11, 19, 48, 15, 37},
  {20, 7, 5, 4, 12, 19, 48, 15, 36},
  {20, 7, 6, 4, 12, 19, 47, 15, 35},
  {20, 7, 7, 4, 13, 19, 47, 15, 34},
  {20, 7, 8, 4, 14, 19, 46, 15, 32},
  {20, 7, 9, 4, 15, 19, 46, 15, 31},
  {20, 7, 10, 4, 16, 19, 45, 15, 30},
  {20, 7, 11, 4, 16, 19, 45, 15, 28},
  {20, 7, 12, 4, 17, 19, 44, 15, 27},
  {20, 7, 13, 4, 18, 19, 43, 15, 25},
  {20, 7, 14, 4, 19, 19, 43, 15, 23},
  {20, 7, 15, 4, 20, 19, 42, 15, 22},
  {20, 7, 16, 4, 21, 19, 41, 15, 20},
  {20, 7, 17, 4, 22, 19, 40, 15, 18},
  {20, 7, 18, 4, 23, 19, 39, 15, 16},
  {20, 7, 19, 4, 24, 19, 39, 15, 14},
  {20, 7, 20, 4, 25, 19, 38, 15, 13},
  {20, 7, 21, 4, 26, 19, 37, 15, 11},
  {20, 7, 22, 4, 27, 19, 36, 15, 8},
  {20, 7, 23, 4, 28, 19, 35, 15, 6},
  {20, 7, 24, 4, 29, 19, 34, 15, 4},
  {20, 7, 25, 4, 30, 19, 32, 15, 2},
  {20, 7, 26, 4, 32, 19, 31, 15, 0},
  {20, 7, 27, 4, 33, 19, 30, 14, 57},
  {20, 7, 28, 4, 34, 19, 29, 14, 55},
  {20, 7, 29, 4, 35, 19, 28, 14, 53},
  {20, 7, 30, 4, 36, 19, 27, 14, 50},
  {20, 7, 31, 4, 37, 19, 25, 14, 48},
  {20, 8, 1, 4, 39, 19, 24, 14, 45},
  {20, 8, 2, 4, 40, 19, 23, 14, 43},
  {20, 8, 3, 4, 41, 19, 21, 14, 40},
  {20, 8, 4, 4, 42, 19, 20, 14, 38},
  {20, 8, 5, 4, 43, 19, 19, 14, 35},
  {20, 8, 6, 4, 44, 19, 17, 14, 33},
  {20, 8, 7, 4, 46, 19, 16, 14, 30},
  {20, 8, 8, 4, 47, 19, 14, 14, 27},
  {20, 8, 9, 4, 48, 19, 13, 14, 24},
  {20, 8, 10, 4, 49, 19, 11, 14, 22},
  {20, 8, 11, 4, 51, 19, 10, 14, 19},
  {20, 8, 12, 4, 52, 19, 8, 14, 16},
  {20, 8, 13, 4, 53, 19, 6, 14, 13},
  {20, 8, 14, 4, 54, 19, 5, 14, 10},
  {20, 8, 15, 4, 56, 19, 3, 14, 8},
  {20, 8, 16, 4, 57, 19, 2, 14, 5},
  {20, 8, 17, 4, 58, 19, 0, 14, 2},
  {20, 8, 18, 4, 59, 18, 58, 13, 59},
  {20, 8, 19, 5, 0, 18, 56, 13, 56},
  {20, 8, 20, 5, 2, 18, 55, 13, 53},
  {20, 8, 21, 5, 3, 18, 53, 13, 50},
  {20, 8, 22, 5, 4, 18, 51, 13, 47},
  {20, 8, 23, 5, 5, 18, 49, 13, 44},
  {20, 8, 24, 5, 7, 18, 48, 13, 41},
  {20, 8, 25, 5, 8, 18, 46, 13, 38},
  {20, 8, 26, 5, 9, 18, 44, 13, 35},
  {20, 8, 27, 5, 10, 18, 42, 13, 32},
  {20, 8, 28, 5, 12, 18, 40, 13, 29},
  {20, 8, 29, 5, 13, 18, 39, 13, 26},
  {20, 8, 30, 5, 14, 18, 37, 13, 23},
  {20, 8, 31, 5, 15, 18, 35, 13, 20},
  {20, 9, 1, 5, 17, 18, 33, 13, 17},
  {20, 9, 2, 5, 18, 18, 31, 13, 13},
  {20, 9, 3, 5, 19, 18, 29, 13, 10},
  {20, 9, 4, 5, 20, 18, 27, 13, 7},
  {20, 9, 5, 5, 21, 18, 25, 13, 4},
  {20, 9, 6, 5, 23, 18, 24, 13, 1},
  {20, 9, 7, 5, 24, 18, 22, 12, 58},
  {20, 9, 8, 5, 25, 18, 20, 12, 55},
  {20, 9, 9, 5, 26, 18, 18, 12, 51},
  {20, 9, 10, 5, 28, 18, 16, 12, 48},
  {20, 9, 11, 5, 29, 18, 14, 12, 45},
  {20, 9, 12, 5, 30, 18, 12, 12, 42},
  {20, 9, 13, 5, 31, 18, 10, 12, 39},
  {20, 9, 14, 5, 33, 18, 8, 12, 35},
  {20, 9, 15, 5, 34, 18, 6, 12, 32},
  {20, 9, 16, 5, 35, 18, 4, 12, 29},
  {20, 9, 17, 5, 36, 18, 2, 12, 26},
  {20, 9, 18, 5, 38, 18, 0, 12, 23},
  {20, 9, 19, 5, 39, 17, 58, 12, 20},
  {20, 9, 20, 5, 40, 17, 56, 12, 16},
  {20, 9, 21, 5, 41, 17, 54, 12, 13},
  {20, 9, 22, 5, 43, 17, 52, 12, 10},
  {20, 9, 23, 5, 44, 17, 50, 12, 7},
  {20, 9, 24, 5, 45, 17, 49, 12, 3},
  {20, 9, 25, 5, 46, 17, 47, 12, 0},
  {20, 9, 26, 5, 48, 17, 45, 11, 57},
  {20, 9, 27, 5, 49, 17, 43, 11, 54},
  {20, 9, 28, 5, 50, 17, 41, 11, 51},
  {20, 9, 29, 5, 51, 17, 39, 11, 47},
  {20, 9, 30, 5, 53, 17, 37, 11, 44},
  {20, 10, 1, 5, 54, 17, 35, 11, 41},
  {20, 10, 2, 5, 55, 17, 33, 11, 38},
  {20, 10, 3, 5, 56, 17, 31, 11, 35},
  {20, 10, 4, 5, 58, 17, 29, 11, 31},
  {20, 10, 5, 5, 59, 17, 27, 11, 28},
  {20, 10, 6, 6, 0, 17, 25, 11, 25},
  {20, 10, 7, 6, 2, 17, 24, 11, 22},
  {20, 10, 8, 6, 3, 17, 22, 11, 19},
  {20, 10, 9, 6, 4, 17, 20, 11, 16},
  {20, 10, 10, 6, 6, 17, 18, 11, 12},
  {20, 10, 11, 6, 7, 17, 16, 11, 9},
  {20, 10, 12, 6, 8, 17, 14, 11, 6},
  {20, 10, 13, 6, 10, 17, 13, 11, 3},
  {20, 10, 14, 6, 11, 17, 11, 11, 0},
  {20, 10, 15, 6, 12, 17, 9, 10, 57},
  {20, 10, 16, 6, 14, 17, 7, 10, 54},
  {20, 10, 17, 6, 15, 17, 5, 10, 50},
  {20, 10, 18, 6, 16, 17, 4, 10, 47},
  {20, 10, 19, 6, 18, 17, 2, 10, 44},
  {20, 10, 20, 6, 19, 17, 0, 10, 41},
  {20, 10, 21, 6, 20, 16, 59, 10, 38},
  {20, 10, 22, 6, 22, 16, 57, 10, 35},
  {20, 10, 23, 6, 23, 16, 55, 10, 32},
  {20, 10, 24, 6, 25, 16, 54, 10, 29},
  {20, 10, 25, 6, 26, 16, 52, 10, 26},
  {20, 10, 26, 6, 27, 16, 50, 10, 23},
  {20, 10, 27, 6, 29, 16, 49, 10, 20},
  {20, 10, 28, 6, 30, 16, 47, 10, 17},
  {20, 10, 29, 6, 32, 16, 46, 10, 14},
  {20, 10, 30, 6, 33, 16, 44, 10, 11},
  {20, 10, 31, 6, 34, 16, 43, 10, 8},
  {20, 11, 1, 6, 36, 16, 41, 10, 5},
  {20, 11, 2, 6, 37, 16, 40, 10, 3},
  {20, 11, 3, 6, 39, 16, 38, 10, 0},
  {20, 11, 4, 6, 40, 16, 37, 9, 57},
  {20, 11, 5, 6, 41, 16, 36, 9, 54},
  {20, 11, 6, 6, 43, 16, 34, 9, 51},
  {20, 11, 7, 6, 44, 16, 33, 9, 49},
  {20, 11, 8, 6, 46, 16, 32, 9, 46},
  {20, 11, 9, 6, 47, 16, 30, 9, 43},
  {20, 11, 10, 6, 49, 16, 29, 9, 41},
  {20, 11, 11, 6, 50, 16, 28, 9, 38},
  {20, 11, 12, 6, 51, 16, 27, 9, 36},
  {20, 11, 13, 6, 53, 16, 26, 9, 33},
  {20, 11, 14, 6, 54, 16, 25, 9, 31},
  {20, 11, 15, 6, 56, 16, 24, 9, 28},
  {20, 11, 16, 6, 57, 16, 23, 9, 26},
  {20, 11, 17, 6, 58, 16, 22, 9, 23},
  {20, 11, 18, 7, 0, 16, 21, 9, 21},
  {20, 11, 19, 7, 1, 16, 20, 9, 19},
  {20, 11, 20, 7, 2, 16, 19, 9, 17},
  {20, 11, 21, 7, 4, 16, 18, 9, 14},
  {20, 11, 22, 7, 5, 16, 17, 9, 12},
  {20, 11, 23, 7, 6, 16, 17, 9, 10},
  {20, 11, 24, 7, 8, 16, 16, 9, 8},
  {20, 11, 25, 7, 9, 16, 15, 9, 6},
  {20, 11, 26, 7, 10, 16, 15, 9, 4},
  {20, 11, 27, 7, 11, 16, 14, 9, 3},
  {20, 11, 28, 7, 13, 16, 13, 9, 1},
  {20, 11, 29, 7, 14, 16, 13, 8, 59},
  {20, 11, 30, 7, 15, 16, 12, 8, 57},
  {20, 12, 1, 7, 16, 16, 12, 8, 56},
  {20, 12, 2, 7, 17, 16, 12, 8, 54},
  {20, 12, 3, 7, 18, 16, 11, 8, 53},
  {20, 12, 4, 7, 20, 16, 11, 8, 51},
  {20, 12, 5, 7, 21, 16, 11, 8, 50},
  {20, 12, 6, 7, 22, 16, 11, 8, 49},
  {20, 12, 7, 7, 23, 16, 10, 8, 48},
  {20, 12, 8, 7, 24, 16, 10, 8, 47},
  {20, 12, 9, 7, 25, 16, 10, 8, 46},
  {20, 12, 10, 7, 26, 16, 10, 8, 45},
  {20, 12, 11, 7, 26, 16, 10, 8, 44},
  {20, 12, 12, 7, 27, 16, 10, 8, 43},
  {20, 12, 13, 7, 28, 16, 11, 8, 42},
  {20, 12, 14, 7, 29, 16, 11, 8, 42},
  {20, 12, 15, 7, 30, 16, 11, 8, 41},
  {20, 12, 16, 7, 30, 16, 11, 8, 41},
  {20, 12, 17, 7, 31, 16, 11, 8, 40},
  {20, 12, 18, 7, 32, 16, 12, 8, 40},
  {20, 12, 19, 7, 32, 16, 12, 8, 40},
  {20, 12, 20, 7, 33, 16, 13, 8, 40},
  {20, 12, 21, 7, 33, 16, 13, 8, 40},
  {20, 12, 22, 7, 34, 16, 14, 8, 40},
  {20, 12, 23, 7, 34, 16, 14, 8, 40},
  {20, 12, 24, 7, 35, 16, 15, 8, 40},
  {20, 12, 25, 7, 35, 16, 15, 8, 40},
  {20, 12, 26, 7, 35, 16, 16, 8, 41},
  {20, 12, 27, 7, 36, 16, 17, 8, 41},
  {20, 12, 28, 7, 36, 16, 18, 8, 42},
  {20, 12, 29, 7, 36, 16, 18, 8, 43},
  {20, 12, 30, 7, 36, 16, 19, 8, 43},
  {20, 12, 31, 8, 36, 17, 20, 8, 44}
};

#endif // CALSOL_H
