#include <stdlib.h> 
#include <string.h>

#include "math_na.h"

//****************************************************************************
// 1. Allgemeine Math_Funktionen
//****************************************************************************

//  ---------------------------------------------------------------------------
/* 1.1 */   double   det(double M[3][3])
//  ---------------------------------------------------------------------------
{ 
   return ( M[0][0]*M[1][1]*M[2][2] + M[0][1]*M[1][2]*M[2][0] + M[0][2]*M[1][0]*M[2][1]
           -M[0][0]*M[1][2]*M[2][1] - M[0][1]*M[1][0]*M[2][2] - M[0][2]*M[1][1]*M[2][0] );
}

//  ---------------------------------------------------------------------------
/* 1.2 */   double   Norm (double P[3])
//  ---------------------------------------------------------------------------
{ 
   return (sqrt(P[0]*P[0] + P[1]*P[1] + P[2]*P[2]));
}

//  ---------------------------------------------------------------------------
/* 1.3*/    double   Normieren (double P1[3], double P2[3])
//  ---------------------------------------------------------------------------
// P2 kann neues P1 sein;  Normieren (P,P) also erlaubt.
{ 
   double Betrag;


   Betrag = Norm(P1);

   if (Betrag > 0)
   {
         P2[0] = P1[0] / Betrag ;
         P2[1] = P1[1] / Betrag ;
         P2[2] = P1[2] / Betrag ;
   }
   else
   {
         P2[0] = P1[0];
         P2[1] = P1[1];
         P2[2] = P1[2];
   }

   return (Betrag);
}
//  ---------------------------------------------------------------------------
 double Senkrecht(double P[3],double P1[3], double P2[3])
//  ---------------------------------------------------------------------------
{
    // Erzeugung eines Dreibeins aus einem Vektor P 

    // P1 soll senkreht auf P stehen
    P1[0] =  P[1] + P[2];
    P1[1] = -P[0] - P[2];
    P1[2] = -P[0] + P[1];

    // P2 soll senkrecht zu P und P1 sein
    Vektor_Kreuz_Vektor( P, P1, P2 );

    // Rückgabe muss 0 sein, sonst Vektoren nicht senkrecht zueinander
    return( sqrt( P[0]*P1[0]+P[1]*P1[1]+P[2]*P1[2])
           +sqrt( P[0]*P2[0]+P[1]*P2[1]+P[2]*P2[2])
           +sqrt(P2[0]*P1[0]+P2[1]*P1[1]+P2[2]*P1[2]));
}


//  ---------------------------------------------------------------------------
/* 1.4 */   double   Vektor_Kreuz_Vektor(double A[3], double B[3], double C[3])
//  ---------------------------------------------------------------------------
// C kann auch als neu gesetzten  Eingangsvektor gewaehlt werden
// Vektor_Kreuz_Vektor(A,B,A) und Vektor_Kreuz_Vektor(A,B,B) also erlaubt
{ 
   double CC[2];

   CC[0] = A[1] * B[2] - A[2] * B[1];
   CC[1] = A[2] * B[0] - A[0] * B[2];
   //CC[2] = A[0] * B[1] - A[1] * B[0];
   C[2] = A[0] * B[1] - A[1] * B[0];

   C[0] = CC[0];
   C[1] = CC[1];
   //C[2] = CC[2];

   return Norm(C);
}

//  ---------------------------------------------------------------------------
/* 1.5 */   double   Mult_Matrix_Vektor (double M[3][3], double P1[3], double P2[3])
//  ---------------------------------------------------------------------------
// P2 kann neues P1 sein;  Mult_Matrix_Vektor ( M,P,P) also erlaubt.
{
   int j;
   double P[3];

   P[0] = 0;
   P[1] = 0;
   P[2] = 0;
   
   for ( j = 0 ; j < 3 ; j++ )
   {
      P[0] += M[0][j] * P1[j] ;
      P[1] += M[1][j] * P1[j] ;
      P[2] += M[2][j] * P1[j] ;
   }
   
   P2[0] = P[0] ;
   P2[1] = P[1] ;
   P2[2] = P[2] ;

   return (Norm(P));
}
//  ---------------------------------------------------------------------------
/* 1.5a*/   double   Mult_Vektor_Vektor ( double dVektor1[3], double dVektor2[3] )
//  ---------------------------------------------------------------------------
{
   return ( dVektor1[0] * dVektor2[0] + dVektor1[1] * dVektor2[1] + dVektor1[2] * dVektor2[2] );
}

