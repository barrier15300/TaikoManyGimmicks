#pragma once
#include "System/include.h"

enum class ImageID {
	// Title
	GAME_TITLE_BGSTAGE,

	// SongSelect
	GAME_SONGSELECT_BG,
	GAME_SONGSELECT_BGSTAGE,
	GAME_SONGSELECT_BOX,

	// Playing
	GAME_PLAYING_BG,
	GAME_PLAYING_BG_CLEAR,
	GAME_PLAYING_BGSTAGE,
	GAME_PLAYING_BGSTAGE_CLEAR,

	GAME_PLAYING_SCROLLFIELD_BG,
	GAME_PLAYING_SCROLLFIELD_BG_NORMAL,
	GAME_PLAYING_SCROLLFIELD_BG_EXPERT,
	GAME_PLAYING_SCROLLFIELD_BG_MASTER,
	GAME_PLAYING_SCROLLFIELD_DON,
	GAME_PLAYING_SCROLLFIELD_KA,
	GAME_PLAYING_SCROLLFIELD_ROLL,
	GAME_PLAYING_SCROLLFIELD_GOGO,
	GAME_PLAYING_SCROLLFIELD_HIT,

	GAME_PLAYING_MINITAIKO,
	GAME_PLAYING_MINITAIKO_DON,
	GAME_PLAYING_MINITAIKO_KA,

	GAME_PLAYING_NOTE,
	GAME_PLAYING_BARLINE,
	GAME_PLAYING_BARLINE_BRANCH,

	GAME_PLAYING_GOGOFIRE,

	GAME_PLAYING_JUDGESTRING,
	GAME_PLAYING_SCORENUMBER,

	GAME_PLAYING_COMBOSTRING1,
	GAME_PLAYING_COMBOSTRING2,
	GAME_PLAYING_COMBOSTRING3,
	GAME_PLAYING_COMBONUMBER1,
	GAME_PLAYING_COMBONUMBER2,
	GAME_PLAYING_COMBONUMBER3,

	GAME_PLAYING_COURSESYMBOL_EASY,
	GAME_PLAYING_COURSESYMBOL_NORMAL,
	GAME_PLAYING_COURSESYMBOL_HARD,
	GAME_PLAYING_COURSESYMBOL_ONI,
	GAME_PLAYING_COURSESYMBOL_EDIT,

	GAME_PLAYING_EXPLOSION_UP,
	GAME_PLAYING_EXPLOSION_DOWN,
	GAME_PLAYING_EXPLOSION_SOUL,

	GAME_PLAYING_SOULPROGRESSBAR,
	GAME_PLAYING_SOUL,

	// Result
	GAME_RESULT_BGSTAGE_CLEAR,
	GAME_RESULT_BGSTAGE_FAILED,
	GAME_RESULT_BOARD,

	GAME_RESULT_NUMBER_GOOD,
	GAME_RESULT_NUMBER_OK,
	GAME_RESULT_NUMBER_BAD,
	GAME_RESULT_NUMBER_ROLL,
	GAME_RESULT_NUMBER_MAXCOMBO,
	GAME_RESULT_NUMBER_RATE,

	GAME_RESULT_STRING_GOOD,
	GAME_RESULT_STRING_OK,
	GAME_RESULT_STRING_BAD,
	GAME_RESULT_STRING_ROLL,
	GAME_RESULT_STRING_MAXCOMBO,
	GAME_RESULT_STRING_RATE,

	// Config
	GAME_CONFIG_BG,
	GAME_CONFIG_BGSTAGE,
	GAME_CONFIG_BOARD,
	GAME_CONFIG_BOX,

	ImageCount
};

enum class FontID {
	// Title
	GAME_TITLE_TITLE,
	GAME_TITLE_CAPTION,

	// Select
	GAME_SONGSELECT_TITLE,
	GAME_SONGSELECT_SUBTITLE,

	// Playing
	GAME_PLAYING_TITLE,
	GAME_PLAYING_SUBTITLE,

	// Result
	GAME_RESULT_TITLE,
	GAME_RESULT_SUBTITLE,

	// Config
	GAME_CONFIG_TITLE,
	GAME_CONFIG_CAPTION,

	FontCount
};

enum class SoundID {
	GAME_SOUND_DON,
	GAME_SOUND_KA,
	GAME_SOUND_BALLOON,
	SoundCount
};

enum class ConfigID {
	AutoPlayFlag,
	ConstantSCROLL,
	ConstantBPM,
	AddSCROLL,
	StableVSyncFlag,
	JudgeGood,
	JudgeOk,
	JudgeBad,
	JudgeOffset,
	SongPlaySpeed,
	SongBlankTime,
	SongOffset,
	SongVolume,
	SEVolume,
	ChartHeaderSound,
	JiroBPMDelayFlag,
	DrawingOperationRangeWidth,
	DrawingOperationRangeHeight,
	RandomRate,
	RollSpeed,
	ChartCreateMode,
	GridScreen,
	DebugScreen,
	ConfigCount
};

#ifndef __ANDROID__
enum class KeyConfigID {
	LeftDonInput1,
	LeftDonInput2,
	LeftDonInput3,
	LeftDonInput4,
	LeftDonInput5,
	LeftDonInput6,
	LeftDonInput7,
	LeftDonInput8,
	LeftDonInput9,
	LeftDonInput10,

	LeftKaInput1,
	LeftKaInput2,
	LeftKaInput3,
	LeftKaInput4,
	LeftKaInput5,
	LeftKaInput6,
	LeftKaInput7,
	LeftKaInput8,
	LeftKaInput9,
	LeftKaInput10,

