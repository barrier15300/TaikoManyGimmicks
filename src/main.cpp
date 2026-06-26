#include "main.h"
#include "ChartLib.h"
#ifndef __ANDROID__
#include "resource.h"
#endif

#define TS(x) to_string(x)
#define DTS(x,d) double_to_string(x,d)

double DrawRangeL = 0;
double DrawRangeR = 0;
double DrawRangeU = 0;
double DrawRangeD = 0;
double NoteDrawRate = 0;

TimeData SelectMotion;
TimeData LoadMotion;

int LoadScreen = 0;
int MemNowFlameCount = 0;

DATEDATA datetime;

inline void DrawCombo(const long& num, const double& bouncerate) {
	long _num = num;
	int Count = (int)log10(num) + 1;
	int NumberType = -1;
	int StringType = -1;

	if (num >= FL.ComboChange[0] && num < FL.ComboChange[1]) { NumberType = (int)ImageID::GAME_PLAYING_COMBONUMBER1; StringType = (int)ImageID::GAME_PLAYING_COMBOSTRING1; }
	if (num >= FL.ComboChange[1] && num < FL.ComboChange[2]) { NumberType = (int)ImageID::GAME_PLAYING_COMBONUMBER2; StringType = (int)ImageID::GAME_PLAYING_COMBOSTRING2; }
	if (num >= FL.ComboChange[2]) { NumberType = (int)ImageID::GAME_PLAYING_COMBONUMBER3; StringType = (int)ImageID::GAME_PLAYING_COMBOSTRING3; }

	if (NumberType != -1 && StringType != -1) {
		double offset = (FL.Image[NumberType].width * FL.NumberCompressionRates[(int)NumberCompressionRateType::Playing_Combo]) * ((double)Count / 2) - (FL.Image[NumberType].width * FL.NumberCompressionRates[(int)NumberCompressionRateType::Playing_Combo] / 2);

		for (int i = 0; i < Count; ++i) {
			DrawExtendGraphF(
				FL.Image[NumberType].x - (FL.Image[NumberType].width / 2) + offset,
				FL.Image[NumberType].y - (FL.Image[NumberType].height / 2) - (FL.Image[NumberType].height * bouncerate),
				FL.Image[NumberType].x + (FL.Image[NumberType].width / 2) + offset,
				FL.Image[NumberType].y + (FL.Image[NumberType].height / 2),
				FL.Image[NumberType].handle[_num % 10],
				TRUE
			);
			offset -= FL.Image[NumberType].width * FL.NumberCompressionRates[(int)NumberCompressionRateType::Playing_Combo];
			_num /= 10;
		}
		DrawGraphF(
				FL.Image[StringType].x - (FL.Image[StringType].width / 2),
				FL.Image[StringType].y - (FL.Image[StringType].height / 2),
				FL.Image[StringType].handle[0],
				TRUE
		);
	}
}

void DrawScore(const long& num, const double& bouncerate, const double& m_add_x = 0, const double& m_add_y = 0) {
	int NumberType = (int)ImageID::GAME_PLAYING_SCORENUMBER;
	long DrawValue = num;

	double offset = 0;
	do {
		if (FL.Image[NumberType].handle.size() != 0) {
			DrawExtendGraphF(
				FL.Image[NumberType].x + m_add_x - (FL.Image[NumberType].width / 2) - offset,
				FL.Image[NumberType].y + m_add_y - (FL.Image[NumberType].height / 2) - (FL.Image[NumberType].height * bouncerate),
				FL.Image[NumberType].x + m_add_x + (FL.Image[NumberType].width / 2) - offset,
				FL.Image[NumberType].y + m_add_y + (FL.Image[NumberType].height / 2),
				FL.Image[NumberType].handle[DrawValue % 10],
				TRUE
			);
		}
		offset += (FL.Image[NumberType].width * FL.NumberCompressionRates[(int)NumberCompressionRateType::Playing_Score]);
		DrawValue /= 10;
	} while (DrawValue > 0);
}

inline void DrawResultValue(const long& num, string type) {
	int NumberType = -1;
	int StringType = -1;
	int CompressionRateType = -1;
	long DrawValue = num;

	if (type == "Good") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_GOOD;
		StringType = (int)ImageID::GAME_RESULT_STRING_GOOD;
		CompressionRateType = (int)NumberCompressionRateType::Result_Good;
	}
	else if (type == "Ok") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_OK;
		StringType = (int)ImageID::GAME_RESULT_STRING_OK;
		CompressionRateType = (int)NumberCompressionRateType::Result_Ok;
	}
	else if (type == "Bad") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_BAD;
		StringType = (int)ImageID::GAME_RESULT_STRING_BAD;
		CompressionRateType = (int)NumberCompressionRateType::Result_Bad;
	}
	else if (type == "Roll") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_ROLL;
		StringType = (int)ImageID::GAME_RESULT_STRING_ROLL;
		CompressionRateType = (int)NumberCompressionRateType::Result_Roll;
	}
	else if (type == "MaxCombo") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_MAXCOMBO;
		StringType = (int)ImageID::GAME_RESULT_STRING_MAXCOMBO;
		CompressionRateType = (int)NumberCompressionRateType::Result_MaxCombo;
	}
	else if (type == "Rate") {
		NumberType = (int)ImageID::GAME_RESULT_NUMBER_RATE;
		StringType = (int)ImageID::GAME_RESULT_STRING_RATE;
		CompressionRateType = (int)NumberCompressionRateType::Result_Rate;
	}

	if (NumberType != -1 && StringType != -1 && CompressionRateType != -1) {
		double offset = 0;

		do{
			if (FL.Image[NumberType].handle.size() != 0) {
				DrawExtendGraphF(
						FL.Image[NumberType].x - (FL.Image[NumberType].width / 2) - offset,
						FL.Image[NumberType].y - (FL.Image[NumberType].height / 2),
						FL.Image[NumberType].x + (FL.Image[NumberType].width / 2) - offset,
						FL.Image[NumberType].y + (FL.Image[NumberType].height / 2),
						FL.Image[NumberType].handle[DrawValue % 10],
						TRUE
				);
			}
			offset += (FL.Image[NumberType].width * FL.NumberCompressionRates[CompressionRateType]);
			DrawValue /= 10;
		} while (DrawValue > 0);

		if (FL.Image[StringType].handle.size() != 0) {
			DrawGraphF(
					FL.Image[StringType].x - (FL.Image[StringType].width / 2),
					FL.Image[StringType].y - (FL.Image[StringType].height / 2),
					FL.Image[StringType].handle[0],
					TRUE
			);
		}
	}
}

struct GetTJANotePos {
	inline double operator()(const TJANoteData& data, const double& nowtime, const double& nowbpm, const bool& real) {
		double _pos = 0;

		if (SCROLLType::Normal == CD.ScrollType) {
			_pos = ((data.AbsTime - nowtime) / (240 / data.BPM));
		}
		else if (SCROLLType::BMScroll == CD.ScrollType || SCROLLType::HBScroll == CD.ScrollType) {
			double _bpm = data.BMFlag || data.BpmSpawnFlag ? data.BPM : nowbpm;
			_pos = (((data.BMFlag || data.BpmSpawnFlag ? data.AbsTime : data.BMTime) - nowtime) / (240 / _bpm));
		}

		if (data.SuddenFlag == 1) {
			_pos = (data.SuddenMotionMs / (240 / data.BPM));
		}

		_pos *= NoteDrawRate * (SCROLLType::BMScroll != CD.ScrollType ? (real ? data.Scroll : data.Scrolli) : (real ? 1 : 0));

		return _pos;
	}
};

struct GetTMGNotePos {
	inline double operator()(const TMGNoteData &data, const double &nowtime, const double &nowbpm, const double &nowhispeed, const bool &real) {
		double _pos = 0;
		complex<double> _polar;

		if (SCROLLType::Normal == CD.ScrollType) {
			_pos = (((data.AbsTime + data.JudgeDelayMs) - nowtime) / (240 / data.BPM));
		}
		else if (SCROLLType::BMScroll == CD.ScrollType || SCROLLType::HBScroll == CD.ScrollType) {
			double _bpm = data.BMFlag ? data.BPM * data.HiSpeed : nowbpm * nowhispeed;
			_pos = ((((data.BMFlag ? data.AbsTime : data.BMTime) + (data.JudgeDelayMs / (_bpm / data.BPM))) - nowtime) / (240 / _bpm));
		}

		if (data.SuddenFlag == 1) {
			_pos = (data.SuddenMotionMs / (240 / data.BPM));
		}

		if (data.ScrollPolarFlag) {
			_polar = PolarPosToComplexPos(data.ScrollRadius, (DX_TWO_PI / data.ScrollDivValue) * data.ScrollTheta);
			_pos *= NoteDrawRate * (SCROLLType::BMScroll != CD.ScrollType ? (real ? _polar.real() : (_polar.imag() * -1)) : (real ? 1 : 0));
		}
		else {
			_pos *= NoteDrawRate * (SCROLLType::BMScroll != CD.ScrollType ? (real ? data.Scroll : data.Scrolli) : (real ? 1 : 0));
		}

		_pos += real ? data.JudgeDelayX : data.JudgeDelayY;

		return _pos;
	}
};

void OriginalDrawGraph(const ImageData& Data, const int& Index = 0, const double& x = 0, const double& y = 0) {
    if (Index < 0 || Index >= Data.handle.size() || Data.handle.size() == 0) { return; }
    DrawExtendGraphF(
            x + Data.x - (Data.width / 2),
            y + Data.y - (Data.height / 2),
            x + Data.x + (Data.width / 2),
            y + Data.y + (Data.height / 2),
            Data.handle[Index],
            TRUE
    );
}

inline void DrawStringandBG(float x, float y, unsigned int scolor, unsigned int bgcolor, int bgalpha, string str) {
	const TCHAR* _str = str.c_str();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bgalpha);
	DrawBox(x, y, x + GetDrawStringWidth(_str, strlenDx(_str)), y + 16, bgcolor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawStringF(x, y, _str, scolor);
}

//
// 
// 
// 
// 
// DrawThread
// 
// 
// 
// 

