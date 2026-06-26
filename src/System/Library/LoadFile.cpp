#include "LoadFile.h"

vector<string> FontList;

inline void AddFont(string FontPath) {
#ifndef __ANDROID__
	for (int i = 0; i < FontList.size(); ++i) {
		if (FontList[i] == FontPath) {
			return;
		}
	}
	AddFontResourceEx(FontPath.c_str(), FR_PRIVATE, NULL);
	FontList.push_back(FontPath);
#else
	// TODO: フォント読み込みむずいけどいつか作る
#endif
}

void FileLoad::LoadConfigFile() {
	vector<string> list;
	if (!LoadFileTEXT(list, "config.ini")) { MakeConfigFile(); return; }
	for (int i = 0, size = list.size(); i < size; ++i) {
		string val = strtrim(list[i].substr(list[i].find(":") + 1, list[i].size() - (list[i].find(":") + 1)));
		string ID = strtrim(list[i].substr(0, list[i].find(":")));
		if (val == "") { val = to_string(INT32_MAX); }
		if ("AutoPlayFlag" == ID) { stob(&AutoPlayFlag, val); }
		if ("SkinFolderName" == ID) { SkinFolderName = val; }
		if ("WaitVSyncFlag" == ID) { stob(&WaitVSyncFlag, val); }
		if ("StableVSyncFlag" == ID) { stob(&StableVSyncFlag, val); }
		if ("ConstantSCROLL" == ID) { ConstantSCROLL = stod(val); }
		if ("ConstantBPM" == ID) { ConstantBPM = stod(val); }
		if ("AddSCROLL" == ID) { AddSCROLL = stod(val); }
		if ("JudgeGood" == ID) { JudgeGood = stod(val); }
		if ("JudgeOk" == ID) { JudgeOk = stod(val); }
		if ("JudgeBad" == ID) { JudgeBad = stod(val); }
		if ("JudgeOffset" == ID) { JudgeOffset = stod(val); }
		if ("SongPlaySpeed" == ID) { SongPlaySpeed = stod(val); }
		if ("SongBlankTime" == ID) { SongBlankTime = stod(val); }
		if ("SongOffset" == ID) { SongOffset = stod(val); }
		if ("SongVolume" == ID) { SongVolume = stoi(val); }
		if ("SEVolume" == ID) { SEVolume = stoi(val); }
		if ("ChartHeaderSound" == ID) { stob(&ChartHeaderSound, val); }
		if ("JiroBPMDelayFlag" == ID) { stob(&JiroBPMDelayFlag, val); }
		if ("WindowSizeWidth" == ID) { WindowSizeWidth = stoi(val); }
		if ("WindowSizeHeight" == ID) { WindowSizeHeight = stoi(val); }
		if ("DrawingOperationRangeWidth" == ID) { DrawingOperationRangeWidth = stod(val); }
		if ("DrawingOperationRangeHeight" == ID) { DrawingOperationRangeHeight = stod(val); }
		if ("RandomRate" == ID) { RandomRate = stoi(val); }
		if ("RollSpeed" == ID) { RollSpeed = stod(val); }
		if ("ChartCreateMode" == ID) { stob(&ChartCreateMode, val); }
		if ("GridScreen" == ID) { stob(&GridScreen, val); }
		if ("DebugScreen" == ID) { stob(&DebugScreen, val); }
#ifndef __ANDROID__
		if ("Error" == ID) { Error = val == "Error"; }
#endif
	}
}
void FileLoad::MakeConfigFile() {
	std::ofstream ofs("config.ini");

#define ELEM_TRANSFORM(v) #v":" << v

	ofs << ELEM_TRANSFORM(AutoPlayFlag) << std::endl;
	ofs << ELEM_TRANSFORM(SkinFolderName) << std::endl;
	ofs << ELEM_TRANSFORM(WaitVSyncFlag) << std::endl;
	ofs << ELEM_TRANSFORM(StableVSyncFlag) << std::endl;
	ofs << ELEM_TRANSFORM(WindowSizeWidth) << std::endl;
	ofs << ELEM_TRANSFORM(WindowSizeHeight) << std::endl;
	ofs << ELEM_TRANSFORM(ConstantSCROLL) << std::endl;
	ofs << ELEM_TRANSFORM(ConstantBPM) << std::endl;
	ofs << ELEM_TRANSFORM(AddSCROLL) << std::endl;
	ofs << ELEM_TRANSFORM(JudgeGood) << std::endl;
	ofs << ELEM_TRANSFORM(JudgeOk) << std::endl;
	ofs << ELEM_TRANSFORM(JudgeBad) << std::endl;
	ofs << ELEM_TRANSFORM(JudgeOffset) << std::endl;
	ofs << ELEM_TRANSFORM(SongPlaySpeed) << std::endl;
	ofs << ELEM_TRANSFORM(SongBlankTime) << std::endl;
	ofs << ELEM_TRANSFORM(SongOffset) << std::endl;
	ofs << ELEM_TRANSFORM(SongVolume) << std::endl;
	ofs << ELEM_TRANSFORM(SEVolume) << std::endl;
	ofs << ELEM_TRANSFORM(ChartHeaderSound) << std::endl;
	ofs << ELEM_TRANSFORM(JiroBPMDelayFlag) << std::endl;
	ofs << ELEM_TRANSFORM(DrawingOperationRangeWidth) << std::endl;
	ofs << ELEM_TRANSFORM(DrawingOperationRangeHeight) << std::endl;
	ofs << ELEM_TRANSFORM(RandomRate) << std::endl;
	ofs << ELEM_TRANSFORM(RollSpeed) << std::endl;
	ofs << ELEM_TRANSFORM(ChartCreateMode) << std::endl;
	ofs << ELEM_TRANSFORM(GridScreen) << std::endl;
	ofs << ELEM_TRANSFORM(DebugScreen) << std::endl;

#undef ELEM_TRANSFORM

	ofs.close();
}

