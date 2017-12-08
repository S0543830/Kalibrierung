#ifndef  _MATH_NA_H_
#define  _MATH_NA_H_

#ifndef M_PI
#define M_PI   3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2   (M_PI/2.0) 
#endif

#ifndef M_PI_4
#define M_PI_4   (M_PI/4.0) 
#endif

#ifndef M_PI_LONG
#define M_PI_LONG 3.1415926535897932384626433832795028841971693993751
#endif

#ifndef M_DEG2RAD
#define M_DEG2RAD (M_PI_LONG/180.0)
#endif

#ifndef M_RAD2DEG
#define M_RAD2DEG (180.0/M_PI_LONG)
#endif

// Standard math-Funktionen einbinden
#include <math.h>

/*********************** Allgemeine Math_Funktionen ***********************/
/* 1 */   double   det(double [3][3]);
/* 2 */   double   Norm(double [3]);
/* 3 */   double   Normieren(double [3], double [3]);
		    double   Senkrecht(double P[3],double P1[3], double P2[3]);
/* 4 */   double   Vektor_Kreuz_Vektor(double[3], double [3], double [3]);
/* 5 */   double   Mult_Matrix_Vektor (double [3][3], double [3], double [3]);
/* 5a*/   double   Mult_Vektor_Vektor ( double dVektor1[3], double dVektor2[3]);
/* 6 */   void     Mult_Matrix_Matrix(double[3][3], double [3][3], double [3][3]);
/* 7 */   void     Winkel_Matrix(char , double, double[3][3]);
/* 8 */   void     Drei_Winkel_Matrix(char [4], double ,  double ,  double , double [3][3]);
/* 9 */   int      Drehmatrix_3Winkel(char Achsen[4], double M[3][3], double w[3][2]);
/* 10*/   void     Drehvektor_Winkel_Matrix(double [3], double , double [3][3]);
/* 11*/   int      Drehmatrix_Drehvektor_Winkel( double [3][3],  double[3],  double*  );
/* 12*/   int      Drehmatrix_Quaternion( double [3][3],  double [4]) ;
/* 13*/   int      Quaternion_Drehmatrix(  double [4], double [3][3]);
/* 14*/   void     Frame_Invers( char [4],  double [6], double [2][6]);
/* 15*/   void     Mult_Frame( char [4],  double [6], double [6], double [6]);
/* 16*/   int      Quadrat_Matrix_Invers(int ,  double **,  double **);
/* 17*/   int      PseudoInvers( int, int,  double **,  double **);
/* 18*/   int      ORI_Typ( char [4] );
/* 19*/   void     MFrame( double HD[4], double F[4][4]) ; 
/* 20*/   void     MULT_MFrame( double F1[4][4], double F2[4][4], double F3[4][4] );
/* 21*/   double   OrthoNormieren( double X[3], double Y[3], int Stufe, double XN[3], double YN[3] );
/* 22*/   double   GeradenAbstand( double dP0_1[3], double dP0_2[3], double dRichtungsVektor_1[3], double dRichtungsVektor_2[3] );
/* 23*/   double   PunktGeradenAbstand( double dPunkt[3], double dP0[3], double dRichtungsVektor[3] );
          double   GeradenAbstandsVektor( double dP0_1[3], double dP0_2[3], double dRichtungsVektor_1[3], double dRichtungsVektor_2[3], double dAbstandsVektor_1_2[3] );
int sign (double d);
int sign (float f);
int sign (int i);

#endif //_MATH_NA_H_
