#include "sub.h"
#include "ChartLib.h"

FileLoad _FL;
FileLoad& FL = _FL;
SecneType ProgressFlag = SecneType::Title;

bool NowLoadSongListDatas = false;

double &&FeedPal = 0;

#ifndef __ANDROID__
string DragFilePath = "";
#endif

double Note_ox;
double Note_oy;

bool CourseSelect = false;
int CourseSelector = 0;

bool ConfigSelect = false;
bool KeyConfigSelect = false;
bool KeySeter = false;
int ConfigGenreSelector;
int ConfigSelector = 0;
int KeyConfigSelector = 0;
int InputHandle = 0;
char InputString[256];

int SongSelector = 0;
vector<SongData> SongList;

double AnimationTime[(int)Animation::A_Count];
TimeData ChartCreateMotion;

HitNoteData HitNoteDatas[HitNoteLimiter];
int HitNoteDatasIndex = 0;

ChartData _CD;

ChartData& CD = _CD;

bool ScreenFlipFlag = false;

int SubProcessCount = 0;
TimeData SubProcessTime;

enum class GridConfig {
	ComplexGridBeat,
	ComplexGridBeatWidth,
	ComplexGridBeatHeight,
	ComplexGridRate,
	ComplexGridRateWidth,
	ComplexGridRateHeight,
	PolarGridBeat,
	PolarGridBeatWidth,
	PolarGridBeatHeight,
	PolarGridRate,
	PolarGridRateWidth,
	PolarGridRateHeight,
	GridType,
	ValueType,
	Count
};
bool NowGridConfigFlag = false;
bool GridConfigs[(int)GridConfig::Count];
double GridConfigValue[(int)GridConfig::Count];

#ifndef __ANDROID__
char KeyBuf[256];
int KeyInt[256];

#else
TOUCHINPUTPOINT TouchList[TOUCHINPUTPOINT_MAX];
int TouchNum = 0;
bool TouchProc = false;
#endif

int GetNowFlameCount(const double& MsTime) {
	return (int)floor(MsTime / VSYNCTIME);
}

inline void FeedInBack(bool InBackflag, double feedtime = 350) {
	TimeData _feedtime;
	if (feedtime <= 0) {
		feedtime = 0.1;
	}
	while (FeedPal != 0) {}
	_feedtime.start();
	if (InBackflag) {
		while (_feedtime.GetNowTime() < feedtime) {
			FeedPal = 255 * (_feedtime.GetNowTime() / feedtime);
		}
	}
	else {
		while (_feedtime.GetNowTime() < feedtime) {
			FeedPal = 255 * (1 - (_feedtime.GetNowTime() / feedtime));
		}
	}
	FeedPal = 0;
}

void AddHitNoteData(const HitNoteData& data) {
	HitNoteDatas[HitNoteDatasIndex] = data;
	HitNoteDatasIndex++;
	if (HitNoteDatasIndex >= HitNoteLimiter) {
		HitNoteDatasIndex = 0;
	}
}

#ifndef __ANDROID__
inline bool KeyLongProcess(int KeyType, void(*func)()) {
	if (KeyBuf[KeyType] == 1) {
		func();
		return true;
	}
	return false;
}

inline bool KeyProcess(int KeyType, void(*func)()) {
	if (KeyBuf[KeyType] == 1 && KeyInt[KeyType] == 0) {
		KeyInt[KeyType] = 1;
		func();
		return true;
	}
	else if (KeyBuf[KeyType] == 0 && KeyInt[KeyType] == 1) { KeyInt[KeyType] = 0; }
	return false;
}
#else
inline void TouchProcess(void(*func)(), int ox = 0, int oy = 0, int w = 0, int h = 0, void(*func2)() = []{}){
	for (int i = 0; i < TouchNum; ++i){
		if ((ox - (w / 2)) < TouchList[i].PositionX && (oy - (h / 2)) < TouchList[i].PositionY && (ox + (w / 2)) > TouchList[i].PositionX && (oy + (h / 2)) > TouchList[i].PositionY) {
			func();
		}
		else if (w == 0 && h == 0) {
			func();
		}
		else {
			func2();
		}
		TouchProc = true;
	}
}

inline void OvalTouchProcess(void(*func)(int flag), int ox, int oy, int rx, int ry, void(*func2)(int flag) = [](int x){}){
	int LRflag = 0;
	for (int i = 0; i < TouchNum; ++i) {
		double _tx = TouchList[i].PositionX - ox;
		double _ty = TouchList[i].PositionY - oy;
		double theta = atan2(TouchList[i].PositionY - oy, TouchList[i].PositionX - ox);
		double M = sqrt(pow(_tx, 2) + pow(_ty, 2));
		double _m = sqrt(
				(pow(rx, 2) * pow(cos(atan(rx * tan(theta) / ry)), 2)) +
				(pow(ry, 2) * pow(sin(atan(rx * tan(theta) / ry)), 2)));

		if (theta < -(DX_PI / 2)) { LRflag = 1; }
		else { LRflag = 2; }

		if (_m > M) {
			func(LRflag);
		} else {
			func2(LRflag);
		}
		TouchProc = true;
	}
}

#endif

inline void TJANoteHit(char type) {
	bool DonOrKaFlag = type == '1';
	if (DonOrKaFlag) {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
		AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
	}
	if (!DonOrKaFlag) {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
		AnimationTime[(int)Animation::SCROLLFIELD_KA] = CD.NowTime;
	}

	for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
		if (ChartNoteHitProc(CD.TJANoteDatas[i], CD, DonOrKaFlag)) { break; }
	}
}

inline void TMGNoteHit(char type) {
	bool DonOrKaFlag = type == '1';
	if (DonOrKaFlag) {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
		AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
	}
	if (!DonOrKaFlag) {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
		AnimationTime[(int)Animation::SCROLLFIELD_KA] = CD.NowTime;
	}

	for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
		if (ChartNoteHitProc(CD.TMGNoteDatas[i], CD, DonOrKaFlag)) { break; }
	}
}

void ProcessThread() {

	Note_ox = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2) + 75;
	Note_oy = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y;

	FL.JudgeOk = FL.JudgeGood > FL.JudgeOk ? FL.JudgeGood : FL.JudgeOk;
	FL.JudgeBad = FL.JudgeOk > FL.JudgeBad ? FL.JudgeOk : FL.JudgeBad;

	SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
	SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
	SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);

	SubProcessTime.start();

	while (ProcessMessage() != -1) {
#ifndef __ANDROID__
		GetHitKeyStateAll(KeyBuf);

		KeyProcess(FL.QuickAccessKey[(int)QuickAccessID::AutoPlayFlag], [] {
			FL.AutoPlayFlag = !FL.AutoPlayFlag;
			});
		KeyProcess(FL.QuickAccessKey[(int)QuickAccessID::DebugScreen], [] {
			FL.DebugScreen = !FL.DebugScreen;
			});
		KeyProcess(FL.QuickAccessKey[(int)QuickAccessID::GridScreen], [] {
			FL.GridScreen = !FL.GridScreen;
			});
		KeyProcess(FL.QuickAccessKey[(int)QuickAccessID::ChartCreateMode], [] {
			FL.ChartCreateMode = !FL.ChartCreateMode;
			});
		KeyProcess(KEY_INPUT_F1, [] {
			NowLoadSongListDatas = true;
			WaitTimer(100);
			SongList = FL.LoadChartFiles("Songs");
			NowLoadSongListDatas = false;
			});
		KeyProcess(KEY_INPUT_F2, [] {
			ScreenFlipFlag = !ScreenFlipFlag;
			});

		if (GetDragFileNum() != 0) {
			char path[256];
			GetDragFilePath(path);
			DragFileInfoClear();
			DragFilePath = path;
			string ext = filesystem::path(DragFilePath).extension().string();
			if (ext == ".tja" || ext == ".tmg") {
				CourseSelector = 3;
				ProgressFlag = SecneType::ChartLoad;
			}
			else {
				DragFilePath = "";
			}
		}
#else
		TouchNum = GetTouchInputDownLog(TouchList, TOUCHINPUTPOINT_MAX);
#endif
		SecneType changeprogress = ProgressFlag;

		if (ProgressFlag == SecneType::Title) {
			TitleProcess();
		}
		else if (ProgressFlag == SecneType::SongSelect) {
			SongSelectProcess();
		}
		else if (ProgressFlag == SecneType::ChartLoad) {
			ChartLoadProcess();
		}
		else if (ProgressFlag == SecneType::Playing) {
			PlayingProcess();
		}
		else if (ProgressFlag == SecneType::Result) {
			ResultProcess();
		}
		else if (ProgressFlag == SecneType::Config) {
			ConfigProcess();
		}
		else if (ProgressFlag == SecneType::End) { break; }

		if (changeprogress != ProgressFlag) {
			if (ProgressFlag == SecneType::Playing) {
				if (!CD.ChartCreateMode) { thread([] { FeedInBack(false); }).detach(); }
			}
			else if (ProgressFlag == SecneType::ChartLoad) {
				// None
			}
			else {
				FeedInBack(false);
			}
		}

		if (CD.SoulProgress < 0) { CD.SoulProgress = 0; }
		if (CD.SoulProgress > CD.SoulMaxProgress) { CD.SoulProgress = CD.SoulMaxProgress; }

		if (SubProcessTime.GetNowTime() >= 1000) {
			SubProcessTime.start();
			SubProcessCount = 1;
		}
		else {
			++SubProcessCount;
		}
	}

	ProgressFlag = SecneType::End;
}

// processmain
inline void TitleProcess() {
#ifndef __ANDROID__
	if (KeyProcess(KEY_INPUT_ESCAPE, [] {
		FeedInBack(true);
		ProgressFlag = SecneType::End;
		})) {
		return;
	}
	for (int i = 0; i < 10; ++i) {
		if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			FeedInBack(true);
			ProgressFlag = SecneType::SongSelect;
			})) {
			break;
		}
		if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			FeedInBack(true);
			ProgressFlag = SecneType::SongSelect;
			})) {
			break;
		}
	}
#else
	TouchProcess([] {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
		ProgressFlag = SecneType::SongSelect;
		});
#endif
}

inline void SongSelectProcess() {
#ifndef __ANDROID__
	KeyLongProcess(KEY_INPUT_LSHIFT, [] {
		KeyProcess(KEY_INPUT_TAB, [] {
			ProgressFlag = SecneType::Config;
			});
		});
	if (!CourseSelect) {
		if (KeyProcess(KEY_INPUT_ESCAPE, [] {
			FeedInBack(true);
			ProgressFlag = SecneType::Title;
			})) {
			return;
		}
		for (int i = 0; i < 10; ++i) {
			if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
				CourseSelect = true;
				})) {
				break;
			}
			if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
				CourseSelect = true;
				})) {
				break;
			}
			if (KeyProcess(FL.KEY_INPUT_LK[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
				SongSelector = SongSelector - 1 <= -1 ? SongSelector = SongList.size() - 1 : SongSelector - 1;
				AnimationTime[(int)Animation::SongSelect_MotionTime] -= 100;
				while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
				})) {
				break;
			}
			if (KeyProcess(FL.KEY_INPUT_RK[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
				SongSelector = SongSelector + 1 >= SongList.size() ? SongSelector = 0 : SongSelector + 1;
				AnimationTime[(int)Animation::SongSelect_MotionTime] += 100;
				while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
				})) {
				break;
			}
		}
		if (KeyProcess(KEY_INPUT_UP, [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector - 1 <= -1 ? SongSelector = SongList.size() - 1 : SongSelector - 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] -= 100;
			while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
			})) {
			return;
		}
		if (KeyProcess(KEY_INPUT_LEFT, [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector - 1 <= -1 ? SongSelector = SongList.size() - 1 : SongSelector - 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] -= 100;
			while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
			})) {
			return;
		}
		if (KeyProcess(KEY_INPUT_DOWN, [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector + 1 >= SongList.size() ? SongSelector = 0 : SongSelector + 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] += 100;
			while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
			})) {
			return;
		}
		if (KeyProcess(KEY_INPUT_RIGHT, [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector + 1 >= SongList.size() ? SongSelector = 0 : SongSelector + 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] += 100;
			while (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) { if (ProcessMessage() == -1) { break; } }
			})) {
			return;
		}
	}
	else {
		if (KeyProcess(KEY_INPUT_ESCAPE, [] {
			CourseSelect = false;
			})) {
			return;
		}
		for (int i = 0; i < 10; ++i) {
			if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
				if (CourseSelector == -1) { CourseSelect = false; }
				else if (SongList[SongSelector].course[CourseSelector]) {
					ProgressFlag = SecneType::ChartLoad;
				}
				})) { break; }
			if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
				if (CourseSelector == -1) { CourseSelect = false; }
				else if (SongList[SongSelector].course[CourseSelector]) {
					ProgressFlag = SecneType::ChartLoad;
				}
				})) { break; }
			if (KeyProcess(FL.KEY_INPUT_LK[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
				CourseSelector = CourseSelector - 1 < -1 ? -1 : CourseSelector - 1;
				})) { break; }
			if (KeyProcess(FL.KEY_INPUT_RK[i], [] {
				PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
				CourseSelector = CourseSelector + 1 > 4 ? 4 : CourseSelector + 1;
				})) { break; }
		}
	}
#else
	if (!CourseSelect) {
		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector - 1 <= -1 ? SongSelector = SongList.size() - 1 : SongSelector - 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] -= 100;
			}, (FL.ScreenX / 10), FL.ScreenY / 2, (FL.ScreenX / 5), (FL.ScreenY));

		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			SongSelector = SongSelector + 1 >= SongList.size() ? SongSelector = 0 : SongSelector + 1;
			AnimationTime[(int)Animation::SongSelect_MotionTime] += 100;
			}, FL.ScreenX - (FL.ScreenX / 10), FL.ScreenY / 2, (FL.ScreenX / 5), (FL.ScreenY));

		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			CourseSelect = true;
			}, (FL.ScreenX / 2), (FL.ScreenY / 2), FL.ScreenX - (FL.ScreenX / 5 * 2), (FL.ScreenY));
	}
	else {
		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			CourseSelector = CourseSelector - 1 < -1 ? -1 : CourseSelector - 1;
			}, (FL.ScreenX / 10), FL.ScreenY / 2, (FL.ScreenX / 5), (FL.ScreenY));

		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
			CourseSelector = CourseSelector + 1 > 4 ? 4 : CourseSelector + 1;
			}, FL.ScreenX - (FL.ScreenX / 10), FL.ScreenY / 2, (FL.ScreenX / 5), (FL.ScreenY));

		TouchProcess([] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			if (CourseSelector == -1) { CourseSelect = false; }
			else if (SongList[SongSelector].course[CourseSelector]) {
				ProgressFlag = SecneType::ChartLoad;
			}
			}, (FL.ScreenX / 2), (FL.ScreenY / 2), FL.ScreenX - (FL.ScreenX / 5 * 2), (FL.ScreenY));
	}
#endif
}

