#include "ExLib.h"
#include "sub.h"

// StringFormula
double StringFormula::operator()(string formula) {
	vector<OPData> Values;
	OPData item;
	string Val = "";
	int Operation = 0;
	bool _ASFlag = false;

	for (int i = 0, size = formula.size(); i < size; ++i) {
		if (formula[i] == ' ') { continue; }
		if (formula[i] == '+' ||
			formula[i] == '-') {
			if (_ASFlag) {
				item = OPData();
				switch (formula[i])
				{
				case '+':
					item.Type = OPType::Add;
					break;
				case '-':
					item.Type = OPType::Subtract;
					break;
				}
				item.Value = stod(Val);
				Values.push_back(item);
				Val = "";
				++Operation;
				_ASFlag = false;
			}
			else {
				Val += formula[i];
			}
		}
		else if (
			formula[i] == '*' ||
			formula[i] == '/' ||
			formula[i] == '%') {
			_ASFlag = false;
			item = OPData();
			switch (formula[i])
			{
			case '*':
				item.Type = OPType::Multiplication;
				break;
			case '/':
				item.Type = OPType::Division;
				break;
			case '%':
				item.Type = OPType::Mod;
				break;
			}
			item.Value = stod(Val);
			Values.push_back(item);
			Val = "";
			++Operation;
		}
		else if (formula[i] == '(') {
			int LBrackets = 1;
			string Brackets = "";
			for (int j = i + 1; j < size; ++j) {
				if (formula[j] == '(') { ++LBrackets; }
				if (formula[j] == ')') { --LBrackets; }
				if (formula[j] == ')' && LBrackets == 0) {
					auto _temp = split(Brackets, ',');
					double _r = StringFormula()(_temp[0]);
					if (Val == "Math.sin") { _r = sin(StringFormula()(_temp[0])); }
					if (Val == "Math.cos") { _r = cos(StringFormula()(_temp[0])); }
					if (Val == "Math.tan") { _r = tan(StringFormula()(_temp[0])); }
					if (Val == "Math.sec") { _r = 1 / cos(StringFormula()(_temp[0])); }
					if (Val == "Math.csc") { _r = 1 / sin(StringFormula()(_temp[0])); }
					if (Val == "Math.cot") { _r = 1 / tan(StringFormula()(_temp[0])); }
					if (Val == "Math.asin") { _r = asin(StringFormula()(_temp[0])); }
					if (Val == "Math.acos") { _r = acos(StringFormula()(_temp[0])); }
					if (Val == "Math.atan") { _r = atan(StringFormula()(_temp[0])); }
					if (Val == "Math.asec") { _r = 1 / acos(StringFormula()(_temp[0])); }
					if (Val == "Math.acsc") { _r = 1 / asin(StringFormula()(_temp[0])); }
					if (Val == "Math.acot") { _r = 1 / atan(StringFormula()(_temp[0])); }
					if (Val == "Math.sinh") { _r = sinh(StringFormula()(_temp[0])); }
					if (Val == "Math.cosh") { _r = cosh(StringFormula()(_temp[0])); }
					if (Val == "Math.tanh") { _r = tanh(StringFormula()(_temp[0])); }
					if (Val == "Math.sech") { _r = 1 / cosh(StringFormula()(_temp[0])); }
					if (Val == "Math.csch") { _r = 1 / sinh(StringFormula()(_temp[0])); }
					if (Val == "Math.coth") { _r = 1 / tanh(StringFormula()(_temp[0])); }
					if (Val == "Math.asinh") { _r = asinh(StringFormula()(_temp[0])); }
					if (Val == "Math.acosh") { _r = acosh(StringFormula()(_temp[0])); }
					if (Val == "Math.atanh") { _r = atanh(StringFormula()(_temp[0])); }
					if (Val == "Math.asech") { _r = 1 / acosh(StringFormula()(_temp[0])); }
					if (Val == "Math.acsch") { _r = 1 / asinh(StringFormula()(_temp[0])); }
					if (Val == "Math.acoth") { _r = 1 / atanh(StringFormula()(_temp[0])); }
					if (Val == "Math.pow") { _r = pow(StringFormula()(_temp[0]), StringFormula()(_temp[1])); }
					if (Val == "Math.log") { _r = log(StringFormula()(_temp[0])); }
					if (Val == "Math.log2") { _r = log2(StringFormula()(_temp[0])); }
					if (Val == "Math.log10") { _r = log10(StringFormula()(_temp[0])); }
					if (Val == "Math.exp") { _r = exp(StringFormula()(_temp[0])); }
					if (Val == "Math.sqrt") { _r = sqrt(StringFormula()(_temp[0])); }
					if (Val == "Math.cbrt") { _r = cbrt(StringFormula()(_temp[0])); }
					Val = double_to_string(_r);
					i = j;
					break;
				}
				Brackets += formula[j];
			}
		}
		else {
			_ASFlag = true;
			Val += formula[i];
		}
	}
	item.Value = stod(Val), item.Type = OPType::Null;
	Values.push_back(item);

	// Mod
	for (int i = 0; i < Operation; ++i) {
		if (Values[i].Type == OPType::Mod) {
			Values[i + 1].Value = (int)Values[i].Value % (int)Values[i + 1].Value;
			Values.erase(Values.begin() + i);
			--Operation;
			--i;
		}
	}
	
	// Division
	for (int i = 0; i < Operation; ++i) {
		if (Values[i].Type == OPType::Division) {
			Values[i + 1].Value = Values[i].Value / Values[i + 1].Value;
			Values.erase(Values.begin() + i);
			--Operation;
			--i;
		}
	}
	
	// Multiplication
	for (int i = 0; i < Operation; ++i) {
		if (Values[i].Type == OPType::Multiplication) {
			Values[i + 1].Value = Values[i].Value * Values[i + 1].Value;
			Values.erase(Values.begin() + i);
			--Operation;
			--i;
		}
	}

	// Add
	for (int i = 0; i < Operation; ++i) {
		if (Values[i].Type == OPType::Add) {
			Values[i + 1].Value = Values[i].Value + Values[i + 1].Value;
			Values.erase(Values.begin() + i);
			--Operation;
			--i;
		}
	}

	//Subtract
	for (int i = 0; i < Operation; ++i) {
		if (Values[i].Type == OPType::Subtract) {
			Values[i + 1].Value = Values[i].Value - Values[i + 1].Value;
			Values.erase(Values.begin() + i);
			--Operation;
			--i;
		}
	}

	return Values[0].Value;
}

