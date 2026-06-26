#pragma once
#include "System/include.h"

extern FileLoad& FL;

#define NOWTIME ConvSysPerformanceCountToNanoSeconds(GetNowSysPerformanceCount())
#define VSYNCTIME (1000. / GetRefreshRate())

enum class SecneType {
	End,
	Title,
	SongSelect,
	ChartLoad,
	Playing,
	Result,
	Config,
};

enum class Animation {
	// SongSelect
	SongSelect_MotionTime,
	SongSelect_Deploy,
	
	// Playing
	COMBOBOUNCE,
	SCOREBOUNCE,

	SCROLLFIELD_DON,
	SCROLLFIELD_KA,
	SCROLLFIELD_ROLL,
	SCROLLFIELD_HIT,

	GOGOTime,

	MINITAIKO_DON_L,
	MINITAIKO_DON_R,
	MINITAIKO_KA_L,
	MINITAIKO_KA_R,

	Branch,

	TJACreate_Motion,

	NowTime,

	A_Count
};

enum class SCROLLType {
	Normal,
	BMScroll,
	HBScroll
};

enum class BranchType {
	Null,
	Normal,
	Expert,
	Master,
	NotDisplay,
};

enum class IfBranchType {
	Null,
	Parfect,
	Roll,
	Score,
};

enum class ABranchType {
	Null,
	Normal_Expert,
	Normal_Master,
	Expert_Normal,
	Expert_Master,
	Master_Normal,
	Master_Expert,
};

enum class GOGOTime {
	Null,
	GOGOStart,
	GOGOEnd,
};

enum class NoteSpawnType {
	Init,
	Spawn,
	Vanish,
	Dispose_Spawn,
	Dispose_Vanish,
};

enum class GradationType {
	Null,
	Set,
	Start,
	End
};

enum class GradationParamType {
	Scroll,
	Scrolli,
	ScrollRadius,
	ScrollDivValue,
	ScrollTheta,
	JudgeDelayMs,
	JudgeDelayX,
	JudgeDelayY,
	ColorR,
	ColorG,
	ColorB,
	Alpha,
	Size,
	Angle,
	BarlineSizeW,
	BarlineSizeH,
	Count
};

class NoteSpawnData {
public:
	double Ms = 0;
	NoteSpawnType Type = NoteSpawnType::Init;
};

class GradationData {
public:
	GradationType Flag = GradationType::Null;
	EasingType1 MotionType = EasingType1::In;
	EasingType2 LineType = EasingType2::Linear;
	double OffsetMs = 0;
	double MotionTime = 0;
	double MotionMs[(int)GradationParamType::Count]{};
	vector<double> Param[(int)GradationParamType::Count];
	int Phase[(int)GradationParamType::Count]{};
};

class TJANoteData {
public:
	double AbsTime = 0;
	double RelaTime = 0;
	double BMTime = 0;
	double DelayTime = 0;
	
	double Scroll = 1;
	double Scrolli = 0;
	
	double BPM = 0;
	double Beat = 1;

	BranchType IsBranch = BranchType::Null;
	bool BranchStart = false;
	bool Section = false;
	bool LevelHold = false;
	
	double SuddenSpawnMs = 0;
	double SuddenMotionMs = 0;
	int SuddenFlag = 0;

	double JPOSMotionMs = 0;
	double JPOSMotionPx_x = 0;
	double JPOSAddPx_x = 0;
	int JPOSFlag = 0;

	bool RollFlag = false;
	unsigned int RollEndIndex = 0;
	int BalloonCount = 0;
	int BalloonFlag = 0;

	GOGOTime GOGOFlag = GOGOTime::Null;

	bool BpmchangeFlag = false;
	bool BpmSpawnFlag = false;

	bool BMFlag = false;

	bool DummyFlag = false;

	bool HitFlag = false;
	
	bool Barline = false;

	bool DisplayFlag = true;

	string Lyric = "";

	char Type = '0';
};

class TMGNoteData {
public:
	double AbsTime = 0;
	double RelaTime = 0;
	double BMTime = 0;
	double DelayTime = 0;
	