inline void ChartLoadProcess() {
	WaitVSync(2);
	bool TrueLoadFlag = false;

	//Init
	CourseSelect = false;

	if (!CD.ChartCreateMode) {
		DeleteSoundMem(CD.Song.handle);
		CD = ChartData();
	}
	else {
		int _barindex = CD.BarlineIndex,
			_TitleStrLen = CD.TitleStrLen,
			_SubTitleStrLen = CD.SubTitleStrLen,
			_Songhandle = CD.Song.handle,
			_SongFrequency = CD.Song.frequency;
		string _Title = CD.Title;
		string _SubTitle = CD.SubTitle;
		CD = ChartData();
		CD.BarlineIndex = _barindex;
		CD.ChartCreateMode = true;
		CD.Title = _Title;
		CD.SubTitle = _SubTitle;
		CD.TitleStrLen = _TitleStrLen;
		CD.SubTitleStrLen = _SubTitleStrLen;
		CD.Song.handle = _Songhandle;
		CD.Song.frequency = _SongFrequency;
	}

	for (int i = 0; i < (int)Animation::A_Count; ++i) {
		AnimationTime[i] = DBL_MAX * -1;
	}

	for (int i = 0; i < HitNoteLimiter; ++i) {
		HitNoteDatas[i] = HitNoteData();
	}
	
	string FilePath =
#ifndef __ANDROID__
			DragFilePath != "" ? DragFilePath :
#endif
			SongList[SongSelector].path;
	bool TMGChartConvertFlag = false;
	bool IfTMGChartConvertFlag = false;

	// ChartLoad
#ifndef __ANDROID__
	if (filesystem::path(FilePath).extension() == ".tja") {
#else
	if (__ndk1::__fs::filesystem::path(FilePath).extension() == ".tja") {
#endif
		CD.TJAChartFlag = true;
		int barlinecount[2]{ 0,0 };
		int
				barlinenote = 0,
				branchcount = 0;
		double
				offset = 0,
				branchaddtime = 0;
		bool
				startflag = false,
				readbarflag = false,
				nowbarlineoffflag = false,
				nowbranchstartflag = false,
				branchstartfindflag = false;
		int NoteCount = 0;
		int RollStartIndex = 0;
		bool RollStartFlag = false, DelaySpawnFlag = false;
		
		TJANoteData _TJAMainData;
		TJANoteData& TJAMainData = _TJAMainData;
		TJANoteData _TJAMemData;
		TJANoteData& TJAMemData = _TJAMemData;

		vector<int> balloon;

		vector<string> list;
		SongData PlaySongData =
#ifndef __ANDROID__
				DragFilePath != "" ? FL.LoadTJAChartData(FilePath) :
#endif
				SongList[SongSelector];

		if (!LoadFileTEXT(list, FilePath)) {
#ifndef __ANDROID__
			MessageBox(NULL, TEXT((FilePath + "を\n読み込むことができませんでした").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
			ProgressFlag = SecneType::SongSelect;
			CD = ChartData();
			return;
		}
		bool ConvertFlag = false;
		for (int i = 0; i < list.size(); ++i) {
			if (list[i].find("#DUMMYSTART") == 0) { ConvertFlag = true; }
			if (list[i].find("#DUMMYEND") == 0) { ConvertFlag = true; }
			if (list[i].find("#NOTESPAWN") == 0) { ConvertFlag = true; }
			if (list[i].find("#JUDGEDELAY") == 0) { ConvertFlag = true; }
			if (list[i].find("#GRADATION") == 0) { ConvertFlag = true; }
			if (list[i].find("#ANGLE") == 0) { ConvertFlag = true; }
			if (list[i].find("#COLOR") == 0) { ConvertFlag = true; }
			if (list[i].find("#SIZE") == 0) { ConvertFlag = true; }
			if (list[i].find("#BARLINESIZE") == 0) { ConvertFlag = true; }
			if (list[i].find("#RESETCOMMAND") == 0) { ConvertFlag = true; }
			if (list[i].find("#HISPEED") == 0) { ConvertFlag = true; }
		}
		TMGChartConvertFlag = ConvertFlag;

		for (int i = 0; i < 5; ++i) {
			if (!(CourseSelector != i && PlaySongData.course[i])) { continue; }
			for (int j = PlaySongData.startindex[i]; j < list.size(); ++j) {
				if (list[j].find("#END") == 0) {
					list[j] = "";
					break;
				}
				list[j] = "";
			}
		}

#ifndef __ANDROID__
		if (DragFilePath != "") {
			DragFilePath = "";
			CourseSelector = 0;
		}
#endif

		int BPMDelayFlag = 0;
		int SongVol = 0;
		
		int errorindex = 0;
		TJAMainData = TJANoteData();
		CD.TJANoteDatas.push_back(TJANoteData());
		CD.BarlineDatas.push_back(BarlineData());
		try {
			if (TMGChartConvertFlag) { throw TJANoteData(); }
			for (int i = 0; i < list.size(); ++i) {
				++errorindex;
				// header
				if (list[i].find("TITLE:") == 0) {
					string title = strtrim(list[i].substr(6, list[i].size() - 6));
					CD.Title = title;
				}
				if (list[i].find("SUBTITLE:") == 0) {
					string subtitle = strtrim(list[i].substr(9, list[i].size() - 9));
					if (subtitle.find("--") == 0) {
						CD.SubTitleDispFlag = false;
					}
					else if (subtitle.find("++") == 0) {
						CD.SubTitleDispFlag = true;
					}
					subtitle = subtitle.find("--") == 0 || subtitle.find("++") == 0 ? subtitle.replace(0, 2, "") : subtitle;
					CD.SubTitle = subtitle;
				}
				if (list[i].find("BPM:") == 0) {
					TJAMainData.BPM = stod(strtrim(list[i].substr(4, list[i].size() - 4)));
				}
				if (list[i].find("WAVE:") == 0) {
					auto path = 
#ifndef __ANDROID__
					(filesystem::path(PlaySongData.path).parent_path().string() + "\\" +
#else
					(string)FL.ApplicationPath + "/" + (__fs::filesystem::path(PlaySongData.path).parent_path().string() + "\\" +
#endif
								 strtrim(list[i].substr(5, list[i].size() - 5)));
					if (!CD.ChartCreateMode) {
						CD.Song.handle = LoadSoundMem(path.c_str());
						if (CD.Song.handle != -1) {
							CD.Song.frequency = GetFrequencySoundMem(CD.Song.handle);
							int _frequency = CD.Song.frequency * FL.SongPlaySpeed;
							FL.SongPlaySpeed = (double)_frequency / CD.Song.frequency;
							SetFrequencySoundMem(_frequency, CD.Song.handle);
						}
					}
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);

					ChangeVolumeSoundMem(SongVol, CD.Song.handle);
				}
				if (list[i].find("OFFSET:") == 0) {
					offset = (stod(strtrim(list[i].substr(7, list[i].size() - 7))) * -1000);
				}
				if (list[i].find("BALLOON:") == 0) {
					auto a = split(strtrim(list[i].substr(8, list[i].size() - 8)), ',');
					for (int i = 0; i < a.size(); ++i) {
						balloon.push_back(stoi(strtrim(a[i])));
					}
				}
				if (FL.ChartHeaderSound) {
					if (list[i].find("SONGVOL:") == 0) {
						int pal = (255 * (stod(strtrim(list[i].substr(8, list[i].size() - 8))) / 100) * (FL.SongVolume / 100.));
						SongVol = pal;
						ChangeVolumeSoundMem(SongVol, CD.Song.handle);
					}
					if (list[i].find("SEVOL:") == 0) {
						int pal = (255 * (stod(strtrim(list[i].substr(6, list[i].size() - 6))) / 100) * (FL.SEVolume / 100.));
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);
					}
				}

				// Command
				if (list[i].find("#BMSCROLL") == 0) {
					CD.ScrollType = SCROLLType::BMScroll;
				}
				if (list[i].find("#HBSCROLL") == 0) {
					CD.ScrollType = SCROLLType::HBScroll;
				}
				if (list[i].find("#START") == 0) {
					CD.BlankTime = (offset < 240000 / TJAMainData.BPM) ? (240000 / TJAMainData.BPM) - offset : 0;
					CD.BlankTime += FL.SongBlankTime * 1000;
					TJAMainData.AbsTime += offset + CD.BlankTime + (FL.SongOffset * 1000);
					startflag = true;
					CD.TJANoteDatas[0].BPM = TJAMainData.BPM;
					CD.TJANoteDatas[0].RelaTime = TJAMainData.AbsTime;
				}
				if (list[i].find("#END") == 0) {
					startflag = false;
				}
				if (list[i].find("#SCROLL") == 0) {
					if (list[i].find("i") != string::npos) {
						int Uindex = list[i].rfind("+") == string::npos ? 0 : list[i].rfind("+");
						int Dindex = list[i].rfind("-") == string::npos ? 0 : list[i].rfind("-");

						bool Flag = Uindex > Dindex;


						if (list[i].rfind("+") != string::npos && Flag) {
							string real = strtrim(list[i].substr(7, list[i].rfind("+") - 7));
							string imag = strtrim(list[i].substr(list[i].rfind("+") + 1, list[i].rfind("i") - (list[i].rfind("+") + 1)));
							TJAMainData.Scroll = real == "" ? 0 : stod(getnumstring(real));
							TJAMainData.Scrolli = imag == "" ? -1 : stod(getnumstring(imag)) * -1;
						}
						if (list[i].rfind("-") != string::npos && !Flag) {
							string real = strtrim(list[i].substr(7, list[i].rfind("-") - 7));
							string imag = strtrim(list[i].substr(list[i].rfind("-") + 1, list[i].rfind("i") - (list[i].rfind("-") + 1)));
							TJAMainData.Scroll = real == "" ? 0 : stod(getnumstring(real));
							TJAMainData.Scrolli = imag == "" ? 1 : stod(getnumstring(imag));
						}
					}
					else {
						TJAMainData.Scroll = stod(getnumstring(strtrim(list[i].substr(7, list[i].size() - 7))));
						TJAMainData.Scrolli = 0;
					}
				}
				if (list[i].find("#BPMCHANGE") == 0) {
					//if (BPMDelayFlag < 2) { BPMDelayFlag = 2; }
					TJAMainData.BPM = stod(getnumstring(strtrim(list[i].substr(10, list[i].size() - 10))));
					TJAMainData.BpmchangeFlag = true;
				}
				if (list[i].find("#MEASURE") == 0) {
					auto a = split(strtrim(list[i].substr(8, list[i].size() - 8)), '/');
					TJAMainData.Beat = stod(getnumstring(a[0])) / stod(getnumstring(a[1]));
				}
				if (list[i].find("#DELAY") == 0) {
					TJAMainData.AbsTime += stod(getnumstring(strtrim(list[i].substr(6, list[i].size() - 6)))) * 1000;
					CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].DelayTime += stod(getnumstring(strtrim(list[i].substr(6, list[i].size() - 6)))) * 1000;
				}
				if (list[i].find("#SECTION") == 0) {
					TJAMainData.Section = true;
				}
				if (list[i].find("#LEVELHOLD") == 0) {
					TJAMainData.LevelHold = true;
				}
				if (list[i].find("#BRANCHSTART") == 0) {
					auto a = split(strtrim(list[i].substr(12, list[i].size() - 12)), ',');
					BranchData item;
					switch (a[0][0])
					{
						case 'p':
							item.Type = IfBranchType::Parfect;
							item.ExpertBranch = stod(a[1]);
							item.MasterBranch = stod(a[2]);
							break;
						case 'r':
							item.Type = IfBranchType::Roll;
							item.ExpertBranch = stod(a[1]);
							item.MasterBranch = stod(a[2]);
							break;
						case 's':
							item.Type = IfBranchType::Score;
							item.ExpertBranch = stod(a[1]);
							item.MasterBranch = stod(a[2]);
							break;
					}
					branchcount = 0;
					item.AbsTime = CD.BarlineDatas[CD.BarlineDatas.size() - 1].AbsTime - 150;
					item.StartMs = TJAMainData.AbsTime;
					item.Start = true;
					item.BranchFlag = true;
					CD.BranchDatas.push_back(item);
					nowbranchstartflag = true;
					branchstartfindflag = true;
					branchaddtime = 0;
				}
				if (nowbranchstartflag) {
					if (list[i].find("#N") == 0) {
						if (branchcount == 0) { TJAMemData = TJAMainData; }
						TJAMainData = TJAMemData;
						if (branchcount != 0) { CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TJAMainData.BranchStart = true; }
						TJAMainData.IsBranch = BranchType::Normal;
						TJAMainData.IsBranch = BranchType::Normal;
						branchaddtime = 0;
						++branchcount;
					}
					if (list[i].find("#E") == 0 && list[i].find("#END") != 0) {
						if (branchcount == 0) { TJAMemData = TJAMainData; }
						TJAMainData = TJAMemData;
						if (branchcount != 0) { CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TJAMainData.BranchStart = true; }
						TJAMainData.IsBranch = BranchType::Expert;
						TJAMainData.IsBranch = BranchType::Expert;
						branchaddtime = 0;
						++branchcount;
					}
					if (list[i].find("#M") == 0 && list[i].find("#MEASURE") != 0) {
						if (branchcount == 0) { TJAMemData = TJAMainData; }
						TJAMainData = TJAMemData;
						if (branchcount != 0) { CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TJAMainData.BranchStart = true; }
						TJAMainData.IsBranch = BranchType::Master;
						TJAMainData.IsBranch = BranchType::Master;
						branchaddtime = 0;
						++branchcount;
					}
					if (list[i].find("#BRANCHEND") == 0) {
						if (branchcount == 0) { TJAMainData.BranchStart = true; }
						TJAMainData.IsBranch = BranchType::Null;
						TJAMainData.IsBranch = BranchType::Null;
						nowbranchstartflag = false;
						branchaddtime = 0;
						branchcount = 0;
					}
				}
				if (list[i].find("#BARLINEOFF") == 0) {
					nowbarlineoffflag = true;
				}
				if (list[i].find("#BARLINEON") == 0) {
					nowbarlineoffflag = false;
				}
				if (list[i].find("#GOGOSTART") == 0) {
					TJAMainData.GOGOFlag = GOGOTime::GOGOStart;
				}
				if (list[i].find("#GOGOEND") == 0) {
					TJAMainData.GOGOFlag = GOGOTime::GOGOEnd;
				}
				if (list[i].find("#SUDDEN") == 0) {
					TJAMainData.SuddenFlag = 2;
					auto a = split(strtrim(list[i].substr(7, list[i].size() - 7)), ' ');

					TJAMainData.SuddenSpawnMs = stod(getnumstring(a[0])) * 1000;
					TJAMainData.SuddenMotionMs = stod(getnumstring(a[1])) * 1000;

					if (stod(a[0]) <= 0 || stod(a[1]) <= 0) {
						TJAMainData.SuddenFlag = 0;
						TJAMainData.SuddenSpawnMs = 0;
						TJAMainData.SuddenMotionMs = 0;
					}
				}
				if (list[i].find("#JPOSSCROLL") == 0) {
					TJAMainData.JPOSFlag = 2;
					auto a = split(strtrim(list[i].substr(11, list[i].size() - 11)), ' ');
					TJAMainData.JPOSMotionMs = stod(getnumstring(a[0])) * 1000;
					TJAMainData.JPOSMotionPx_x = stod(getnumstring(a[2])) == 1 ? stod(getnumstring(a[1])) : stod(getnumstring(a[1])) * -1;
				}
				if (list[i].find("#LYRIC") == 0) {
					TJAMainData.Lyric = strtrim(list[i].substr(6, list[i].size() - 6));
				}

				if (list[i].find("#") != string::npos) { continue; }

				if (startflag) {
					bool barendflag = false;
					if (!readbarflag) {
						readbarflag = true;
						for (int j = i; j < list.size(); ++j) {
							if (list[j].find("#BPMCHANGE") == 0) {
								BPMDelayFlag = 2;
							}
							if (list[j].find("#") != string::npos) { continue; }
							for (int l = 0; l < list[j].size(); ++l) {
								if (list[j][l] >= '0' && list[j][l] <= '9') {
									barlinenote++;
								}
								if (list[j][l] == ',') {
									barlinecount[0]++;
									barendflag = true;
									break;
								}
							}
							if (barendflag) { break; }
						}
					}
					for (int j = 0; j < list[i].size(); ++j) {
						bool ChartFlag = (list[i][j] >= '0' && list[i][j] <= '9');
						bool EndFlag = list[i][j] == ',';
						bool EmptyFlag = barlinenote == 0;
						if (ChartFlag || EndFlag || EmptyFlag) {

							if (EndFlag && !EmptyFlag) {
								readbarflag = false;
								barlinenote = 0;
								break;
							}

							if (barlinecount[0] != barlinecount[1] && BPMDelayFlag > 0) { --BPMDelayFlag; }

							double temptime = 0;
							double barlinenotetime = 0;
							temptime = 240000. / TJAMainData.BPM * TJAMainData.Beat;
							barlinenotetime = temptime / (barlinenote == 0 ? 1 : barlinenote);
							if (FL.JiroBPMDelayFlag && BPMDelayFlag >= 0) {
								temptime = (long)((240000. / TJAMainData.BPM) * TJAMainData.Beat);
								barlinenotetime = temptime / (barlinenote == 0 ? 1 : barlinenote);
							}

							if (!EmptyFlag) {
								TJAMainData.RelaTime = barlinenotetime;
							}
							else {
								TJAMainData.RelaTime = temptime;
							}

							if (barlinecount[0] != barlinecount[1]) {
								barlinecount[1] = barlinecount[0];
								BarlineData bitem;
								bitem.AbsTime = TJAMainData.AbsTime;
								CD.BarlineDatas.push_back(bitem);
								TJAMainData.Barline = !nowbarlineoffflag;
							}

							TJAMainData.Type = list[i][j];

							bool removeflag =
								!TJAMainData.Barline &&
								!TJAMainData.BpmchangeFlag &&
								TJAMainData.GOGOFlag == GOGOTime::Null &&
								TJAMainData.Type == '0' &&
								TJAMainData.JPOSFlag == 0 &&
								TJAMainData.Lyric == "" &&
								TJAMainData.DelayTime == 0 &&
								!TJAMainData.BranchStart &&
								!TJAMainData.Section &&
								!TJAMainData.LevelHold;

							if (!removeflag) {
								CD.TJANoteDatas.push_back(TJAMainData);
							}
							else {
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].RelaTime += TJAMainData.RelaTime;
							}

							if (!EmptyFlag) {
								TJAMainData.AbsTime += barlinenotetime;
							}
							else {
								TJAMainData.AbsTime += temptime;
							}

							if (nowbranchstartflag) {
								branchaddtime += TJAMainData.RelaTime * (TJAMainData.BPM > 0 ? 1 : -1);
							}

							TJAMainData.Barline = false;

							TJAMainData.GOGOFlag = GOGOTime::Null;

							TJAMainData.BpmchangeFlag = false;

							TJAMainData.BranchStart = false;
							TJAMainData.Section = false;
							TJAMainData.LevelHold = false;

							TJAMainData.JPOSFlag = 0;
							TJAMainData.JPOSMotionMs = 0;
							TJAMainData.JPOSMotionPx_x = 0;

							if (!removeflag && !CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].DummyFlag && (CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].IsBranch == BranchType::Null || CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].IsBranch == BranchType::Normal) && (CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Type >= '1' && CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Type <= '4')) {
								++NoteCount;
							}
							if (!removeflag && FL.RandomRate > GetRand(100)) {
								TJANoteData _temp = CD.TJANoteDatas[CD.TJANoteDatas.size() - 1];
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Type = _temp.Type == '1' ? '2' : _temp.Type == '2' ? '1' : _temp.Type == '3' ? '4' : _temp.Type == '4' ? '3' : _temp.Type;
							}
							if (!removeflag) {
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Scroll *= FL.AddSCROLL;
							}
							if (FL.ConstantBPM != 0) {
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].BPM = FL.ConstantBPM;
							}
							if (FL.ConstantSCROLL != 0) {
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Scroll = FL.ConstantSCROLL;
								CD.TJANoteDatas[CD.TJANoteDatas.size() - 1].Scrolli = 0;
							}

							if (!RollStartFlag && (TJAMainData.Type >= '5' && TJAMainData.Type <= '7') || TJAMainData.Type == '9') {
								RollStartIndex = CD.TJANoteDatas.size() - 1;
								RollStartFlag = !RollStartFlag;
							}
							else if (RollStartFlag && TJAMainData.Type > '0' && TJAMainData.Type <= '9') {
								CD.TJANoteDatas[RollStartIndex].RollEndIndex = CD.TJANoteDatas.size() - 1;
								RollStartFlag = !RollStartFlag;
							}

							if (EndFlag) {
								readbarflag = false;
								barlinenote = 0;
								break;
							}
						}
					}
				}
			}
			for (int i = 0, b = 0; i < CD.TJANoteDatas.size() && b < balloon.size(); ++i) {
				if (CD.TJANoteDatas[i].Type == '7' || CD.TJANoteDatas[i].Type == '9') {
					CD.TJANoteDatas[i].BalloonCount = balloon[b];
					b++;
				}
			}

			CD.AddScore = (int)round(100000. / NoteCount);
			CD.SoulMaxProgress = NoteCount * 100;
			CD.TJANoteDatas[0].BpmchangeFlag = true;

			TrueLoadFlag = true;
			CD.NowBranchFlag = branchstartfindflag ? BranchType::Normal : BranchType::Null;
		}
		catch (...) {
			if (!TMGChartConvertFlag) {
#ifndef __ANDROID__
				MessageBox(NULL, TEXT((FilePath + "の\n" + to_string(errorindex) + "行目でよろしくない書き方をしています").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
				CD.TJANoteDatas.clear();
				ProgressFlag = SecneType::SongSelect;
				return;
			}
			else {
#ifndef __ANDROID__
				int flag = MessageBox(NULL, TEXT((FilePath +
					"に\n" +
					".tmg専用のver0.6α以前の命令文が検出されました\n" +
					".tmg用の譜面に変換しますか？\n\n" +
					"※注意※\n\n" +
					"完璧な変換を保証しません\n" +
					"譜面ファイルは.tmgとして上書きされます\n" +
					".tmgから.tjaに戻すことはできません\n").c_str()),
					TEXT("Convert？"), MB_ICONQUESTION | MB_OKCANCEL);
#else
				int flag = 1;
				// TODO: Androidの画面に出す処理を書く
#endif
				CD.TJANoteDatas.clear();
				if (flag == 1) {
					IfTMGChartConvertFlag = true;
				}
			}
		}
	}
	if (IfTMGChartConvertFlag) {
		CD.NowChartConvert = true;
		vector<string> list;
		bool binaryflag = false;
		if (!LoadFileTEXT(list, FilePath)) {
#ifndef __ANDROID__
			MessageBox(NULL, TEXT((FilePath + "を\n読み込むことができませんでした").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
			ProgressFlag = SecneType::SongSelect;
			CD = ChartData();
			return;
		}

		int errorindex = 0;
		int AddIndex = 0;
		try {
			for (int i = 0; i < list.size(); ++i, errorindex = i) {
				if (list[i].find("#BMSCROLL") == 0) {
					list[i] = "#BMSCROLL()";
				}
				if (list[i].find("#HBSCROLL") == 0) {
					list[i] = "#HBSCROLL()";
				}
				if (list[i].find("#START") == 0) {
					list[i] = "#START()";
				}
				if (list[i].find("#END") == 0) {
					list[i] = "#END()";
				}
				if (list[i].find("#SCROLL") == 0) {
					string data = strtrim(list[i].substr(7, list[i].size() - 7));
					if (data.find("i") != string::npos) {
						int Uindex = list[i].rfind("+") == string::npos ? 0 : list[i].rfind("+");
						int Dindex = list[i].rfind("-") == string::npos ? 0 : list[i].rfind("-");

						bool Flag = Uindex > Dindex;

						if (list[i].rfind("+") != string::npos && Flag) {
							string real = strtrim(list[i].substr(7, list[i].rfind("+") - 7));
							string imag = strtrim(list[i].substr(list[i].rfind("+") + 1, list[i].rfind("i") - (list[i].rfind("+") + 1)));
							list[i] = "#SCROLL(" + real + ", " + imag + ")";
						}
						if (list[i].rfind("-") != string::npos && !Flag) {
							string real = strtrim(list[i].substr(7, list[i].rfind("-") - 7));
							string imag = strtrim(list[i].substr(list[i].rfind("-") + 1, list[i].rfind("i") - (list[i].rfind("-") + 1)));
							list[i] = "#SCROLL(" + real + ", -" + imag + ")";
						}
					}
					else {
						list[i] = "#SCROLL(" + data + ")";
					}
				}
				if (list[i].find("#BPMCHANGE") == 0) {
					string data = strtrim(list[i].substr(10, list[i].size() - 10));
					list[i] = "#BPMCHANGE(" + data + ")";
				}
				if (list[i].find("#MEASURE") == 0) {
					string data = strtrim(list[i].substr(8, list[i].size() - 8));
					list[i] = "#MEASURE(" + data + ")";
				}
				if (list[i].find("#DELAY") == 0) {
					string data = strtrim(list[i].substr(6, list[i].size() - 6));
					list[i] = "#DELAY(" + data + ")";
				}
				if (list[i].find("#SECTION") == 0) {
					list[i] = "#SECTION()";
				}
				if (list[i].find("#LEVELHOLD") == 0) {
					list[i] = "#LEVELHOLD()";
				}
				if (list[i].find("#BRANCHSTART") == 0) {
					string data = strtrim(list[i].substr(12, list[i].size() - 12));
					list[i] = "#BRANCHSTART(" + data + ")";
				}
				if (list[i].find("#N") == 0 && list[i].find("#NOTESPAWN") != 0) {
					list[i] = "#N()";
				}
				if (list[i].find("#E") == 0 && list[i].find("#END") != 0) {
					list[i] = "#E()";
				}
				if (list[i].find("#M") == 0 && list[i].find("#MEASURE") != 0) {
					list[i] = "#M()";
				}
				if (list[i].find("#BRANCHEND") == 0) {
					list[i] = "#BRANCHEND()";
				}
				if (list[i].find("#BARLINEOFF") == 0) {
					list[i] = "#BARLINEOFF()";
				}
				if (list[i].find("#BARLINEON") == 0) {
					list[i] = "#BARLINEON()";
				}
				if (list[i].find("#GOGOSTART") == 0) {
					list[i] = "#GOGOSTART()";
				}
				if (list[i].find("#GOGOEND") == 0) {
					list[i] = "#GOGOEND()";
				}
				if (list[i].find("#DUMMYSTART") == 0) {
					list[i] = "#DUMMYSTART()";
				}
				if (list[i].find("#DUMMYEND") == 0) {
					list[i] = "#DUMMYEND()";
				}
				if (list[i].find("#SUDDEN") == 0) {
					auto data = split(strtrim(list[i].substr(7, list[i].size() - 7)), ' ');
					list[i] = "#SUDDEN(" + data[0] + ", " + data[1] + ")";
					if (stod(data[0]) <= 0 && stod(data[1]) <= 0) {
						list[i] = "#SUDDEN(0)";
					}
				}
				if (list[i].find("#JPOSSCROLL") == 0) {
					auto data = split(strtrim(list[i].substr(11, list[i].size() - 11)), ' ');
					double _ms = stod(getnumstring(data[0]));
					if ("default" == data[1]) {
						list[i] = "#JPOSSCROLL(" + double_to_string(_ms) + ", Default)";
					}
					else if (data[1].find("/") != string::npos) {
						double _motion = stoi(getnumstring(data[2])) == 1 ? StringFormula()(data[1]) : StringFormula()(data[1]) * -1;
						list[i] = "#JPOSSCROLL(" + double_to_string(_ms) + ", SCROLLFIELD.width * " + double_to_string(_motion) + ")";
					}
					else {
						double _motion = stoi(getnumstring(data[2])) == 1 ? stod(getnumstring(data[1])) : stod(getnumstring(data[1])) * -1;
						list[i] = "#JPOSSCROLL(" + double_to_string(_ms) + ", " + double_to_string(_motion) + ")";
					}
				}
				if (list[i].find("#LYRIC") == 0) {
					string data = strtrim(list[i].substr(6, list[i].size() - 6));
					list[i] = "#LYRIC(" + data + ")";
				}
				if (list[i].find("#NOTESPAWN") == 0) {
					auto data = split(strtrim(list[i].substr(10, list[i].size() - 10)), ' ');
					list[i] = (string)"#NOTESPAWN(" + (data[0] == "0" ? "Init" : data[0] == "1" ? "Spawn, " + data[1] : data[0] == "2" ? "Vanish, " + data[1] : "") + ")";
				}
				if (list[i].find("#JUDGEDELAY") == 0) {
					auto data = split(strtrim(list[i].substr(11, list[i].size() - 11)), ' ');
					string val = "";
					for (int j = 1; j < data.size(); ++j) {
						val += ", " + data[j];
					}
					list[i] = (string)"#JUDGEDELAY(" + (data[0] == "0" ? "Init" : data[0] == "1" ? "Sec" + val : data[0] == "2" ? "Pos" + val : data[0] == "3" ? "Sec_Pos" + val : "") + ")";
				}
				if (list[i].find("#ANGLE") == 0) {
					string data = strtrim(list[i].substr(6, list[i].size() - 6));
					list[i] = "#ANGLE(" + data + ")";
				}
				if (list[i].find("#SIZE") == 0) {
					string data = strtrim(list[i].substr(5, list[i].size() - 5));
					list[i] = "#SIZE(" + data + ")";
				}
				if (list[i].find("#COLOR") == 0) {
					auto data = split(strtrim(list[i].substr(6, list[i].size() - 6)), ' ');
					list[i] = "#COLOR(" + data[0] + ", " + data[1] + ", " + data[2] + ")";
					++i;
					list.insert(list.begin() + i, "#ALPHA(" + data[3] + ")");
					++AddIndex;
				}
				if (list[i].find("#BARLINESIZE") == 0) {
					auto data = split(strtrim(list[i].substr(12, list[i].size() - 12)), ' ');
					list[i] = "#BARLINESIZE(" + data[0] + ", " + data[1] + ")";
				}
				if (list[i].find("#GRADATION") == 0) {
					auto data = split(strtrim(list[i].substr(10, list[i].size() - 10)), ' ');
					bool gradationendflag = false;
					bool ProcEnd = false;
					if ("start" == data[0]) {
						auto GetStrEasingType1 = [](string _data) {
							string _type = "";
							if ((int)EasingType1::In == stoi(_data)) { _type = "In"; }
							if ((int)EasingType1::Out == stoi(_data)) { _type = "Out"; }
							if ((int)EasingType1::InOut == stoi(_data)) { _type = "InOut"; }
							if ((int)EasingType1::OutIn == stoi(_data)) { _type = "OutIn"; }
							return _type;
						};
						auto GetStrEasingType2 = [](string _data) {
							string _type = "";
							if ((int)EasingType2::Linear == stoi(_data)) { _type = "Linear"; }
							if ((int)EasingType2::Sine == stoi(_data)) { _type = "Sine"; }
							if ((int)EasingType2::Quad == stoi(_data)) { _type = "Quad"; }
							if ((int)EasingType2::Cubic == stoi(_data)) { _type = "Cubic"; }
							if ((int)EasingType2::Quart == stoi(_data)) { _type = "Quart"; }
							if ((int)EasingType2::Quint == stoi(_data)) { _type = "Quint"; }
							if ((int)EasingType2::Expo == stoi(_data)) { _type = "Expo"; }
							if ((int)EasingType2::Circ == stoi(_data)) { _type = "Circ"; }
							if ((int)EasingType2::Back == stoi(_data)) { _type = "Back"; }
							if ((int)EasingType2::Elastic == stoi(_data)) { _type = "Elastic"; }
							if ((int)EasingType2::Bounce == stoi(_data)) { _type = "Bounce"; }
							return _type;
						};
						list[i] = (string)"#GRADATION(" + ("start" == data[0] ? "Set" : "") + ", " + GetStrEasingType1(data[2]) + ", " + GetStrEasingType2(data[3]) + ", " + data[1] + ", " + data[1] + ")";
						++i;
						
						bool _srcflag[(int)GradationParamType::Count]{};
						bool _delimflag[(int)GradationParamType::Count]{};
						double _src[(int)GradationParamType::Count]{};
						double _delim[(int)GradationParamType::Count]{};
						for (int j = 0; j < (int)GradationParamType::Count; ++j) {
							_srcflag[j] = 0;
							_delimflag[j] = 0;
							_src[j] = 0;
							_delim[j] = 0;
						}
						for (; i < list.size();) {
							if (list[i].find("#SCROLL") == 0) {
								if (list[i].find("i") != string::npos) {
									int Uindex = list[i].rfind("+") == string::npos ? 0 : list[i].rfind("+");
									int Dindex = list[i].rfind("-") == string::npos ? 0 : list[i].rfind("-");

									bool Flag = Uindex > Dindex;

									if (list[i].rfind("+") != string::npos && Flag) {
										string real = strtrim(list[i].substr(7, list[i].rfind("+") - 7));
										string imag = strtrim(list[i].substr(list[i].rfind("+") + 1, list[i].rfind("i") - (list[i].rfind("+") + 1)));
										_src[(int)GradationParamType::Scroll] = real == "" ? 0 : stod(getnumstring(real));
										_src[(int)GradationParamType::Scrolli] = imag == "" ? -1 : stod(getnumstring(imag));
									}
									if (list[i].rfind("-") != string::npos && !Flag) {
										string real = strtrim(list[i].substr(7, list[i].rfind("-") - 7));
										string imag = strtrim(list[i].substr(list[i].rfind("-") + 1, list[i].rfind("i") - (list[i].rfind("-") + 1)));
										_src[(int)GradationParamType::Scroll] = real == "" ? 0 : stod(getnumstring(real));
										_src[(int)GradationParamType::Scrolli] = imag == "" ? 1 : stod(getnumstring(imag)) * -1;
									}
									_srcflag[(int)GradationParamType::Scroll] = true;
									_srcflag[(int)GradationParamType::Scrolli] = true;
								}
								else if (list[i].find(",") != string::npos) {
									auto scdata = split(strtrim(list[i].substr(7, list[i].size() - 7)), ',');
									complex<double> _complex = PolarPosToComplexPos(stod(scdata[0]), (DX_TWO_PI / stod(scdata[1]) * stod(scdata[2])));
									_src[(int)GradationParamType::Scroll] = _complex.real();
									_src[(int)GradationParamType::Scrolli] = _complex.imag();

									_srcflag[(int)GradationParamType::Scroll] = true;
									_srcflag[(int)GradationParamType::Scrolli] = true;
								}
								else {
									_src[(int)GradationParamType::Scroll] = stod(strtrim(list[i].substr(7, list[i].size() - 7)));
									_src[(int)GradationParamType::Scrolli] = 0;
								
									_srcflag[(int)GradationParamType::Scroll] = true;
									_srcflag[(int)GradationParamType::Scrolli] = true;
								}
							}
							if (list[i].find("#JUDGEDELAY") == 0) {
								auto jddata = split(strtrim(list[i].substr(11, list[i].size() - 11)), ' ');
								if (stoi(jddata[0]) == 0) {
									_src[(int)GradationParamType::JudgeDelayMs] = 0;
									_src[(int)GradationParamType::JudgeDelayX] = 0;
									_src[(int)GradationParamType::JudgeDelayY] = 0;
								}
								if (stoi(jddata[0]) == 1) {
									_src[(int)GradationParamType::JudgeDelayMs] = stod(jddata[1]);
									_src[(int)GradationParamType::JudgeDelayX] = 0;
									_src[(int)GradationParamType::JudgeDelayY] = 0;
								}
								if (stoi(jddata[0]) == 2) {
									_src[(int)GradationParamType::JudgeDelayMs] = 0;
									_src[(int)GradationParamType::JudgeDelayX] = stod(jddata[1]);
									_src[(int)GradationParamType::JudgeDelayY] = stod(jddata[2]);
								}
								if (stoi(jddata[0]) == 3) {
									_src[(int)GradationParamType::JudgeDelayMs] = stod(jddata[1]);
									_src[(int)GradationParamType::JudgeDelayX] = stod(jddata[2]);
									_src[(int)GradationParamType::JudgeDelayY] = stod(jddata[3]);
								}
								_srcflag[(int)GradationParamType::JudgeDelayMs] = true;
								_srcflag[(int)GradationParamType::JudgeDelayX] = true;
								_srcflag[(int)GradationParamType::JudgeDelayY] = true;
							}
							if (list[i].find("#COLOR") == 0) {
								auto crdata = split(strtrim(list[i].substr(6, list[i].size() - 6)), ' ');
								_src[(int)GradationParamType::ColorR] = stod(crdata[0]);
								_src[(int)GradationParamType::ColorG] = stod(crdata[1]);
								_src[(int)GradationParamType::ColorB] = stod(crdata[2]);
								_src[(int)GradationParamType::Alpha] = stod(crdata[3]);

								_srcflag[(int)GradationParamType::ColorR] = true;
								_srcflag[(int)GradationParamType::ColorG] = true;
								_srcflag[(int)GradationParamType::ColorB] = true;
								_srcflag[(int)GradationParamType::Alpha] = true;
							}
							if (list[i].find("#SIZE") == 0) {
								_src[(int)GradationParamType::Size] = stod(getnumstring(strtrim(list[i].substr(5, list[i].size() - 5))));

								_srcflag[(int)GradationParamType::Size] = true;
							}
							if (list[i].find("#ANGLE") == 0) {
								_src[(int)GradationParamType::Angle] = stod(getnumstring(strtrim(list[i].substr(6, list[i].size() - 6))));

								_srcflag[(int)GradationParamType::Angle] = true;
							}
							if (list[i].find("#GRADATION") == 0) {
								auto graddata = split(strtrim(list[i].substr(10, list[i].size() - 10)), ' ');
								if ("end" == graddata[0]) {
									for (; i < list.size();) {
										bool ReedFlag = true;
										if (list[i].find("#SCROLL") == 0) {
											if (list[i].find("i") != string::npos) {
												int Uindex = list[i].rfind("+") == string::npos ? 0 : list[i].rfind("+");
												int Dindex = list[i].rfind("-") == string::npos ? 0 : list[i].rfind("-");

												bool Flag = Uindex > Dindex;

												if (list[i].rfind("+") != string::npos && Flag) {
													string real = strtrim(list[i].substr(7, list[i].rfind("+") - 7));
													string imag = strtrim(list[i].substr(list[i].rfind("+") + 1, list[i].rfind("i") - (list[i].rfind("+") + 1)));
													_delim[(int)GradationParamType::Scroll] = real == "" ? 0 : stod(getnumstring(real));
													_delim[(int)GradationParamType::Scrolli] = imag == "" ? -1 : stod(getnumstring(imag));
												}
												if (list[i].rfind("-") != string::npos && !Flag) {
													string real = strtrim(list[i].substr(7, list[i].rfind("-") - 7));
													string imag = strtrim(list[i].substr(list[i].rfind("-") + 1, list[i].rfind("i") - (list[i].rfind("-") + 1)));
													_delim[(int)GradationParamType::Scroll] = real == "" ? 0 : stod(getnumstring(real));
													_delim[(int)GradationParamType::Scrolli] = imag == "" ? 1 : stod(getnumstring(imag)) * -1;
												}
												_delimflag[(int)GradationParamType::Scroll] = true;
												_delimflag[(int)GradationParamType::Scrolli] = true;
											}
											else if (list[i].find(",") != string::npos) {
												auto scdata = split(strtrim(list[i].substr(7, list[i].size() - 7)), ',');
												complex<double> _complex = PolarPosToComplexPos(stod(scdata[0]), (DX_TWO_PI / stod(scdata[1]) * stod(scdata[2])));
												_delim[(int)GradationParamType::Scroll] = _complex.real();
												_delim[(int)GradationParamType::Scrolli] = _complex.imag();

												_delimflag[(int)GradationParamType::Scroll] = true;
												_delimflag[(int)GradationParamType::Scrolli] = true;
											}
											else {
												_delim[(int)GradationParamType::Scroll] = stod(getnumstring(strtrim(list[i].substr(7, list[i].size() - 7))));
												_delim[(int)GradationParamType::Scrolli] = 0;

												_delimflag[(int)GradationParamType::Scroll] = true;
												_delimflag[(int)GradationParamType::Scrolli] = true;
											}
										}
										if (list[i].find("#JUDGEDELAY") == 0) {
											auto jddata = split(strtrim(list[i].substr(11, list[i].size() - 11)), ' ');
											if (stoi(jddata[0]) == 0) {
												_delim[(int)GradationParamType::JudgeDelayMs] = 0;
												_delim[(int)GradationParamType::JudgeDelayX] = 0;
												_delim[(int)GradationParamType::JudgeDelayY] = 0;
											}
											if (stoi(jddata[0]) == 1) {
												_delim[(int)GradationParamType::JudgeDelayMs] = stod(jddata[1]);
												_delim[(int)GradationParamType::JudgeDelayX] = 0;
												_delim[(int)GradationParamType::JudgeDelayY] = 0;
											}
											if (stoi(jddata[0]) == 2) {
												_delim[(int)GradationParamType::JudgeDelayMs] = 0;
												_delim[(int)GradationParamType::JudgeDelayX] = stod(jddata[1]);
												_delim[(int)GradationParamType::JudgeDelayY] = stod(jddata[2]);
											}
											if (stoi(jddata[0]) == 3) {
												_delim[(int)GradationParamType::JudgeDelayMs] = stod(jddata[1]);
												_delim[(int)GradationParamType::JudgeDelayX] = stod(jddata[2]);
												_delim[(int)GradationParamType::JudgeDelayY] = stod(jddata[3]);
											}

											_delimflag[(int)GradationParamType::JudgeDelayMs] = true;
											_delimflag[(int)GradationParamType::JudgeDelayX] = true;
											_delimflag[(int)GradationParamType::JudgeDelayY] = true;
										}
										if (list[i].find("#COLOR") == 0) {
											auto crdata = split(strtrim(list[i].substr(6, list[i].size() - 6)), ' ');
											_delim[(int)GradationParamType::ColorR] = stod(crdata[0]);
											_delim[(int)GradationParamType::ColorG] = stod(crdata[1]);
											_delim[(int)GradationParamType::ColorB] = stod(crdata[2]);
											_delim[(int)GradationParamType::Alpha] = stod(crdata[3]);

											_delimflag[(int)GradationParamType::ColorR] = true;
											_delimflag[(int)GradationParamType::ColorG] = true;
											_delimflag[(int)GradationParamType::ColorB] = true;
											_delimflag[(int)GradationParamType::Alpha] = true;
										}
										if (list[i].find("#SIZE") == 0) {
											_delim[(int)GradationParamType::Size] = stod(getnumstring(strtrim(list[i].substr(5, list[i].size() - 5))));

											_delimflag[(int)GradationParamType::Size] = true;
										}
										if (list[i].find("#ANGLE") == 0) {
											_delim[(int)GradationParamType::Angle] = stod(getnumstring(strtrim(list[i].substr(6, list[i].size() - 6))));

											_delimflag[(int)GradationParamType::Angle] = true;
										}

										for (int j = 0; j < (int)GradationParamType::Count; ++j) {
											if (_srcflag[j] != _delimflag[j]) { ReedFlag = false; break; }
										}
										++AddIndex;
										list.erase(list.begin() + i);
										if (ReedFlag) { ProcEnd = true; break; }
									}
								}
							}
							if (ProcEnd) { break; }
							++AddIndex;
							list.erase(list.begin() + i);
						}
						string WriteData = "";
						if (_srcflag[(int)GradationParamType::Scroll] && _srcflag[(int)GradationParamType::Scrolli] &&
							_delimflag[(int)GradationParamType::Scroll] && _delimflag[(int)GradationParamType::Scrolli]) {
							WriteData = "#SCROLL(" + 
								double_to_string(_src[(int)GradationParamType::Scroll]) + "~" + double_to_string(_delim[(int)GradationParamType::Scroll]) + ", " +
								double_to_string(_src[(int)GradationParamType::Scrolli]) + "~" + double_to_string(_delim[(int)GradationParamType::Scrolli]) + ")";
							list.insert(list.begin() + i, WriteData);
							++i;
						}
						if (_srcflag[(int)GradationParamType::JudgeDelayMs] && _srcflag[(int)GradationParamType::JudgeDelayX] && _srcflag[(int)GradationParamType::JudgeDelayY] &&
							_delimflag[(int)GradationParamType::JudgeDelayMs] && _delimflag[(int)GradationParamType::JudgeDelayX] && _delimflag[(int)GradationParamType::JudgeDelayY]) {
							WriteData = "#JUDGEDELAY(Sec_Pos, " + 
								double_to_string(_src[(int)GradationParamType::JudgeDelayMs]) + "~" + double_to_string(_delim[(int)GradationParamType::JudgeDelayMs]) + ", " + 
								double_to_string(_src[(int)GradationParamType::JudgeDelayX]) + "~" + double_to_string(_delim[(int)GradationParamType::JudgeDelayX]) + ", " + 
								double_to_string(_src[(int)GradationParamType::JudgeDelayY]) + "~" + double_to_string(_delim[(int)GradationParamType::JudgeDelayY]) + ")";
							list.insert(list.begin() + i, WriteData);
							--AddIndex;
							++i;
						}
						if (_srcflag[(int)GradationParamType::ColorR] && _srcflag[(int)GradationParamType::ColorG] && _srcflag[(int)GradationParamType::ColorB] &&
							_delimflag[(int)GradationParamType::ColorR] && _delimflag[(int)GradationParamType::ColorG] && _delimflag[(int)GradationParamType::ColorB]) {
							WriteData = "#COLOR(" +
								double_to_string(_src[(int)GradationParamType::ColorR]) + "~" + double_to_string(_delim[(int)GradationParamType::ColorR]) + ", " +
								double_to_string(_src[(int)GradationParamType::ColorG]) + "~" + double_to_string(_delim[(int)GradationParamType::ColorG]) + ", " +
								double_to_string(_src[(int)GradationParamType::ColorB]) + "~" + double_to_string(_delim[(int)GradationParamType::ColorB]) + ")";
							list.insert(list.begin() + i, WriteData);
							--AddIndex;
							++i;
						}
						if (_srcflag[(int)GradationParamType::Alpha] &&
							_delimflag[(int)GradationParamType::Alpha]) {
							WriteData = "#ALPHA(" + 
								double_to_string(_src[(int)GradationParamType::Alpha]) + "~" + double_to_string(_delim[(int)GradationParamType::Alpha]) + ")";
							list.insert(list.begin() + i, WriteData);
							--AddIndex;
							++i;
						}
						if (_srcflag[(int)GradationParamType::Size] &&
							_delimflag[(int)GradationParamType::Size]) {
							WriteData = "#SIZE(" + 
								double_to_string(_src[(int)GradationParamType::Size]) + "~" + double_to_string(_delim[(int)GradationParamType::Size]) + ")";
							list.insert(list.begin() + i, WriteData);
							--AddIndex;
							++i;
						}
						if (_srcflag[(int)GradationParamType::Angle] && 
							_delimflag[(int)GradationParamType::Angle]) {
							WriteData = "#ANGLE(" + 
								double_to_string(_src[(int)GradationParamType::Angle]) + "~" + double_to_string(_delim[(int)GradationParamType::Angle]) + ")";
							list.insert(list.begin() + i, WriteData);
							--AddIndex;
							++i;
						}
						WriteData = (string)"#GRADATION(" + "Start" + ")";
						--AddIndex;
						list.insert(list.begin() + i, WriteData);
					}
					else {
						list[i] = (string)"#GRADATION(" + ("end" == data[0] ? "Start" : "init" == data[0] ? "End" : "") + ")";
					}
				}
				if (list[i].find("#RESETCOMMAND") == 0) {
					list[i] = "#RESETCOMMAND()";
				}
				if (list[i].find("#HISPEED") == 0) {
					string data = strtrim(list[i].substr(8, list[i].size() - 8));
					list[i] = "#HISPEED(" + data + ")";
				}
			}
		}
		catch (...) {
#ifndef __ANDROID__
			MessageBox(NULL, TEXT((FilePath + "の\n" + to_string(errorindex + AddIndex) + "行目でよろしくない書き方をしていたため、変換作業を中止します\nファイルの中身が変わっていることはないのでご安心を").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
			ProgressFlag = SecneType::SongSelect;
			CD = ChartData();
			return;
		}

		if (binaryflag && !WriteBinaryFileTEXT(list, FilePath)) {}
		else if (!WriteFileTEXT(list, FilePath)) {
#ifndef __ANDROID__
			MessageBox(NULL, TEXT((FilePath + "を変換後の内容にできませんでした\nファイルの中身が変わっていることはないのでご安心を").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
			ProgressFlag = SecneType::SongSelect;
			CD = ChartData();
			return;
		}

#ifndef __ANDROID__
		FilePath = filesystem::path(FilePath).replace_extension(".tmg").string();
		filesystem::rename(SongList[SongSelector].path, FilePath);
#else
		FilePath = __fs::filesystem::path(FilePath).replace_extension(".tmg").string();
		__fs::filesystem::rename(SongList[SongSelector].path, FilePath);
#endif
		SongList[SongSelector] = FL.LoadTMGChartData(FilePath);
		CD = ChartData();
	}
#ifndef __ANDROID__
	if (filesystem::path(FilePath).extension() == ".tmg") {
#else
	if (__ndk1::__fs::filesystem::path(FilePath).extension() == ".tmg") {
#endif
		
		CD.NowChartConvert = false;
		CD.TMGChartFlag = true;

		SongData PlaySongData =
#ifndef __ANDROID__
			DragFilePath != "" ? FL.LoadTMGChartData(FilePath) :
#endif
			SongList[SongSelector];
		string PlaySongPath =
#ifndef __ANDROID__
		(DragFilePath != "") ? FL.LoadTMGChartData(FilePath).path :
#endif
			SongList[SongSelector].path;

#ifndef __ANDROID__
		if (DragFilePath != "") {
			DragFilePath = "";
			CourseSelector = 0;
		}
#endif
		int barlinecount[2]{ 0,0 };
		int
			barlinenote = 0,
			branchcount = 0,
			gradationstartindex = 0;
		double
			offset = 0,
			BPM = 0,
			branchaddtime = 0;
		bool
			startflag = false,
			readbarflag = false,
			nowbarlineoffflag = false,
			nowbranchstartflag = false,
			branchstartfindflag = false;
		int NoteCount = 0;
		int RollStartIndex = 0;
		bool RollStartFlag = false;

		TMGNoteData _TMGMainData;
		TMGNoteData& TMGMainData = _TMGMainData;
		TMGNoteData _TMGMemData;
		TMGNoteData& TMGMemData = _TMGMemData;

		vector<int> balloon;

		TMGMainData = TMGNoteData();

		CD.TMGNoteDatas.push_back(TMGNoteData());
		CD.BarlineDatas.push_back(BarlineData());

		int errorindex = 0;
		try {
			auto GetStringData = [](string str) {
				vector<string> _result;
				string _src = str.substr(str.find('(') + 1, str.rfind(')') - (str.find('(') + 1));
				string _temp;
				for (int i = 0; i < _src.size(); ++i) {
					if (_src[i] == ',') {
						_result.push_back(_temp);
						_temp = "";
					}
					else if (_src[i] == '(') {
						int LBrackets = 0;
						_temp += _src[i];
						for (int j = i + 1; j < _src.size(); ++j) {
							if (_src[j] == '(') { ++LBrackets; }
							if (_src[j] == ')' && LBrackets == 0) {
								i = j;
								_temp += _src[j];
								break;
							}
							if (_src[j] == ')') { --LBrackets; }
							_temp += _src[j];
						}
					}
					else {
						_temp += _src[i];
					}
				}
				_result.push_back(_temp);
				for (int i = 0; i < _result.size(); i++) {
					_result[i] = strtrim(_result[i]);
				}
				return _result;
			};
			auto GetGradationData = [](string str) {
				auto data = split(str, '~');
				vector<double> result;
				for (int i = 0, size = data.size(); i < size; ++i) {
					result.push_back(StringFormula()(strtrim(data[i])));
				}
				return result;
			};

			vector<string> vlist; {
				fstream fs(FilePath);
				if (!fs.is_open()) {
#ifndef __ANDROID__
					MessageBox(NULL, TEXT((FilePath + "を\n読み込むことができませんでした").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
					throw TMGNoteData();
				}

				auto GetInclude = [&](auto f, string path, vector<string>& _list) -> int {
					fstream _fs(path);
					if (!_fs.is_open()) {
#ifndef __ANDROID__
						MessageBox(NULL, TEXT((path + "を\n読み込むことができませんでした").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
						throw TMGNoteData();
					}
					string _line;
					while (getline(_fs, _line)) {
#ifdef __ANDROID__
                        if (_line.find("\r") != string::npos) {_line = _line.replace(_line.find("\r"), 1, ""); }
#endif

                        if (_line.find("#INCLUDE") == 0) {
							auto data = GetStringData(_line);
							auto _path =
#ifndef __ANDROID__
							(filesystem::path(path).parent_path().string() + "\\" +
#else
								(__fs::filesystem::path(path).parent_path().string() + "/" +
#endif
									data[0]);
							f(f, _path, _list);
						}
						else {
							_list.push_back(removecommentout(_line));
						}
					}
					return 0;
								};
                string line;
                while (getline(fs, line)) {
#ifdef __ANDROID__
                    if (line.find("\r") != string::npos) {line = line.replace(line.find("\r"), 1, ""); }
#endif
                    if (line.find("#INCLUDE") == 0) {
						auto data = GetStringData(line);
						auto _path =
#ifndef __ANDROID__
						(filesystem::path(FilePath).parent_path().string() + "\\" +
#else
							(string)FL.ApplicationPath + "/" + (__fs::filesystem::path(FilePath).parent_path().string() + "/" +
#endif
								data[0]);
						GetInclude(GetInclude, _path, vlist);
					}
					else {
						vlist.push_back(removecommentout(line));
					}
				}
			}

			if (PlaySongData.path == FilePath) {
				for (int i = 0; i < 5; ++i) {
					if (!(CourseSelector != i && PlaySongData.course[i])) { continue; }
					for (int j = PlaySongData.startindex[i]; j < vlist.size(); ++j) {
						if (vlist[j].find("#END()") == 0) {
							vlist[j] = "";
							break;
						}
						vlist[j] = "";
					}
				}
			}

			for (int i = 0; i < vlist.size(); ++i, errorindex = i) {
				if (startflag) {
#define DEFINECUT(x, y) for(int j = 0; vlist[i].find(x) != string::npos; ++j) vlist[i] = vlist[i].replace(vlist[i].find(x), strlen(x), double_to_string(y))
					DEFINECUT("SCROLLFIELD.width", FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width);
					DEFINECUT("Math.pi", DX_PI);
					DEFINECUT("Math.random", GetRand(INT32_MAX));
					DEFINECUT("Screen.Center.x", (FL.ScreenX / 2) - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2) + FL.JudgeOrigin));
					DEFINECUT("Screen.Center.y", (FL.ScreenY / 2) - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y));
					DEFINECUT("NowValue.Scroll", TMGMainData.Scroll);
					DEFINECUT("NowValue.Scrolli", TMGMainData.Scrolli);
					DEFINECUT("NowValue.Radius", TMGMainData.ScrollRadius);
					DEFINECUT("NowValue.DivValue", TMGMainData.ScrollDivValue);
					DEFINECUT("NowValue.Theta", TMGMainData.ScrollTheta);
					DEFINECUT("NowValue.BPM", TMGMainData.BPM);
					DEFINECUT("NowValue.Measure", TMGMainData.Beat);
				}
				// header
				if (vlist[i].find("TITLE:") == 0) {
					string title = strtrim(vlist[i].substr(6, vlist[i].size() - 6));
					if (title.find("--") == 0) {
						CD.TitleDispFlag = false;
					}
					else if (title.find("++") == 0) {
						CD.TitleDispFlag = true;
					}
					title = title.find("--") == 0 || title.find("++") == 0 ? title.replace(0, 2, "") : title;
					CD.Title = title;
				}
				if (vlist[i].find("SUBTITLE:") == 0) {
					string subtitle = strtrim(vlist[i].substr(9, vlist[i].size() - 9));
					if (subtitle.find("--") == 0) {
						CD.SubTitleDispFlag = false;
					}
					else if (subtitle.find("++") == 0) {
						CD.SubTitleDispFlag = true;
					}
					subtitle = subtitle.find("--") == 0 || subtitle.find("++") == 0 ? subtitle.replace(0, 2, "") : subtitle;
					CD.SubTitle = subtitle;
				}
				if (vlist[i].find("BPM:") == 0) {
					TMGMainData.BPM = stod(strtrim(vlist[i].substr(4, vlist[i].size() - 4)));
					BPM = stod(strtrim(vlist[i].substr(4, vlist[i].size() - 4)));
				}
				if (vlist[i].find("WAVE:") == 0) {
					auto path =
#ifndef __ANDROID__
					(filesystem::path(PlaySongData.path).parent_path().string() + "\\" +
#else
						(string)FL.ApplicationPath + "/" + (__fs::filesystem::path(PlaySongData.path).parent_path().string() + "\\" +
#endif
							strtrim(vlist[i].substr(5, vlist[i].size() - 5)));
					if (!CD.ChartCreateMode) {
						CD.Song.handle = LoadSoundMem(path.c_str());
						if (CD.Song.handle != -1) {
							CD.Song.frequency = GetFrequencySoundMem(CD.Song.handle);
							int _frequency = CD.Song.frequency * FL.SongPlaySpeed;
							FL.SongPlaySpeed = (double)_frequency / CD.Song.frequency;
							SetFrequencySoundMem(_frequency, CD.Song.handle);
						}
					}
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);
				}
				if (vlist[i].find("OFFSET:") == 0) {
					offset = (stod(strtrim(vlist[i].substr(7, vlist[i].size() - 7))) * -1000);
				}
				if (vlist[i].find("BALLOON:") == 0) {
					auto a = split(strtrim(vlist[i].substr(8, vlist[i].size() - 8)), ',');
					for (int j = 0; j < a.size(); ++j) {
						balloon.push_back(stoi(strtrim(a[j])));
					}
				}
				if (FL.ChartHeaderSound) {
					if (vlist[i].find("SONGVOL:") == 0) {
						ChangeVolumeSoundMem((int)(255 * (stod(strtrim(vlist[i].substr(8, vlist[i].size() - 8))) / 100) * (FL.SongVolume / 100.)), CD.Song.handle);
					}
					if (vlist[i].find("SEVOL:") == 0) {
						int pal = (255 * (stod(strtrim(vlist[i].substr(6, vlist[i].size() - 6))) / 100) * (FL.SEVolume / 100.));
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
						ChangeVolumeSoundMem(pal, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);
					}
				}

				// Command
				if (vlist[i].find("#BMSCROLL()") == 0) {
					CD.ScrollType = SCROLLType::BMScroll;
				}
				if (vlist[i].find("#HBSCROLL()") == 0) {
					CD.ScrollType = SCROLLType::HBScroll;
				}
				if (vlist[i].find("#START()") == 0) {
					CD.BlankTime = offset < 240000 / TMGMainData.BPM ? (240000 / TMGMainData.BPM) - offset : 0;
					CD.BlankTime += FL.SongBlankTime * 1000;
					TMGMainData.AbsTime += offset + CD.BlankTime + (FL.SongOffset * 1000);
					startflag = true;
					CD.TMGNoteDatas[0].BPM = TMGMainData.BPM;
					CD.TMGNoteDatas[0].RelaTime = TMGMainData.AbsTime;
				}
				if (vlist[i].find("#END()") == 0) {
					startflag = false;
				}
				if (vlist[i].find("#SCROLL") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 1:
							TMGMainData.Scroll = StringFormula()(data[0]);
							TMGMainData.Scrolli = 0;
							TMGMainData.ScrollPolarFlag = false;
							break;
						case 2:
							TMGMainData.Scroll = StringFormula()(data[0]);
							TMGMainData.Scrolli = StringFormula()(data[1]) * -1;
							TMGMainData.ScrollPolarFlag = false;
							break;
						case 3:
							TMGMainData.ScrollRadius = StringFormula()(data[0]);
							TMGMainData.ScrollDivValue = StringFormula()(data[1]);
							TMGMainData.ScrollTheta = StringFormula()(data[2]);
							TMGMainData.ScrollPolarFlag = true;
							break;
						}
					}
					else {
						switch (data.size()) {
						case 1:
							TMGMainData.Gradation.Param[(int)GradationParamType::Scroll] = GetGradationData(data[0]);
							TMGMainData.ScrollPolarFlag = false;
							break;
						case 2:
							TMGMainData.Gradation.Param[(int)GradationParamType::Scroll] = GetGradationData(data[0]);
							TMGMainData.Gradation.Param[(int)GradationParamType::Scrolli] = GetGradationData(data[1]);
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::Scrolli].size(); ++j) {
								TMGMainData.Gradation.Param[(int)GradationParamType::Scrolli][j] *= -1;
							}
							TMGMainData.ScrollPolarFlag = false;
							break;
						case 3:
							TMGMainData.Gradation.Param[(int)GradationParamType::ScrollRadius] = GetGradationData(data[0]);
							TMGMainData.Gradation.Param[(int)GradationParamType::ScrollDivValue] = GetGradationData(data[1]);
							TMGMainData.Gradation.Param[(int)GradationParamType::ScrollTheta] = GetGradationData(data[2]);
							TMGMainData.ScrollPolarFlag = true;
							break;
						}
					}
				}
				if (vlist[i].find("#BPMCHANGE") == 0) {
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 1:
						TMGMainData.BPM = StringFormula()(data[0]);
						TMGMainData.BpmChangeFlag = true;
						break;
					}
				}
				if (vlist[i].find("#MEASURE") == 0) {
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 1:
						TMGMainData.Beat = StringFormula()(data[0]);
						break;
					}
				}
				if (vlist[i].find("#DELAY") == 0) {
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 1:
						TMGMainData.AbsTime += StringFormula()(data[0]) * 1000;
						CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].DelayTime += StringFormula()(data[0]) * 1000;
						break;
					}
				}
				if (vlist[i].find("#SECTION()") == 0) {
					TMGMainData.Section = true;
				}
				if (vlist[i].find("#LEVELHOLD()") == 0) {
					TMGMainData.LevelHold = true;
				}
				if (vlist[i].find("#BRANCHSTART") == 0) {
					auto data = GetStringData(vlist[i]);
					BranchData item;
					switch (data[0][0]) {
					case 'p':
						item.Type = IfBranchType::Parfect;
						item.ExpertBranch = StringFormula()(data[1]);
						item.MasterBranch = StringFormula()(data[2]);
						break;
					case 'r':
						item.Type = IfBranchType::Roll;
						item.ExpertBranch = StringFormula()(data[1]);
						item.MasterBranch = StringFormula()(data[2]);
						break;
					case 's':
						item.Type = IfBranchType::Score;
						item.ExpertBranch = StringFormula()(data[1]);
						item.MasterBranch = StringFormula()(data[2]);
						break;
					}
					branchcount = 0;
					item.AbsTime = CD.BarlineDatas[CD.BarlineDatas.size() - 1].AbsTime - 150;
					item.StartMs = TMGMainData.AbsTime;
					item.Start = true;
					item.BranchFlag = true;
					CD.BranchDatas.push_back(item);
					nowbranchstartflag = true;
					branchstartfindflag = true;
					branchaddtime = 0;
				}
				if (nowbranchstartflag) {
					if (vlist[i].find("#N()") == 0) {
						if (branchcount == 0) { TMGMemData = TMGMainData; }
						TMGMainData = TMGMemData;
						if (branchcount != 0) { CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TMGMainData.BranchStart = true; }
						TMGMainData.IsBranch = BranchType::Normal;
						TMGMainData.DrawBranch = BranchType::Normal;
						branchaddtime = 0;
						++branchcount;
					}
					if (vlist[i].find("#E()") == 0) {
						if (branchcount == 0) { TMGMemData = TMGMainData; }
						TMGMainData = TMGMemData;
						if (branchcount != 0) { CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TMGMainData.BranchStart = true; }
						TMGMainData.IsBranch = BranchType::Expert;
						TMGMainData.DrawBranch = BranchType::Expert;
						branchaddtime = 0;
						++branchcount;
					}
					if (vlist[i].find("#M()") == 0) {
						if (branchcount == 0) { TMGMemData = TMGMainData; }
						TMGMainData = TMGMemData;
						if (branchcount != 0) { CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].RelaTime += -branchaddtime; }
						if (branchcount == 0) { TMGMainData.BranchStart = true; }
						TMGMainData.IsBranch = BranchType::Master;
						TMGMainData.DrawBranch = BranchType::Master;
						branchaddtime = 0;
						++branchcount;
					}
					if (vlist[i].find("#BRANCHEND()") == 0) {
						if (branchcount == 0) { TMGMainData.BranchStart = true; }
						TMGMainData.IsBranch = BranchType::Null;
						TMGMainData.DrawBranch = BranchType::Null;
						nowbranchstartflag = false;
						branchaddtime = 0;
						branchcount = 0;
					}
				}
				if (vlist[i].find("#BARLINEOFF()") == 0) {
					nowbarlineoffflag = true;
				}
				if (vlist[i].find("#BARLINEON()") == 0) {
					nowbarlineoffflag = false;
				}
				if (vlist[i].find("#GOGOSTART()") == 0) {
					TMGMainData.GOGOFlag = GOGOTime::GOGOStart;
				}
				if (vlist[i].find("#GOGOEND()") == 0) {
					TMGMainData.GOGOFlag = GOGOTime::GOGOEnd;
				}
				if (vlist[i].find("#SUDDEN") == 0) {
					TMGMainData.SuddenFlag = 2;
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 1:
						TMGMainData.SuddenFlag = 0;
						TMGMainData.SuddenSpawnMs = 0;
						TMGMainData.SuddenMotionMs = 0;
						break;
					case 2:
						if (StringFormula()(data[0]) <= 0 && StringFormula()(data[1]) <= 0) {
							TMGMainData.SuddenFlag = 0;
							TMGMainData.SuddenSpawnMs = 0;
							TMGMainData.SuddenMotionMs = 0;
						}
						else {
							TMGMainData.SuddenSpawnMs = StringFormula()(data[0]) * 1000;
							TMGMainData.SuddenMotionMs = StringFormula()(data[1]) * 1000;
						}
						break;
					}
				}
				if (vlist[i].find("#JPOSSCROLL") == 0) {
					TMGMainData.JPOSFlag = 2;
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 2:
						TMGMainData.JPOSMotionMs = StringFormula()(data[0]) * 1000;
						if ("Default" == data[1]) {
							TMGMainData.JPOSDefaultPosFlag = true;
						}
						else {
							TMGMainData.JPOSMotionPx_x = StringFormula()(data[1]);
						}
						break;
					case 3:
						TMGMainData.JPOSMotionMs = StringFormula()(data[0]) * 1000;
						if ("Default" == data[1]) {
							TMGMainData.JPOSDefaultPosFlag = true;
						}
						else {
							TMGMainData.JPOSMotionPx_x = StringFormula()(data[1]);
						}
						if ("Default" == data[2]) {
							TMGMainData.JPOSDefaultPosFlag = true;
						}
						else {
							TMGMainData.JPOSMotionPx_y = StringFormula()(data[2]);
						}
						break;
					}
				}
				if (vlist[i].find("#LYRIC") == 0) {
					TMGMainData.LyricChangeFlag = true;
					auto data = GetStringData(vlist[i]);
					TMGMainData.Lyric = data[0];
				}
				if (vlist[i].find("#DUMMYSTART()") == 0) {
					TMGMainData.DummyFlag = true;
				}
				if (vlist[i].find("#DUMMYEND()") == 0) {
					TMGMainData.DummyFlag = false;
				}
				if (vlist[i].find("#JUDGEDELAY") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {

						if ("Init" == data[0] || "0" == data[0]) {
							TMGMainData.JudgeDelayMs = 0;
							TMGMainData.JudgeDelayX = 0;
							TMGMainData.JudgeDelayY = 0;
						}
						if ("Sec" == data[0] || "1" == data[0]) {
							TMGMainData.JudgeDelayMs = StringFormula()(data[1]) * 1000;
							TMGMainData.JudgeDelayX = 0;
							TMGMainData.JudgeDelayY = 0;
						}
						if ("Pos" == data[0] || "2" == data[0]) {
							TMGMainData.JudgeDelayMs = 0;
							TMGMainData.JudgeDelayX = StringFormula()(data[1]);
							TMGMainData.JudgeDelayY = StringFormula()(data[2]);
						}
						if ("Sec_Pos" == data[0] || "3" == data[0]) {
							TMGMainData.JudgeDelayMs = StringFormula()(data[1]) * 1000;
							TMGMainData.JudgeDelayX = StringFormula()(data[2]);
							TMGMainData.JudgeDelayY = StringFormula()(data[3]);
						}
					}
					else {
						if ("Sec" == data[0] || "1" == data[0]) {
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs] = GetGradationData(data[1]);
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs].size(); ++j) {
								TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs][j] *= 1000;
							}
						}
						if ("Pos" == data[0] || "2" == data[0]) {
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayX] = GetGradationData(data[1]);
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayY] = GetGradationData(data[2]);
						}
						if ("Sec_Pos" == data[0] || "3" == data[0]) {
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs] = GetGradationData(data[1]);
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs].size(); ++j) {
								TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayMs][j] *= 1000;
							}
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayX] = GetGradationData(data[2]);
							TMGMainData.Gradation.Param[(int)GradationParamType::JudgeDelayY] = GetGradationData(data[3]);
						}
					}
				}
				if (vlist[i].find("#NOTESPAWN") == 0) {
					NoteSpawnData item;
					auto data = GetStringData(vlist[i]);
					if ("Init" == data[0] || "0" == data[0]) {
						item.Type = NoteSpawnType::Init;
						item.Ms = 0;
						TMGMainData.DisplayFlag = true;
					}
					if ("Spawn" == data[0] || "1" == data[0]) {
						item.Type = NoteSpawnType::Spawn;
						item.Ms = StringFormula()(data[1]) * 1000;
					}
					if ("Vanish" == data[0] || "2" == data[0]) {
						item.Type = NoteSpawnType::Vanish;
						item.Ms = StringFormula()(data[1]) * 1000;
					}
					if (TMGMainData.NoteSpawn.size() == 1 && TMGMainData.NoteSpawn[0].Type == NoteSpawnType::Init) {
						if (item.Type == NoteSpawnType::Spawn) {
							TMGMainData.DisplayFlag = false;
						}
						if (item.Type == NoteSpawnType::Vanish) {
							TMGMainData.DisplayFlag = true;
						}
						TMGMainData.NoteSpawn[0] = item;
					}
					else {
						TMGMainData.NoteSpawn.push_back(item);
						if (TMGMainData.NoteSpawn.size() == 1) {
							if (TMGMainData.NoteSpawn[0].Type == NoteSpawnType::Spawn) {
								TMGMainData.DisplayFlag = false;
							}
							if (TMGMainData.NoteSpawn[0].Type == NoteSpawnType::Vanish) {
								TMGMainData.DisplayFlag = true;
							}
							if (TMGMainData.NoteSpawn[0].Type == NoteSpawnType::Init) {
								TMGMainData.DisplayFlag = true;
							}
						}
					}
				}
				if (vlist[i].find("#ANGLE") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 1:
							TMGMainData.Angle = (DX_PI / 180) * StringFormula()(data[0]);
							break;
						}
					}
					else {
						switch (data.size()) {
						case 1:
							TMGMainData.Gradation.Param[(int)GradationParamType::Angle] = GetGradationData(data[0]);
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::Angle].size(); ++j) {
								TMGMainData.Gradation.Param[(int)GradationParamType::Angle][j] *= (DX_PI / 180);
							}
							break;
						}
					}
				}
				if (vlist[i].find("#SIZE") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 1:
							TMGMainData.Size = StringFormula()(data[0]);
							break;
						}
					}
					else {
						switch (data.size()) {
						case 1:
							TMGMainData.Gradation.Param[(int)GradationParamType::Size] = GetGradationData(data[0]);
							break;
						}
					}
				}
				if (vlist[i].find("#COLOR") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 3:
							TMGMainData.ColorR = StringFormula()(data[0]);
							TMGMainData.ColorG = StringFormula()(data[1]);
							TMGMainData.ColorB = StringFormula()(data[2]);
							break;
						}
					}
					else {
						switch (data.size()) {
						case 3:
							TMGMainData.Gradation.Param[(int)GradationParamType::ColorR] = GetGradationData(data[0]);
							TMGMainData.Gradation.Param[(int)GradationParamType::ColorG] = GetGradationData(data[1]);
							TMGMainData.Gradation.Param[(int)GradationParamType::ColorB] = GetGradationData(data[2]);
							break;
						}
					}
				}
				if (vlist[i].find("#ALPHA") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 1:
							TMGMainData.Alpha = StringFormula()(data[0]);
							break;
						}
					}
					else {
						switch (data.size()) {
						case 1:
							TMGMainData.Gradation.Param[(int)GradationParamType::Alpha] = GetGradationData(data[0]);
							break;
						}
					}
				}
				if (vlist[i].find("#BARLINESIZE") == 0) {
					auto data = GetStringData(vlist[i]);
					if (TMGMainData.Gradation.Flag != GradationType::Set) {
						switch (data.size()) {
						case 2:
							TMGMainData.BarlineSizeW = StringFormula()(data[0]);
							TMGMainData.BarlineSizeH = StringFormula()(data[1]);
							if (StringFormula()(data[0]) < 0) {
								TMGMainData.BarlineSizeW = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].width;
							}
							if (StringFormula()(data[1]) < 0) {
								TMGMainData.BarlineSizeH = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height;
							}
							break;
						}
					}
					else {
						switch (data.size()) {
						case 2:
							TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeW] = GetGradationData(data[0]);
							TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeH] = GetGradationData(data[1]);
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeW].size(); ++j) {
								if (TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeW][j] < 0) {
									TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeW][j] = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].width;
								}
							}
							for (int j = 0; j < TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeH].size(); ++j) {
								if (TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeH][j] < 0) {
									TMGMainData.Gradation.Param[(int)GradationParamType::BarlineSizeH][j] = FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height;
								}
							}
							break;
						}
					}
				}
				if (vlist[i].find("#GRADATION") == 0) {
					auto data = GetStringData(vlist[i]);
					auto strEasingType1 = [](string data) {
						EasingType1 _type = EasingType1::In;
						if ("In" == data || "0" == data) { _type = EasingType1::In; }
						if ("Out" == data || "1" == data) { _type = EasingType1::Out; }
						if ("InOut" == data || "2" == data) { _type = EasingType1::InOut; }
						if ("OutIn" == data || "3" == data) { _type = EasingType1::OutIn; }
						return _type;
					};
					auto strEasingType2 = [](string data) {
						EasingType2 _type = EasingType2::Linear;
						if ("Linear" == data || "0" == data) { _type = EasingType2::Linear; }
						if ("Sine" == data || "1" == data) { _type = EasingType2::Sine; }
						if ("Quad" == data || "2" == data) { _type = EasingType2::Quad; }
						if ("Cubic" == data || "3" == data) { _type = EasingType2::Cubic; }
						if ("Quart" == data || "4" == data) { _type = EasingType2::Quart; }
						if ("Quint" == data || "5" == data) { _type = EasingType2::Quint; }
						if ("Expo" == data || "6" == data) { _type = EasingType2::Expo; }
						if ("Circ" == data || "7" == data) { _type = EasingType2::Circ; }
						if ("Back" == data || "8" == data) { _type = EasingType2::Back; }
						if ("Elastic" == data || "9" == data) { _type = EasingType2::Elastic; }
						if ("Bounce" == data || "10" == data) { _type = EasingType2::Bounce; }
						return _type;
					};
					if ("Set" == data[0]) {
						TMGMainData.Gradation.Flag = GradationType::Set;
						TMGMainData.Gradation.MotionType = strEasingType1(data[1]);
						TMGMainData.Gradation.LineType = strEasingType2(data[2]);
						TMGMainData.Gradation.OffsetMs = StringFormula()(data[3]) * 1000;
						double _time = StringFormula()(data[4]) * 1000;
						TMGMainData.Gradation.MotionTime = _time;
						for (int j = 0; j < (int)GradationParamType::Count; ++j) {
							TMGMainData.Gradation.MotionMs[j] = _time;
						}
					}
					if ("Start" == data[0] && TMGMainData.Gradation.Flag == GradationType::Set) {
						TMGMainData.Gradation.Flag = GradationType::Start;
						for (int j = 0; j < (int)GradationParamType::Count; ++j) {
							if (TMGMainData.Gradation.Param[j].size() > 1) {
								TMGMainData.Gradation.MotionMs[j] /= TMGMainData.Gradation.Param[j].size() - 1;
							}
							else {
								TMGMainData.Gradation.MotionMs[j] = 1;
							}
						}
						GradationData _GData = TMGMainData.Gradation;
						gradationstartindex = CD.TMGNoteDatas.size();
						int index = 0;
						index = (int)GradationParamType::Scroll; if (_GData.Param[index].size() > 0) { TMGMainData.Scroll = _GData.Param[index][0]; }
						index = (int)GradationParamType::Scrolli; if (_GData.Param[index].size() > 0) { TMGMainData.Scrolli = _GData.Param[index][0]; }
						index = (int)GradationParamType::ScrollRadius; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollRadius = _GData.Param[index][0]; }
						index = (int)GradationParamType::ScrollDivValue; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollDivValue = _GData.Param[index][0]; }
						index = (int)GradationParamType::ScrollTheta; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollTheta = _GData.Param[index][0]; }
						index = (int)GradationParamType::JudgeDelayMs; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayMs = _GData.Param[index][0]; }
						index = (int)GradationParamType::JudgeDelayX; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayX = _GData.Param[index][0]; }
						index = (int)GradationParamType::JudgeDelayY; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayY = _GData.Param[index][0]; }
						index = (int)GradationParamType::ColorR; if (_GData.Param[index].size() > 0) { TMGMainData.ColorR = _GData.Param[index][0]; }
						index = (int)GradationParamType::ColorG; if (_GData.Param[index].size() > 0) { TMGMainData.ColorG = _GData.Param[index][0]; }
						index = (int)GradationParamType::ColorB; if (_GData.Param[index].size() > 0) { TMGMainData.ColorB = _GData.Param[index][0]; }
						index = (int)GradationParamType::Alpha; if (_GData.Param[index].size() > 0) { TMGMainData.Alpha = _GData.Param[index][0]; }
						index = (int)GradationParamType::Size; if (_GData.Param[index].size() > 0) { TMGMainData.Size = _GData.Param[index][0]; }
						index = (int)GradationParamType::Angle; if (_GData.Param[index].size() > 0) { TMGMainData.Angle = _GData.Param[index][0]; }
						index = (int)GradationParamType::BarlineSizeW; if (_GData.Param[index].size() > 0) { TMGMainData.BarlineSizeW = _GData.Param[index][0]; }
						index = (int)GradationParamType::BarlineSizeH; if (_GData.Param[index].size() > 0) { TMGMainData.BarlineSizeH = _GData.Param[index][0]; }
					}
					if ("End" == data[0] && gradationstartindex >= 0) {
						TMGMainData.Gradation.Flag = GradationType::End;
						int index = 0;
						GradationData _GData = CD.TMGNoteDatas[gradationstartindex].Gradation;
						index = (int)GradationParamType::Scroll; if (_GData.Param[index].size() > 0) { TMGMainData.Scroll = _GData.Param[index].back(); }
						index = (int)GradationParamType::Scrolli; if (_GData.Param[index].size() > 0) { TMGMainData.Scrolli = _GData.Param[index].back(); }
						index = (int)GradationParamType::ScrollRadius; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollRadius = _GData.Param[index].back(); }
						index = (int)GradationParamType::ScrollDivValue; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollDivValue = _GData.Param[index].back(); }
						index = (int)GradationParamType::ScrollTheta; if (_GData.Param[index].size() > 0) { TMGMainData.ScrollTheta = _GData.Param[index].back(); }
						index = (int)GradationParamType::JudgeDelayMs; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayMs = _GData.Param[index].back(); }
						index = (int)GradationParamType::JudgeDelayX; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayX = _GData.Param[index].back(); }
						index = (int)GradationParamType::JudgeDelayY; if (_GData.Param[index].size() > 0) { TMGMainData.JudgeDelayY = _GData.Param[index].back(); }
						index = (int)GradationParamType::ColorR; if (_GData.Param[index].size() > 0) { TMGMainData.ColorR = _GData.Param[index].back(); }
						index = (int)GradationParamType::ColorG; if (_GData.Param[index].size() > 0) { TMGMainData.ColorG = _GData.Param[index].back(); }
						index = (int)GradationParamType::ColorB; if (_GData.Param[index].size() > 0) { TMGMainData.ColorB = _GData.Param[index].back(); }
						index = (int)GradationParamType::Alpha; if (_GData.Param[index].size() > 0) { TMGMainData.Alpha = _GData.Param[index].back(); }
						index = (int)GradationParamType::Size; if (_GData.Param[index].size() > 0) { TMGMainData.Size = _GData.Param[index].back(); }
						index = (int)GradationParamType::Angle; if (_GData.Param[index].size() > 0) { TMGMainData.Angle = _GData.Param[index].back(); }
						index = (int)GradationParamType::BarlineSizeW; if (_GData.Param[index].size() > 0) { TMGMainData.BarlineSizeW = _GData.Param[index].back(); }
						index = (int)GradationParamType::BarlineSizeH; if (_GData.Param[index].size() > 0) { TMGMainData.BarlineSizeH = _GData.Param[index].back(); }
						gradationstartindex = -10000;
					}
				}
				if (vlist[i].find("#RESETCOMMAND()") == 0) {
					double _time = TMGMainData.AbsTime;
					TMGMainData = TMGNoteData();
					TMGMainData.AbsTime = _time;
					TMGMainData.BPM = BPM;
					TMGMainData.NoteSpawn.clear();
					TMGMainData.NoteSpawn.push_back(NoteSpawnData{ 0,NoteSpawnType::Init });
					TMGMainData.Gradation.Flag = GradationType::End;
					gradationstartindex = -10000;
					nowbarlineoffflag = false;
				}
				if (vlist[i].find("#HISPEED") == 0) {
					TMGMainData.HiSpeedChangeFlag = true;
					auto data = GetStringData(vlist[i]);
					switch (data.size())
					{
					case 1:
						double _hispeed = StringFormula()(data[0]);
						TMGMainData.HiSpeed = _hispeed < 1.0E-100 && _hispeed > -1.0E-100 ? 1.0E-100 : _hispeed;
						break;
					}

				}

				if (vlist[i].find("#") != string::npos) { continue; }

				if (startflag) {
					bool barendflag = false;
					if (!readbarflag) {
						for (int j = i; j < vlist.size(); ++j) {
							if (vlist[j].empty() || vlist[j].find("#") != string::npos) { continue; }
							for (int l = 0; l < vlist[j].size(); ++l) {
								if (vlist[j][l] >= '0' && vlist[j][l] <= '9') {
									barlinenote++;
								}
								if (vlist[j][l] == ',') {
									barlinecount[0]++;
									barendflag = true;
									break;
								}
							}
							if (barendflag) { readbarflag = true; break; }
						}
					}
					for (int j = 0; j < vlist[i].size(); ++j) {
						bool ChartFlag = (vlist[i][j] >= '0' && vlist[i][j] <= '9');
						bool EndFlag = vlist[i][j] == ',';
						bool EmptyFlag = barlinenote == 0;
						if (ChartFlag || EndFlag || EmptyFlag) {

							if (EndFlag && !EmptyFlag) {
								readbarflag = false;
								barlinenote = 0;
								break;
							}

							double temptime = 0;
							double barlinenotetime = 0;
							temptime = 240000. / TMGMainData.BPM * TMGMainData.Beat;
							barlinenotetime = temptime / (barlinenote == 0 ? 1 : barlinenote);

							if (!EmptyFlag) {
								TMGMainData.RelaTime = barlinenotetime;
							}
							else {
								TMGMainData.RelaTime = temptime;
							}

							if (barlinecount[0] != barlinecount[1]) {
								barlinecount[1] = barlinecount[0];
								BarlineData bitem;
								bitem.AbsTime = TMGMainData.AbsTime;
								CD.BarlineDatas.push_back(bitem);
								TMGMainData.Barline = !nowbarlineoffflag;
							}

							TMGMainData.Type = vlist[i][j];

							bool removeflag =
								!TMGMainData.Barline &&
								!TMGMainData.BpmChangeFlag &&
								TMGMainData.GOGOFlag == GOGOTime::Null &&
								TMGMainData.Type == '0' &&
								TMGMainData.JPOSFlag == 0 &&
								!TMGMainData.LyricChangeFlag &&
								TMGMainData.DelayTime == 0 &&
								!TMGMainData.BranchStart &&
								!TMGMainData.Section &&
								!TMGMainData.LevelHold &&
								TMGMainData.NoteSpawn.size() == 0 &&
								TMGMainData.Gradation.Flag == GradationType::Null &&
								!TMGMainData.HiSpeedChangeFlag;

							if (!removeflag) {
								CD.TMGNoteDatas.push_back(TMGMainData);
							}
							else {
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].RelaTime += TMGMainData.RelaTime;
							}

							if (!EmptyFlag) {
								TMGMainData.AbsTime += barlinenotetime;
							}
							else {
								TMGMainData.AbsTime += temptime;
							}

							if (nowbranchstartflag) {
								branchaddtime += TMGMainData.RelaTime * (TMGMainData.BPM > 0 ? 1 : -1);
							}

							TMGMainData.Barline = false;
							TMGMainData.NoteSpawn.clear();

							TMGMainData.GOGOFlag = GOGOTime::Null;

							TMGMainData.LyricChangeFlag = false;

							TMGMainData.BpmChangeFlag = false;

							TMGMainData.BranchStart = false;
							TMGMainData.Section = false;
							TMGMainData.LevelHold = false;

							TMGMainData.JPOSDefaultPosFlag = false;
							TMGMainData.JPOSFlag = 0;
							TMGMainData.JPOSMotionMs = 0;
							TMGMainData.JPOSMotionPx_x = 0;

							TMGMainData.Gradation = GradationData();

							TMGMainData.HiSpeedChangeFlag = false;

							if (!removeflag && !CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].DummyFlag && (CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].IsBranch == BranchType::Null || CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].IsBranch == BranchType::Normal) && (CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Type >= '1' && CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Type <= '4')) {
								++NoteCount;
							}
							if (!removeflag && FL.RandomRate > GetRand(100)) {
								TMGNoteData _temp = CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1];
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Type = _temp.Type == '1' ? '2' : _temp.Type == '2' ? '1' : _temp.Type == '3' ? '4' : _temp.Type == '4' ? '3' : _temp.Type;
							}
							if (!removeflag) {
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Scroll *= FL.AddSCROLL;
							}
							if (!removeflag && FL.ConstantBPM != 0) {
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].BPM = FL.ConstantBPM;
							}
							if (!removeflag && FL.ConstantSCROLL != 0) {
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Scroll = FL.ConstantSCROLL;
								CD.TMGNoteDatas[CD.TMGNoteDatas.size() - 1].Scrolli = 0;
							}

							if (!RollStartFlag && (TMGMainData.Type >= '5' && TMGMainData.Type <= '7') || TMGMainData.Type == '9') {
								RollStartIndex = CD.TMGNoteDatas.size() - 1;
								RollStartFlag = !RollStartFlag;
							}
							else if (RollStartFlag && TMGMainData.Type > '0' && TMGMainData.Type <= '9') {
								CD.TMGNoteDatas[RollStartIndex].RollEndIndex = CD.TMGNoteDatas.size() - 1;
								RollStartFlag = !RollStartFlag;
							}

							if (EndFlag) {
								readbarflag = false;
								barlinenote = 0;
								break;
							}
						}
					}
				}
			}

			for (int i = 0, b = 0; i < CD.TMGNoteDatas.size() && b < balloon.size(); ++i) {
				if (CD.TMGNoteDatas[i].Type == '7' || CD.TMGNoteDatas[i].Type == '9') {
					CD.TMGNoteDatas[i].BalloonCount = balloon[b];
					b++;
				}
			}

			CD.AddScore = (int)round(100000. / NoteCount);
			CD.SoulMaxProgress = NoteCount * 100;

			TrueLoadFlag = true;
			CD.NowBranchFlag = branchstartfindflag ? BranchType::Normal : BranchType::Null;
		}
		catch (...) {
#ifndef __ANDROID__
			MessageBox(NULL, TEXT((FilePath + "の\n" + to_string(errorindex) + "行目でよろしくない書き方をしています").c_str()), TEXT("Error"), MB_ICONERROR | MB_OK);
#endif
			CD.TMGNoteDatas.clear();
			ProgressFlag = SecneType::SongSelect;
			return;
		}
	}

	if (TrueLoadFlag) {
		if (!FL.ChartCreateMode) { FeedInBack(true); }
		ProgressFlag = SecneType::Playing;
		CD.ChartCreateMode = FL.ChartCreateMode;
		CD.GetNowTime = NOWTIME / 1000000.;
	}
	else {
		CD = ChartData();
		FeedInBack(true);
		ProgressFlag = SecneType::SongSelect;
	}
}