#ifndef __ANDROID__
void FileLoad::LoadKeyConfigFile() {
	vector<string> list;
	if (!LoadFileTEXT(list, "keyconfig.ini")) { MakeKeyConfigFile(); return; }
	for (int i = 0; i < list.size(); ++i) {
		for (int j = 0; j < 10; ++j) {
			string val = strtrim(list[i].substr(list[i].find(":") + 1, list[i].size() - (list[i].find(":") + 1)));
			string ID = strtrim(list[i].substr(0, list[i].find(":")));
			if (val == "") { val = "0"; }
			if ("LeftDonInput" + to_string(j + 1) == ID) {
				KEY_INPUT_LD[j] = stoi(val);
			}
			if ("LeftKaInput" + to_string(j + 1) == ID) {
				KEY_INPUT_LK[j] = stoi(val);
			}
			if ("RightDonInput" + to_string(j + 1) == ID) {
				KEY_INPUT_RD[j] = stoi(val);
			}
			if ("RightKaInput" + to_string(j + 1) == ID) {
				KEY_INPUT_RK[j] = stoi(val);
			}
			if ("AutoPlayFlagInput" == ID) {
				QuickAccessKey[(int)QuickAccessID::AutoPlayFlag] = stoi(val);
			}
			if ("DebugScreenInput" == ID) {
				QuickAccessKey[(int)QuickAccessID::DebugScreen] = stoi(val);
			}
			if ("GridScreenInput" == ID) {
				QuickAccessKey[(int)QuickAccessID::GridScreen] = stoi(val);
			}
			if ("ChartCreateModeInput" == ID) {
				QuickAccessKey[(int)QuickAccessID::ChartCreateMode] = stoi(val);
			}
		}
	}
}
void FileLoad::MakeKeyConfigFile() {
	std::ofstream ofs("keyconfig.ini");

	auto colmun = [](std::ofstream& stream, const std::string& name, const std::vector<int>& arr)
	{
		for (int i = 0; auto&& elem : arr) {
			stream << name << i << ":" << elem << std::endl;
			++i;
		}
	};

	colmun(ofs, "LeftKaInput", {KEY_INPUT_LK, KEY_INPUT_LK + (sizeof(KEY_INPUT_LK) / sizeof(decltype(KEY_INPUT_LK[0])))});
	colmun(ofs, "LeftDonInput", {KEY_INPUT_LD, KEY_INPUT_LD + (sizeof(KEY_INPUT_LD) / sizeof(decltype(KEY_INPUT_LD[0])))});
	colmun(ofs, "RightDonInput", {KEY_INPUT_RD, KEY_INPUT_RD + (sizeof(KEY_INPUT_RD) / sizeof(decltype(KEY_INPUT_RD[0])))});
	colmun(ofs, "RightKaInput", {KEY_INPUT_RK, KEY_INPUT_RK + (sizeof(KEY_INPUT_RK) / sizeof(decltype(KEY_INPUT_RK[0])))});

	ofs << "AutoPlayFlagInput" << ":" << QuickAccessKey[(int)QuickAccessID::AutoPlayFlag] << std::endl;
	ofs << "DebugScreenInput" << ":" << QuickAccessKey[(int)QuickAccessID::DebugScreen] << std::endl;
	ofs << "GridScreenInput" << ":" << QuickAccessKey[(int)QuickAccessID::GridScreen] << std::endl;
	ofs << "ChartCreateModeInput" << ":" << QuickAccessKey[(int)QuickAccessID::ChartCreateMode] << std::endl;

	ofs.close();
}
#endif