//  ---------------------------------------------------------------------------
/* 1.6 */   void     Mult_Matrix_Matrix(double D1[3][3], double D2[3][3], double D3[3][3])
//---------------------------------------------------------------------------
// Ergebis D3 kann auch  neue gesetzte Eingangsmatrix sein:z.B.  MULT_Matrix_Matrix(D1,D2,D1)
{ 
   int i, j;
   double D[3][3];

   for (i = 0;i < 3;i++)
   {
      for (j = 0;j < 3;j++)
      {
         D[i][j] = 0;

         D[i][j] += D1[i][0] * D2[0][j];
         D[i][j] += D1[i][1] * D2[1][j];
         D[i][j] += D1[i][2] * D2[2][j];
      }
   }

   for (i = 0;i < 3;i++)
   {
      D3[i][0] = D[i][0];
      D3[i][1] = D[i][1];
      D3[i][2] = D[i][2];
   }
}

//  ---------------------------------------------------------------------------
/* 1.7 */   void     Winkel_Matrix(char Achse, double W, double D[3][3])
//---------------------------------------------------------------------------
{ double s, c;

   s = sin(M_PI * W / 180);
   c = cos(M_PI * W / 180);

   if (Achse == 'x')
   {
      D[0][0] = 1;
      D[0][1] = 0;
      D[0][2] = 0;
      D[1][0] = 0;
      D[1][1] =
         c;
      D[1][2] = -s;
      D[2][0] = 0;
      D[2][1] = s;
      D[2][2] = c;
   }

   if (Achse == 'y')
   {
      D[0][0] = c;
      D[0][1] = 0;
      D[0][2] = s;
      D[1][0] = 0;
      D[1][1] = 1;
      D[1][2] = 0;
      D[2][0] = -s;
      D[2][1] = 0;
      D[2][2] = c;
   }

   if (Achse == 'z')
   {
      D[0][0] = c;
      D[0][1] = -s;
      D[0][2] = 0;
      D[1][0] = s;
      D[1][1] = c;
      D[1][2] = 0;
      D[2][0] = 0;
      D[2][1] = 0;
      D[2][2] = 1;
   }
}

//  ---------------------------------------------------------------------------
/* 1.8 */   void     Drei_Winkel_Matrix(char Achsen[4], double A,  double B,  double C, double D[3][3])
//---------------------------------------------------------------------------
{ 
   double D1[3][3], D2[3][3];

   Winkel_Matrix(Achsen[0], A, D);
   Winkel_Matrix(Achsen[1], B, D1);
   Mult_Matrix_Matrix(D, D1, D2);
   Winkel_Matrix(Achsen[2], C, D1);
   Mult_Matrix_Matrix(D2, D1, D);
}