inline void PlayingProcess() {
	// ReturnSongSelect
#ifndef __ANDROID__
if (KeyProcess(KEY_INPUT_ESCAPE, [] {
		DeleteSoundMem(CD.Song.handle);
		CD.ChartCreateMode = false;
		ProgressFlag = SecneType::SongSelect;
	})) { return; }
	if (FL.ChartCreateMode) {
	if (KeyProcess(KEY_INPUT_Q, [] {
			StopSoundMem(CD.Song.handle);
			CD.ChartCreateMode = true;
			ProgressFlag = SecneType::ChartLoad;
			})) { return; }
	}
#else
	TouchProcess([]{
		DeleteSoundMem(CD.Song.handle);
		CD.ChartCreateMode = false;
		ProgressFlag = SecneType::SongSelect;
	},50,50,100,100);
	if (FL.ChartCreateMode){
		TouchProcess([]{
			StopSoundMem(CD.Song.handle);
			CD.ChartCreateMode = true;
			ProgressFlag = SecneType::ChartLoad;
		}, 150, 50, 100, 100);
	}
#endif

	if (CD.ChartCreateMode) {
#define _MOTIONTIME (200)
		if (AnimationTime[(int)Animation::TJACreate_Motion] == DBL_MAX * -1) { AnimationTime[(int)Animation::TJACreate_Motion] = 0; }
		
		if (CD.BarlineIndex < 0) { CD.BarlineIndex = 0; }
		if (CD.BarlineIndex >= CD.BarlineDatas.size()) { CD.BarlineIndex = CD.BarlineDatas.size() - 1; }

		if (!ChartCreateMotion.IsGetNowTime) { ChartCreateMotion.start(); }
		if (ChartCreateMotion.IsGetNowTime && abs(AnimationTime[(int)Animation::TJACreate_Motion]) - ChartCreateMotion.GetNowTime() < 0) {
			ChartCreateMotion.end();
			AnimationTime[(int)Animation::TJACreate_Motion] = 0;
		}

		if (ChartCreateMotion.IsGetNowTime) {
			CD.NowTime = CD.BarlineDatas[CD.BarlineIndex].AbsTime + (CD.BarlineTime * ((abs(AnimationTime[(int)Animation::TJACreate_Motion]) - ChartCreateMotion.GetNowTime()) / (_MOTIONTIME * (abs(AnimationTime[(int)Animation::TJACreate_Motion]) / _MOTIONTIME))) * (AnimationTime[(int)Animation::TJACreate_Motion] < 0 ? 1 : -1));
		}
		else {
			CD.BarlineTime = 0;
			AnimationTime[(int)Animation::TJACreate_Motion] = 0;
			CD.NowTime = CD.BarlineDatas[CD.BarlineIndex].AbsTime;
		}

		if (isnan(CD.NowTime)){CD.NowTime = CD.BarlineDatas[CD.BarlineIndex].AbsTime;}
		AnimationTime[(int)Animation::NowTime] = CD.NowTime;

		if (CD.NowTime < 50){
			CD.NowTime = CD.BarlineDatas[CD.BarlineIndex].AbsTime;
		}

		if (CD.TJAChartFlag) {
			int _ActiveRollNum = 0, _ActiveBalloonNum = 0;
			for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
				ChartSubProc(CD.TJANoteDatas[i], _ActiveRollNum, _ActiveBalloonNum);

				if (CD.ScrollType != SCROLLType::Normal && CD.TJANoteDatas[i].AbsTime < CD.NowTime && CD.TJANoteDatas[i].BpmchangeFlag) {
					if (CD.TJANoteDatas[i].BPM * CD.TJANoteDatas[i].Beat > 0) {
						for (int j = i + 1; j < size; ++j) {
							if (CD.TJANoteDatas[j].BpmchangeFlag && CD.TJANoteDatas[j].BPM * CD.TJANoteDatas[j].Beat < 0) {
								CD.TJANoteDatas[i].BpmchangeFlag = false;
								for (int k = j; k < size; ++k) {
									if (CD.TJANoteDatas[k].AbsTime < CD.TJANoteDatas[j].AbsTime) {
										CD.TJANoteDatas[k].BpmSpawnFlag = true;
									}
									else {
										CD.TJANoteDatas[k].BpmSpawnFlag = false;
									}
								}
								break;
							}
							if (CD.TJANoteDatas[j].BpmchangeFlag && CD.TJANoteDatas[j].BPM * CD.TJANoteDatas[j].Beat > 0) {
								CD.TJANoteDatas[i].BpmchangeFlag = false;
								for (int k = j; k < size; ++k) {
									CD.TJANoteDatas[k].BpmSpawnFlag = false;
								}
								break;
							}
						}
					}
					else {
						CD.TJANoteDatas[i].BpmchangeFlag = false;
					}
				}
			}
			CD.NowActiveRollNum = _ActiveRollNum;
			CD.NowActiveBalloonNum = _ActiveBalloonNum;
		}
		if (CD.TMGChartFlag) {
			int _ActiveRollNum = 0, _ActiveBalloonNum = 0;
			for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
				ChartSubProc(CD.TMGNoteDatas[i], _ActiveRollNum, _ActiveBalloonNum);
			}
			CD.NowActiveRollNum = _ActiveRollNum;
			CD.NowActiveBalloonNum = _ActiveBalloonNum;
		}