// string
string removecommentout(string str) {
	return str.find("//") != string::npos ? str.replace(str.find("//"), str.size() - str.find("//"), "") : str;
}

string strtrim(string str) {
	for (;;) {
		if (str[0] == ' ') {
			str = str.replace(0, 1, "");
		}
		else if (str[0] == '	') {
			str = str.replace(0, 1, "");
		}
		else {
			break;
		}
	}
	for (int i = str.size() - 1; i >= 0; --i) {
		if (str[i] == ' ') {
			str = str.replace(i, 1, "");
		}
		else if (str[i] == '	') {
			str = str.replace(i, 1, "");
		}
		else {
			break;
		}
	}
	return str;
}

vector<string> split(const string& s, char delim) {
	vector<string> elems;
	string item;
	for (char ch : s) {
		if (ch == delim) {
			if (!item.empty())
				elems.push_back(item);
			item.clear();
		}
		else {
			item += ch;
		}
	}
	if (!item.empty())
		elems.push_back(item);
	return elems;
}

string getnumstring(string str) {
	string result = "";
	for (int i = 0, size = str.size(); i < size; i++) {
		if ((str[i] >= '0' && str[i] <= '9') || 
			str[i] == '.' ||
			str[i] == '+' ||
			str[i] == '-'
			) {
			result.push_back(str[i]);
		}
	}
	return result;
}