//  ---------------------------------------------------------------------------
/* 1.9 */   int      Drehmatrix_3Winkel(char Achsen[4], double M[3][3], double w[3][2])
//-------------------------------------------------------------------------------
{ // Wert=0; wenn nur zwei Lösungen eindeutig,
   // sonst unendlich viele Lösungen j:  w[0][j]+ Wert*w[2][j] = const.
   // Ausgegeben werden :  w[0][0] für w[2][0]=0
   //                      w[0][1]= w[0][0]- Wert*w[2][1]  für  w[2][1]=100
   int i, j, H[3], d123, n = 0, v;
   double s= 0.0, c = 0.0, sB, cB;
   char Typ[10] = "exakt" ;

   v = 0;

   for (i = 0;i < 3;i++)
      H[i] = 0;

   for (i = 0;i < 3;i++)
   {
      if (Achsen[i] == 'x')
         H[i] = 0;

      if (Achsen[i] == 'y')
         H[i] = 1;

      if (Achsen[i] == 'z')
         H[i] = 2;
   }

   for (j = 0;j < 2;j++)
      for (i = 0;i < 3;i++)
         w[i][j] = 99999.99;

   if (Achsen[0] == Achsen[2])
   {
      cB = M[ H[0]][H[0]];

      if (cB < -1)
      {
         if (cB > -1.000001)
            cB = -1;
         else
            return ( -11);
      }

      if (cB > 1)
      {
         if (cB < 1.000001)
            cB = 1;
         else
            return ( -10);
      }

      sB = sqrt(fabs(1 - cB * cB));
      w[1][0] = 180 * atan2( sB, cB) / M_PI;
      w[1][1] = 180 * atan2( -sB, cB) / M_PI;

      if (sB == 0 && strcmp(Typ, "exakt") != 0)
      {
         w[1][0] += 0.0001;
         w[1][1] += 0.0001;
         sB = sin(M_PI * w[1][0] / 180);
         cB = cos(M_PI * w[1][0] / 180);
      }

      if (sB > 0)
      {
         s = sB * M[ H[1]][H[0]];

         if (H[0] == 0)
         {
            if (H[1] == 1)
               c = -sB * M[2][H[0]];
            else
               c = sB * M[1][H[0]];
         }

         if (H[0] == 1)
         {
            if (H[1] == 0)
               c = sB * M[2][H[0]];
            else
               c = -sB * M[0][H[0]];
         }

         if (H[0] == 2)
         {
            if (H[1] == 0)
               c = -sB * M[1][H[0]];
            else
               c = sB * M[0][H[0]];
         }

         w[0][0] = 180 * atan2( s, c) / M_PI;
         w[0][1] = 180 * atan2( -s, -c) / M_PI;
         s = sB * M[H[0]][H[1]];

         if (H[0] == 0)
         {
            if (H[1] == 1)
               c = + sB * M[H[0]][2];
            else
               c = -sB * M[H[0]][1];
         }

         if (H[0] == 1)
         {
            if (H[1] == 0)
               c = -sB * M[H[0]][2];
            else
               c = sB * M[H[0]][0];
         }

         if (H[0] == 2)
         {
            if (H[1] == 0)
               c = sB * M[H[0]][1];
            else
               c = -sB * M[H[0]][0];
         }

         w[2][0] = 180 * atan2( s, c) / M_PI;
         w[2][1] = 180 * atan2( -s, -c) / M_PI;
      }

      else
      {
         if (H[0] == 0)
         {
            if (H[1] == 1)
            {
               n = 2;
               v = 1;
            }

            if (H[1] == 2)
            {
               n = 1;
               v = -1;
            }
         }

         if (H[0] == 1)
         {
            if (H[1] == 0)
            {
               n = 2;
               v = -1;
            }

            if (H[1] == 2)
            {
               n = 0;
               v = 1;
            }
         }

         if (H[0] == 2)
         {
            if (H[1] == 0)
            {
               n = 1;
               v = 1;
            }

            if (H[1] == 1)
            {
               n = 0;
               v = -1;
            }
         }

         w[2][0] = 0;
         w[0][0] = 180 * atan2( v * M[n][H[1]], M[H[1]][H[1]]) / M_PI;

         v = (int)cB;
         w[2][1] = 100;
         w[0][1] = w[0][0] - v * w[2][1];
      }


   }

   else
   {
      if ( (H[0] == 0 && H[1] == 1 && H[2] == 2)
            || (H[0] == 1 && H[1] == 2 && H[2] == 0)
            || (H[0] == 2 && H[1] == 0 && H[2] == 1) )
         d123 = 1;
      else
         d123 = -1;


      sB = d123 * M[ H[0]][H[2]];

      if (sB < -1)
      {
         if (sB > -1.000001)
            sB = -1;
         else
            return ( -11);
      }

      if (sB > 1)
      {
         if (sB < 1.000001)
            sB = 1;
         else
            return ( -10);
      }

      cB = sqrt(fabs(1 - sB * sB));
      w[1][0] = 180 * atan2( sB, cB) / M_PI;
      w[1][1] = 180 * atan2( sB, -cB) / M_PI;

      if (cB == 0 && strcmp(Typ, "exakt") != 0)
      {
         w[1][0] += 0.0001;
         w[1][1] += 0.0001;
         sB = sin(M_PI * w[1][0] / 180);
         cB = cos(M_PI * w[1][0] / 180);
      }

      if (cB > 0)
      {
         s = -cB * d123 * M[ H[1]][H[2]];
         c = cB * M[ H[2]][H[2]];
         w[0][0] = 180 * atan2( s, c) / M_PI;
         w[0][1] = 180 * atan2( -s, -c) / M_PI;

         c = cB * M[ H[0]][H[0]];
         s = -cB * d123 * M[ H[0]][H[1]];
         w[2][0] = 180 * atan2( s, c) / M_PI;
         w[2][1] = 180 * atan2( -s, -c) / M_PI;
      }

      else
      {
         v = (int)sB * d123;
         w[2][0] = 0;
         w[0][0] = 180 * atan2( d123 * M[ H[2]][H[1]], M[ H[1]][H[1]]) / M_PI;

         w[2][1] = 100;
         w[0][1] = w[0][0] - v * w[2][1];
      }
   }

   return (v);
}