#ifndef __ANDROID__
		KeyProcess(KEY_INPUT_PGUP, [] {
			++CD.BarlineIndex;
			if (CD.BarlineIndex < CD.BarlineDatas.size()) {
				CD.BarlineTime += CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BarlineDatas[CD.BarlineIndex - 1].AbsTime;
				AnimationTime[(int)Animation::TJACreate_Motion] += _MOTIONTIME;
			}
			});
		KeyProcess(KEY_INPUT_PGDN, [] {
			--CD.BarlineIndex;
			if (CD.BarlineIndex >= 0) {
				CD.BarlineTime += CD.BarlineDatas[CD.BarlineIndex + 1].AbsTime - CD.BarlineDatas[CD.BarlineIndex].AbsTime;
				AnimationTime[(int)Animation::TJACreate_Motion] -= _MOTIONTIME;
			}
			});
		KeyProcess(KEY_INPUT_END, [] {
			if (CD.BarlineIndex < CD.BarlineDatas.size() - 1) {
				CD.BarlineTime += CD.BarlineDatas[CD.BarlineDatas.size() - 1].AbsTime - CD.BarlineDatas[CD.BarlineIndex].AbsTime;
				AnimationTime[(int)Animation::TJACreate_Motion] += _MOTIONTIME;
				CD.BarlineIndex = CD.BarlineDatas.size() - 1;
			}
			});
		KeyProcess(KEY_INPUT_HOME, [] {
			if (CD.BarlineIndex > 0) {
				CD.BarlineTime += CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BarlineDatas[0].AbsTime;
				AnimationTime[(int)Animation::TJACreate_Motion] -= _MOTIONTIME;
				CD.BarlineIndex = 0;
			}
			});
		KeyProcess(KEY_INPUT_SPACE, [] {
			SetCurrentPositionSoundMem((long)(CD.Song.frequency * ((CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BlankTime) / 1000.)), CD.Song.handle);
			CD.ChartCreateMode = false;
			CD.GetNowTime = NOWTIME / 1000000.;
			if ((CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BlankTime) > 0) {
				PlaySoundMem(CD.Song.handle, DX_PLAYTYPE_BACK, FALSE);
			}
			});

		if (FL.GridScreen) {
			NowGridConfigFlag = KeyLongProcess(KEY_INPUT_F3, [] {
				KeyProcess(KEY_INPUT_1, [] {

					});

				});
		}
