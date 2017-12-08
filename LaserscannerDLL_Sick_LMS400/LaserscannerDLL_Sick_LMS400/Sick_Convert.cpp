#include "Sick_Convert.h"
#include <sstream>

template<class T> T fromString(const std::string& s)
{
	std::istringstream stream(s);
	T t;
	stream >> t;
	return t;
}

int getInt(std::string stIN  )
{
	return fromString<int>(stIN);
}

unsigned int getUnsignedInt(std::string stIN)
{
	return fromString<unsigned int>(stIN);
}

float getFloat(std::string stIN)
{
	return fromString<float>(stIN);
}

double getDouble(std::string stIN)
{
	return fromString<double>(stIN);
}

double Rad2Grad(double dRad)
{
	return dRad/(DEF_PI/180);
}

double Grad2Rad(double dGrad)
{
	return dGrad*(DEF_PI/180);
}


