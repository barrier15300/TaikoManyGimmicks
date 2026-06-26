#pragma once
#ifndef __ANDROID__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;

#include "DxLib.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstdio>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <algorithm>
#include <complex>

#ifndef __ANDROID__
//#include <boost/multiprecision/cpp_dec_float.hpp>
//namespace mp = boost::multiprecision;
//using float1024 = mp::number<mp::cpp_dec_float<1024>>;
#endif

#include "Library/LoadFile.h"
#include "Library/ExLib.h"