#else
		OvalTouchProcess([](int x) {
							 SetCurrentPositionSoundMem((long)(CD.Song.frequency * ((CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BlankTime) / 1000)), CD.Song.handle);
							 CD.ChartCreateMode = false;
							 CD.GetNowTime = NOWTIME / 1000000.;
							 PlaySoundMem(CD.Song.handle, DX_PLAYTYPE_BACK, FALSE);
			},
						 FL.ScreenX / 2, FL.ScreenY, (FL.ScreenX / 2) * 0.9, (FL.ScreenY / 2) * 0.95,
						 [](int x){
							 if (x == 1){
								 --CD.BarlineIndex;
								 if (CD.BarlineIndex >= 0) {
									 CD.BarlineTime += CD.BarlineDatas[CD.BarlineIndex + 1].AbsTime - CD.BarlineDatas[CD.BarlineIndex].AbsTime;
									 AnimationTime[(int)Animation::TJACreate_Motion] -= _MOTIONTIME;
								 }
							 }
							 else if (x == 2){
								 ++CD.BarlineIndex;
								 if (CD.BarlineIndex < CD.BarlineDatas.size()) {
									 CD.BarlineTime += CD.BarlineDatas[CD.BarlineIndex].AbsTime - CD.BarlineDatas[CD.BarlineIndex - 1].AbsTime;
									 AnimationTime[(int)Animation::TJACreate_Motion] += _MOTIONTIME;
								 }
							 }
		}
		);