#ifndef __ANDROID__
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int DrawThread(void) {
#endif
#ifndef __ANDROID__
	setlocale(LC_ALL, "");
	SetWindowIconID(IDI_ICON1);
	SetOutApplicationLogValidFlag(FALSE);
	SetFontCacheCharNum(1024);
	FL.LoadConfigFile();
	FL.LoadSkinConfig();
	SetAlwaysRunFlag(TRUE);
	SetWaitVSyncFlag(FL.WaitVSyncFlag ? TRUE : FALSE);
	SetGraphMode(FL.ScreenX, FL.ScreenY, FL.ColorBit);
	FL.LoadKeyConfigFile();
	ChangeWindowMode(TRUE);
	SetMainWindowText("TaikoManyGimmicks ver0.6.6α");
	SetDoubleStartValidFlag(TRUE);
	SetDragFileValidFlag(TRUE);
	SetWindowSize(FL.WindowSizeWidth, FL.WindowSizeHeight);
#endif

#ifndef __ANDROID__
	if (DxLib_Init() == -1) {
		MessageBox(NULL, TEXT("初期化失敗、ソフトを終了します"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return -1;
	}
#endif

	FL.LoadSkinImage();

	SongList = FL.LoadChartFiles("Songs");

	DrawRangeL = (FL.ScreenX / 2.f) - FL.DrawingOperationRangeWidth;
	DrawRangeR = (FL.ScreenX / 2.f) + FL.DrawingOperationRangeWidth;
	DrawRangeU = (FL.ScreenY / 2.f) - FL.DrawingOperationRangeHeight;
	DrawRangeD = (FL.ScreenY / 2.f) + FL.DrawingOperationRangeHeight;
	NoteDrawRate = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 1000;

	{
		LoadScreen = MakeScreen(500, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, TRUE);
		SetDrawScreen(LoadScreen);
		complex<double> N0 = complex<double>(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2);
		complex<double> f = PolarPosToComplexPos(N0.real() - 2, 0);

		complex<double> N1 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2));
		complex<double> N2 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2));
		complex<double> N3 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2));
		complex<double> N4 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2));
		double cnote_x = FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2;
		double cnote_y = FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2;
		double dnote_x = 500 - (FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2);
		double dnote_y = FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2;
		DrawModiGraphF(
				Note_ox + cnote_x + f.real() + N1.real(),
				Note_oy + cnote_y + f.imag() + N1.imag(),
				Note_ox + dnote_x - f.real() + N2.real(),
				Note_oy + dnote_y - f.imag() + N2.imag(),
				Note_ox + dnote_x - f.real() + N3.real(),
				Note_oy + dnote_y - f.imag() + N3.imag(),
				Note_ox + cnote_x + f.real() + N4.real(),
				Note_oy + cnote_y + f.imag() + N4.imag(),
				FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[6],
				TRUE
		);
		DrawModiGraphF(
				Note_ox + dnote_x + N1.real(),
				Note_oy + dnote_y + N1.imag(),
				Note_ox + dnote_x + N2.real(),
				Note_oy + dnote_y + N2.imag(),
				Note_ox + dnote_x + N3.real(),
				Note_oy + dnote_y + N3.imag(),
				Note_ox + dnote_x + N4.real(),
				Note_oy + dnote_y + N4.imag(),
				FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[7],
				TRUE
		);
		DrawModiGraphF(
				Note_ox + cnote_x + N1.real(),
				Note_oy + cnote_y + N1.imag(),
				Note_ox + cnote_x + N2.real(),
				Note_oy + cnote_y + N2.imag(),
				Note_ox + cnote_x + N3.real(),
				Note_oy + cnote_y + N3.imag(),
				Note_ox + cnote_x + N4.real(),
				Note_oy + cnote_y + N4.imag(),
				FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[5],
				TRUE
		);
	}

	thread processthread(ProcessThread);

#ifndef __ANDROID__
	if (FL.Error) {
		SetBeepFrequency(1000);
		PlayBeep();
	}
#endif

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawScreen(DX_SCREEN_BACK);

	int rand = 0;
	int MemFlameCount = 0;

	GetDateTime(&datetime);

	while (ProcessMessage() != -1) {
		if (FL.StableVSyncFlag) {
			while (MemNowFlameCount == GetNowFlameCount(CD.NowTime) && ProgressFlag == SecneType::Playing && !CD.ChartCreateMode) { printf("ラグ検知"); }
			MemNowFlameCount = GetNowFlameCount(CD.NowTime);
		}

#ifndef __ANDROID__
		if (FL.Error) {
			InitSoundMem();
			SetMainWindowText(GetErrorString("TaikoManyGimmicks ver0.7β", 95, 32).c_str());
		}
#endif
		if (ProgressFlag == SecneType::Title) {
			TitleDraw();
		}
		else if (ProgressFlag == SecneType::SongSelect) {
			rand = GetRand(105);
			SongSelectDraw();
		}
		else if (ProgressFlag == SecneType::ChartLoad) {
			ChartLoadDraw(rand);
		}
		else if (ProgressFlag == SecneType::Playing) {
			Note_ox = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2) + FL.JudgeOrigin + CD.Note_addx;
			Note_oy = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y + CD.Note_addy;
			PlayingDraw();
		}
		else if (ProgressFlag == SecneType::Result) {
			ResultDraw();
		}
		else if (ProgressFlag == SecneType::Config) {
			ConfigDraw();
		}
		else if (ProgressFlag == SecneType::End) { break; }

		if (FeedPal > 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, FeedPal);
			DrawBox(0,0,FL.ScreenX,FL.ScreenY,GetColor(0,0,0),TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		if (ProgressFlag != SecneType::ChartLoad && !FL.DebugScreen) {
			int FPS = GetFPS();
			int TPS = 1000 / (SubProcessTime.GetNowTime() / SubProcessCount);
			DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%d/%d/%d", FPS, GetRefreshRate(), FPS - GetRefreshRate());
			DrawFormatString(0, 16, GetColor(255, 255, 255), "TPS:%d", TPS);
		}
		
		if (NowLoadSongListDatas) {
			DrawString(0, 16, "曲リスト読み込み中...", GetColor(255, 255, 255));
			ScreenFlip();
			while (NowLoadSongListDatas) { if (ProcessMessage() == -1) { break; } }
		}
		ScreenFlip();
		ClearDrawScreen();
	}

	ProgressFlag = SecneType::End;

	processthread.join();

	InitGraph();
	InitFontToHandle();
	InitSoundMem();

#ifndef __ANDROID__
	DxLib_End();
#endif
	return 0;
}

inline void TitleDraw() {

	{
		string title = "TaikoManyGimmicks";
#ifndef __ANDROID__
		if (FL.Error) {
			title = GetErrorString(title, 100000, -50000);
		}
#endif
		double offset =
				FL.Font[(int)FontID::GAME_TITLE_TITLE].position == 0 ? 0 :
				FL.Font[(int)FontID::GAME_TITLE_TITLE].position == 1 ? GetDrawStringWidthToHandle(title.c_str(), strlenDx(title.c_str()), FL.Font[(int)FontID::GAME_TITLE_TITLE].handle) / 2.f :
				FL.Font[(int)FontID::GAME_TITLE_TITLE].position == 2 ? GetDrawStringWidthToHandle(title.c_str(), strlenDx(title.c_str()), FL.Font[(int)FontID::GAME_TITLE_TITLE].handle) : 0;
		DrawStringFToHandle(
				FL.Font[(int)FontID::GAME_TITLE_TITLE].x - (offset),
				FL.Font[(int)FontID::GAME_TITLE_TITLE].y,
				title.c_str(),
				GetColor(255, 255, 255),
				FL.Font[(int)FontID::GAME_TITLE_TITLE].handle,
				GetColor(0, 0, 0)
		);
	}
	{
		string title = "面をたたいてスタート！";
		
		if (datetime.Year == 2023 && datetime.Mon == 8 && datetime.Day == 17) {
			title = "1st Anniversary!";
		}

#ifndef __ANDROID__
		if (FL.Error) {
			title = GetErrorString(title, 100000, -50000);
		}
#endif
		double offset =
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].position == 0 ? 0 :
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].position == 1 ? GetDrawStringWidthToHandle(title.c_str(), strlenDx(title.c_str()), FL.Font[(int)FontID::GAME_TITLE_CAPTION].handle) / 2.f :
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].position == 2 ? GetDrawStringWidthToHandle(title.c_str(), strlenDx(title.c_str()), FL.Font[(int)FontID::GAME_TITLE_CAPTION].handle) : 0;
		DrawStringFToHandle(
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].x - (offset),
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].y,
				title.c_str(),
				GetColor(255, 255, 255),
				FL.Font[(int)FontID::GAME_TITLE_CAPTION].handle,
				GetColor(0, 0, 0)
		);
	}

	DrawString(0, 32, FL.Name.c_str(), GetColor(255, 255, 255));
	DrawFormatString(0, 48, GetColor(255, 255, 255), "%dx%d", FL.ScreenX, FL.ScreenY);
}

inline void SongSelectDraw() {
	DrawGraphF(
			FL.Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE].x - (FL.Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE].width / 2),
			FL.Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE].y - (FL.Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE].height / 2),
			FL.Image[(int)ImageID::GAME_SONGSELECT_BGSTAGE].handle[0],
			TRUE
	);

#ifdef __ANDROID__
	DrawBox(0,0,FL.ScreenX / 5,FL.ScreenY,GetColor(100,100,255),FALSE);
	DrawBox(FL.ScreenX / 5,0,FL.ScreenX - (FL.ScreenX / 5),FL.ScreenY,GetColor(255,100,100),FALSE);
	DrawBox(FL.ScreenX - (FL.ScreenX / 5),0,FL.ScreenX,FL.ScreenY,GetColor(100,100,255),FALSE);
#endif

	if (!CourseSelect) {
		static TimeData DeployMotion;
		if (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) {
			if (AnimationTime[(int)Animation::SongSelect_MotionTime] == DBL_MAX * -1) { AnimationTime[(int)Animation::SongSelect_MotionTime] = 0; }
			if (!SelectMotion.IsGetNowTime) { SelectMotion.start(); }
			if (abs(AnimationTime[(int)Animation::SongSelect_MotionTime]) - SelectMotion.GetNowTime() < 0) {
				SelectMotion.end();
				AnimationTime[(int)Animation::SongSelect_MotionTime] = 0;
			}
		}
		for (int k = -1; k < 2; ++k) {
			for (int i = 0, size = SongList.size(); i < size; ++i) {
				SongData& SDitem = SongList[i];
				double x = (32. * ((size * k) + i - SongSelector)) + FL.ScreenX / 2.;
				double y = (80. * ((size * k) + i - SongSelector)) + FL.ScreenY / 2.;

				if (!(y >= (FL.ScreenY / 2.) - 0.0001 && y <= (FL.ScreenY / 2.) + 0.0001)) {
					y += 80. * LTZF(y - (FL.ScreenY / 2.));
				}
				x += ((abs(AnimationTime[(int)Animation::SongSelect_MotionTime]) - SelectMotion.GetNowTime()) / 100.) * 32 * LTZF(AnimationTime[(int)Animation::SongSelect_MotionTime]);
				y += ((abs(AnimationTime[(int)Animation::SongSelect_MotionTime]) - SelectMotion.GetNowTime()) / 100.) * 80 * LTZF(AnimationTime[(int)Animation::SongSelect_MotionTime]);
				if (y > (FL.ScreenY / 2.) - 160 && y < (FL.ScreenY / 2.) + 160) {
					y += (((100 * ZF(AnimationTime[(int)Animation::SongSelect_MotionTime])) - fmod(SelectMotion.GetNowTime(), 100.)) / 100.) * 80 * LTZF(AnimationTime[(int)Animation::SongSelect_MotionTime]);
				}

				
				if (DrawRangeU < y && DrawRangeD > y && DrawRangeL < x && DrawRangeR > x) {
					DrawExtendGraphF(
						x - FL.Image[(int)ImageID::GAME_SONGSELECT_BOX].width / 2,
						y - FL.Image[(int)ImageID::GAME_SONGSELECT_BOX].height / 2,
						x + FL.Image[(int)ImageID::GAME_SONGSELECT_BOX].width / 2,
						y + FL.Image[(int)ImageID::GAME_SONGSELECT_BOX].height / 2,
						FL.Image[(int)ImageID::GAME_SONGSELECT_BOX].handle[0],
						TRUE
					);
					double offset =
						FL.Font[(int)FontID::GAME_SONGSELECT_TITLE].position == 0 ? 0 :
						FL.Font[(int)FontID::GAME_SONGSELECT_TITLE].position == 1 ? SDitem.titlestrlen / 2. :
						FL.Font[(int)FontID::GAME_SONGSELECT_TITLE].position == 2 ? SDitem.titlestrlen : 0;

					DrawStringFToHandle(
						x - offset,
						y - FL.Font[(int)FontID::GAME_SONGSELECT_TITLE].size / 2.,
#ifndef __ANDROID__
						FL.Error ? GetErrorString(SDitem.title, 100000, -50000).c_str() :
#endif
                        SDitem.title.c_str(),
						GetColor(255, 255, 255),
						FL.Font[(int)FontID::GAME_SONGSELECT_TITLE].handle,
						GetColor(0, 0, 0)
					);
				}
			}
		}
	}
	else {
		if (AnimationTime[(int)Animation::SongSelect_MotionTime] != 0) {
			if (AnimationTime[(int)Animation::SongSelect_MotionTime] == DBL_MAX * -1) { AnimationTime[(int)Animation::SongSelect_MotionTime] = 0; }
			if (!SelectMotion.IsGetNowTime) { SelectMotion.start(); }
			if (abs(AnimationTime[(int)Animation::SongSelect_MotionTime]) - SelectMotion.GetNowTime() < 0) {
				SelectMotion.end();
				AnimationTime[(int)Animation::SongSelect_MotionTime] = 0;
			}
		}
		SongData& SDitem = SongList[SongSelector];
		DrawString(0, 32, SDitem.title.c_str(), GetColor(255, 255, 255));
		DrawString(0, 48, SDitem.subtitle.c_str(), GetColor(255, 255, 255));
		DrawString(0, (CourseSelector * 16) + 80, "→", GetColor(255, 255, 255));
		string course[5]{ "Easy","Normal","Hard","Oni","Edit" };
		DrawString(16, 64, "Return To SongSelect", GetColor(255, 255, 255));
		for (int i = 0; i < 5; ++i) {
			if (SDitem.course[i]) {
				DrawString(16, (i * 16) + 80, course[i].c_str(), GetColor(255, 255, 255));
				DrawString(80, (i * 16) + 80, ("★×" + to_string(SDitem.level[i])).c_str(), GetColor(255, 255, 255));
			}
		}
	}
}