	double Scroll = 1;
	double Scrolli = 0;

	bool ScrollPolarFlag = false;
	double ScrollRadius = 0;
	double ScrollDivValue = 0;
	double ScrollTheta = 0;

	double HiSpeed = 1;
	bool HiSpeedChangeFlag = false;

	double BPM = 0;
	bool BpmChangeFlag = false;
	bool BMFlag = false;
	double Beat = 1;

	BranchType DrawBranch = BranchType::Null;
	BranchType IsBranch = BranchType::Null;
	bool BranchStart = false;
	bool Section = false;
	bool LevelHold = false;
	
	double SuddenSpawnMs = 0;
	double SuddenMotionMs = 0;
	int SuddenFlag = 0;

	double JPOSMotionMs = 0;
	double JPOSMotionPx_x = 0;
	double JPOSMotionPx_y = 0;
	double JPOSAddPx_x = 0;
	double JPOSAddPx_y = 0;
	int JPOSFlag = 0;
	bool JPOSDefaultPosFlag = false;

	vector<NoteSpawnData> NoteSpawn;

	double JudgeDelayMs = 0;
	double JudgeDelayX = 0;
	double JudgeDelayY = 0;

	double ColorR = 255;
	double ColorG = 255;
	double ColorB = 255;
	double Alpha = 255;

	double Size = 1;

	double Angle = 0;

	double BarlineSizeW = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].width;
	double BarlineSizeH = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height;
	bool Barline = false;

	GradationData Gradation;

	bool RollFlag = false;
	unsigned int RollEndIndex = 0;
	int BalloonCount = 0;
	int BalloonFlag = 0;

	string Lyric = "";
	bool LyricChangeFlag = false;

	GOGOTime GOGOFlag = GOGOTime::Null;

	bool DummyFlag = false;

	bool HitFlag = false;
	
	bool DisplayFlag = true;

	char Type = '0';
};

class HitNoteData {
public:
	HitNoteData(const string& type = "", int note = 0, int score = 0) {
		note -= 48;
		int goodbig = note == 1 || note == 2 ? 0 : 2;
		int okbig = note == 1 || note == 2 ? 1 : 3;
		Score_Num = score;
		if (type == "good") {
			ExplosionUp_Type = goodbig;
			ExplosionDown_Type = goodbig;
			JudgeString_Type = 0;
			HitNote_Type = note;
		}
		else if (type == "ok") {
			ExplosionUp_Type = okbig;
			ExplosionDown_Type = okbig;
			JudgeString_Type = 1;
			HitNote_Type = note;
		}
		else if (type == "bad") {
			JudgeString_Type = 2;
			ExplosionDown_Dispose = true;
			ExplosionUp_Dispose = true;
			HitNote_Dispose = true;
			Score_Dispose = true;
		}
		else if (type == "roll") {
			HitNote_Type = note;
			ExplosionDown_Dispose = true;
			ExplosionUp_Dispose = true;
			JudgeString_Dispose = true;
		}
		else {
			ExplosionUp_Dispose = true;
			ExplosionDown_Dispose = true;
			JudgeString_Dispose = true;
			HitNote_Dispose = true;
			Score_Dispose = true;
		}
	}

	double ExplosionUp_Ms = 0;
	int ExplosionUp_Type = 0;
	int ExplosionUp_Flame = 0;
	
	double ExplosionDown_Ms = 0;
	int ExplosionDown_Type = 0;
	int ExplosionDown_Flame = 0;

	double JudgeString_Ms = 0;
	double JudgeString_GetMs = -1;
	double JudgeString_Y = 0;
	int JudgeString_Type = 0;
	double JudgeString_Alpha = 255;

	double HitNote_Ms = 0;
	double HitNote_GetMs = -1;
	int HitNote_Type = 0;
	double HitNote_X = 0;
	double HitNote_Y = 0;
	double HitNote_Yellow = 0;
	double HitNote_White = 0;
	double HitNote_Alpha = 0;

	double Score_Ms = 0;
	double Score_GetMs = -1;
	int Score_Num = 0;
	double Score_X = 0;
	double Score_Y = 0;
	double Score_Alpha = 255;