//  ---------------------------------------------------------------------------
/* 1.10*/   void     Drehvektor_Winkel_Matrix(double DV[3], double phi, double D[3][3])
//  ---------------------------------------------------------------------------
{ 
   int i;
   double s, c;

   c = M_PI * phi / 180;
   s = sin(c);
   c = cos(c);

   D[0][0] = c ;
   D[0][1] = -DV[2] * s ;
   D[0][2] = DV[1] * s;
   D[1][0] = DV[2] * s ;
   D[1][1] = c ;
   D[1][2] = -DV[0] * s;
   D[2][0] = -DV[1] * s ;
   D[2][1] = DV[0] * s ;
   D[2][2] = c;

   for (i = 0;i < 3;i++)
   {
      D[i][0] += (1 - c) * DV[i] * DV[0];
      D[i][1] += (1 - c) * DV[i] * DV[1];
      D[i][2] += (1 - c) * DV[i] * DV[2];
   }

}


//  ---------------------------------------------------------------------------
/* 1.11*/   int      Drehmatrix_Drehvektor_Winkel( double D[3][3],  double DV[3],  double *Winkel )
//  ---------------------------------------------------------------------------
{
   int i;
   double c, d;

   *Winkel = 0;

   DV[0] = 0;
   DV[1] = 0;
   DV[2] = 0;

   c = ( D[0][0] + D[1][1] + D[2][2] - 1 ) / 2;

   if (c > 1)
   {
      if (c < 1.00001)
         c = 1 ;
      else
         return ( -10);
   }

   if (c < -1)
   {
      if (c > -1.00001)
         c = -1 ;
      else
         return ( -11);
   }

   if (c == 1)
   {
      DV[0] = 1;
      Winkel = 0;
      return (0);
   }

   if (c == -1)
   {
      for (i = 0;i < 3;i++)
      {
         d = (1 - D[i][i]) / 2;

         if (d < 0)
         {
            if (d > -0.00001)
               d = 0;
         }
         else
            return ( -13);

         DV[i] = sqrt(d);
      }

      if (D[0][1] < 0)
         DV[1] = -DV[1];

      if (D[0][2] < 0)
         DV[2] = -DV[2];

      if ( DV[1]*DV[2]*D[1][2] < 0)
      {
         DV[0] = -DV[0];
         DV[1] = -DV[1];
         DV[2] = -DV[2];
      }

      return (0);
   }

   *Winkel = 180 * atan2(sqrt(1 - c * c), c) / M_PI;

   DV[0] = - ( D[1][2] - D[2][1] ) / 2;
   DV[1] = - ( D[2][0] - D[0][2] ) / 2;
   DV[2] = - ( D[0][1] - D[1][0] ) / 2;

   c = sqrt( DV[0] * DV[0] + DV[1] * DV[1] + DV[2] * DV[2]);

   if (c == 0)
      return ( -14);

   DV[0] /= c;
   DV[1] /= c;
   DV[2] /= c;

   return (0);
}

//  ---------------------------------------------------------------------------
/* 1.12*/   int      Drehmatrix_Quaternion( double M[3][3],  double Quat[4])
// ---------------------------------------------------------------------------
{ 
   int i;
   double q[4], Norm;

   Quat[0] = 0;
   Quat[1] = 0;
   Quat[2] = 0;
   Quat[3] = 0;

   q[0] = (1 + M[0][0] + M[1][1] + M[2][2]) / 4;

   if (q[0] < 0)
      return ( -1);

   q[0] = sqrt(q[0]);

   q[1] = (1 + M[0][0] - M[1][1] - M[2][2]) / 4;

   if (q[1] < 0)
      return ( -2);

   q[1] = sqrt(q[1]);

   if (M[2][1] - M[1][2] < 0)
      q[1] = -q[1];


   q[2] = (1 - M[0][0] + M[1][1] - M[2][2]) / 4;

   if (q[2] < 0)
      return ( -3);

   q[2] = sqrt(q[2]);

   if (M[0][2] - M[2][0] < 0)
      q[2] = -q[2];

   q[3] = (1 - M[0][0] - M[1][1] + M[2][2]) / 4;

   if (q[3] < 0)
      return ( -3);

   q[3] = sqrt(q[3]);

   if (M[1][0] - M[0][1] < 0)
      q[3] = -q[3];

   Norm = 0;

   for (i = 0;i < 4;i++)
      Norm += q[i] * q[i];

   if (fabs(Norm - 1) > 0.000001)
      return ( -4);

   Norm = sqrt(Norm);

   Quat[0] = q[0] / Norm;
   Quat[1] = q[1] / Norm;
   Quat[2] = q[2] / Norm;
   Quat[3] = q[3] / Norm;

   return (0);
}

