#ifndef __SICKCONV_H_INCLUDED__
#define __SICKCONV_H_INCLUDED__

#include <string>

#ifndef DEF_PI
#define DEF_PI 3.1415926535897932384626433832795028841971693993751
#endif

int getInt(std::string stIN );
unsigned int getUnsignedInt(std::string stIN  );
float getFloat(std::string stIN);
double getDouble(std::string stIN);

double Rad2Grad(double dRad);
double Grad2Rad(double dGrad);

template<class T> T fromString(const std::string& s);
#endif