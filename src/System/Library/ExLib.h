#pragma once
#include "System/include.h"

// StringFormula
class StringFormula {
	enum class OPType {
		Null,
		Add,
		Subtract,
		Multiplication,
		Division,
		Mod,
	};

	enum class MethodType {
		sin,
		cos,
		tan,
		asin,
		acos,
		atan,
		sinh,
		cosh,
		tanh,
		asinh,
		acosh,
		atanh,

	};
	
	struct OPData {
		double Value = 0;
		OPType Type = OPType::Null;
	};
	
public:
	double operator()(string formula);
};

// string
string removecommentout(string str);

string strtrim(string str);

vector<string> split(const string& s, char delim);

string getnumstring(string str);

void stob(bool* Buf, string str);

string double_to_string(double d, int digit = 32);

// complex
complex<double> PolarPosToComplexPos(double magunitude, double theta);

double GetMagnitude(complex<double> Pos);

double GetTheta(complex<double> Pos);

// math
int LTZF(double x);

int ZF(double x);

// FileSystem
bool LoadFileTEXT(vector<string>& list, string path);

bool WriteFileTEXT(const vector<string>& list, string path);

bool LoadBinaryFileTEXT(vector<string>& list, string path);

bool WriteBinaryFileTEXT(const vector<string>& list, string path);

// Easing
enum class EasingType1 {
	In,
	Out,
	InOut,
	OutIn
};

enum class EasingType2 {
	Linear,
	Sine,
	Quad,
	Cubic,
	Quart,
	Quint,
	Expo,
	Circ,
	Back,
	Elastic,
	Bounce
};

double GetEasingRate(const double& x, const EasingType1& type1, const EasingType2& type2);

#ifndef __ANDROID__
// Error
string GetErrorString(string str, int max, int offset);
#endif