void stob(bool* Buf, string str) {
	if (strcmp(str.c_str(), "true") == 0) { *Buf = true; }
	if (strcmp(str.c_str(), "false") == 0) { *Buf = false; }
}

string double_to_string(double d, int digit)
{
	ostringstream oss;

	oss << setprecision(digit) << setiosflags(ios::fixed) << d;

	return oss.str();
}

// complex
complex<double> PolarPosToComplexPos(double magunitude, double theta) {
	return complex<double>(magunitude * cos(theta), magunitude * sin(theta));
}

double GetMagnitude(complex<double> Pos) {
	return sqrt(pow(Pos.real(), 2) + pow(Pos.imag(), 2));
}

double GetTheta(complex<double> Pos) {
	return atan2(Pos.imag(), Pos.real());
}

// math
int LTZF(double x) { return (x < 0 ? -1 : 1); }

int ZF(double x) { return x == 0 ? 0 : 1; }

// FileSystem
bool LoadFileTEXT(vector<string>& list, string path) {
	fstream _fs(path);
	if (!_fs.is_open()) { return false; }
	
	LONGLONG _temp = 0;
	_fs.read((char*)&_temp, sizeof(LONGLONG));
	if (_temp == (-9223372036854775807 - 1)) {
		return LoadBinaryFileTEXT(list, path);
	}

	_fs.close();
	fstream fs(path);

	list.clear();
	string line;
	bool ASCIIFlag = false;
	while (getline(fs, line)) {
#ifdef __ANDROID__
		if (line.find("\r") != string::npos) {line = line.replace(line.find("\r"), 1, ""); }
#endif
		list.push_back(strtrim(removecommentout(line)));
	}

	fs.close();
	return true;
}

bool WriteFileTEXT(const vector<string>& list, string path) {
	fstream fs(path, ios::out);
	if (!fs.is_open()) { return false; }

	for (int i = 0; i < list.size(); ++i){
		fs << list[i] << endl;
	}

	fs.close();
	return true;
}

bool LoadBinaryFileTEXT(vector<string>& list, string path) {
	list.clear();
	ifstream fs(path, ios::in | ios::binary);
	if (!fs.is_open()) { return false; }

	LONGLONG isbinary = 0;
	fs.read((char*)&isbinary, sizeof(LONGLONG));
	if (isbinary != (-9223372036854775807 - 1)) { return false; }

	string line;
	while (!fs.eof()) {
#ifdef __ANDROID__
		if (line.find("\r") != string::npos) { line = line.replace(line.find("\r"), 1, ""); }
#endif
		LONGLONG _temp = 0;
		fs.read((char*)&_temp, sizeof(LONGLONG));
		if (_temp == 9223372036854775807) {
			list.push_back(strtrim(removecommentout(line)));
			line = "";
		}
		else {
			_temp /= 43;
			_temp /= 103;
			_temp /= 89;
			line += (char)(_temp - 13);
		}
	}

	fs.close();
	return true;
}

bool WriteBinaryFileTEXT(const vector<string>& list, string path) {
	ofstream fs(path, ios::out | ios::binary);
	if (!fs.is_open()) { return false; }

	LONGLONG isbinary = (-9223372036854775807 - 1);
	fs.write((char*)&isbinary, sizeof(LONGLONG));
	for (int i = 0; i < list.size(); ++i){
		for (int j = 0, size = list[i].size(); j < size; ++j) {
			LONGLONG _temp = list[i][j] + 13;
			_temp *= 127;
			_temp *= 223;
			_temp *= 337;
			fs.write((char*)&_temp,sizeof(LONGLONG));
		}
		LONGLONG bsn = 9223372036854775807;
		fs.write((char*)&bsn, sizeof(LONGLONG));
	}

	fs.close();
	return true;
}