#endif
	}
	else
    {
		// GetNowTime
		CD.NowTime = (((NOWTIME / 1000000. - CD.GetNowTime) * FL.SongPlaySpeed) + (FL.ChartCreateMode ? CD.BarlineDatas[CD.BarlineIndex].AbsTime : 0));
		AnimationTime[(int)Animation::NowTime] = CD.NowTime;

		// PlaySong
		if (CD.BlankTime < CD.NowTime && CD.BlankTime + 128 > CD.NowTime && CheckSoundMem(CD.Song.handle) == 0) {
			PlaySoundMem(CD.Song.handle, DX_PLAYTYPE_BACK);
		}

		// GoResult
		if (CD.BlankTime + 128 < CD.NowTime && CheckSoundMem(CD.Song.handle) == 0) {
			if (FL.ChartCreateMode) {
				StopSoundMem(CD.Song.handle);
				CD.ChartCreateMode = true;
				ProgressFlag = SecneType::ChartLoad;
				return;
			}
			DeleteSoundMem(CD.Song.handle);
			FeedInBack(true);
			ProgressFlag = SecneType::Result;
		}

		// ChatMianProcess
		if (CD.TJAChartFlag) {
			int _ActiveRollNum = 0, _ActiveBalloonNum = 0;
			for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
				ChartSubProc(CD.TJANoteDatas[i], _ActiveRollNum, _ActiveBalloonNum);
				
				
				if (CD.ScrollType != SCROLLType::Normal && CD.TJANoteDatas[i].AbsTime < CD.NowTime && CD.TJANoteDatas[i].BpmchangeFlag) {
					do {
						if (CD.TJANoteDatas[i].BPM * CD.TJANoteDatas[i].Beat < 0) {
							CD.TJANoteDatas[i].BpmchangeFlag = false;
							break;
						}
						CD.TJANoteDatas[i].BpmchangeFlag = false;
						for (int j = i + 1; j < size; ++j) {
							if (!CD.TJANoteDatas[j].BpmchangeFlag) {
								continue;
							}

							if (CD.TJANoteDatas[j].BPM * CD.TJANoteDatas[j].Beat < 0) {
								for (int k = j; k < size; ++k) {
									CD.TJANoteDatas[k].BpmSpawnFlag = CD.TJANoteDatas[k].AbsTime < CD.TJANoteDatas[j].AbsTime;
								}
								break;
							}
							else {
								for (int k = j; k < size; ++k) {
									CD.TJANoteDatas[k].BpmSpawnFlag = false;
								}
								break;
							}
						}
					} while (false);
				}
			}
			CD.NowActiveRollNum = _ActiveRollNum;
			CD.NowActiveBalloonNum = _ActiveBalloonNum;
			
			if (CD.NowBranchFlag != BranchType::Null && !CD.LevelHold) {
				for (int i = 0, size = CD.BranchDatas.size(); i < size; ++i) {
					if (CD.BranchDatas[i].AbsTime < CD.NowTime && CD.BranchDatas[i].Start) {
						for (int j = 0, nsize = CD.TJANoteDatas.size(); j < nsize; ++j) {
							if (CD.TJANoteDatas[j].AbsTime >= CD.BranchDatas[i].StartMs) { continue; }
							if (CD.TJANoteDatas[j].IsBranch != CD.NowBranchFlag && CD.TJANoteDatas[j].IsBranch != BranchType::Null) {
								CD.TJANoteDatas[j].HitFlag = true;
								CD.TJANoteDatas[j].IsBranch = BranchType::NotDisplay;
								CD.TJANoteDatas[j].IsBranch = BranchType::NotDisplay;
							}
							else if (CD.TJANoteDatas[j].IsBranch == CD.NowBranchFlag) {
								CD.TJANoteDatas[j].IsBranch = BranchType::Null;
								CD.TJANoteDatas[j].IsBranch = BranchType::Null;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Parfect) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchRate && CD.BranchDatas[i].MasterBranch > CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Roll) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchRoll && CD.BranchDatas[i].MasterBranch > CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Score) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchScore && CD.BranchDatas[i].MasterBranch > CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						CD.BranchDatas[i].BranchFlag = false;
						CD.BranchDatas[i].Start = false;
						AnimationTime[(int)Animation::Branch] = AnimationTime[(int)Animation::NowTime];
					}
				}
			}
			

		}
		if (CD.TMGChartFlag) {
			int _ActiveRollNum = 0, _ActiveBalloonNum = 0;
			for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
				ChartSubProc(CD.TMGNoteDatas[i], _ActiveRollNum, _ActiveBalloonNum);
			}
			CD.NowActiveRollNum = _ActiveRollNum;
			CD.NowActiveBalloonNum = _ActiveBalloonNum;
			
			if (CD.NowBranchFlag != BranchType::Null && !CD.LevelHold) {
				for (int i = 0, size = CD.BranchDatas.size(); i < size; ++i) {
					if (CD.BranchDatas[i].AbsTime < CD.NowTime && CD.BranchDatas[i].Start) {
						for (int j = 0, nsize = CD.TMGNoteDatas.size(); j < nsize; ++j) {
							if (CD.TMGNoteDatas[j].AbsTime >= CD.BranchDatas[i].StartMs) { continue; }
							if (CD.TMGNoteDatas[j].IsBranch != CD.NowBranchFlag && CD.TMGNoteDatas[j].IsBranch != BranchType::Null) {
								CD.TMGNoteDatas[j].HitFlag = true;
								CD.TMGNoteDatas[j].IsBranch = BranchType::NotDisplay;
								CD.TMGNoteDatas[j].DrawBranch = BranchType::NotDisplay;
							}
							else if (CD.TMGNoteDatas[j].IsBranch == CD.NowBranchFlag) {
								CD.TMGNoteDatas[j].IsBranch = BranchType::Null;
								CD.TMGNoteDatas[j].DrawBranch = BranchType::Null;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Parfect) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchRate && CD.BranchDatas[i].MasterBranch > CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchRate) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Roll) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchRoll && CD.BranchDatas[i].MasterBranch > CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchRoll) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						if (CD.BranchDatas[i].Type == IfBranchType::Score) {
							if (CD.BranchDatas[i].ExpertBranch > CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Normal :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Normal : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Normal;
							}
							else if (CD.BranchDatas[i].ExpertBranch <= CD.BranchScore && CD.BranchDatas[i].MasterBranch > CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Expert :
										 (CD.NowBranchFlag == BranchType::Master ? ABranchType::Master_Expert : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Expert;
							}
							else if (CD.BranchDatas[i].MasterBranch <= CD.BranchScore) {
								CD.NowBranchAnimation =
										(CD.NowBranchFlag == BranchType::Normal ? ABranchType::Normal_Master :
										 (CD.NowBranchFlag == BranchType::Expert ? ABranchType::Expert_Master : ABranchType::Null));
								CD.NowBranchFlag = BranchType::Master;
							}
						}
						CD.BranchDatas[i].BranchFlag = false;
						CD.BranchDatas[i].Start = false;
						AnimationTime[(int)Animation::Branch] = AnimationTime[(int)Animation::NowTime];
					}
				}
			}
		}

		if (CD.HitNote != 0 && CD.BranchHitNote != 0) {
			CD.Rate = (CD.Good + (CD.Ok / 2.)) / (double)CD.HitNote * 100;
			CD.BranchRate = (CD.BranchGood + (CD.BranchOk / 2.)) / (double)CD.BranchHitNote * 100;
		}

		// PlayingMainProcess
		if (FL.AutoPlayFlag) {
			static bool LeftRightFlag = false;
			if (CD.TJAChartFlag) {
				for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
					bool NoteBranch = CD.TJANoteDatas[i].IsBranch == CD.NowBranchFlag || CD.TJANoteDatas[i].IsBranch == BranchType::Null;
					bool HitFlag = CD.TJANoteDatas[i].AbsTime < CD.NowTime && !CD.TJANoteDatas[i].HitFlag && NoteBranch;
					int randrate = 100;
					bool TypeFlag = CD.TJANoteDatas[i].Type >= '1' && CD.TJANoteDatas[i].Type <= '4';
					if (HitFlag && TypeFlag) {
						bool DonOrKaFlag = CD.TJANoteDatas[i].Type == '1' || CD.TJANoteDatas[i].Type == '3';
						bool BigNoteFlag = CD.TJANoteDatas[i].Type == '3' || CD.TJANoteDatas[i].Type == '4';
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						AnimationTime[(int)Animation::COMBOBOUNCE] = CD.NowTime;
						AnimationTime[(int)Animation::SCOREBOUNCE] = CD.NowTime;
						if (DonOrKaFlag) {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
							AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
							if (!BigNoteFlag) {
								AnimationTime[LeftRightFlag ? (int)Animation::MINITAIKO_DON_R : (int)Animation::MINITAIKO_DON_L] = CD.NowTime;
							}
							else {
								AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
								AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
							}
						}
						else {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
							AnimationTime[(int)Animation::SCROLLFIELD_KA] = CD.NowTime;
							if (!BigNoteFlag) {
								AnimationTime[LeftRightFlag ? (int)Animation::MINITAIKO_KA_R : (int)Animation::MINITAIKO_KA_L] = CD.NowTime;
							}
							else {
								AnimationTime[(int)Animation::MINITAIKO_KA_L] = CD.NowTime;
								AnimationTime[(int)Animation::MINITAIKO_KA_R] = CD.NowTime;
							}
						}
						AddHitNoteData(HitNoteData("good", CD.TJANoteDatas[i].Type));
						++CD.Combo;
						if (GetRand(100) < randrate) { ++CD.BranchGood; }
						else { ++CD.BranchOk; }
						++CD.HitNote; ++CD.BranchHitNote;
						CD.SoulProgress += CD.SoulAddProgress;
						CD.TJANoteDatas[i].Type = '0';
						LeftRightFlag = !LeftRightFlag;
					}
					if (CD.NowActiveRollNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime && CD.TJANoteDatas[i].RollFlag) {
						PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
						bool BigRollFlag = CD.TJANoteDatas[i].Type == '6';
						AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						AnimationTime[(int)Animation::SCOREBOUNCE] = CD.NowTime;
						if (!BigRollFlag) {
							AnimationTime[LeftRightFlag ? (int)Animation::MINITAIKO_DON_R : (int)Animation::MINITAIKO_DON_L] = CD.NowTime;
							LeftRightFlag = !LeftRightFlag;
						}
						else {
							AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
							AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
						}
						++CD.BranchRoll;
						AddHitNoteData(HitNoteData("roll", BigRollFlag ? '3' : '1'));
					}
					if (CD.NowActiveBalloonNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime && CD.TJANoteDatas[i].BalloonFlag == 1) {
						PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
						AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						AnimationTime[LeftRightFlag ? (int)Animation::MINITAIKO_DON_R : (int)Animation::MINITAIKO_DON_L] = CD.NowTime;
						AnimationTime[(int)Animation::SCOREBOUNCE] = CD.NowTime;
						CD.TJANoteDatas[i].BalloonCount--;
						LeftRightFlag = !LeftRightFlag;
						if (CD.TJANoteDatas[i].BalloonCount <= 0) {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle, DX_PLAYTYPE_BACK);
							AddHitNoteData(HitNoteData("roll", '3'));
							CD.TJANoteDatas[i].BalloonFlag = 0;
							CD.TJANoteDatas[i].Type = '0';
							continue;
						}
						++CD.BranchRoll;
					}
				}
				if (CD.NowActiveRollNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime) {
					CD.WaitRollTime = CD.NowTime;
				}
				if (CD.NowActiveBalloonNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime) {
					CD.WaitRollTime = CD.NowTime;
				}
			}
			if (CD.TMGChartFlag) {
				for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
					bool NoteBranch = CD.TMGNoteDatas[i].IsBranch == CD.NowBranchFlag || CD.TMGNoteDatas[i].IsBranch == BranchType::Null;
					bool HitFlag = CD.TMGNoteDatas[i].AbsTime < CD.NowTime && !CD.TMGNoteDatas[i].HitFlag && !CD.TMGNoteDatas[i].DummyFlag && NoteBranch;
					int randrate = 100;
					bool TypeFlag = CD.TMGNoteDatas[i].Type >= '1' && CD.TMGNoteDatas[i].Type <= '4';
					if (HitFlag && TypeFlag) {
						bool DonOrKaFlag = CD.TMGNoteDatas[i].Type == '1' || CD.TMGNoteDatas[i].Type == '3';
						bool BigNoteFlag = CD.TMGNoteDatas[i].Type == '3' || CD.TMGNoteDatas[i].Type == '4';
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						AnimationTime[(int)Animation::COMBOBOUNCE] = CD.NowTime;
						AnimationTime[(int)Animation::SCOREBOUNCE] = CD.NowTime;
						if (DonOrKaFlag) {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
							AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
							AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
							if (BigNoteFlag) {
								AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
							}
						}
						if (!DonOrKaFlag) {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].handle, DX_PLAYTYPE_BACK);
							AnimationTime[(int)Animation::SCROLLFIELD_KA] = CD.NowTime;
							AnimationTime[(int)Animation::MINITAIKO_KA_L] = CD.NowTime;
							if (BigNoteFlag) {
								AnimationTime[(int)Animation::MINITAIKO_KA_R] = CD.NowTime;
							}
						}
						AddHitNoteData(HitNoteData("good", CD.TMGNoteDatas[i].Type));
						++CD.Combo;
						if (GetRand(100) < randrate) { ++CD.BranchGood; }
						else { ++CD.BranchOk; }
						++CD.HitNote; ++CD.BranchHitNote;
						CD.SoulProgress += CD.SoulAddProgress;
						CD.TMGNoteDatas[i].Type = '0';
					}
					if (CD.NowActiveRollNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime && CD.TMGNoteDatas[i].RollFlag) {
						PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
						bool BigRollFlag = CD.TMGNoteDatas[i].Type == '6';
						AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						if (!BigRollFlag) {
							AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
						}
						else {
							AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
							AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
						}
						++CD.BranchRoll;
						AddHitNoteData(HitNoteData("roll", BigRollFlag ? '3' : '1'));
					}
					if (CD.NowActiveBalloonNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime && CD.TMGNoteDatas[i].BalloonFlag == 1) {
						PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
						AnimationTime[(int)Animation::SCROLLFIELD_DON] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = CD.NowTime;
						AnimationTime[(int)Animation::SCROLLFIELD_HIT] = CD.NowTime;
						AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
						CD.TMGNoteDatas[i].BalloonCount--;
						if (CD.TMGNoteDatas[i].BalloonCount <= 0) {
							PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle, DX_PLAYTYPE_BACK);
							AddHitNoteData(HitNoteData("roll", '3'));
							CD.TMGNoteDatas[i].BalloonFlag = 0;
							CD.TMGNoteDatas[i].Type = '0';
							continue;
						}
						++CD.BranchRoll;
					}
				}
				if (CD.NowActiveRollNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime) {
					CD.WaitRollTime = CD.NowTime;
				}
				if (CD.NowActiveBalloonNum > 0 && CD.WaitRollTime + (1000 / FL.RollSpeed) < CD.NowTime) {
					CD.WaitRollTime = CD.NowTime;
				}
			}
		}
		else {
#ifndef __ANDROID__
			if (CD.TJAChartFlag) {
				for (int i = 0; i < 10; ++i) {
				if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
					TJANoteHit('1');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
					TJANoteHit('1');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_LK[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_KA_L] = CD.NowTime;
					TJANoteHit('2');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_RK[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_KA_R] = CD.NowTime;
					TJANoteHit('2');
						})) { break; }
				}
			}
			if (CD.TMGChartFlag) {
				for (int i = 0; i < 10; ++i) {
				if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_DON_L] = CD.NowTime;
					TMGNoteHit('1');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_DON_R] = CD.NowTime;
					TMGNoteHit('1');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_LK[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_KA_L] = CD.NowTime;
					TMGNoteHit('2');
						})) { break; }
				if (KeyProcess(FL.KEY_INPUT_RK[i], [] {
						AnimationTime[(int)Animation::MINITAIKO_KA_R] = CD.NowTime;
					TMGNoteHit('2');
						})) { break; }
				}
			}