inline void ChartLoadDraw(int rand) {
	if (CD.NowChartConvert) {
		DrawString(0, 0, "ChartConvert...", GetColor(255, 255, 255));
	}
	else {
		DrawString(0, 0, ((CD.TJAChartFlag ? "TJAChart " : CD.TMGChartFlag ? "TMGChart " : "Empty ") + (string)" NowLoading...").c_str(), GetColor(255, 255, 255));
	}
	if (!LoadMotion.IsGetNowTime) { LoadMotion.start(); }
	string RandomTips[12]{
			"離れたところで判定させたい？そんな時はJUDGEDELAYっていう命令文を使おう！",
			"歌詞を表示させたい？そんな時はLYRICっていう命令文を使おう！",
			"Config.iniファイルでこのシミュの設定をいろいろ変えられます",
			"ノーツを斜めから飛ばしたりしてみたい？そんな時は複素数スクロールを使ってみよう！(readme.txtに詳しい説明あり)",
			"ノーツの出現をやりたい？そんな時はNOTESPAWNっていう命令文を使おう！",
			"ダミーノーツを使いたい？そんな時はDUMMYSTARTっていう命令文を使おう！",
			"とりあえず手を洗おう！風邪予防大事だからね！",
			"とりあえず手を洗おう！風邪予防大事だからね！",
			"とりあえず手を洗おう！風邪予防大事だからね！",
			"とりあえず手を洗おう！風邪予防大事だからね！",
			"これを見ている人はいるのでしょうか？",
			"これは豆知識ではない",
	};
	string RandomOmikuzi[6]{
			"今回のおみくじは...大吉！譜面制作がはかどる一日となるでしょう",
			"今回のおみくじは...中吉！プレイが少しうまくなる一日となるでしょう",
			"今回のおみくじは...吉！良くもないが悪くもない平和な一日となるでしょう",
			"今回のおみくじは...小吉！もしかしたらちょっと不幸なことが起こるかもしれない一日となるでしょう",
			"今回のおみくじは...凶！少し不幸なことが起こるかもしれない一日となるでしょう",
			"今回のおみくじは...大凶！運勢は最悪ですが努力をすれば必ず報われる時が来る一日となるでしょう",
	};
	if (rand < 100) {
		DrawString(0, 16, ("Tips：" + RandomTips[rand % 12]).c_str(), GetColor(255, 255, 255));
	}
	else {
		DrawString(0, 16, RandomOmikuzi[rand - 100].c_str(), GetColor(255, 255, 255));
	}
	

	DrawRotaGraph3F(
			125,
			720 - 125,
			250,
			FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2,
			0.5,
			0.5,
			DX_PI * (LoadMotion.GetNowTime() / 200),
			LoadScreen,
			TRUE
	);
	DrawRotaGraph3F(
			125,
			720 - 125,
			250,
			FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2,
			0.5,
			0.5,
			DX_PI * (LoadMotion.GetNowTime() / 200) + DX_PI / 2,
			LoadScreen,
			TRUE
	);

	if (CD.Title != "") {
		CD.TitleStrLen = GetDrawStringWidthToHandle(TEXT(CD.Title.c_str()), strlenDx(TEXT(CD.Title.c_str())), FL.Font[(int)FontID::GAME_PLAYING_TITLE].handle);
	}
	if (CD.SubTitle != "") {
		CD.SubTitleStrLen = GetDrawStringWidthToHandle(TEXT(CD.SubTitle.c_str()), strlenDx(TEXT(CD.SubTitle.c_str())), FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].handle);
	}
}

inline void PlayingDraw() {
	if (!LoadMotion.IsGetNowTime) { LoadMotion.end(); }
	double NoteBranchMotion = 0;

	OriginalDrawGraph(FL.Image[(int)(CD.SoulProgress > (CD.SoulMaxProgress * 0.8) ? ImageID::GAME_PLAYING_BGSTAGE_CLEAR : ImageID::GAME_PLAYING_BGSTAGE)]);


	OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG]);