void FileLoad::LoadSkinConfig() {
	vector<string> list;
	if (!LoadFileTEXT(list,  "Skins/" + SkinFolderName + "/skinconfig.ini")) { return; }
	for (int i = 0; i < list.size(); ++i) {
		if (list[i].find("Config") != string::npos) {
			string datas = strtrim(list[i].substr(list[i].find("{") + 1, list[i].find("}") - (list[i].find("{") + 1)));
			string ID = strtrim(list[i].substr(list[i].find(":") + 1, list[i].find("{") - (list[i].find(":") + 1)));
			if ("SKIN_INFO" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("name" == a) { Name = val; }
					if ("rx" == a) { ScreenX = stoi(val); }
					if ("ry" == a) { ScreenY = stoi(val); }
					if ("colorbit" == a) { ColorBit = stoi(val); }
				}
			}
			if ("SKIN_PLAYING_HITNOTE" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("displayflag" == a) { stob(&HitNoteDispFlag, val); }
					if ("hitheight" == a) { HitNoteHeight = stod(val); }
				}
			}
			if ("SKIN_PLAYING_SCROLLFIELDEXTENSION" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flag" == a) { stob(&SCROLLFieldExtensionFlag, val); }
				}
			}
			if ("SKIN_PLAYING_SCROLLFIELD_DON" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::ScrollField_Don] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_SCROLLFIELD_KA" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::ScrollField_Ka] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_SCROLLFIELD_ROLL" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::ScrollField_Roll] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_SCROLLFIELD_HIT" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::ScrollField_Hit] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_MINITAIKO_DON" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::MiniTaiko_Don] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_MINITAIKO_KA" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("flashms" == a) { FlashMs[(int)FlashMsType::MiniTaiko_Ka] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_GOGOFIRE" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("nextflamems" == a) { NextFlameMs[(int)NextFlameMsType::GogoFire] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_EXPLOSION_UP" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("nextflamems" == a) { NextFlameMs[(int)NextFlameMsType::Explosion_Up] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_EXPLOSION_DOWN" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("nextflamems" == a) { NextFlameMs[(int)NextFlameMsType::Explosion_Down] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_EXPLOSION_SOUL" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("nextflamems" == a) { NextFlameMs[(int)NextFlameMsType::Explosion_Soul] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_SOUL" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("nextflamems" == a) { NextFlameMs[(int)NextFlameMsType::Soul] = stod(val); }
				}
			}
			if ("SKIN_PLAYING_COMBONUMBER" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Playing_Combo] = stod(val); }
					if ("bouncerate" == a) { ComboBounceRate = stod(val); }
					if ("changepoint1" == a) { ComboChange[0] = stoi(val); }
					if ("changepoint2" == a) { ComboChange[1] = stoi(val); }
					if ("changepoint3" == a) { ComboChange[2] = stoi(val); }
				}
			}
			if ("SKIN_PLAYING_SCORENUMBER" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Playing_Score] = stod(val); }
					if ("bouncerate" == a) { ScoreBounceRate = stod(val); }
					if ("addscore_r" == a) { AddScoreColor[0] = stoi(val); }
					if ("addscore_g" == a) { AddScoreColor[1] = stoi(val); }
					if ("addscore_b" == a) { AddScoreColor[2] = stoi(val); }
				}
			}
			if ("SKIN_PLAYING_JUDGEORIGIN" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("px" == a) { JudgeOrigin = stod(val); }
				}
			}
			
			if ("SKIN_RESULT_NUMBER_SCORE" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Score] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_GOOD" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Good] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_OK" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Ok] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_BAD" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Bad] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_ROLL" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Roll] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_MAXCOMBO" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_MaxCombo] = stod(val); }
				}
			}
			if ("SKIN_RESULT_NUMBER_RATE" == ID) {
				auto data = split(datas, ',');
				for (int i = 0; i < data.size(); ++i) {
					string val = data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1));
					string a = strtrim(data[i].substr(0, data[i].find(":")));
					if ("compressionrate" == a) { NumberCompressionRates[(int)NumberCompressionRateType::Result_Rate] = stod(val); }
				}
			}
		}
	}
}