#else
			if (CD.TJAChartFlag) {
				OvalTouchProcess([](int x) {
									 if (x == 1) {
										 AnimationTime[(int) Animation::MINITAIKO_DON_L] = CD.NowTime;
										 TJANoteHit('1');
									 } else if (x == 2) {
										 AnimationTime[(int) Animation::MINITAIKO_DON_R] = CD.NowTime;
										 TJANoteHit('1');
									 }
								 },
								 FL.ScreenX / 2, FL.ScreenY, (FL.ScreenX / 2) * 0.9, (FL.ScreenY / 2) * 0.95,
								 [](int x) {
									 if (x == 1) {
										 AnimationTime[(int) Animation::MINITAIKO_KA_L] = CD.NowTime;
										 TJANoteHit('2');
									 } else if (x == 2) {
										 AnimationTime[(int) Animation::MINITAIKO_KA_R] = CD.NowTime;
										 TJANoteHit('2');
									 }
								 }
				);
			}
			if (CD.TMGChartFlag) {
				OvalTouchProcess([](int x) {
									 if (x == 1) {
										 AnimationTime[(int) Animation::MINITAIKO_DON_L] = CD.NowTime;
										 TMGNoteHit('1');
									 } else if (x == 2) {
										 AnimationTime[(int) Animation::MINITAIKO_DON_R] = CD.NowTime;
										 TMGNoteHit('1');
									 }
								 },
								 FL.ScreenX / 2, FL.ScreenY, (FL.ScreenX / 2) * 0.9, (FL.ScreenY / 2) * 0.95,
								 [](int x) {
									 if (x == 1) {
										 AnimationTime[(int) Animation::MINITAIKO_KA_L] = CD.NowTime;
										 TMGNoteHit('2');
									 } else if (x == 2) {
										 AnimationTime[(int) Animation::MINITAIKO_KA_R] = CD.NowTime;
										 TMGNoteHit('2');
									 }
								 }
				);
			}