//  ---------------------------------------------------------------------------
/* 1.13*/   int      Quaternion_Drehmatrix(  double Quat[4], double M[3][3])
// ---------------------------------------------------------------------------
{

   M[0][0] = 2 * ( Quat[0] * Quat[0] + Quat[1] * Quat[1]) - 1;
   M[0][1] = 2 * ( Quat[1] * Quat[2] - Quat[0] * Quat[3]) ;
   M[0][2] = 2 * ( Quat[1] * Quat[3] + Quat[0] * Quat[2]) ;

   M[1][0] = 2 * ( Quat[2] * Quat[1] + Quat[0] * Quat[3]) ;
   M[1][1] = 2 * ( Quat[0] * Quat[0] + Quat[2] * Quat[2]) - 1 ;
   M[1][2] = 2 * ( Quat[2] * Quat[3] - Quat[0] * Quat[1]);

   M[2][0] = 2 * ( Quat[3] * Quat[1] - Quat[0] * Quat[2]) ;
   M[2][1] = 2 * ( Quat[2] * Quat[3] + Quat[0] * Quat[1]) ;

   M[2][2] = 2 * ( Quat[0] * Quat[0] + Quat[3] * Quat[3]) - 1;
   return (0);

}

//  ---------------------------------------------------------------------------
/* 1.14*/   void     Frame_Invers( char Achsen[4],  double T_AB[6], double T_BA[2][6])
//---------------------------------------------------------------------------
{
   double D_AB[3][3], D_BA[3][3], w[3][2], P[3];
   int i, j;



   Drei_Winkel_Matrix( Achsen, T_AB[3], T_AB[4], T_AB[5], D_AB);

   for (i = 0;i < 3;i++)
      for (j = 0;j < 3;j++)
         D_BA[i][j] = D_AB[j][i];

   Drehmatrix_3Winkel(Achsen, D_BA, w);

   for (i = 0;i < 3;i++)
      P[i] = T_AB[i];

   Mult_Matrix_Vektor( D_BA, P, P);

   for (i = 0;i < 3;i++)
   {
      T_BA[0][i] = -P[i];
      T_BA[0][i + 3] = w[i][0];
      T_BA[1][i] = -P[i];
      T_BA[1][i + 3] = w[i][1];
   }


}

//  ---------------------------------------------------------------------------
/* 1.15*/   void     Mult_Frame( char Achsen[4],  double T1[6], double T2[6], double T3[6])
//---------------------------------------------------------------------------
{
   double D1[3][3], D2[3][3], w[3][2], phi[3];
   int i;

   for (i = 0;i < 3;i++)
      phi[i] = T1[i + 3];

   Drei_Winkel_Matrix( Achsen, phi[0], phi[1], phi[2], D1);



   for (i = 0;i < 3;i++)
      phi[i] = T2[i + 3];

   Drei_Winkel_Matrix( Achsen, phi[0], phi[1], phi[2], D2);

   for (i = 0;i < 3;i++)
      phi[i] = T2[i];

   Mult_Matrix_Vektor(D1, phi, phi);

   Mult_Matrix_Matrix(D1, D2, D2);


   Drehmatrix_3Winkel(Achsen, D2, w);


   for (i = 0;i < 3;i++)
   {
      T3[i] = T1[i] + phi[i];
      T3[i + 3] = w[i][0];
   }

}