#ifdef __ANDROID__
	DrawBox(20, 20, 40, 80, GetColor(255, 255, 255), TRUE);
	DrawBox(60, 20, 80, 80, GetColor(255, 255, 255), TRUE);

    if (FL.ChartCreateMode){
        DrawBox(100,0,200,100, GetColor(255,0,255),TRUE);
    }

	if (!FL.AutoPlayFlag) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawOvalAA(FL.ScreenX / 2, FL.ScreenY, (FL.ScreenX / 2) * 0.9, (FL.ScreenY / 2) * 0.95, 65536, GetColor(255, 255, 255), TRUE);
		DrawOvalAA(FL.ScreenX / 2, FL.ScreenY, (FL.ScreenX / 2) * 0.9 - 5, (FL.ScreenY / 2) * 0.95 - 5, 65536, GetColor(0, 0, 0), FALSE, 10);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#endif

	SetDrawArea(
		FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2),
		FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2),
		FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x + (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2),
		FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y + (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2)
	);

	switch (CD.NowBranchFlag) {
	case BranchType::Normal:
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL]);
		break;
	case BranchType::Expert:
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT]);
		break;
	case BranchType::Master:
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER]);
		break;
	}

	if (CD.NowBranchAnimation != ABranchType::Null) {
		double _one = ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::Branch]) / 100);
		double _motiony = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height * GetEasingRate(_one, EasingType1::Out, EasingType2::Sine);
		NoteBranchMotion = _motiony;
		if (_one > 1) {
			CD.NowBranchAnimation = ABranchType::Null;
		}
		switch (CD.NowBranchAnimation) {
		case ABranchType::Normal_Expert:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL], 0, 0, _motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT], 0, 0, -FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height + _motiony);
			break;
		case ABranchType::Normal_Master:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL], 0, 0, _motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER], 0, 0, -FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height + _motiony);
			break;
		case ABranchType::Expert_Normal:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT], 0, 0, -_motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL], 0, 0, FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - _motiony);
			break;
		case ABranchType::Expert_Master:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT], 0, 0, _motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER], 0, 0, -FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height + _motiony);
			break;
		case ABranchType::Master_Normal:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER], 0, 0, -_motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_NORMAL], 0, 0, FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - _motiony);
			break;
		case ABranchType::Master_Expert:
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_MASTER], 0, 0, -_motiony);
			OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG_EXPERT], 0, 0, FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - _motiony);
			break;
		}
	}

	SetDrawAreaFull();

	if (AnimationTime[(int)Animation::SCROLLFIELD_DON] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::ScrollField_Don]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::SCROLLFIELD_DON]) / 100)));
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_DON]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::SCROLLFIELD_KA] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::ScrollField_Ka]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::SCROLLFIELD_KA]) / 100)));
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_KA]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::SCROLLFIELD_ROLL] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::ScrollField_Roll]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::SCROLLFIELD_ROLL]) / 100)));
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_ROLL]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::SCROLLFIELD_HIT] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::ScrollField_Hit]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::SCROLLFIELD_HIT]) / 100)));
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_HIT]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (CD.GOGOFlag) {
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_GOGO]);
	}

	OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO]);

	OriginalDrawGraph(FL.Image[
		CourseSelector == 0 ? 
		(int)ImageID::GAME_PLAYING_COURSESYMBOL_EASY : 
		CourseSelector == 1 ? 
		(int)ImageID::GAME_PLAYING_COURSESYMBOL_NORMAL : 
		CourseSelector == 2 ? 
		(int)ImageID::GAME_PLAYING_COURSESYMBOL_HARD : 
		CourseSelector == 3 ? 
		(int)ImageID::GAME_PLAYING_COURSESYMBOL_ONI : 
		(int)ImageID::GAME_PLAYING_COURSESYMBOL_EDIT
	]);

	if (AnimationTime[(int)Animation::MINITAIKO_DON_L] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::MiniTaiko_Don]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * GetEasingRate((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::MINITAIKO_DON_L]) / 100, EasingType1::In, EasingType2::Quart)));
		DrawGraphF(
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].x - FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].width,
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].y - (FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].height / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].handle[0],
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::MINITAIKO_DON_R] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::MiniTaiko_Don]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * GetEasingRate((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::MINITAIKO_DON_R]) / 100, EasingType1::In, EasingType2::Quart)));
		DrawGraphF(
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].x,
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].y - (FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].height / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_DON].handle[1],
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::MINITAIKO_KA_L] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::MiniTaiko_Ka]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * GetEasingRate((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::MINITAIKO_KA_L]) / 100, EasingType1::In, EasingType2::Quart)));
		DrawGraphF(
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].x - FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].width,
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].y - (FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].height / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].handle[0],
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (AnimationTime[(int)Animation::MINITAIKO_KA_R] > AnimationTime[(int)Animation::NowTime] - FL.FlashMs[(int)FlashMsType::MiniTaiko_Ka]) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 * GetEasingRate((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::MINITAIKO_KA_R]) / 100, EasingType1::In, EasingType2::Quart)));
		DrawGraphF(
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].x,
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].y - (FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].height / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_MINITAIKO_KA].handle[1],
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawScore(CD.Score,
		AnimationTime[(int)Animation::SCOREBOUNCE] > AnimationTime[(int)Animation::NowTime] - 100 ?
		(FL.ScoreBounceRate - 1) * sin(DX_PI * ((CD.NowTime - AnimationTime[(int)Animation::SCOREBOUNCE]) / 100)) : 0
	);

	DrawCombo(CD.Combo,
		AnimationTime[(int)Animation::COMBOBOUNCE] > AnimationTime[(int)Animation::NowTime] - 200 ?
		(FL.ComboBounceRate - 1) * sin(DX_PI * ((CD.NowTime - AnimationTime[(int)Animation::COMBOBOUNCE]) / 200)) : 0
	);

	for (int i = 0; i < FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].dx; ++i) {
		int _max = (CD.SoulMaxProgress / (FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].dx + 1));
		if (_max == 0) { break; }
		if ((CD.SoulProgress / _max) > i) {
			DrawGraphF(
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].x + FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].width * i,
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].y,
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].handle[i + FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].dx],
				TRUE
			);
		}
		else {
			DrawGraphF(
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].x + FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].width * i,
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].y,
				FL.Image[(int)ImageID::GAME_PLAYING_SOULPROGRESSBAR].handle[i],
				TRUE
			);
		}
	}

	{
		int index =
			CD.SoulProgress >= CD.SoulMaxProgress ?
			(int)(AnimationTime[(int)Animation::NowTime] / FL.NextFlameMs[(int)NextFlameMsType::Soul]) % ((FL.Image[(int)ImageID::GAME_PLAYING_SOUL].dx * FL.Image[(int)ImageID::GAME_PLAYING_SOUL].dy) - 1) + 1 : 0;
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_SOUL], index);
	}

	OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_NOTE], 0, Note_ox, Note_oy);

	if (CD.GOGOFlag) {
		int _flame = ((AnimationTime[(int)Animation::NowTime] - AnimationTime[(int)Animation::GOGOTime]) / FL.NextFlameMs[(int)NextFlameMsType::GogoFire]);
		if (_flame != 0) { CD.GOGOFlame += _flame; AnimationTime[(int)Animation::GOGOTime] = AnimationTime[(int)Animation::NowTime]; }
		if (CD.GOGOFlame >= FL.Image[(int)ImageID::GAME_PLAYING_GOGOFIRE].dx * FL.Image[(int)ImageID::GAME_PLAYING_GOGOFIRE].dy) {
			CD.GOGOFlame -= FL.Image[(int)ImageID::GAME_PLAYING_GOGOFIRE].dx * FL.Image[(int)ImageID::GAME_PLAYING_GOGOFIRE].dy;
		}
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_GOGOFIRE], CD.GOGOFlame, Note_ox, Note_oy);
	}

	// HitNote(Down & Score)
	for (int i = 0; i < HitNoteLimiter; ++i) {
		HitNoteProcDisp_DownScore(HitNoteDatas[i]);
	}

	if (CD.ChartCreateMode && FL.GridScreen) {
		ClearDrawScreen();

		double _l = FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 16;
		int _color = GetColor(255, 255, 255);
		for (int i = (Note_ox / -_l), size = ((FL.ScreenX - Note_ox) / _l); i <= size; ++i) {
			DrawLineAA(
				Note_ox + (i * _l),
				0,
				Note_ox + (i * _l),
				FL.ScreenY,
				_color,
				3
			);
			DrawFormatStringF(
				Note_ox + (i * _l) + 3,
				Note_oy + 3,
				GetColor(255, 255, 255),
				"%d", i
			);
		}
		for (int i = (Note_oy / -_l), size = ((FL.ScreenY - Note_oy) / _l); i <= size; ++i) {
			DrawLineAA(
				0,
				Note_oy + (i * _l),
				FL.ScreenX,
				Note_oy + (i * _l),
				_color,
				3
			);
			DrawFormatStringF(
				Note_ox + 3,
				Note_oy + (i * _l) + 3,
				GetColor(255, 255, 255),
				"%d", i
			);
		}
		DrawLineAA(
			0,
			Note_oy,
			FL.ScreenX,
			Note_oy,
			GetColor(255, 0, 0),
			3
		);
		DrawLineAA(
			Note_ox,
			0,
			Note_ox,
			FL.ScreenY,
			GetColor(0,255,0),
			3
		);
	}

	if (!FL.SCROLLFieldExtensionFlag) {
		SetDrawArea(
			FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].x + (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].width / 2),
			FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].y + (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2)
		);
	}

	// Note
	if (CD.TJAChartFlag) {
		double Nowtime = FL.StableVSyncFlag ?
			VSYNCTIME * MemNowFlameCount : CD.ChartCreateMode ?
			CD.NowTime : FL.ChartCreateMode ?
			(((NOWTIME / 1000000.) - CD.GetNowTime) * FL.SongPlaySpeed) + CD.BarlineDatas[CD.BarlineIndex].AbsTime : ((NOWTIME / 1000000.) - CD.GetNowTime) * FL.SongPlaySpeed;

		double _NoteAddx = 0;
		for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
			ChartDrawProc(CD.TJANoteDatas[i], CD.NowBPM, _NoteAddx, CD.NowLyric);
		}
		CD.Note_addx = _NoteAddx;

		if (CD.ScrollType != SCROLLType::Normal) {
			double _addms = CD.TJANoteDatas[0].AbsTime;
			for (int i = 0, size = CD.TJANoteDatas.size(); i < size; ++i) {
				TJANoteData& item = CD.TJANoteDatas[i];
				if (item.BMFlag || i == 0) { _addms = item.AbsTime; item.BMTime = item.AbsTime; continue; }
				_addms += CD.TJANoteDatas[i - 1].RelaTime / (CD.NowBPM / CD.TJANoteDatas[i - 1].BPM) + CD.TJANoteDatas[i - 1].DelayTime / (CD.NowBPM / CD.TJANoteDatas[i - 1].BPM);
				item.BMTime = _addms;
			}
		}

		for (int i = CD.TJANoteDatas.size() - 1; i >= 0; --i) {
			TJANoteData& item = CD.TJANoteDatas[i];
			if (!item.Barline && (item.Type == '0' || item.Type == ',' || item.Type == '8')) { continue; }

			double note_x = GetTJANotePos()(item, Nowtime, CD.NowBPM, true);
			double note_y = GetTJANotePos()(item, Nowtime, CD.NowBPM, false);

			bool InRangeFlag = (DrawRangeL < Note_ox + note_x && DrawRangeR > Note_ox + note_x && DrawRangeU < Note_oy + note_y && DrawRangeD > Note_oy + note_y);
			bool SuddenDispFlag = item.SuddenFlag == 0 || item.SuddenFlag == 1;
			bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;

			if (!InRangeFlag && (item.Type == '5' || item.Type == '6')) {
				if (item.RollFlag) { InRangeFlag = true; }
				else {
					double _sx = GetTJANotePos()(CD.TJANoteDatas[i], Nowtime, CD.NowBPM, true);
					double _sy = GetTJANotePos()(CD.TJANoteDatas[i], Nowtime, CD.NowBPM, false);
					double _ex = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, true);
					double _ey = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, false);
					InRangeFlag =
						(DrawRangeL < Note_ox + _sx && DrawRangeR > Note_ox + _sx && DrawRangeU < Note_oy + _sy && DrawRangeD > Note_oy + _sy) ||
						(DrawRangeL < Note_ox + _ex && DrawRangeR > Note_ox + _ex && DrawRangeU < Note_oy + _ey && DrawRangeD > Note_oy + _ey);
				}
			}
			if (item.Type == '7' || item.Type == '9') {
				if (item.BalloonFlag == 1) { InRangeFlag = true; }
				else if (CD.TJANoteDatas[item.RollEndIndex].AbsTime < Nowtime) {
					note_x = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, true);
					note_y = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, false);
					InRangeFlag = (DrawRangeL < Note_ox + note_x && DrawRangeR > Note_ox + note_x && DrawRangeU < Note_oy + note_y && DrawRangeD > Note_oy + note_y);
				}
			}

			switch (CD.NowBranchAnimation) {
			case ABranchType::Normal_Expert:
				if (item.IsBranch == BranchType::Normal) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.IsBranch == BranchType::Expert) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Normal_Master:
				if (item.IsBranch == BranchType::Normal) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.IsBranch == BranchType::Master) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Expert_Normal:
				if (item.IsBranch == BranchType::Expert) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.IsBranch == BranchType::Normal) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Expert_Master:
				if (item.IsBranch == BranchType::Expert) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.IsBranch == BranchType::Master) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Master_Normal:
				if (item.IsBranch == BranchType::Master) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.IsBranch == BranchType::Normal) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Master_Expert:
				if (item.IsBranch == BranchType::Master) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.IsBranch == BranchType::Expert) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			}

			if (InRangeFlag && SuddenDispFlag && NoteBranch) {

				double NoteTheta = atan2(item.Scrolli, item.Scroll);

				complex<double> N0 = complex<double>(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2);

				complex<double> N1 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + NoteTheta);
				complex<double> N2 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + NoteTheta);
				complex<double> N3 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + NoteTheta);
				complex<double> N4 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + NoteTheta);
				
				if (item.Barline) {
					complex<double> B0 = complex<double>(0, FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height / 2);

					complex<double> B1 = PolarPosToComplexPos(GetMagnitude(B0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height / 2, 0) + NoteTheta);
					complex<double> B4 = PolarPosToComplexPos(GetMagnitude(B0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height / 2, 0) + NoteTheta);

					int BranchFlag = item.BranchStart ? (int)ImageID::GAME_PLAYING_BARLINE_BRANCH : (int)ImageID::GAME_PLAYING_BARLINE;

					DrawLineAA(
						Note_ox + note_x + B1.real(),
						Note_oy + note_y + B1.imag(),
						Note_ox + note_x + B4.real(),
						Note_oy + note_y + B4.imag(),
						GetColor(FL.Image[BranchFlag].r, FL.Image[BranchFlag].g, FL.Image[BranchFlag].b),
						FL.Image[BranchFlag].width
					);

					if (FL.DebugScreen) {
						complex<double> f = PolarPosToComplexPos(FL.Image[BranchFlag].width / 2, NoteTheta);
						DrawLineAA(
							Note_ox + note_x - f.real() + B1.real(),
							Note_oy + note_y - f.imag() + B1.imag(),
							Note_ox + note_x - f.real() + B4.real(),
							Note_oy + note_y - f.imag() + B4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real() + B1.real(),
							Note_oy + note_y - f.imag() + B1.imag(),
							Note_ox + note_x + f.real() + B1.real(),
							Note_oy + note_y + f.imag() + B1.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real() + B4.real(),
							Note_oy + note_y - f.imag() + B4.imag(),
							Note_ox + note_x + f.real() + B4.real(),
							Note_oy + note_y + f.imag() + B4.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x + f.real() + B4.real(),
							Note_oy + note_y + f.imag() + B4.imag(),
							Note_ox + note_x + f.real() + B1.real(),
							Note_oy + note_y + f.imag() + B1.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real(),
							Note_oy + note_y - f.imag(),
							Note_ox + note_x,
							Note_oy + note_y,
							GetColor(0, 0, 255),
							2
						);
					}
				}
				if (item.Type >= '1' && item.Type <= '4') {

					DrawModiGraphF(
						Note_ox + note_x + N1.real(),
						Note_oy + note_y + N1.imag(),
						Note_ox + note_x + N2.real(),
						Note_oy + note_y + N2.imag(),
						Note_ox + note_x + N3.real(),
						Note_oy + note_y + N3.imag(),
						Note_ox + note_x + N4.real(),
						Note_oy + note_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[item.Type - 48],
						TRUE
					);

					if (FL.DebugScreen) {
						DrawLineAA(
							Note_ox + note_x + N1.real(),
							Note_oy + note_y + N1.imag(),
							Note_ox + note_x + N4.real(),
							Note_oy + note_y + N4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x + N1.real(),
							Note_oy + note_y + N1.imag(),
							Note_ox + note_x + N2.real(),
							Note_oy + note_y + N2.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x + N2.real(),
							Note_oy + note_y + N2.imag(),
							Note_ox + note_x + N3.real(),
							Note_oy + note_y + N3.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x + N3.real(),
							Note_oy + note_y + N3.imag(),
							Note_ox + note_x + N4.real(),
							Note_oy + note_y + N4.imag(),
							GetColor(0, 255, 0),
							2
						);
						complex<double> f = PolarPosToComplexPos(N0.real() - 2, NoteTheta);
						DrawLineAA(
							Note_ox + note_x - f.real(),
							Note_oy + note_y - f.imag(),
							Note_ox + note_x,
							Note_oy + note_y,
							GetColor(0, 0, 255),
							2
						);
					}
				}
				if (item.Type == '5' || item.Type == '6') {
					double cnote_x = GetTJANotePos()(item, Nowtime, CD.NowBPM, true)
						, cnote_y = GetTJANotePos()(item, Nowtime, CD.NowBPM, false)
						, dnote_x = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, true)
						, dnote_y = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, false);

					bool BigRollFlag = item.Type == '6';

					double cdnote_x = dnote_x - cnote_x;
					double cdnote_y = dnote_y - cnote_y;

					double RollTheta = atan2(cdnote_y, cdnote_x);
					complex<double> f = PolarPosToComplexPos(N0.real() - 2, RollTheta);

					N1 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + RollTheta);
					N2 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + RollTheta);
					N3 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + RollTheta);
					N4 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) + RollTheta);

					DrawModiGraphF(
						Note_ox + cnote_x + f.real() + N1.real(),
						Note_oy + cnote_y + f.imag() + N1.imag(),
						Note_ox + dnote_x - f.real() + N2.real(),
						Note_oy + dnote_y - f.imag() + N2.imag(),
						Note_ox + dnote_x - f.real() + N3.real(),
						Note_oy + dnote_y - f.imag() + N3.imag(),
						Note_ox + cnote_x + f.real() + N4.real(),
						Note_oy + cnote_y + f.imag() + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 9 : 6],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + dnote_x + N1.real(),
						Note_oy + dnote_y + N1.imag(),
						Note_ox + dnote_x + N2.real(),
						Note_oy + dnote_y + N2.imag(),
						Note_ox + dnote_x + N3.real(),
						Note_oy + dnote_y + N3.imag(),
						Note_ox + dnote_x + N4.real(),
						Note_oy + dnote_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 10 : 7],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + cnote_x + N1.real(),
						Note_oy + cnote_y + N1.imag(),
						Note_ox + cnote_x + N2.real(),
						Note_oy + cnote_y + N2.imag(),
						Note_ox + cnote_x + N3.real(),
						Note_oy + cnote_y + N3.imag(),
						Note_ox + cnote_x + N4.real(),
						Note_oy + cnote_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 8 : 5],
						TRUE
					);
					
					if (FL.DebugScreen) {
						DrawLineAA(
							Note_ox + cnote_x + N1.real(),
							Note_oy + cnote_y + N1.imag(),
							Note_ox + cnote_x + N4.real(),
							Note_oy + cnote_y + N4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + cnote_x + N1.real(),
							Note_oy + cnote_y + N1.imag(),
							Note_ox + dnote_x + N2.real(),
							Note_oy + dnote_y + N2.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + dnote_x + N2.real(),
							Note_oy + dnote_y + N2.imag(),
							Note_ox + dnote_x + N3.real(),
							Note_oy + dnote_y + N3.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + dnote_x + N3.real(),
							Note_oy + dnote_y + N3.imag(),
							Note_ox + cnote_x + N4.real(),
							Note_oy + cnote_y + N4.imag(),
							GetColor(0, 255, 0),
							2
						);
						DrawLineAA(
							Note_ox + cnote_x - f.real(),
							Note_oy + cnote_y - f.imag(),
							Note_ox + cnote_x,
							Note_oy + cnote_y,
							GetColor(0, 0, 255),
							2
						);
					}
				}
				if (item.Type == '7' || item.Type == '9') {

					if (item.BalloonFlag == 1) {
						note_x = 0;
						note_y = 0;
					}
					if (item.BalloonFlag == 2) {
						note_x = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, true);
						note_y = GetTJANotePos()(CD.TJANoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, false);
					}

					bool KusudamaFlag = item.Type == '9';

					complex<double> f = PolarPosToComplexPos(N0.real() * 2, NoteTheta);

					DrawModiGraphF(
						Note_ox + note_x + N1.real(),
						Note_oy + note_y + N1.imag(),
						Note_ox + note_x + N2.real(),
						Note_oy + note_y + N2.imag(),
						Note_ox + note_x + N3.real(),
						Note_oy + note_y + N3.imag(),
						Note_ox + note_x + N4.real(),
						Note_oy + note_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[KusudamaFlag ? 13 : 11],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + note_x + f.real() + N1.real(),
						Note_oy + note_y + f.imag() + N1.imag(),
						Note_ox + note_x + f.real() + N2.real(),
						Note_oy + note_y + f.imag() + N2.imag(),
						Note_ox + note_x + f.real() + N3.real(),
						Note_oy + note_y + f.imag() + N3.imag(),
						Note_ox + note_x + f.real() + N4.real(),
						Note_oy + note_y + f.imag() + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[KusudamaFlag ? 14 : 12],
						TRUE
					);

					if (FL.DebugScreen) {
						SetDrawAddColor(0, 0, 0);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

						if (KusudamaFlag) {
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(255, 0, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N2.real(),
								Note_oy + note_y + N2.imag(),
								GetColor(0, 255, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N2.real(),
								Note_oy + note_y + N2.imag(),
								Note_ox + note_x + N3.real(),
								Note_oy + note_y + N3.imag(),
								GetColor(0, 255, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N3.real(),
								Note_oy + note_y + N3.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(0, 255, 0),
								2
							);
							f = PolarPosToComplexPos(N0.real() - 2, NoteTheta);
							DrawLineAA(
								Note_ox + note_x - f.real(),
								Note_oy + note_y - f.imag(),
								Note_ox + note_x,
								Note_oy + note_y,
								GetColor(0, 0, 255),
								2
							);
						}
						else {
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(255, 0, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + f.real() + N2.real(),
								Note_oy + note_y + f.imag() + N2.imag(),
								GetColor(0, 255, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + f.real() + N2.real(),
								Note_oy + note_y + f.imag() + N2.imag(),
								Note_ox + note_x + f.real() + N3.real(),
								Note_oy + note_y + f.imag() + N3.imag(),
								GetColor(0, 255, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + f.real() + N3.real(),
								Note_oy + note_y + f.imag() + N3.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(0, 255, 0),
								2
							);
							f = PolarPosToComplexPos(N0.real() - 2, NoteTheta);
							DrawLineAA(
								Note_ox + note_x - f.real(),
								Note_oy + note_y - f.imag(),
								Note_ox + note_x,
								Note_oy + note_y,
								GetColor(0, 0, 255),
								2
							);
						}
					}
				}
			}
		}
		if (CD.ChartCreateMode) {
			DrawStringandBG(Note_ox, Note_oy - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2), GetColor(255, 255, 255), GetColor(16, 16, 32), 192, "(" + to_string(CD.BarlineIndex) + "/" + to_string(CD.BarlineDatas.size() - 1) + ")");
		}
	}
	if (CD.TMGChartFlag) {
		double Nowtime = FL.StableVSyncFlag ?
			VSYNCTIME * MemNowFlameCount : CD.ChartCreateMode ?
			CD.NowTime : FL.ChartCreateMode ?
			(((NOWTIME / 1000000.) - CD.GetNowTime) * FL.SongPlaySpeed) + CD.BarlineDatas[CD.BarlineIndex].AbsTime : ((NOWTIME / 1000000.) - CD.GetNowTime) * FL.SongPlaySpeed;

		static int _nearstnote = 0;
		static bool MouseClick = false;
		static int MouseX = 0, MouseY = 0, InputType = 0;
		static int _mwindex = 0;
		bool _down = false;
		int _mx = 0, _my = 0;
		if (GetMouseInputLog2(NULL, &_mx, &_my, &InputType, TRUE) != -1) {
			_down = (InputType == MOUSE_INPUT_LOG_UP);
			if (_down) {
				MouseX = _mx;
				MouseY = _my;
				MouseClick = !MouseClick;
			}
		}

		double _NoteAddx = 0;
		double _NoteAddy = 0;
		for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
			ChartDrawProc(CD.TMGNoteDatas[i], CD.NowBPM, _NoteAddx, _NoteAddy, CD.NowLyric, CD.NowHiSpeed, i);
		}
		CD.Note_addx = _NoteAddx;
		CD.Note_addy = _NoteAddy;

		if (CD.ScrollType != SCROLLType::Normal) {
			double _addms = CD.TMGNoteDatas[0].AbsTime;
			for (int i = 0, size = CD.TMGNoteDatas.size(); i < size; ++i) {
				if (CD.TMGNoteDatas[i].BMFlag || i == 0) { _addms = CD.TMGNoteDatas[i].AbsTime; CD.TMGNoteDatas[i].BMTime = CD.TMGNoteDatas[i].AbsTime; continue; }
                _addms += (CD.TMGNoteDatas[i - 1].RelaTime + CD.TMGNoteDatas[i - 1].DelayTime) / ((CD.NowBPM * CD.NowHiSpeed) / (CD.TMGNoteDatas[i - 1].BPM * CD.TMGNoteDatas[i - 1].HiSpeed));
                CD.TMGNoteDatas[i].BMTime = _addms;
			}
		}

		for (int i = CD.TMGNoteDatas.size() - 1; i >= 0; --i) {
			TMGNoteData& item = CD.TMGNoteDatas[i];
			if (!item.Barline && (item.Type == '0' || item.Type == ',' || item.Type == '8')) { continue; }

			double note_x = GetTMGNotePos()(item, Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
			double note_y = GetTMGNotePos()(item, Nowtime, CD.NowBPM, CD.NowHiSpeed, false);

			bool InRangeFlag = (DrawRangeL < Note_ox + note_x && DrawRangeR > Note_ox + note_x && DrawRangeU < Note_oy + note_y && DrawRangeD > Note_oy + note_y) || (item.Barline || item.Type == '5' || item.Type == '6' || item.Type == '7' || item.Type == '9');
			bool SuddenDispFlag = item.SuddenFlag == 0 || item.SuddenFlag == 1;
			bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;
			bool NoteDispFlag = InRangeFlag && SuddenDispFlag && item.DisplayFlag;

			if (!InRangeFlag && (item.Type == '5' || item.Type == '6')) {
				if (item.RollFlag) { InRangeFlag = true; }
				else {
					double _sx = GetTMGNotePos()(CD.TMGNoteDatas[i], Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
					double _sy = GetTMGNotePos()(CD.TMGNoteDatas[i], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);
					double _ex = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
					double _ey = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);
					InRangeFlag = 
						(DrawRangeL < Note_ox + _sx && DrawRangeR > Note_ox + _sx && DrawRangeU < Note_oy + _sy && DrawRangeD > Note_oy + _sy) || 
						(DrawRangeL < Note_ox + _ex && DrawRangeR > Note_ox + _ex && DrawRangeU < Note_oy + _ey && DrawRangeD > Note_oy + _ey);
				}
			}
			if (item.Type == '7' || item.Type == '9') {
				if (item.BalloonFlag == 1) { InRangeFlag = true; }
				else if (CD.TMGNoteDatas[item.RollEndIndex].AbsTime < Nowtime) {
					note_x = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
					note_y = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);
					InRangeFlag = (DrawRangeL < Note_ox + note_x && DrawRangeR > Note_ox + note_x && DrawRangeU < Note_oy + note_y && DrawRangeD > Note_oy + note_y);
				}
			}

			switch (CD.NowBranchAnimation) {
			case ABranchType::Normal_Expert:
				if (item.DrawBranch == BranchType::Normal) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Expert) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Normal_Master:
				if (item.DrawBranch == BranchType::Normal) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Master) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Expert_Normal:
				if (item.DrawBranch == BranchType::Expert) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Normal) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Expert_Master:
				if (item.DrawBranch == BranchType::Expert) { NoteBranch = true; note_y += NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Master) { NoteBranch = true; note_y -= FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Master_Normal:
				if (item.DrawBranch == BranchType::Master) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Normal) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			case ABranchType::Master_Expert:
				if (item.DrawBranch == BranchType::Master) { NoteBranch = true; note_y -= NoteBranchMotion; }
				if (item.DrawBranch == BranchType::Expert) { NoteBranch = true; note_y += FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height - NoteBranchMotion; }
				break;
			}

			if (NoteDispFlag && NoteBranch) {

				if (MouseClick && _down) {
					double _tx = GetTMGNotePos()(CD.TMGNoteDatas[_nearstnote], Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
					double _ty = GetTMGNotePos()(CD.TMGNoteDatas[_nearstnote], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);
					double _m = sqrt(pow(MouseX - (Note_ox + note_x), 2) + pow(MouseY - (Note_oy + note_y), 2));
					if (_m < 64 && _m < sqrt(pow(MouseX - (Note_ox + _tx), 2) + pow(MouseY - (Note_oy + _ty), 2))) {
						_nearstnote = i;
					}
				}

				SetDrawAddColor((int)(item.ColorR - 255), (int)(item.ColorG - 255), (int)(item.ColorB - 255));
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, item.Alpha);

				double NoteTheta = (item.ScrollPolarFlag ? (DX_TWO_PI / item.ScrollDivValue) * item.ScrollTheta * -1 : atan2(item.Scrolli, item.Scroll)) + item.Angle;

				complex<double> N0 = complex<double>((FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width / 2) * item.Size, (FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height / 2) * item.Size);

				complex<double> N1 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + NoteTheta);
				complex<double> N2 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + NoteTheta);
				complex<double> N3 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + NoteTheta);
				complex<double> N4 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + NoteTheta);

				if (item.Barline) {
					complex<double> B0 = complex<double>(0, (item.BarlineSizeH / 2) * item.Size);

					complex<double> B1 = PolarPosToComplexPos(GetMagnitude(B0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height, 0) + NoteTheta);
					complex<double> B4 = PolarPosToComplexPos(GetMagnitude(B0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_BARLINE].height, 0) + NoteTheta);

					int BranchFlag = item.BranchStart ? (int)ImageID::GAME_PLAYING_BARLINE_BRANCH : (int)ImageID::GAME_PLAYING_BARLINE;

					DrawLineAA(
						Note_ox + note_x + B1.real(),
						Note_oy + note_y + B1.imag(),
						Note_ox + note_x + B4.real(),
						Note_oy + note_y + B4.imag(),
						GetColor(FL.Image[BranchFlag].r, FL.Image[BranchFlag].g, FL.Image[BranchFlag].b),
						item.BarlineSizeW * item.Size
					);

					if (FL.DebugScreen) {
						SetDrawAddColor(0, 0, 0);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						int _color = item.DummyFlag ? GetColor(128, 128, 128) : GetColor(0, 255, 0);
						complex<double> f = PolarPosToComplexPos(item.BarlineSizeW * item.Size / 2, NoteTheta);
						DrawLineAA(
							Note_ox + note_x - f.real() + B1.real(),
							Note_oy + note_y - f.imag() + B1.imag(),
							Note_ox + note_x - f.real() + B4.real(),
							Note_oy + note_y - f.imag() + B4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real() + B1.real(),
							Note_oy + note_y - f.imag() + B1.imag(),
							Note_ox + note_x + f.real() + B1.real(),
							Note_oy + note_y + f.imag() + B1.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real() + B4.real(),
							Note_oy + note_y - f.imag() + B4.imag(),
							Note_ox + note_x + f.real() + B4.real(),
							Note_oy + note_y + f.imag() + B4.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + note_x + f.real() + B4.real(),
							Note_oy + note_y + f.imag() + B4.imag(),
							Note_ox + note_x + f.real() + B1.real(),
							Note_oy + note_y + f.imag() + B1.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + note_x - f.real(),
							Note_oy + note_y - f.imag(),
							Note_ox + note_x,
							Note_oy + note_y,
							GetColor(0, 0, 255),
							2
						);
					}
					SetDrawAddColor((int)(item.ColorR - 255), (int)(item.ColorG - 255), (int)(item.ColorB - 255));
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, item.Alpha);
				}
				if (item.Type >= '1' && item.Type <= '4') {

					DrawModiGraphF(
						Note_ox + note_x + N1.real(),
						Note_oy + note_y + N1.imag(),
						Note_ox + note_x + N2.real(),
						Note_oy + note_y + N2.imag(),
						Note_ox + note_x + N3.real(),
						Note_oy + note_y + N3.imag(),
						Note_ox + note_x + N4.real(),
						Note_oy + note_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[item.Type - 48],
						TRUE
					);

					if (FL.DebugScreen) {
						SetDrawAddColor(0, 0, 0);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						int _color = item.DummyFlag ? GetColor(128, 128, 128) : GetColor(0, 255, 0);
						DrawLineAA(
							Note_ox + note_x + N1.real(),
							Note_oy + note_y + N1.imag(),
							Note_ox + note_x + N4.real(),
							Note_oy + note_y + N4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + note_x + N1.real(),
							Note_oy + note_y + N1.imag(),
							Note_ox + note_x + N2.real(),
							Note_oy + note_y + N2.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + note_x + N2.real(),
							Note_oy + note_y + N2.imag(),
							Note_ox + note_x + N3.real(),
							Note_oy + note_y + N3.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + note_x + N3.real(),
							Note_oy + note_y + N3.imag(),
							Note_ox + note_x + N4.real(),
							Note_oy + note_y + N4.imag(),
							_color,
							2
						);
						complex<double> f = PolarPosToComplexPos(N0.real(), NoteTheta);
						DrawLineAA(
							Note_ox + note_x - f.real(),
							Note_oy + note_y - f.imag(),
							Note_ox + note_x,
							Note_oy + note_y,
							GetColor(0, 0, 255),
							2
						);
					}
				}
				if (item.Type == '5' || item.Type == '6') {
					double cnote_x = GetTMGNotePos()(item, Nowtime, CD.NowBPM, CD.NowHiSpeed, true)
						, cnote_y = GetTMGNotePos()(item, Nowtime, CD.NowBPM, CD.NowHiSpeed, false)
						, dnote_x = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, true)
						, dnote_y = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);

					bool BigRollFlag = item.Type == '6';

					double cdnote_x = dnote_x - cnote_x;
					double cdnote_y = dnote_y - cnote_y;

					double RollTheta = atan2(cdnote_y, cdnote_x);
					complex<double> f = PolarPosToComplexPos(N0.real() - 2, RollTheta);

					N1 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + RollTheta);
					N2 = PolarPosToComplexPos(GetMagnitude(N0), atan2(-FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + RollTheta);
					N3 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + RollTheta);
					N4 = PolarPosToComplexPos(GetMagnitude(N0), atan2(FL.Image[(int)ImageID::GAME_PLAYING_NOTE].height, -FL.Image[(int)ImageID::GAME_PLAYING_NOTE].width) + RollTheta);

					DrawModiGraphF(
						Note_ox + cnote_x + f.real() + N1.real(),
						Note_oy + cnote_y + f.imag() + N1.imag(),
						Note_ox + dnote_x - f.real() + N2.real(),
						Note_oy + dnote_y - f.imag() + N2.imag(),
						Note_ox + dnote_x - f.real() + N3.real(),
						Note_oy + dnote_y - f.imag() + N3.imag(),
						Note_ox + cnote_x + f.real() + N4.real(),
						Note_oy + cnote_y + f.imag() + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 9 : 6],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + dnote_x + N1.real(),
						Note_oy + dnote_y + N1.imag(),
						Note_ox + dnote_x + N2.real(),
						Note_oy + dnote_y + N2.imag(),
						Note_ox + dnote_x + N3.real(),
						Note_oy + dnote_y + N3.imag(),
						Note_ox + dnote_x + N4.real(),
						Note_oy + dnote_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 10 : 7],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + cnote_x + N1.real(),
						Note_oy + cnote_y + N1.imag(),
						Note_ox + cnote_x + N2.real(),
						Note_oy + cnote_y + N2.imag(),
						Note_ox + cnote_x + N3.real(),
						Note_oy + cnote_y + N3.imag(),
						Note_ox + cnote_x + N4.real(),
						Note_oy + cnote_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[BigRollFlag ? 8 : 5],
						TRUE
					);
					
					if (FL.DebugScreen) {
						SetDrawAddColor(0, 0, 0);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						int _color = item.DummyFlag ? GetColor(128, 128, 128) : GetColor(0, 255, 0);

						DrawLineAA(
							Note_ox + cnote_x + N1.real(),
							Note_oy + cnote_y + N1.imag(),
							Note_ox + cnote_x + N4.real(),
							Note_oy + cnote_y + N4.imag(),
							GetColor(255, 0, 0),
							2
						);
						DrawLineAA(
							Note_ox + cnote_x + N1.real(),
							Note_oy + cnote_y + N1.imag(),
							Note_ox + dnote_x + N2.real(),
							Note_oy + dnote_y + N2.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + dnote_x + N2.real(),
							Note_oy + dnote_y + N2.imag(),
							Note_ox + dnote_x + N3.real(),
							Note_oy + dnote_y + N3.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + dnote_x + N3.real(),
							Note_oy + dnote_y + N3.imag(),
							Note_ox + cnote_x + N4.real(),
							Note_oy + cnote_y + N4.imag(),
							_color,
							2
						);
						DrawLineAA(
							Note_ox + cnote_x - f.real(),
							Note_oy + cnote_y - f.imag(),
							Note_ox + cnote_x,
							Note_oy + cnote_y,
							GetColor(0, 0, 255),
							2
						);
					}
				}
				if (item.Type == '7' || item.Type == '9') {

					if (item.BalloonFlag == 1) {
						note_x = 0;
						note_y = 0;
					}
					if (item.BalloonFlag == 2) {
						note_x = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, true);
						note_y = GetTMGNotePos()(CD.TMGNoteDatas[item.RollEndIndex], Nowtime, CD.NowBPM, CD.NowHiSpeed, false);
					}

					bool KusudamaFlag = item.Type == '9';

					complex<double> f = PolarPosToComplexPos(N0.real() * 2, NoteTheta);

					DrawModiGraphF(
						Note_ox + note_x + N1.real(),
						Note_oy + note_y + N1.imag(),
						Note_ox + note_x + N2.real(),
						Note_oy + note_y + N2.imag(),
						Note_ox + note_x + N3.real(),
						Note_oy + note_y + N3.imag(),
						Note_ox + note_x + N4.real(),
						Note_oy + note_y + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[KusudamaFlag ? 13 : 11],
						TRUE
					);
					DrawModiGraphF(
						Note_ox + note_x + f.real() + N1.real(),
						Note_oy + note_y + f.imag() + N1.imag(),
						Note_ox + note_x + f.real() + N2.real(),
						Note_oy + note_y + f.imag() + N2.imag(),
						Note_ox + note_x + f.real() + N3.real(),
						Note_oy + note_y + f.imag() + N3.imag(),
						Note_ox + note_x + f.real() + N4.real(),
						Note_oy + note_y + f.imag() + N4.imag(),
						FL.Image[(int)ImageID::GAME_PLAYING_NOTE].handle[KusudamaFlag ? 14 : 12],
						TRUE
					);
					
					if (FL.DebugScreen) {
						SetDrawAddColor(0, 0, 0);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						int _color = item.DummyFlag ? GetColor(128, 128, 128) : GetColor(0, 255, 0);

						if (KusudamaFlag) {
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(255, 0, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N2.real(),
								Note_oy + note_y + N2.imag(),
								_color,
								2
							);
							DrawLineAA(
								Note_ox + note_x + N2.real(),
								Note_oy + note_y + N2.imag(),
								Note_ox + note_x + N3.real(),
								Note_oy + note_y + N3.imag(),
								_color,
								2
							);
							DrawLineAA(
								Note_ox + note_x + N3.real(),
								Note_oy + note_y + N3.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								_color,
								2
							);
							f = PolarPosToComplexPos(N0.real() - 2, NoteTheta);
							DrawLineAA(
								Note_ox + note_x - f.real(),
								Note_oy + note_y - f.imag(),
								Note_ox + note_x,
								Note_oy + note_y,
								GetColor(0, 0, 255),
								2
							);
						}
						else {
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								GetColor(255, 0, 0),
								2
							);
							DrawLineAA(
								Note_ox + note_x + N1.real(),
								Note_oy + note_y + N1.imag(),
								Note_ox + note_x + f.real() + N2.real(),
								Note_oy + note_y + f.imag() + N2.imag(),
								_color,
								2
							);
							DrawLineAA(
								Note_ox + note_x + f.real() + N2.real(),
								Note_oy + note_y + f.imag() + N2.imag(),
								Note_ox + note_x + f.real() + N3.real(),
								Note_oy + note_y + f.imag() + N3.imag(),
								_color,
								2
							);
							DrawLineAA(
								Note_ox + note_x + f.real() + N3.real(),
								Note_oy + note_y + f.imag() + N3.imag(),
								Note_ox + note_x + N4.real(),
								Note_oy + note_y + N4.imag(),
								_color,
								2
							);
							f = PolarPosToComplexPos(N0.real() - 2, NoteTheta);
							DrawLineAA(
								Note_ox + note_x - f.real(),
								Note_oy + note_y - f.imag(),
								Note_ox + note_x,
								Note_oy + note_y,
								GetColor(0, 0, 255),
								2
							);
						}
					}
				}

				SetDrawAddColor(0, 0, 0);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
		if (FL.DebugScreen && MouseClick) {
			int _addindex = 0;
			static int linenum = 0;
			_mwindex -= GetMouseWheelRotVol();
			if (_mwindex < 0) { _mwindex = 0; }
			if (_mwindex > linenum - 16) { _mwindex = linenum - 16; }
			static const int drawstrlen = GetDrawStringWidth("AbsTime        : 10000.0000", strlenDx("AbsTime        : 10000.0000"));
			static const int drawVstrlen = 256;
			if (MouseX > FL.ScreenX - drawstrlen) { MouseX = FL.ScreenX - drawstrlen; }
			if (MouseY > FL.ScreenY - drawVstrlen) { MouseY = FL.ScreenY - drawVstrlen; }
			auto nline = [](int n) {return n * 16; };
			auto _linep = [&_addindex, &nline](const string& str) { if (_addindex - _mwindex >= 0 && _addindex - _mwindex < 16) { DrawStringandBG(MouseX, MouseY + nline(_addindex - _mwindex), GetColor(255, 255, 255), GetColor(16, 16, 32), 192, str); }; ++_addindex; };
			auto& linep = _linep;

			linep("AbsTime        : "+TS(CD.TMGNoteDatas[_nearstnote].AbsTime));
			linep("RelaTime       : "+TS(CD.TMGNoteDatas[_nearstnote].RelaTime));
			linep("BMTime         : "+TS(CD.TMGNoteDatas[_nearstnote].BMTime));
			linep("DelayTime      : "+TS(CD.TMGNoteDatas[_nearstnote].DelayTime));

			linep("Scroll         : "+TS(CD.TMGNoteDatas[_nearstnote].Scroll));
			linep("Scrolli        : "+TS(CD.TMGNoteDatas[_nearstnote].Scrolli));

			linep("ScrollRadius   : "+TS(CD.TMGNoteDatas[_nearstnote].ScrollRadius));
			linep("ScrollDivValue : "+TS(CD.TMGNoteDatas[_nearstnote].ScrollDivValue));
			linep("ScrollTheta    : "+TS(CD.TMGNoteDatas[_nearstnote].ScrollTheta));

			linep("HiSpeed        : "+TS(CD.TMGNoteDatas[_nearstnote].HiSpeed));

			linep("BPM            : "+TS(CD.TMGNoteDatas[_nearstnote].BPM));
			linep("BMFlag         : "+TS(CD.TMGNoteDatas[_nearstnote].BMFlag));
			linep("Beat           : "+TS(CD.TMGNoteDatas[_nearstnote].Beat));
			
			linep("DrawBranch     : "+TS((int)CD.TMGNoteDatas[_nearstnote].DrawBranch));
			linep("IsBranch       : "+TS((int)CD.TMGNoteDatas[_nearstnote].IsBranch));
			linep("BranchStart    : "+TS(CD.TMGNoteDatas[_nearstnote].BranchStart));
			linep("Section        : "+TS(CD.TMGNoteDatas[_nearstnote].Section));
			linep("LevelHold      : "+TS(CD.TMGNoteDatas[_nearstnote].LevelHold));

			linep("SuddenFlag     : "+TS(CD.TMGNoteDatas[_nearstnote].SuddenFlag));
			linep("SuddenSpawnMs  : "+TS(CD.TMGNoteDatas[_nearstnote].SuddenSpawnMs));
			linep("SuddenMotionMs : "+TS(CD.TMGNoteDatas[_nearstnote].SuddenMotionMs));
			
			linep("JPOSFlag       : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSFlag));
			linep("JPOSMotionMs   : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSMotionMs));
			linep("JPOSMotionPx_x : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSMotionPx_x));
			linep("JPOSMotionPx_y : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSMotionPx_y));
			linep("JPOSAddPx_x    : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSAddPx_x));
			linep("JPOSAddPx_y    : "+TS(CD.TMGNoteDatas[_nearstnote].JPOSAddPx_y));

			linep("JudgeDelayMs   : "+TS(CD.TMGNoteDatas[_nearstnote].JudgeDelayMs));
			linep("JudgeDelayX    : "+TS(CD.TMGNoteDatas[_nearstnote].JudgeDelayX));
			linep("JudgeDelayY    : "+TS(CD.TMGNoteDatas[_nearstnote].JudgeDelayY));
			
			linep("ColorR         : "+TS(CD.TMGNoteDatas[_nearstnote].ColorR));
			linep("ColorG         : "+TS(CD.TMGNoteDatas[_nearstnote].ColorG));
			linep("ColorB         : "+TS(CD.TMGNoteDatas[_nearstnote].ColorB));
			linep("Alpha          : "+TS(CD.TMGNoteDatas[_nearstnote].Alpha));

			linep("Size           : "+TS(CD.TMGNoteDatas[_nearstnote].Size));

			linep("Angle          : "+TS(CD.TMGNoteDatas[_nearstnote].Angle));

			linep("Barline        : "+TS(CD.TMGNoteDatas[_nearstnote].Barline));
			linep("BarlineSizeW   : "+TS(CD.TMGNoteDatas[_nearstnote].BarlineSizeW));
			linep("BarlineSizeH   : "+TS(CD.TMGNoteDatas[_nearstnote].BarlineSizeH));

			linep("GOGOFlag       : "+TS((int)CD.TMGNoteDatas[_nearstnote].GOGOFlag));

			linep("DummyFlag      : "+TS(CD.TMGNoteDatas[_nearstnote].DummyFlag));
			linep("HitFlag        : "+TS(CD.TMGNoteDatas[_nearstnote].HitFlag));
			linep("DisplayFlag    : "+TS(CD.TMGNoteDatas[_nearstnote].DisplayFlag));
			linep((string)"Type           : "+CD.TMGNoteDatas[_nearstnote].Type);

			linenum = _addindex;
		}
		if (CD.ChartCreateMode) {
			DrawStringandBG(Note_ox, Note_oy - (FL.Image[(int)ImageID::GAME_PLAYING_SCROLLFIELD_BG].height / 2), GetColor(255, 255, 255), GetColor(16, 16, 32), 192, "(" + to_string(CD.BarlineIndex) + "/" + to_string(CD.BarlineDatas.size() - 1) + ")");
		}
	}

	if (!FL.SCROLLFieldExtensionFlag) {
		SetDrawAreaFull();
	}

	// HitNote(Up & Judge & HitNote)
	for (int i = 0; i < HitNoteLimiter; ++i) {
		HitNoteProcDisp_HitNoteUpJudge(HitNoteDatas[i]);
	}

	DrawString(330, 333, CD.NowLyric.c_str(), GetColor(0, 0, 0));

	if (CD.TitleDispFlag) {
		double offset =
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].position == 0 ? 0 :
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].position == 1 ? CD.TitleStrLen / 2.f :
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].position == 2 ? CD.TitleStrLen : 0;
		DrawStringFToHandle(
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].x - (offset),
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].y,
			CD.Title.c_str(),
			GetColor(255, 255, 255),
			FL.Font[(int)FontID::GAME_PLAYING_TITLE].handle,
			GetColor(0, 0, 0)
		);
	}
	if (CD.SubTitleDispFlag) {
		double offset =
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].position == 0 ? 0 :
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].position == 1 ? CD.SubTitleStrLen / 2.f :
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].position == 2 ? CD.SubTitleStrLen : 0;
		DrawStringFToHandle(
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].x - (offset),
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].y,
			CD.SubTitle.c_str(),
			GetColor(255, 255, 255),
			FL.Font[(int)FontID::GAME_PLAYING_SUBTITLE].handle,
			GetColor(0, 0, 0)
		);
	}

	if (FL.DebugScreen) {
		auto nline = [](int n) {return n * 16; };
		DrawStringandBG(0, nline(0), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "NowTime:"+DTS(CD.NowTime,8)+"*"+TS(FL.SongPlaySpeed));
		DrawStringandBG(0, nline(1), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "NowBPM:"+DTS(CD.NowBPM,8));
		DrawStringandBG(0, nline(2), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "NowHiSpeed:"+DTS(CD.NowHiSpeed,8));
		DrawStringandBG(0, nline(3), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "NowActiveRollNum:(r:"+TS(CD.NowActiveRollNum)+",b:"+TS(CD.NowActiveBalloonNum)+")");
		DrawStringandBG(0, nline(4), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "Soul:("+TS(CD.SoulProgress)+"/"+TS(CD.SoulMaxProgress)+")");
		DrawStringandBG(0, nline(5), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "Score:(g:"+TS(CD.Good)+",o:"+TS(CD.Ok)+",b:"+TS(CD.Bad)+",l:"+TS(CD.Roll)+",r:"+DTS(CD.Rate,3)+")");
		DrawStringandBG(0, nline(6), GetColor(255, 255, 255), GetColor(128, 128, 128), 128, "BranchScore:(g:"+TS(CD.BranchGood)+",o:"+TS(CD.BranchOk)+",b:"+TS(CD.BranchBad)+",l:"+TS(CD.BranchRoll)+",r:"+DTS(CD.BranchRate,3)+")");
	}
	//DrawFormatString(0, 160, GetColor(255, 255, 255), "%lf", CD.NowTime);
}