#endif
		}
	}
}

inline void ResultProcess() {
#ifndef __ANDROID__
	if (KeyProcess(KEY_INPUT_ESCAPE, [] {
		FeedInBack(true);
		ProgressFlag = SecneType::SongSelect;
		})) {
		return;
	}
	for (int i = 0; i < 10; ++i) {
		if (KeyProcess(FL.KEY_INPUT_LD[i], [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			FeedInBack(true);
			ProgressFlag = SecneType::SongSelect;
			})) {
			break;
		}
		if (KeyProcess(FL.KEY_INPUT_RD[i], [] {
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
			FeedInBack(true);
			ProgressFlag = SecneType::SongSelect;
			})) {
			break;
		}
	}
#else
	TouchProcess([] {
		PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].handle, DX_PLAYTYPE_BACK);
		ProgressFlag = SecneType::SongSelect;
		});
#endif
}

inline void ConfigProcess() {
#ifndef __ANDROID__
	if (!ConfigSelect && !KeyConfigSelect) {
		if (KeyProcess(KEY_INPUT_ESCAPE, [] {
			FeedInBack(true);
			ProgressFlag = SecneType::SongSelect;
			})) {
			return;
		}
		KeyProcess(KEY_INPUT_UP, [] {
			ConfigGenreSelector = ConfigGenreSelector - 1 < 0 ? 0 : ConfigGenreSelector - 1;
			});
		KeyProcess(KEY_INPUT_LEFT, [] {
			ConfigGenreSelector = ConfigGenreSelector - 1 < 0 ? 0 : ConfigGenreSelector - 1;
			});
		KeyProcess(KEY_INPUT_DOWN, [] {
			ConfigGenreSelector = ConfigGenreSelector + 1 > 2 - 1 ? 2 - 1 : ConfigGenreSelector + 1;
			});
		KeyProcess(KEY_INPUT_RIGHT, [] {
			ConfigGenreSelector = ConfigGenreSelector + 1 > 2 - 1 ? 2 - 1 : ConfigGenreSelector + 1;
			});
		KeyProcess(KEY_INPUT_RETURN, [] {
			ConfigSelect = ConfigGenreSelector == 0 ? true : false;
			KeyConfigSelect = ConfigGenreSelector == 1 ? true : false;
			});
	}
	if (ConfigSelect) {
		KeyProcess(KEY_INPUT_ESCAPE, [] {
			ConfigSelect = false;
			});
		KeyProcess(KEY_INPUT_UP, [] {
			ConfigSelector = ConfigSelector - 1 < 0 ? 0 : ConfigSelector - 1;
			});
		KeyProcess(KEY_INPUT_LEFT, [] {
			ConfigSelector = ConfigSelector - 1 < 0 ? 0 : ConfigSelector - 1;
			});
		KeyProcess(KEY_INPUT_DOWN, [] {
			ConfigSelector = ConfigSelector + 1 > (int)ConfigID::ConfigCount - 1 ? (int)ConfigID::ConfigCount - 1 : ConfigSelector + 1;
			});
		KeyProcess(KEY_INPUT_RIGHT, [] {
			ConfigSelector = ConfigSelector + 1 > (int)ConfigID::ConfigCount - 1 ? (int)ConfigID::ConfigCount - 1 : ConfigSelector + 1;
			});

		// MainSeter
		try {
			KeyProcess(KEY_INPUT_RETURN, [] {
				InputHandle = MakeKeyInput(255, FALSE, FALSE, FALSE);
				SetActiveKeyInput(InputHandle);
				while (CheckKeyInput(InputHandle) == 0) {
					if (ProcessMessage() == -1) { break; }
				}
				GetKeyInputString(InputString, InputHandle);
				if (ConfigSelector == (int)ConfigID::AutoPlayFlag) {
					 stob(&FL.AutoPlayFlag, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::ConstantSCROLL) {
					FL.ConstantSCROLL = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::ConstantBPM) {
					FL.ConstantBPM = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::AddSCROLL) {
					FL.AddSCROLL = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::StableVSyncFlag) {
					stob(&FL.StableVSyncFlag, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::JudgeGood) {
					FL.JudgeGood = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::JudgeOk) {
					FL.JudgeOk = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::JudgeBad) {
					FL.JudgeBad = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::JudgeOffset) {
					FL.JudgeOffset = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::SongPlaySpeed) {
					FL.SongPlaySpeed = stod((string)InputString);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_DON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_DON].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_KA].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_KA].handle);
					SetFrequencySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].frequency * FL.SongPlaySpeed, FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle);
				}
				if (ConfigSelector == (int)ConfigID::SongBlankTime) {
					FL.SongBlankTime = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::SongOffset) {
					FL.SongOffset = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::SongVolume) {
					FL.SongVolume = stoi((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::SEVolume) {
					FL.SEVolume = stoi((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::ChartHeaderSound) {
					stob(&FL.ChartHeaderSound, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::JiroBPMDelayFlag) {
					stob(&FL.JiroBPMDelayFlag, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::DrawingOperationRangeWidth) {
					FL.DrawingOperationRangeWidth = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::DrawingOperationRangeHeight) {
					FL.DrawingOperationRangeHeight = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::RandomRate) {
					FL.RandomRate = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::RollSpeed) {
					FL.RollSpeed = stod((string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::ChartCreateMode) {
					stob(&FL.ChartCreateMode, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::GridScreen) {
					stob(&FL.GridScreen, (string)InputString);
				}
				if (ConfigSelector == (int)ConfigID::DebugScreen) {
					stob(&FL.DebugScreen, (string)InputString);
				}
				DeleteKeyInput(InputHandle);
				});
		}
		catch (...) {

		}
	}
	else if (KeyConfigSelect) {
		if (KeyProcess(KEY_INPUT_ESCAPE, [] {
			KeyConfigSelect = false;
			})) {
			return;
		}
		KeyProcess(KEY_INPUT_UP, [] {
			KeyConfigSelector = KeyConfigSelector - 1 < 0 ? 0 : KeyConfigSelector - 1;
			});
		KeyProcess(KEY_INPUT_LEFT, [] {
			KeyConfigSelector = KeyConfigSelector - 1 < 0 ? 0 : KeyConfigSelector - 1;
			});
		KeyProcess(KEY_INPUT_DOWN, [] {
			KeyConfigSelector = KeyConfigSelector + 1 > (int)KeyConfigID::KeyConfigCount - 1 ? (int)KeyConfigID::KeyConfigCount - 1 : KeyConfigSelector + 1;
			});
		KeyProcess(KEY_INPUT_RIGHT, [] {
			KeyConfigSelector = KeyConfigSelector + 1 > (int)KeyConfigID::KeyConfigCount - 1 ? (int)KeyConfigID::KeyConfigCount - 1 : KeyConfigSelector + 1;
			});

		KeyProcess(KEY_INPUT_RETURN, [] {
			KeySeter = true;
			int Key = 0;
			while (Key == 0) {
				Key = CheckHitKeyAll();
				if (Key == 28 || Key == 156) {
					Key = 0;
				}
			}
			KeyBuf[Key] = 1;
			KeyInt[Key] = 1;
			if (KeyConfigSelector >= 0 && KeyConfigSelector < 10) {
				FL.KEY_INPUT_LD[KeyConfigSelector - 0] = Key;
			}
			if (KeyConfigSelector >= 10 && KeyConfigSelector < 20) {
				FL.KEY_INPUT_LK[KeyConfigSelector - 10] = Key;
			}
			if (KeyConfigSelector >= 20 && KeyConfigSelector < 30) {
				FL.KEY_INPUT_RD[KeyConfigSelector - 20] = Key;
			}
			if (KeyConfigSelector >= 30 && KeyConfigSelector < 40) {
				FL.KEY_INPUT_RK[KeyConfigSelector - 30] = Key;
			}
			if (KeyConfigSelector >= 40 && KeyConfigSelector < 44) {
				FL.QuickAccessKey[KeyConfigSelector - 40] = Key;
			}
			KeySeter = false;
			});
	}
#else
	// TODO: タッチ操作でコンフィグ変更
#endif
}