// Easing
double GetEasingRate(const double& x, const EasingType1& type1, const EasingType2& type2) {
	EasingType2 ease = EasingType2::Linear;
	if (type2 == EasingType2::Linear) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x;
		}
		if (type1 == EasingType1::Out) {
			return x;
		}
		if (type1 == EasingType1::InOut) {
			return x;
		}
		if (type1 == EasingType1::OutIn) {
			return x;
		}
	}
	if (type2 == EasingType2::Sine) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return 1 - cos(x * DX_PI / 2);
		}
		if (type1 == EasingType1::Out) {
			return sin(x * DX_PI / 2);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Quad) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x * x;
		}
		if (type1 == EasingType1::Out) {
			return 1 - (1 - x) * (1 - x);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Cubic) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x * x * x;
		}
		if (type1 == EasingType1::Out) {
			return 1 - pow(1 - x, 3);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Quart) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x * x * x * x;
		}
		if (type1 == EasingType1::Out) {
			return 1 - pow(1 - x, 4);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Quint) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x * x * x * x * x;
		}
		if (type1 == EasingType1::Out) {
			return 1 - pow(1 - x, 5);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Expo) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return x == 0 ? 0 : pow(2, 10 * x - 10);
		}
		if (type1 == EasingType1::Out) {
			return x == 1 ? 1 : 1 - pow(2, -10 * x);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Circ) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return 1 - sqrt(1 - pow(x, 2));
		}
		if (type1 == EasingType1::Out) {
			return sqrt(1 - pow(x - 1, 2));
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Back) {
		ease = type2;
		const double c1 = 1.70158;
		const double c3 = c1 + 1;
		if (type1 == EasingType1::In) {
			return c3 * x * x * x - c1 * x * x;
		}
		if (type1 == EasingType1::Out) {
			return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x -0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Elastic) {
		ease = type2;
		const double c4 = 2 * DX_PI / 3;
		if (type1 == EasingType1::In) {
			return x == 0
				? 0
				: x == 1
				? 1
				: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
		}
		if (type1 == EasingType1::Out) {
			return x == 0
				? 0
				: x == 1
				? 1
				: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	if (type2 == EasingType2::Bounce) {
		ease = type2;
		if (type1 == EasingType1::In) {
			return 1 - GetEasingRate(1 - x, EasingType1::Out, EasingType2::Bounce);
		}
		if (type1 == EasingType1::Out) {
			const double n1 = 7.5625;
			const double d1 = 2.75;
			
			double x2 = 0;

			if (x < 1 / d1) {
				x2 = n1 * x * x;
			}
			else if (x < 2 / d1) {
				x2 = n1 * (x - (1.5 / d1)) * x + 0.75;
				//x2 = x2 > 1 ? 1 - (x2 - 1) : x2;
			}
			else if (x < 2.5 / d1) {
				x2 = n1 * (x - (2.25 / d1)) * x + 0.9375;
				//x2 = x2 > 1 ? 1 - (x2 - 1) : x2;
			}
			else {
				x2 = n1 * (x - (2.625 / d1)) * x + 0.984375;
				//x2 = x2 > 1 ? 1 - (x2 - 1) : x2;
			}
			//OutputDebugString((LPCSTR)("x(" + double_to_string(x) + ") = " + double_to_string(x2) + "\n").c_str());
			return x2;
		}
		if (type1 == EasingType1::InOut) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::In, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::Out, ease) / 2);
		}
		if (type1 == EasingType1::OutIn) {
			return x < 0.5 ?
				GetEasingRate(x / 0.5, EasingType1::Out, ease) / 2 :
				0.5 + (GetEasingRate((x - 0.5) / 0.5, EasingType1::In, ease) / 2);
		}
	}
	return x;
}

#ifndef __ANDROID__
// Error
string GetErrorString(string str, int max, int offset) {
	for (int i = 0; i < str.size(); ++i) {
		str[i] = GetRand(max) + offset;
	}
	return str;
}
#endif