inline void ResultDraw() {
	DrawGraphF(
			FL.Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR].x - (FL.Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR].width / 2),
			FL.Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR].y - (FL.Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR].height / 2),
			FL.Image[(int)ImageID::GAME_RESULT_BGSTAGE_CLEAR].handle[0],
			TRUE
	);
	DrawGraphF(
			FL.Image[(int)ImageID::GAME_RESULT_BOARD].x - (FL.Image[(int)ImageID::GAME_RESULT_BOARD].width / 2),
			FL.Image[(int)ImageID::GAME_RESULT_BOARD].y - (FL.Image[(int)ImageID::GAME_RESULT_BOARD].height / 2),
			FL.Image[(int)ImageID::GAME_RESULT_BOARD].handle[0],
			TRUE
	);

	
	DrawResultValue(CD.Good, "Good");
	DrawResultValue(CD.Ok, "Ok");
	DrawResultValue(CD.Bad, "Bad");
	DrawResultValue(CD.Roll, "Roll");
	DrawResultValue(CD.Combo, "MaxCombo");
	DrawResultValue(CD.Rate, "Rate");

}

inline void ConfigDraw() {
	string ConfigGenre[2]{
			"Config",
			"KeyConfig"
	};
	string Config[(int)ConfigID::ConfigCount]{
			"AutoPlayFlag",
			"ConstantSCROLL",
			"ConstantBPM",
			"AddSCROLL",
			"StableVSyncFlag",
			"JudgeGood",
			"JudgeOk",
			"JudgeBad",
			"JudgeOffset",
			"SongPlaySpeed",
			"SongBlankTime",
			"SongOffset",
			"SongVolume",
			"SEVolume",
			"ChartHeaderSound",
			"JiroBPMDelayFlag",
			"DrawingOperationRangeWidth",
			"DrawingOperationRangeHeight",
			"RandomRate",
			"RollSpeed",
			"ChartCreateMode",
			"GridScreen",
			"DebugScreen"
	};
#ifndef __ANDROID__
	string KeyConfig[(int)KeyConfigID::KeyConfigCount]{
			"LeftDonInput1",
			"LeftDonInput2",
			"LeftDonInput3",
			"LeftDonInput4",
			"LeftDonInput5",
			"LeftDonInput6",
			"LeftDonInput7",
			"LeftDonInput8",
			"LeftDonInput9",
			"LeftDonInput10",

			"LeftKaInput1",
			"LeftKaInput2",
			"LeftKaInput3",
			"LeftKaInput4",
			"LeftKaInput5",
			"LeftKaInput6",
			"LeftKaInput7",
			"LeftKaInput8",
			"LeftKaInput9",
			"LeftKaInput10",

			"RightDonInput1",
			"RightDonInput2",
			"RightDonInput3",
			"RightDonInput4",
			"RightDonInput5",
			"RightDonInput6",
			"RightDonInput7",
			"RightDonInput8",
			"RightDonInput9",
			"RightDonInput10",

			"RightKaInput1",
			"RightKaInput2",
			"RightKaInput3",
			"RightKaInput4",
			"RightKaInput5",
			"RightKaInput6",
			"RightKaInput7",
			"RightKaInput8",
			"RightKaInput9",
			"RightKaInput10",

			"AutoPlayFlagInput",
			"DebugScreenInput",
			"GridScreenInput",
			"ChartCreateModeInput"
	};


	DrawString(0, 360, "＞", GetColor(255, 255, 255));
	for (int i = 0; i < 2; ++i) {
		int y = (32 * (i - ConfigGenreSelector)) + 360;
		if (DrawRangeU < y && DrawRangeD > y) {
			DrawString(16, y, ConfigGenre[i].c_str(), GetColor(255, 255, 255));
		}
	}
	if (ConfigSelect) {
		DrawString(96, 360, "＞", GetColor(255, 255, 255));
		for (int i = 0; i < (int)ConfigID::ConfigCount; ++i) {
			int y = (32 * (i - ConfigSelector)) + 360;
			if (DrawRangeU < y && DrawRangeD > y) {
				DrawString(112, y, Config[i].c_str(), GetColor(255, 255, 255));
			}
		}
		if (InputHandle == GetActiveKeyInput()) {
			DrawString(352, 360, "＞", GetColor(255, 255, 255));
			DrawKeyInputString(368, 360, InputHandle);

			if (ConfigSelector == (int)ConfigID::AutoPlayFlag) {
				DrawString(368, 376, "オートプレイにするかどうかを指定", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::ConstantSCROLL) {
				DrawString(368, 376, "譜面の固定スクロール倍率を指定(0以外にすると譜面の#SCROLLは無視されます)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::ConstantBPM) {
				DrawString(368, 376, "譜面の固定BPMを指定(0以外にすると譜面の#BPMCHANGEは無視されます)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::AddSCROLL) {
				DrawString(368, 376, "譜面の乗算スクロール倍率を指定", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::StableVSyncFlag) {
				DrawString(368, 376, "描画時にVSync(垂直同期信号)のズレを補正するか指定(trueを指定するとさらに無駄のない高品質な描画をします)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::JudgeGood) {
				DrawString(368, 376, "良判定(±ms)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::JudgeOk) {
				DrawString(368, 376, "可判定(±ms)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::JudgeBad) {
				DrawString(368, 376, "不可判定(±ms)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::JudgeOffset) {
				DrawString(368, 376, "判定位置調整(ms)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::SongPlaySpeed) {
				DrawString(368, 376, "曲再生時の速度倍率を指定", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::SongBlankTime) {
				DrawString(368, 376, "曲再生前の空白時間(sec)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::SongOffset) {
				DrawString(368, 376, "曲の再生タイミング調整(sec)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::SongVolume) {
				DrawString(368, 376, "曲音量", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::SEVolume) {
				DrawString(368, 376, "効果音音量", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::ChartHeaderSound) {
				DrawString(368, 376, "tjaまたはtmgファイルのヘッダーにある音量指定を有効にするか", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::JiroBPMDelayFlag) {
				DrawString(368, 376, "tjaファイルのBPMCHANGE命令での音ズレ再現を有効にするか", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::DrawingOperationRangeWidth) {
				DrawString(368, 376, "動的なものを描画する時に画面中心から指定範囲内のみ描画する(±px & 横軸)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::DrawingOperationRangeHeight) {
				DrawString(368, 376, "動的なものを描画する時に画面中心から指定範囲内のみ描画する(±px & 縦軸)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::RandomRate) {
				DrawString(368, 376, "譜面のドンとカを入れ替える確率", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::RollSpeed) {
				DrawString(368, 376, "オート先生の連打をたたくスピード(打/sec)", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::ChartCreateMode) {
				DrawString(368, 376, "譜面制作モードにするかどうか", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::GridScreen) {
				DrawString(368, 376, "譜面制作モードが有効になっているときにグリッドを表示させるかどうか", GetColor(255, 255, 255));
			}
			if (ConfigSelector == (int)ConfigID::DebugScreen) {
				DrawString(368, 376, "デバッグ情報を表示させるかどうか", GetColor(255, 255, 255));
			}
		}
	}
	if (KeyConfigSelect) {
		DrawString(96, 360, "＞", GetColor(255, 255, 255));
		DrawString(274, 360, "＞", GetColor(255, 255, 255));
		for (int i = 0; i < (int)KeyConfigID::KeyConfigCount; ++i) {
			int y = (32 * (i - KeyConfigSelector)) + 360;
			if (DrawRangeU < y && DrawRangeD > y) {
				DrawString(112, y, KeyConfig[i].c_str(), GetColor(255, 255, 255));
				if (i >= 0 && i < 10) {
					DrawFormatString(290, y, GetColor(255, 255, 255), "%d", FL.KEY_INPUT_LD[i - 0]);
				}
				if (i >= 10 && i < 20) {
					DrawFormatString(290, y, GetColor(255, 255, 255), "%d", FL.KEY_INPUT_LK[i - 10]);
				}
				if (i >= 20 && i < 30) {
					DrawFormatString(290, y, GetColor(255, 255, 255), "%d", FL.KEY_INPUT_RD[i - 20]);
				}
				if (i >= 30 && i < 40) {
					DrawFormatString(290, y, GetColor(255, 255, 255), "%d", FL.KEY_INPUT_RK[i - 30]);
				}
				if (i >= 40 && i < 44) {
					DrawFormatString(290, y, GetColor(255, 255, 255), "%d", FL.QuickAccessKey[i - 40]);
				}
			}
		}
		if (KeySeter) {
			DrawString(290, 376, "↑何かのキーを押して設定してください", GetColor(255, 255, 255));

		}
	}
#else
// TODO: UIを自作する
#endif
}