//  ---------------------------------------------------------------------------
/* 1.16*/   int      Quadrat_Matrix_Invers(int Dim,  double **M,  double **Inv)
// ---------------------------------------------------------------------------
// Eingabe: Dimension Dim und quadratische Matrix M
// Ausgabe: Inverse Matrix Inv
//          Wert=0, wenn alles O.K.
//          Wert<0: -Wert=letzte Dimension <=Dim, für die M invertierbar ist.
//Achtung:  M[0][0] darf nicht Null sein (verfahrensspezifisch bedingt)
//           alternativ kann  in PseudoInvers mit Dim1=Dim2 genutzt werden.
// ---------------------------------------------------------------------------
{
   int i, j, n, fehler;

   double Inv_nn, *Inv_i_n, *Inv_n_i;


   fehler = 0;


   Inv_i_n = (double *)calloc(Dim, sizeof(double ));
   Inv_n_i = (double *)calloc(Dim, sizeof(double ));

   if (M[0][0] == 0)
   {
      fehler = -1;

      // Änderung MM: Speicher muss freigegeben werden wenn die Funktion hier zurückkehrt !!! 
      free(Inv_n_i);
      free(Inv_i_n);

      return (fehler);
   }

   Inv[0][0] = 1 / M[0][0];


   for (n = 1;n < Dim;n++)
   {
      for (i = 0;i < n;i++)
      {
         Inv_i_n[i] = 0;
         Inv_n_i[i] = 0;

         for (j = 0;j < n;j++)
         {
            Inv_n_i[i] += M[n][j] * Inv[j][i];
            Inv_i_n[i] += Inv[i][j] * M[j][n];
         }
      }

      Inv_nn = M[n][n];

      for (i = 0;i < n;i++)
         Inv_nn -= Inv_n_i[i] * M[i][n];

      if (fabs(Inv_nn) < 0.0000010*0.01)
         fehler = -n - 1;
      else
      {
         Inv[n][n] = 1 / Inv_nn;

         for (i = 0;i < n;i++)
         {
            Inv[i][n] = Inv_i_n[i];
            Inv[n][i] = Inv_n_i[i];
         }

         for (i = 0;i < n;i++)
            for (j = 0;j < n;j++)
               Inv[i][j] += Inv[n][n] * Inv[i][n] * Inv[n][j];

         for (i = 0;i < n;i++)
         {
            Inv[n][i] = -Inv[n][n] * Inv[n][i];
            Inv[i][n] = -Inv[n][n] * Inv[i][n];
         }
      }
   }

   free(Inv_n_i);
   free(Inv_i_n);
   
   return (fehler);
}


//  ---------------------------------------------------------------------------
/* 1.17*/   int      PseudoInvers(int Dim1,  int Dim2,  double **M,  double **Inv)
// ---------------------------------------------------------------------------
// Eingabe: Dim1 x Dim2 -  Matrix M
// Ausgabe: Dim2 x Dim1-  PseudoInverse  Inv
//          Wert=0, wenn alles O.K.
//          Wert<0: Wert<0: -Wert=letzte Dimension <=Dim, für die M invertierbar ist.
// ---------------------------------------------------------------------------
{ int i, j, k, Dim;
   double **MM, **MI;
   int Wert = 0;

   for (i = 0;i < Dim1;i++)
      for (j = 0;j < Dim2;j++)
         Inv[i][j] = 0;



   if (Dim1 >= Dim2)
      Dim = Dim2;
   else
      Dim = Dim1;

   MM = (double**)calloc(Dim, sizeof(double *) );

   for (i = 0;i < Dim;i++)
      MM[i] = (double*)calloc(Dim, sizeof(double ) );

   MI = (double**)calloc(Dim, sizeof(double *) );

   for (i = 0;i < Dim;i++)
      MM[i] = (double*)calloc(Dim, sizeof(double ) );

   if (Dim1 >= Dim2)
   {
      for (i = 0;i < Dim2;i++)
         for (j = 0;j < Dim2;j++)
         {
            MM[i][j] = 0;

            for (k = 0;k < Dim1;k++)
               MM[i][j] += M[k][i] * M[k][j];
         }

      Wert = Quadrat_Matrix_Invers(Dim2, MM, MI);

      if (Wert == 0)
      {
         for (i = 0;i < Dim2;i++)
            for (j = 0;j < Dim1;j++)
               for (k = 0;k < Dim1;k++)
                  Inv[i][j] += MI[i][k] * M[j][k];
      }

   }

   else
   {
      for (i = 0;i < Dim1;i++)
         for (j = 0;j < Dim1;j++)
         {
            MM[i][j] = 0;

            for (k = 0;k < Dim2;k++)
               MM[i][j] += M[i][k] * M[j][k];
         }

      Wert = Quadrat_Matrix_Invers(Dim2, MM, MI);

      if (Wert == 0)
      {
         for (i = 0;i < Dim2;i++)
            for (j = 0;j < Dim1;j++)
               for (k = 0;k < Dim1;k++)
                  Inv[i][j] += M[k][j] * MI[k][j] ;
      }
   }


   for (i = 0;i < Dim;i++)
      free(MM[i]);

   free(MM);

   for (i = 0;i < Dim;i++)
      free(MI[i]);

   free(MI);

   return (Wert);
}