	RightDonInput1,
	RightDonInput2,
	RightDonInput3,
	RightDonInput4,
	RightDonInput5,
	RightDonInput6,
	RightDonInput7,
	RightDonInput8,
	RightDonInput9,
	RightDonInput10,

	RightKaInput1,
	RightKaInput2,
	RightKaInput3,
	RightKaInput4,
	RightKaInput5,
	RightKaInput6,
	RightKaInput7,
	RightKaInput8,
	RightKaInput9,
	RightKaInput10,

	K_AutoPlayFlagInput,
	K_DebugScreenInput,
	K_GridScreenInput,
	K_TJACreateModeInput,

	KeyConfigCount
};

enum class QuickAccessID {
	AutoPlayFlag,
	DebugScreen,
	GridScreen,
	ChartCreateMode,
	QuickAccessKeyCount
};
#endif

enum class FlashMsType {
	ScrollField_Don,
	ScrollField_Ka,
	ScrollField_Roll,
	ScrollField_Hit,

	MiniTaiko_Don,
	MiniTaiko_Ka,
	
	Count
};

enum class NextFlameMsType {
	GogoFire,

	Explosion_Up,
	Explosion_Down,
	Explosion_Soul,

	Soul,

	Count
};

enum class NumberCompressionRateType {
	Playing_Combo,
	Playing_Score,

	Result_Score,
	Result_Good,
	Result_Ok,
	Result_Bad,
	Result_Roll,
	Result_MaxCombo,
	Result_Rate,

	Count
};

class ImageData {
public:
	vector<int> handle;
	double x = 0;
	double y = 0;
	double width = DBL_MIN;
	double height = DBL_MIN;
	int dx = 1;
	int dy = 1;
	int r = 0, g = 0, b = 0, a = 0;
};

class FontData {
public:
	int handle = 0;
	int position = 0;
	double x = 0;
	double y = 0;
	int size = 0;
	int thick = 0;
	int edge = 0;
	int space = 0;
};

class SoundData {
public:
	int frequency = 0;
	int handle = 0;
};

class SongData {
public:
	string path = "";
	string title = "";
	string subtitle = "";
	int titlestrlen = 0;
	int subtitlestrlen = 0;
	double bpm = 0;
	int level[5]{ 0,0,0,0,0 };
	int startindex[5]{ 0,0,0,0,0 };
	bool course[5]{ false,false,false,false,false };
	int r = 0, g = 0, b = 0, a = 0;
};

class FileLoad {
public:

	//// config.ini

	// public
	bool AutoPlayFlag = false;
	double ConstantSCROLL = 0;
	double ConstantBPM = 0;
	double AddSCROLL = 1;
	bool StableVSyncFlag = false;
	double JudgeGood = 0;
	double JudgeOk = 0;
	double JudgeBad = 0;
	double JudgeOffset = 0;
	double SongPlaySpeed = 0;
	double SongBlankTime = 0;
	double SongOffset = 0;
	int SongVolume = 100;
	int SEVolume = 100;
	bool ChartHeaderSound = true;
	bool JiroBPMDelayFlag = false;
	double DrawingOperationRangeWidth = 0;
	double DrawingOperationRangeHeight = 0;
	int RandomRate = 0;
	double RollSpeed = 0;
	bool ChartCreateMode = false;
	bool GridScreen = false;
	bool DebugScreen = false;

	// private
	string SkinFolderName = "Default";
	bool WaitVSyncFlag = true;
	int WindowSizeWidth = 1280;
	int WindowSizeHeight = 720;

#ifndef __ANDROID__
	bool Error = false;

	//// keyconfig.ini
	int KEY_INPUT_LK[10] = {KEY_INPUT_D,0,0,0,0,0,0,0,0,0 };
	int KEY_INPUT_LD[10] = {KEY_INPUT_F,0,0,0,0,0,0,0,0,0 };
	int KEY_INPUT_RD[10] = {KEY_INPUT_J,0,0,0,0,0,0,0,0,0 };
	int KEY_INPUT_RK[10] = {KEY_INPUT_K,0,0,0,0,0,0,0,0,0 };

	int QuickAccessKey[(int)QuickAccessID::QuickAccessKeyCount] = { KEY_INPUT_1,KEY_INPUT_2,KEY_INPUT_3,KEY_INPUT_4};
#else
	char ApplicationPath[256];
#endif

	// SkinData
	ImageData Image[(int)ImageID::ImageCount];
	FontData Font[(int)FontID::FontCount];
	SoundData Sound[(int)SoundID::SoundCount];
	string Name = "";
	int ScreenX = 0, ScreenY = 0, ColorBit = 0;
	bool HitNoteDispFlag = false;
	double HitNoteHeight = 0;
	bool SCROLLFieldExtensionFlag = false;
	double FlashMs[(int)FlashMsType::Count]{};
	double NextFlameMs[(int)NextFlameMsType::Count]{};
	int ComboChange[3]{0,0,0};
	double ComboBounceRate = 0;
	double ScoreBounceRate = 0;
	double NumberCompressionRates[(int)NumberCompressionRateType::Count]{};
	double JudgeOrigin = 0;
	int AddScoreColor[3]{ 0,0,0 };

	// ConfigLoad
	void LoadConfigFile();
	void MakeConfigFile();
#ifndef __ANDROID__
	void LoadKeyConfigFile();
	void MakeKeyConfigFile();
#endif
	

	// SkinLoad
	void LoadSkinConfig();
	void LoadSkinImage();

	// ChartFilesLoad
	vector<SongData> LoadChartFiles(string LoadDir);

	// ChartData
	SongData LoadTJAChartData(string path);
	SongData LoadTMGChartData(string path);

private:

	// SkinData
	ImageData LoadSkinImageData(string data);
	FontData LoadSkinFontData(string datas);
	SoundData LoadSkinSoundData(string datas);
};