	bool ExplosionUp_Dispose = false;
	bool ExplosionDown_Dispose = false;
	bool JudgeString_Dispose = false;
	bool HitNote_Dispose = false;
	bool Score_Dispose = false;
};

class BarlineData {
public:
	double AbsTime = 0;
	bool DisplayFlag = false;
};

class BranchData {
public:
	double AbsTime = 0;
	double StartMs = 0;
	bool Start = false;
	bool BranchFlag = false;
	IfBranchType Type = IfBranchType::Null;
	double ExpertBranch = 0;
	double MasterBranch = 0;
};

class ChartData {
public:
	SCROLLType ScrollType = SCROLLType::Normal;
	bool TJAChartFlag = false;
	bool TMGChartFlag = false;
	bool NowChartConvert = false;
	bool ChartCreateMode = false;
	int BarlineIndex = 0;
	double BarlineTime = 0;

	SoundData Song;

	string Title;
	int TitleStrLen = 0;
	bool TitleDispFlag = true;
	
	string SubTitle;
	int SubTitleStrLen = 0;
	bool SubTitleDispFlag = true;

	double BlankTime = 0;

	bool GOGOFlag = false;
	int GOGOFlame = 0;

	BranchType NowBranchFlag = BranchType::Null;
	ABranchType NowBranchAnimation = ABranchType::Null;
	bool LevelHold = false;
	long BranchScore = 0;
	long BranchGood = 0;
	long BranchOk = 0;
	long BranchBad = 0;
	long BranchRoll = 0;
	double BranchRate = 0;

	long Score = 0;
	long Good = 0;
	long Ok = 0;
	long Bad = 0;
	long Combo = 0;
	long Roll = 0;
	double Rate = 0;

	int AddScore = 0;

	long SoulMaxProgress = 0;
	long SoulProgress = 0;
	int SoulAddProgress = 135;

	long HitNote = 0;
	long BranchHitNote = 0;

	int NowActiveRollNum = 0;
	int NowActiveBalloonNum = 0;

	double WaitRollTime = 0;

	double GetNowTime = 0;
	double NowTime = 0;

	double NowBPM = 0;

	double NowHiSpeed = 0;

	string NowLyric = "";

	double Note_addx = 0;
	double Note_addy = 0;

	vector<TJANoteData> TJANoteDatas;
	vector<TMGNoteData> TMGNoteDatas;
	
	vector<BarlineData> BarlineDatas;

	vector<BranchData> BranchDatas;
};

class TimeData {
public:
	inline void start() {
		start_t = NOWTIME;
		IsGetNowTime = true;
	}
	inline void end() {
		start_t = NOWTIME;
		IsGetNowTime = false;
	}
	inline double GetNowTime() {
		long long end_t = NOWTIME;
		return IsGetNowTime ? (double)(end_t - start_t) / 1000000. : 0;
	}
	bool IsGetNowTime = false;
private:
	long long start_t = 0;
};

extern SecneType ProgressFlag;

extern TimeData SelectMotion;

extern double &&FeedPal;

extern bool NowLoadSongListDatas;

extern double Note_ox;
extern double Note_oy;

extern bool CourseSelect;
extern int CourseSelector;

extern bool ConfigSelect;
extern bool KeyConfigSelect;
extern bool KeySeter;
extern int ConfigGenreSelector;
extern int ConfigSelector;
extern int KeyConfigSelector;
extern int InputHandle;
extern char InputString[256];

extern int SongSelector;
extern vector<SongData> SongList;

extern double AnimationTime[(int)Animation::A_Count];

extern ChartData& CD;

extern bool ScreenFlipFlag;

#define HitNoteLimiter 128
extern HitNoteData HitNoteDatas[HitNoteLimiter];

extern int SubProcessCount;
extern TimeData SubProcessTime;

void ProcessThread();

int GetNowFlameCount(const double& MsTime);
void AddHitNoteData(const HitNoteData& data);
inline void TitleProcess();
inline void SongSelectProcess();
inline void ChartLoadProcess();
inline void PlayingProcess();
inline void ResultProcess();
inline void ConfigProcess();