//------------------------------------------------------------------------------
/* 1.18*/   int      ORI_Typ(char Achsen[4])
//------------------------------------------------------------------------------
{

   if ( (Achsen[0] == 'x' || Achsen[0] == 'y' || Achsen[0] == 'z' ) &&
         (Achsen[1] == 'x' || Achsen[1] == 'y' || Achsen[2] == 'z' ) &&
         (Achsen[2] == 'x' || Achsen[2] == 'y' || Achsen[2] == 'z' ) )

      return (1);

   if ( strcmp(Achsen, "DVW") == 0 )
      return (2);

   if ( strcmp(Achsen, "QUA") == 0)
      return (3);

   return ( -1); // ungueltige Achsen
}

void MFrame(double HD[4], double F[4][4])  
{  // HD =  teta, d, a, alpa
   double st,ct,sa,ca;
   
   st=sin(M_PI*HD[0]/180);  ct=cos(M_PI*HD[0]/180);
   sa=sin(M_PI*HD[3]/180);  ca=cos(M_PI*HD[3]/180);
   
   F[0][0]= ct;   F[0][1] = - ca*st;  F[0][2]=  sa*st;  F[0][3] = HD[2]*ct;
   F[1][0]= st;   F[1][1] =   ca*ct;  F[1][2]= -sa*ct;  F[1][3] = HD[2]*st;
   F[2][0]=  0;   F[2][1] =   sa   ;  F[2][2]=  ca   ;  F[2][3] = HD[1]   ;
   F[3][0]=  0;   F[3][1] =    0   ;  F[3][2]=   0   ;  F[3][3] = 1;
}

void MULT_MFrame( double F1[4][4],double F2[4][4],double F3[4][4])
{
   int i,j,k;
   for( i = 0 ; i < 4 ; i++ ) 
      for( j = 0 ; j < 4 ; j++ )
      {
         F3[i][j]=0; 
         
         for( k = 0 ; k < 4 ; k++ ) 
            F3[i][j] += F1[i][k]*F2[k][j];
      }
   
}

double OrthoNormieren( double X[3], double Y[3], int Stufe, double XN[3], double YN[3] )
{
   // Stufe =0:  nur normieren,
   //       =1:  orthonormieren
   // Kriterium fuer Orthogonalisierunmg:    |X-XN|^2 + |Y-YN|^2 + lambda * XN * YN == Minimum
   //                                         XN*YN=!0  und nachträgliche Normierung.
   int i;
   double cN, lambda, temp;
   
   
   if( Normieren( X, XN ) <= 0 )
   { 
      // Erster Vektor hat keine Laenge!
      return( 100 );
   }
   
   if( Normieren( Y, YN ) <= 0 )
   {
      // Zweiter Vektor hat keine Laenge!
      return( 200 );
   }
   
   cN = XN[0] * YN[0] + XN[1] * YN[1] + XN[2] * YN[2];
   
   if( Stufe == 0 || fabs( cN ) < 0.00000001 ) 
   {
      return( cN );
   }
   
   lambda = -( sqrt ( - cN * cN + 1 ) - 1 ) / cN;
   
   for( i = 0 ; i < 3 ; i++ )
   {
      temp  = XN[i];
      XN[i] = XN[i] - lambda * YN[i];
      YN[i] = YN[i] - lambda * temp;
   }
   
   Normieren( XN, XN );
   Normieren( YN, YN );
   
   cN = XN[0] * YN[0] + XN[1] * YN[1] + XN[2] * YN[2];
   
   return( cN );
}

double   GeradenAbstand( double dP0_1[3], double dP0_2[3], double dRichtungsVektor_1[3], double dRichtungsVektor_2[3] )
{
   // Abstand = |( (a1 x a2)(r2 - r1) ) / |(a1 x a2)||
   
   // (a1 x a2)
   double dKreuz[3];
   Vektor_Kreuz_Vektor( dRichtungsVektor_1, dRichtungsVektor_2, dKreuz );

   // (r2 - r1) 
   double dR2R1[3];
   dR2R1[0]= dP0_2[0] - dP0_1[0];
   dR2R1[1]= dP0_2[1] - dP0_1[1];
   dR2R1[2]= dP0_2[2] - dP0_1[2];

   if ( Norm( dKreuz ) == 0) // Parallele Graden
   {
      // Abstand = |( (a1 x (r2 - r1) )| / |(a1 x a2)|
      Vektor_Kreuz_Vektor( dRichtungsVektor_1, dR2R1 , dKreuz );
      return  fabs( Norm( dKreuz ) / Norm( dRichtungsVektor_1 ) );
   }
   else
   {
     return  fabs( Mult_Vektor_Vektor( dKreuz, dR2R1 ) / Norm( dKreuz ) );
   }
}