void FileLoad::LoadSkinImage() {
	InitGraph();
	InitFontToHandle();
	InitSoundMem();
	vector<string> list;
	if (!LoadFileTEXT(list, "Skins/" + SkinFolderName + "/skinconfig.ini")) { return; }

	for (int i = 0; i < list.size(); ++i) {
		if (list[i].find("Image") != string::npos) {
			string datas = strtrim(list[i].substr(list[i].find("{") + 1, list[i].find("}") - (list[i].find("{") + 1)));
			string ID = strtrim(list[i].substr(list[i].find(":") + 1, list[i].find("{") - (list[i].find(":") + 1)));
			// Title
			if ("GAME_TITLE_BGSTAGE" == ID) { Image[(int)ImageID::GAME_TITLE_BGSTAGE] = LoadSkinImageData(datas); }

			// SongSelect
			if ("GAME_SONGSELECT_BG" == ID) { Image[(int)ImageID::GAME_SONGSELECT_BG] = LoadSkinImageData(datas); }
			if ("GAME_SONGSELECT_BGSTAGE" == ID) { Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE] = LoadSkinImageData(datas); }
			if ("GAME_SONGSELECT_BOX" == ID) { Image[(int)ImageID::GAME_SONGSELECT_BOX] = LoadSkinImageData(datas); }

			//Playing
			if ("GAME_PLAYING_BG" == ID) { Image[(int)ImageID::GAME_PLAYING_BG] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_BG_CLEAR" == ID) { Image[(int)ImageID::GAME_PLAYING_BG_CLEAR] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_BGSTAGE" == ID) { Image[(int)ImageID::GAME_PLAYING_BGSTAGE] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_BGSTAGE_CLEAR" == ID) { Image[(int)ImageID::GAME_PLAYING_BGSTAGE_CLEAR] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_SCROLLFIELD_BG" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_BG_NORMAL" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_BG_EXPERT" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_BG_MASTER" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_DON" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_DON] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_KA" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_KA] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_ROLL" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_ROLL] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_GOGO" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_GOGO] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SCROLLFIELD_HIT" == ID) { Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_HIT] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_MINITAIKO" == ID) { Image[(int)ImageID::GAME_PLAYING_MINITAIKO] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_MINITAIKO_DON" == ID) { Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_MINITAIKO_KA" == ID) { Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_NOTE" == ID) { Image[(int)ImageID::GAME_PLAYING_NOTE] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_BARLINE" == ID) { Image[(int)ImageID::GAME_PLAYING_BARLINE] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_BARLINE_BRANCH" == ID) { Image[(int)ImageID::GAME_PLAYING_BARLINE_BRANCH] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_GOGOFIRE" == ID) { Image[(int)ImageID::GAME_PLAYING_GOGOFIRE] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_JUDGESTRING" == ID) { Image[(int)ImageID::GAME_PLAYING_JUDGESTRING] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_SCORENUMBER" == ID) { Image[(int)ImageID::GAME_PLAYING_SCORENUMBER] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_COMBOSTRING1" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBOSTRING1] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COMBOSTRING2" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBOSTRING2] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COMBOSTRING3" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBOSTRING3] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COMBONUMBER1" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBONUMBER1] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COMBONUMBER2" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBONUMBER2] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COMBONUMBER3" == ID) { Image[(int)ImageID::GAME_PLAYING_COMBONUMBER3] = LoadSkinImageData(datas); }
			
			if ("GAME_PLAYING_COURSESYMBOL_EASY" == ID) { Image[(int)ImageID::GAME_PLAYING_COURSESYMBOL_EASY] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COURSESYMBOL_NORMAL" == ID) { Image[(int)ImageID::GAME_PLAYING_COURSESYMBOL_NORMAL] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COURSESYMBOL_HARD" == ID) { Image[(int)ImageID::GAME_PLAYING_COURSESYMBOL_HARD] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COURSESYMBOL_ONI" == ID) { Image[(int)ImageID::GAME_PLAYING_COURSESYMBOL_ONI] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_COURSESYMBOL_EDIT" == ID) { Image[(int)ImageID::GAME_PLAYING_COURSESYMBOL_EDIT] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_EXPLOSION_UP" == ID) { Image[(int)ImageID::GAME_PLAYING_EXPLOSION_UP] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_EXPLOSION_DOWN" == ID) { Image[(int)ImageID::GAME_PLAYING_EXPLOSION_DOWN] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_EXPLOSION_SOUL" == ID) { Image[(int)ImageID::GAME_PLAYING_EXPLOSION_SOUL] = LoadSkinImageData(datas); }

			if ("GAME_PLAYING_SOULPROGRESSBAR" == ID) { Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR] = LoadSkinImageData(datas); }
			if ("GAME_PLAYING_SOUL" == ID) { Image[(int)ImageID::GAME_PLAYING_SOUL] = LoadSkinImageData(datas); }

			// Result
			if ("GAME_RESULT_BGSTAGE_CLEAR" == ID) { Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_BGSTAGE_FAILED" == ID) { Image[(int)ImageID::GAME_RESULT_BGSTAGE_FAILED] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_BOARD" == ID) { Image[(int)ImageID::GAME_RESULT_BOARD] = LoadSkinImageData(datas); }

			if ("GAME_RESULT_NUMBER_GOOD" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_GOOD] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_NUMBER_OK" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_OK] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_NUMBER_BAD" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_BAD] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_NUMBER_ROLL" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_ROLL] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_NUMBER_MAXCOMBO" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_MAXCOMBO] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_NUMBER_RATE" == ID) { Image[(int)ImageID::GAME_RESULT_NUMBER_RATE] = LoadSkinImageData(datas); }

			if ("GAME_RESULT_STRING_GOOD" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_GOOD] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_STRING_OK" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_OK] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_STRING_BAD" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_BAD] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_STRING_ROLL" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_ROLL] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_STRING_MAXCOMBO" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_MAXCOMBO] = LoadSkinImageData(datas); }
			if ("GAME_RESULT_STRING_RATE" == ID) { Image[(int)ImageID::GAME_RESULT_STRING_RATE] = LoadSkinImageData(datas); }
		}
		if (list[i].find("Font") != string::npos) {
			string datas = strtrim(list[i].substr(list[i].find("{") + 1, list[i].find("}") - (list[i].find("{") + 1)));
			string ID = strtrim(list[i].substr(list[i].find(":") + 1, list[i].find("{") - (list[i].find(":") + 1)));
			// Title
			if ("GAME_TITLE_TITLE" == ID) { Font[(int)FontID::GAME_TITLE_TITLE] = LoadSkinFontData(datas); }
			if ("GAME_TITLE_CAPTION" == ID) { Font[(int)FontID::GAME_TITLE_CAPTION] = LoadSkinFontData(datas); }

			// SongSelect
			if ("GAME_SONGSELECT_TITLE" == ID) { Font[(int)FontID::GAME_SONGSELECT_TITLE] = LoadSkinFontData(datas); }
			if ("GAME_SONGSELECT_SUBTITLE" == ID) { Font[(int)FontID::GAME_SONGSELECT_SUBTITLE] = LoadSkinFontData(datas); }

			// Playing
			if ("GAME_PLAYING_TITLE" == ID) { Font[(int)FontID::GAME_PLAYING_TITLE] = LoadSkinFontData(datas); }
			if ("GAME_PLAYING_SUBTITLE" == ID) { Font[(int)FontID::GAME_PLAYING_SUBTITLE] = LoadSkinFontData(datas); }

			// Result
			if ("GAME_RESULT_TITLE" == ID) { Font[(int)FontID::GAME_RESULT_TITLE] = LoadSkinFontData(datas); }
			if ("GAME_RESULT_SUBTITLE" == ID) { Font[(int)FontID::GAME_RESULT_SUBTITLE] = LoadSkinFontData(datas); }

			// Config
			if ("GAME_CONFIG_TITLE" == ID) { Font[(int)FontID::GAME_CONFIG_TITLE] = LoadSkinFontData(datas); }
			if ("GAME_CONFIG_CAPTION" == ID) { Font[(int)FontID::GAME_CONFIG_CAPTION] = LoadSkinFontData(datas); }
		}
		if (list[i].find("Sound") != string::npos) {
			string datas = strtrim(list[i].substr(list[i].find("{") + 1, list[i].find("}") - (list[i].find("{") + 1)));
			string ID = strtrim(list[i].substr(list[i].find(":") + 1, list[i].find("{") - (list[i].find(":") + 1)));
			if ("GAME_SOUND_DON" == ID) { Sound[(int)SoundID::GAME_SOUND_DON] = LoadSkinSoundData(datas); }
			if ("GAME_SOUND_KA" == ID) { Sound[(int)SoundID::GAME_SOUND_KA] = LoadSkinSoundData(datas); }
			if ("GAME_SOUND_BALLOON" == ID) { Sound[(int)SoundID::GAME_SOUND_BALLOON] = LoadSkinSoundData(datas); }
		}
	}
}