// Berechnet den Abstandsvektor zweier windschiefer Geraden
// In: Beide Geraden in Punkt-Richtungsform
// Out: Abstandsrichtungsvektor
// return : Norm des Abstandsvektors ( -1 = Parallele Geraden
double   GeradenAbstandsVektor( double dP0_1[3], double dP0_2[3], double dRichtungsVektor_1[3], double dRichtungsVektor_2[3], double dAbstandsVektor_1_2[3] )
{
   // dAbstandsVektor_1_2 zeigt von Gerade 1 nach Gerade 2
   
   // Mormieren der Richtungsvektoren
   Normieren( dRichtungsVektor_1, dRichtungsVektor_1 );
   Normieren( dRichtungsVektor_2, dRichtungsVektor_2 );

   // Skalarprodukt
   double dSkalar = Mult_Vektor_Vektor(  dRichtungsVektor_1 , dRichtungsVektor_2 );
   double dDet = 1 - dSkalar * dSkalar;


   // Geraden sind parallel
   if ( dDet == 0)
   {
      return -1;
   }

   // (r2 - r1) 
   double dR2R1[3];
   dR2R1[0]= dP0_2[0] - dP0_1[0];
   dR2R1[1]= dP0_2[1] - dP0_1[1];
   dR2R1[2]= dP0_2[2] - dP0_1[2];
   
   double dTempVektor[3];
   dTempVektor[0] = dRichtungsVektor_1[0] - dSkalar * dRichtungsVektor_2[0];
   dTempVektor[1] = dRichtungsVektor_1[1] - dSkalar * dRichtungsVektor_2[1];
   dTempVektor[2] = dRichtungsVektor_1[2] - dSkalar * dRichtungsVektor_2[2];
   
   double dE1 = Mult_Vektor_Vektor( dR2R1, dTempVektor ) / dDet ;
   
   dAbstandsVektor_1_2[0] = dP0_1[0] + dE1 * dRichtungsVektor_1[0];
   dAbstandsVektor_1_2[1] = dP0_1[1] + dE1 * dRichtungsVektor_1[1];
   dAbstandsVektor_1_2[2] = dP0_1[2] + dE1 * dRichtungsVektor_1[2];

   dTempVektor[0] = dSkalar * dRichtungsVektor_1[0] - dRichtungsVektor_2[0];
   dTempVektor[1] = dSkalar * dRichtungsVektor_1[1] - dRichtungsVektor_2[1];
   dTempVektor[2] = dSkalar * dRichtungsVektor_1[2] - dRichtungsVektor_2[2];

   double dE2 = Mult_Vektor_Vektor( dR2R1, dTempVektor ) / dDet ;
   
   dAbstandsVektor_1_2[0] = dP0_2[0] + dE2 * dRichtungsVektor_2[0] - dAbstandsVektor_1_2[0];
   dAbstandsVektor_1_2[1] = dP0_2[1] + dE2 * dRichtungsVektor_2[1] - dAbstandsVektor_1_2[1];
   dAbstandsVektor_1_2[2] = dP0_2[2] + dE2 * dRichtungsVektor_2[2] - dAbstandsVektor_1_2[2];

   return Norm ( dAbstandsVektor_1_2 );
 
}
double   PunktGeradenAbstand( double dPunkt[3], double dP0[3], double dRichtungsVektor[3] )
{
   // Formel Paula Band 1 Seite 102 oder Bartsch S. 242
   // d = | a  x ( r1 - r0 ) | / |a|
   
   // r1 - r0
   double dR1R0[3] = { 0, 0, 0 };
   dR1R0[0] = dPunkt[0] - dP0[0];
   dR1R0[1] = dPunkt[1] - dP0[1];
   dR1R0[2] = dPunkt[2] - dP0[2];

   // a  x ( r1 - r0 )
   double dKreuz[3] = { 0, 0, 0 };
    
   return Vektor_Kreuz_Vektor( dRichtungsVektor, dR1R0, dKreuz ) / Norm ( dRichtungsVektor );
}

int sign (double d)
{
	return ((d >= 0.0) ? 1 : -1);
}

int sign (float f)
{
	return ((f >= 0.0) ? 1 : -1);
}

int sign (int i)
{
	return ((i >= 0.0) ? 1 : -1);
}