ImageData FileLoad::LoadSkinImageData(string datas) {
	ImageData item;
	auto data = split(datas, ',');
	for (int i = 0; i < data.size(); ++i) {
		string val = strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("r" == a) { item.r = stoi(val); }
		if ("g" == a) { item.g = stoi(val); }
		if ("b" == a) { item.b = stoi(val); }
		if ("a" == a) { item.a = stoi(val); }
		if ("dx" == a) { item.dx = stoi(val); }
		if ("dy" == a) { item.dy = stoi(val); }
		if ("x" == a) { item.x = stod(val); }
		if ("y" == a) { item.y = stod(val); }
		if ("width" == a) { item.width = stod(val); }
		if ("height" == a) { item.height = stod(val); }
	}
	for (int i = 0; i < data.size(); ++i) {
#ifndef __ANDROID__
		string val = "Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
#else
		string val = (string)ApplicationPath + "/Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
#endif
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("filepath" == a) {
			int handle = LoadGraph(val.c_str());
			int w = 0;
			int h = 0;
			GetGraphSize(handle, &w, &h);
			if (item.width == DBL_MIN) { item.width = w / (float)item.dx; }
			if (item.height == DBL_MIN) { item.height = h / (float)item.dy; }
			DeleteGraph(handle);
			int* handles = new int[item.dx * item.dy];
			LoadDivGraphF(val.c_str(), item.dx * item.dy, item.dx, item.dy, (float)w / item.dx, (float)h / item.dy, handles);
			for (int j = 0; j < item.dx * item.dy; ++j) {
				item.handle.push_back(handles[j]);
			}
			delete[] handles;
		}
	}
	return item;
}

FontData FileLoad::LoadSkinFontData(string datas) {
	FontData item;
	auto data = split(datas, ',');
	string name = "";
	int size = 0, thick = 0, edge = 0;
	for (int i = 0; i < data.size(); ++i) {
		string val = strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("fontname" == a) { name = val; }
		if ("size" == a) { size = item.size = (int)stod(val); }
		if ("thick" == a) { thick = item.thick = (int)(stod(val) / 2); }
		if ("edge" == a) { edge = item.edge = (int)stod(val); }
		if ("position" == a) { item.position = stoi(val); }
		if ("x" == a) { item.x = stod(val); }
		if ("y" == a) { item.y = stod(val); }
	}
	for (int i = 0; i < data.size(); ++i) {
#ifndef __ANDROID__
		string val = "Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
#else
		string val = ((string)ApplicationPath + "/Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1))));
#endif
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("filepath" == a) {
			AddFont(val);
			item.handle = CreateFontToHandle(TEXT(name.c_str()), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, DX_CHARSET_DEFAULT, edge);
			SetFontSpaceToHandle(-4, item.handle);
		}
	}
	return item;
}

SoundData FileLoad::LoadSkinSoundData(string datas) {
	SoundData item;
	auto data = split(datas, ',');
	int buffernum = 0;
	for (int i = 0; i < data.size(); ++i) {
		string val = strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("buffernum" == a) { buffernum = stoi(val); }
	}
	for (int i = 0; i < data.size(); ++i) {
#ifndef __ANDROID__
		string val = "Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
#else
		string val = (string)ApplicationPath + "/Skins/" + SkinFolderName + "/" + strtrim(data[i].substr(data[i].find(":") + 1, data[i].size() - (data[i].find(":") + 1)));
#endif
		string a = strtrim(data[i].substr(0, data[i].find(":")));
		if ("filepath" == a) {
			item.handle = LoadSoundMem(val.c_str(), buffernum);
			ChangeVolumeSoundMem(255 * (SEVolume / 100.), item.handle);
			item.frequency = GetFrequencySoundMem(item.handle);
		}
	}
	return item;
}

vector<SongData> FileLoad::LoadChartFiles(string LoadDir) {
	vector<SongData> items;
	int i = 0;
#ifndef __ANDROID__
	if (!filesystem::is_directory(LoadDir)) {
		MessageBox(NULL, TEXT("Songsフォルダがありません\n予期しない動作が起こる可能性があります"), TEXT("WARNING"), MB_OK | MB_ICONWARNING);
#else
		if (!__ndk1::__fs::filesystem::is_directory(LoadDir)) {
#endif
		return items;
	}

	int errorcount = 0;
#ifndef __ANDROID__
	for (auto e : filesystem::recursive_directory_iterator(LoadDir)) {
#else
	for (auto e : __ndk1::__fs::filesystem::recursive_directory_iterator(LoadDir)) {
#endif
		try {
			if (e.path().extension() == ".tja") { items.push_back(LoadTJAChartData(e.path().string())); ++i; }
			if (e.path().extension() == ".tmg") { items.push_back(LoadTMGChartData(e.path().string())); ++i; }
		}
		catch (...) {
			errorcount++;
		}
		
#ifdef _DEBUG
		if (i >= 20) { break; }
#endif
	}
	if (errorcount > 0) {
#ifndef __ANDROID__
		MessageBox(NULL, TEXT(("一部譜面ファイルの読み込みに失敗しました\n失敗した数：" + to_string(errorcount)).c_str()), TEXT("WARNING"), MB_OK | MB_ICONWARNING);
#else
		// TODO: 読み込めなかったことを報告する方法を考える
#endif
	}
	if (items.size() == 0) {
#ifndef __ANDROID__
		MessageBox(NULL, TEXT("譜面がありません\n予期しない動作が起こる可能性があります"), TEXT("WARNING"), MB_OK | MB_ICONWARNING);
#else
		// TODO: 譜面が読み込めなかった場合代わりの処理を実行する
#endif
	}
	return items;
}

SongData FileLoad::LoadTJAChartData(string path) {
	SongData item;
	vector<string> list;
	if (!LoadFileTEXT(list, path)) { return item; }
	item.path = path;
	int course = -1;
	int level = 0;
	for (int i = 0; i < list.size(); ++i) {
		string data = strtrim(list[i].substr(list[i].find(":") + 1, list[i].size() - (list[i].find(":") + 1)));
		string ID = strtrim(list[i].substr(0, list[i].find(":")));
		if ("TITLE" == ID) { item.title = data; item.titlestrlen = GetDrawStringWidthToHandle(data.c_str(), strlenDx(data.c_str()), Font[(int)FontID::GAME_SONGSELECT_TITLE].handle); }
		if ("SUBTITLE" == ID) { data = data.find("--") == 0 || data.find("++") == 0 ? data.replace(0, 2, "") : data; item.subtitle = data; item.subtitlestrlen = GetDrawStringWidthToHandle(data.c_str(), strlenDx(data.c_str()), Font[(int)FontID::GAME_SONGSELECT_SUBTITLE].handle); }
		if ("BPM" == ID) { item.bpm = stod(data); }
		if ("LEVEL" == ID) { level = stoi(data); }
		if ("COURSE" == ID) {
			if (data[0] >= '0' && data[0] <= '9') {
				course = stoi(data);
			}
			else {
				for (int i = 0; i < data.size(); ++i) {
					data[i] = tolower(data[i]);
				}
				if ("easy" == data) { course = 0; }
				if ("normal" == data) { course = 1; }
				if ("hard" == data) { course = 2; }
				if ("oni" == data) { course = 3; }
				if ("edit" == data) { course = 4; }
			}
		}
		if ("#START" == strtrim(list[i])) {
			if (course == -1) { item.level[3] = level <= 0 ? 10 : level; item.course[3] = true; item.startindex[3] = i; }
			if (course != -1) { item.level[course] = level <= 0 ? 10 : level; item.course[course] = true; item.startindex[course] = i; }
		}
	}
	return item;
}

SongData FileLoad::LoadTMGChartData(string path) {
	SongData item;
	vector<string> list;
	if (!LoadFileTEXT(list, path)) { return item; }
	item.path = path;
	int course = -1;
	int level = 0;
	for (int i = 0; i < list.size(); ++i) {
		string data = strtrim(list[i].substr(list[i].find(":") + 1, list[i].size() - (list[i].find(":") + 1)));
		string ID = strtrim(list[i].substr(0, list[i].find(":")));
		if ("TITLE" == ID) { data = data.find("--") == 0 || data.find("++") == 0 ? data.replace(0, 2, "") : data; item.title = data; item.titlestrlen = GetDrawStringWidthToHandle(data.c_str(), strlenDx(data.c_str()), Font[(int)FontID::GAME_SONGSELECT_TITLE].handle); }
		if ("SUBTITLE" == ID) { data = data.find("--") == 0 || data.find("++") == 0 ? data.replace(0, 2, "") : data; item.subtitle = data; item.subtitlestrlen = GetDrawStringWidthToHandle(data.c_str(), strlenDx(data.c_str()), Font[(int)FontID::GAME_SONGSELECT_SUBTITLE].handle); }
		if ("BPM" == ID) { item.bpm = stod(data); }
		if ("LEVEL" == ID) { level = stoi(data); }
		if ("COURSE" == ID) {
			if (data[0] >= '0' && data[0] <= '9') {
				course = stoi(data);
			}
			else {
				for (int i = 0; i < data.size(); ++i) {
					data[i] = tolower(data[i]);
				}
				if ("easy" == data) { course = 0; }
				if ("normal" == data) { course = 1; }
				if ("hard" == data) { course = 2; }
				if ("oni" == data) { course = 3; }
				if ("edit" == data) { course = 4; }
			}
		}
		if ("#START()" == strtrim(list[i])) {
			if (course == -1) { item.level[3] = level <= 0 ? 10 : level; item.course[3] = true; course = 3; item.startindex[3] = i; }
			if (course != -1) { item.level[course] = level <= 0 ? 10 : level; item.course[course] = true; item.startindex[course] = i; }
		}
	}
	return item;
}