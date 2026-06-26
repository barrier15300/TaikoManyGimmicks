#include "ChartLib.h"

extern void OriginalDrawGraph(const ImageData& Data, const int& Index, const double& x, const double& y);
extern void DrawScore(const long& num, const double& bouncerate, const double& m_add_x, const double& m_add_y);

bool ChartNoteHitProc(TJANoteData& item, ChartData& cditem, bool& DonOrKaFlag) {
	bool GoodHitFlag = item.AbsTime - FL.JudgeGood < cditem.NowTime && item.AbsTime + FL.JudgeGood > cditem.NowTime;
	bool OkHitFlag = item.AbsTime - FL.JudgeOk < cditem.NowTime && item.AbsTime + FL.JudgeOk > cditem.NowTime;
	bool BadHitFlag = item.AbsTime - FL.JudgeBad < cditem.NowTime && item.AbsTime + FL.JudgeBad > cditem.NowTime;
	bool TypeFlag = DonOrKaFlag ? item.Type == '1' || item.Type == '3' : item.Type == '2' || item.Type == '4';
	bool BigNoteFlag = item.Type == '3' || item.Type == '4';
	bool NoteBranch = item.IsBranch == cditem.NowBranchFlag || item.IsBranch == BranchType::Null;
	bool breakflag = false;
	if (TypeFlag && NoteBranch && !item.HitFlag) {
		if (GoodHitFlag) {
			++cditem.Good; ++cditem.BranchGood;
			++cditem.Combo;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.Score += cditem.AddScore * 10;
			cditem.SoulProgress += cditem.SoulAddProgress;
			AnimationTime[(int)Animation::COMBOBOUNCE] = cditem.NowTime;
			AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
			AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
			AddHitNoteData(HitNoteData("good", item.Type, cditem.AddScore * 10));
			item.HitFlag = true;
			item.Type = '0';
			breakflag = true;
		}
		else if (OkHitFlag) {
			++cditem.Ok; ++cditem.BranchOk;
			++cditem.Combo;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.Score += (cditem.AddScore / 2) * 10;
			cditem.SoulProgress += cditem.SoulAddProgress / 2;
			AnimationTime[(int)Animation::COMBOBOUNCE] = cditem.NowTime;
			AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
			AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
			AddHitNoteData(HitNoteData("ok", item.Type, (cditem.AddScore / 2) * 10));
			item.HitFlag = true;
			item.Type = '0';
			breakflag = true;
		}
		else if (BadHitFlag) {
			++cditem.Bad; ++cditem.BranchBad;
			cditem.Combo = 0;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.SoulProgress -= cditem.SoulAddProgress * 2;
			AddHitNoteData(HitNoteData("bad", item.Type));
			item.HitFlag = true;
			item.Type = '0';
			breakflag = true;
		}
	}
	if (cditem.NowActiveRollNum > 0 && item.RollFlag) {
		AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = cditem.NowTime;
		AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
		AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
		bool BigRollFlag = item.Type == '6';
		AddHitNoteData(HitNoteData("roll", DonOrKaFlag ? BigRollFlag ? '3' : '1' : BigRollFlag ? '4' : '2'));
		++cditem.Roll; ++cditem.BranchRoll;
	}
	if (DonOrKaFlag && cditem.NowActiveBalloonNum > 0 && item.BalloonFlag == 1) {
		AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = cditem.NowTime;
		AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
		AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
		item.BalloonCount--;
		if (item.BalloonCount <= 0) {
			cditem.Roll = item.BalloonCount == 0 ? cditem.Roll + 1 : cditem.Roll;
			cditem.BranchRoll = item.BalloonCount == 0 ? cditem.BranchRoll + 1 : cditem.BranchRoll;
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle, DX_PLAYTYPE_BACK);
			item.BalloonFlag = 0;
			AddHitNoteData(HitNoteData("roll", '3'));
			item.Type = '0';
		}
		else {
			++cditem.Roll; ++cditem.BranchRoll;
		}
	}
	return breakflag;
}

bool ChartNoteHitProc(TMGNoteData& item, ChartData& cditem, bool& DonOrKaFlag) {
	bool GoodHitFlag = item.AbsTime - FL.JudgeGood < cditem.NowTime && item.AbsTime + FL.JudgeGood > cditem.NowTime;
	bool OkHitFlag = item.AbsTime - FL.JudgeOk < cditem.NowTime && item.AbsTime + FL.JudgeOk > cditem.NowTime;
	bool BadHitFlag = item.AbsTime - FL.JudgeBad < cditem.NowTime && item.AbsTime + FL.JudgeBad > cditem.NowTime;
	bool TypeFlag = DonOrKaFlag ? item.Type == '1' || item.Type == '3' : item.Type == '2' || item.Type == '4';
	bool BigNoteFlag = item.Type == '3' || item.Type == '4';
	bool NoteBranch = item.IsBranch == cditem.NowBranchFlag || item.IsBranch == BranchType::Null;
	bool breakflag = false;
	if (TypeFlag && NoteBranch && !item.HitFlag && !item.DummyFlag) {
		if (GoodHitFlag) {
			++cditem.Good; ++cditem.BranchGood;
			++cditem.Combo;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.Score += cditem.AddScore * 10;
			cditem.SoulProgress += cditem.SoulAddProgress;
			AnimationTime[(int)Animation::COMBOBOUNCE] = cditem.NowTime;
			AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
			AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
			item.HitFlag = true;
			AddHitNoteData(HitNoteData("good", item.Type, cditem.AddScore * 10));
			item.Type = '0';
			breakflag = true;
		}
		else if (OkHitFlag) {
			++cditem.Ok; ++cditem.BranchOk;
			++cditem.Combo;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.Score += (cditem.AddScore * 10) / 2;
			cditem.SoulProgress += cditem.SoulAddProgress / 2;
			AnimationTime[(int)Animation::COMBOBOUNCE] = cditem.NowTime;
			AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
			AnimationTime[(int)Animation::SCOREBOUNCE] = cditem.NowTime;
			item.HitFlag = true;
			AddHitNoteData(HitNoteData("ok", item.Type, (cditem.AddScore * 10) / 2));
			item.Type = '0';
			breakflag = true;
		}
		else if (BadHitFlag) {
			++cditem.Bad; ++cditem.BranchBad;
			cditem.Combo = 0;
			++cditem.HitNote; ++cditem.BranchHitNote;
			cditem.SoulProgress -= cditem.SoulAddProgress * 2;
			item.HitFlag = true;
			AddHitNoteData(HitNoteData("bad", item.Type));
			item.Type = '0';
			breakflag = true;
		}
	}
	if (cditem.NowActiveRollNum > 0 && item.RollFlag) {
		AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = cditem.NowTime;
		AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
		bool BigRollFlag = item.Type == '6';
		AddHitNoteData(HitNoteData("roll", BigRollFlag ? DonOrKaFlag ? '3' : '4' : DonOrKaFlag ? '1' : '2'));
		++cditem.Roll; ++cditem.BranchRoll;
	}
	if (DonOrKaFlag && cditem.NowActiveBalloonNum > 0 && item.BalloonFlag == 1) {
		AnimationTime[(int)Animation::SCROLLFIELD_ROLL] = cditem.NowTime;
		AnimationTime[(int)Animation::SCROLLFIELD_HIT] = cditem.NowTime;
		item.BalloonCount--;
		if (item.BalloonCount <= 0) {
			cditem.Roll = item.BalloonCount == 0 ? cditem.Roll + 1 : cditem.Roll;
			cditem.BranchRoll = item.BalloonCount == 0 ? cditem.BranchRoll + 1 : cditem.BranchRoll;
			PlaySoundMem(FL.Sound[(int)SoundID::GAME_SOUND_BALLOON].handle, DX_PLAYTYPE_BACK);
			item.BalloonFlag = 0;
			AddHitNoteData(HitNoteData("roll", '3'));
			item.Type = '0';
		}
		else {
			++cditem.Roll; ++cditem.BranchRoll;
		}
	}
	return breakflag;
}

void HitNoteProcDisp_DownScore(HitNoteData& item) {
	if (!item.ExplosionDown_Dispose && item.ExplosionDown_Flame != -1) {
		if (item.ExplosionDown_Ms == 0) { item.ExplosionDown_Ms = AnimationTime[(int)Animation::NowTime]; }
		int _flame = (AnimationTime[(int)Animation::NowTime] - item.ExplosionDown_Ms) / FL.NextFlameMs[(int)NextFlameMsType::Explosion_Down];
		item.ExplosionDown_Flame = _flame + (FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_DOWN].dx * item.ExplosionDown_Type);
		if (_flame >= FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_DOWN].dx) {
			item.ExplosionDown_Dispose = true;
			item.ExplosionDown_Flame = -1;
		}
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_DOWN], item.ExplosionDown_Flame, Note_ox, Note_oy);
	}
	if (!item.Score_Dispose) {
		item.Score_GetMs = item.Score_GetMs == -1 ? AnimationTime[(int)Animation::NowTime] : item.Score_GetMs;
		item.Score_Ms = AnimationTime[(int)Animation::NowTime] - item.Score_GetMs;
		if (item.Score_Ms >= 0 && item.Score_Ms < 100) {
			item.Score_X = (60 * (1 - GetEasingRate(item.Score_Ms / 100, EasingType1::Out, EasingType2::Back)));
			item.Score_Y = 42.5;
			item.Score_Alpha = 255 * GetEasingRate(item.Score_Ms / 100, EasingType1::Out, EasingType2::Cubic);
		}
		if (item.Score_Ms >= 200 && item.Score_Ms < 300) {
			item.Score_X = 0;
			item.Score_Y = (42.5 * (1 - GetEasingRate((item.Score_Ms - 200) / 100, EasingType1::In, EasingType2::Back)));
			item.Score_Alpha = 255 * (1 - GetEasingRate((item.Score_Ms - 200) / 100, EasingType1::In, EasingType2::Cubic));
		}
		if (item.Score_Ms >= 450) { item.Score_Dispose = true; }
		SetDrawAddColor(FL.AddScoreColor[0] - 255, FL.AddScoreColor[1] - 255, FL.AddScoreColor[2] - 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, item.Score_Alpha);
		DrawScore(item.Score_Num, 0, item.Score_X, -item.Score_Y);
		SetDrawAddColor(0, 0, 0);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void HitNoteProcDisp_HitNoteUpJudge(HitNoteData& item) {
	if (!item.HitNote_Dispose && FL.HitNoteDispFlag) {
		item.HitNote_GetMs = item.HitNote_GetMs == -1 ? AnimationTime[(int)Animation::NowTime] : item.HitNote_GetMs;
		item.HitNote_Ms = AnimationTime[(int)Animation::NowTime] - item.HitNote_GetMs;
		if (item.HitNote_Ms < 500) {
			double _one = (item.HitNote_Ms / 500);
			item.HitNote_X = _one * (FL.Image[(int)ImageID::GAME_PLAYING_SOUL].x - Note_ox);
			item.HitNote_Y = (sin(DX_PI * _one) * -FL.HitNoteHeight) + (_one * (FL.Image[(int)ImageID::GAME_PLAYING_SOUL].y - Note_oy));
		}
		if (item.HitNote_Ms >= 500 && item.HitNote_Ms < 1000) {
			item.HitNote_Yellow = ((item.HitNote_Ms - 500) / 250) > 1 ? 1 : ((item.HitNote_Ms - 500) / 250);
			item.HitNote_White = ((item.HitNote_Ms - 500) / 500);
			item.HitNote_Alpha = ((item.HitNote_Ms - 750) / 250);
			item.HitNote_X = (FL.Image[(int)ImageID::GAME_PLAYING_SOUL].x - Note_ox);
			item.HitNote_Y = (FL.Image[(int)ImageID::GAME_PLAYING_SOUL].y - Note_oy);
		}
		if (item.HitNote_Ms >= 1000) {
			item.HitNote_Dispose = true;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 + (-255 * item.HitNote_Alpha));
		SetDrawAddColor((int)(255 * item.HitNote_Yellow), (int)(255 * item.HitNote_Yellow), (int)((-511 * item.HitNote_Yellow) + (767 * item.HitNote_White)));
		OriginalDrawGraph(
			FL.Image[(int)ImageID::GAME_PLAYING_NOTE],
			item.HitNote_Type,
			Note_ox + item.HitNote_X,
			Note_oy + item.HitNote_Y
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawAddColor(0, 0, 0);
	}
	if (!item.ExplosionUp_Dispose && item.ExplosionUp_Flame != -1) {
		if (item.ExplosionUp_Ms == 0) { item.ExplosionUp_Ms = AnimationTime[(int)Animation::NowTime]; }
		int _flame = (AnimationTime[(int)Animation::NowTime] - item.ExplosionUp_Ms) / FL.NextFlameMs[(int)NextFlameMsType::Explosion_Up];
		item.ExplosionUp_Flame = _flame + (FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_UP].dx * item.ExplosionUp_Type);
		if (_flame >= FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_UP].dx) {
			item.ExplosionUp_Dispose = true;
			item.ExplosionUp_Flame = -1;
		}
		OriginalDrawGraph(FL.Image[(int)ImageID::GAME_PLAYING_EXPLOSION_UP], item.ExplosionUp_Flame, Note_ox, Note_oy);
	}
	if (!item.JudgeString_Dispose) {
		item.JudgeString_GetMs = item.JudgeString_GetMs == -1 ? AnimationTime[(int)Animation::NowTime] : item.JudgeString_GetMs;
		item.JudgeString_Ms = AnimationTime[(int)Animation::NowTime] - item.JudgeString_GetMs;
		if (item.JudgeString_Ms < 80) {
			item.JudgeString_Y = (1 - (item.JudgeString_Ms / 80)) * 17.5;
		}
		if (item.JudgeString_Ms > 180) {
			item.JudgeString_Alpha = 255 * (1 - ((item.JudgeString_Ms - 180) / 100));
		}
		if (item.JudgeString_Ms > 280) {
			item.JudgeString_Dispose = true;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, item.JudgeString_Alpha);
		OriginalDrawGraph(
			FL.Image[(int)ImageID::GAME_PLAYING_JUDGESTRING],
			item.JudgeString_Type,
			Note_ox,
			Note_oy - item.JudgeString_Y
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (item.ExplosionDown_Dispose && item.ExplosionUp_Dispose && item.JudgeString_Dispose && item.HitNote_Dispose && item.Score_Dispose) {
		item = HitNoteData();
	}
}

void ChartDrawProc(TJANoteData& item, double& _NowBPM, double& _JPOSAllpx, string& _NowLyric) {
	bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;
	
	// Sudden
	if (item.AbsTime - item.SuddenSpawnMs < CD.NowTime && item.SuddenFlag == 2 && NoteBranch) {
		item.SuddenFlag = 1;
		if (item.Type == '5' || item.Type == '6') {
			CD.TJANoteDatas[item.RollEndIndex].SuddenFlag = 1;
		}
	}
	if (item.AbsTime - item.SuddenMotionMs < CD.NowTime && item.SuddenFlag == 1 && NoteBranch) {
		item.SuddenFlag = 0;
	}

	// Jpos
	if (item.AbsTime < CD.NowTime && item.JPOSFlag == 2 && NoteBranch) {
		item.JPOSFlag = 1;
	}
	if (item.AbsTime < CD.NowTime && item.JPOSFlag == 1 && NoteBranch) {
		item.JPOSAddPx_x = ((CD.NowTime - item.AbsTime) / item.JPOSMotionMs) * item.JPOSMotionPx_x;
		if (item.AbsTime + item.JPOSMotionMs < CD.NowTime) {
			item.JPOSFlag = 0;
			item.JPOSAddPx_x = item.JPOSMotionPx_x;
		}
	}
	_JPOSAllpx += item.JPOSAddPx_x;

	// other
	if (item.AbsTime < CD.NowTime && NoteBranch) {
		_NowBPM = item.BPM;
		_NowLyric = item.Lyric;
		if (item.LevelHold) {
			CD.LevelHold = true;
		}
		if (item.GOGOFlag == GOGOTime::GOGOStart) {
			item.GOGOFlag = GOGOTime::Null;
			AnimationTime[(int)Animation::GOGOTime] = CD.NowTime;
			CD.GOGOFlag = true;
			CD.GOGOFlame = 0;
		}
		if (item.GOGOFlag == GOGOTime::GOGOEnd) {
			item.GOGOFlag = GOGOTime::Null;
			CD.GOGOFlag = false;
		}
	}
}
									 
void ChartDrawProc(TMGNoteData& item, double& _NowBPM, double& _JPOSAllpx_x, double& _JPOSAllpx_y, string& _NowLyric, double& _NowHiSpeed, const int& _index) {
	bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;

	// Command
	if (item.AbsTime - item.SuddenSpawnMs < CD.NowTime && item.SuddenFlag == 2 && NoteBranch) {
		item.SuddenFlag = 1;
		if (item.Type == '5' || item.Type == '6') {
			CD.TMGNoteDatas[item.RollEndIndex].SuddenFlag = 1;
		}
	}
	if (item.AbsTime - item.SuddenMotionMs < CD.NowTime && item.SuddenFlag == 1 && NoteBranch) {
		item.SuddenFlag = 0;
	}

	if (item.AbsTime < CD.NowTime && item.JPOSFlag == 2 && NoteBranch) {
		item.JPOSFlag = 1;
	}
	if (item.AbsTime < CD.NowTime && item.JPOSFlag == 1 && NoteBranch) {
		if (item.JPOSDefaultPosFlag) { item.JPOSMotionPx_x = _JPOSAllpx_x * -1; }
		if (item.JPOSDefaultPosFlag) { item.JPOSMotionPx_y = _JPOSAllpx_y * -1; }
		item.JPOSAddPx_x = ((CD.NowTime - item.AbsTime) / item.JPOSMotionMs) * item.JPOSMotionPx_x;
		item.JPOSAddPx_y = ((CD.NowTime - item.AbsTime) / item.JPOSMotionMs) * item.JPOSMotionPx_y;
		if (item.AbsTime + item.JPOSMotionMs < CD.NowTime) {
			item.JPOSFlag = 0;
			item.JPOSAddPx_x = item.JPOSMotionPx_x;
			item.JPOSAddPx_y = item.JPOSMotionPx_y;
		}
	}
	_JPOSAllpx_x += item.JPOSAddPx_x;
	_JPOSAllpx_y += item.JPOSAddPx_y;

	for (int j = 0, Nsize = item.NoteSpawn.size(); j < Nsize && NoteBranch; ++j) {
		if (item.NoteSpawn[j].Type == NoteSpawnType::Init || 
			item.NoteSpawn[j].Type == NoteSpawnType::Dispose_Spawn || 
			item.NoteSpawn[j].Type == NoteSpawnType::Dispose_Vanish) { 
			continue; 
		}
		if (item.AbsTime - item.NoteSpawn[j].Ms < CD.NowTime && item.NoteSpawn[j].Type == NoteSpawnType::Spawn) {
			for (int k = _index, size = CD.TMGNoteDatas.size(); k < size; ++k) {
				bool breakflag = false;
				for (int l = 0, nsize = CD.TMGNoteDatas[k].NoteSpawn.size(); l < nsize && k != _index; ++l) {
					if (CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Dispose_Spawn || 
						CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Spawn || 
						CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Init) {
						breakflag = true;
						break;
					}
				}
				if (breakflag) { break; }
				CD.TMGNoteDatas[k].DisplayFlag = true;
			}
			item.NoteSpawn[j].Type = NoteSpawnType::Dispose_Spawn;
		}
		if (item.AbsTime - item.NoteSpawn[j].Ms < CD.NowTime && item.NoteSpawn[j].Type == NoteSpawnType::Vanish) {
			for (int k = _index, size = CD.TMGNoteDatas.size(); k < size; ++k) {
				bool breakflag = false;
				for (int l = 0, nsize = CD.TMGNoteDatas[k].NoteSpawn.size(); l < nsize && k != _index; ++l) {
					if (CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Dispose_Vanish || 
						CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Vanish || 
						CD.TMGNoteDatas[k].NoteSpawn[l].Type == NoteSpawnType::Init) { 
						breakflag = true;
						break; 
					}
				}
				if (breakflag) { break; }
				CD.TMGNoteDatas[k].DisplayFlag = false;
			}
			item.NoteSpawn[j].Type = NoteSpawnType::Dispose_Vanish;
		}
	}

	GradationData& _Gradation = item.Gradation;
	if (item.AbsTime - _Gradation.OffsetMs < CD.NowTime && _Gradation.Flag == GradationType::Start && NoteBranch) {
		double _rate[(int)GradationParamType::Count];
		int index = 0;
		bool EndFlag = false;
		if (item.AbsTime - _Gradation.OffsetMs + _Gradation.MotionTime < CD.NowTime) {
			_Gradation.Flag = GradationType::Null; EndFlag = true;
		}
		for (int j = 0, GradSize = (int)GradationParamType::Count; j < GradSize; ++j) {
			_rate[j] = 0;
			double __one = ((CD.NowTime + _Gradation.OffsetMs - item.AbsTime) / _Gradation.MotionMs[j]);
			_Gradation.Phase[j] = EndFlag ? _Gradation.Param[j].size() > 1 ? _Gradation.Param[j].size() - 2 : 1 : (int)__one;
			_rate[j] = EndFlag ? 1 : GetEasingRate(fmod(__one, 1), _Gradation.MotionType, _Gradation.LineType);
		}
#define GSIZE (_Gradation.Param[index].size() > 1)
#define SUBSTITUTE (_Gradation.Param[index][_Gradation.Phase[index]])
#define ADD ((_Gradation.Param[index][_Gradation.Phase[index] + 1] - _Gradation.Param[index][_Gradation.Phase[index]]) * _rate[index])
		for (int j = _index, size = CD.TMGNoteDatas.size(); j < size; ++j) {
			TMGNoteData& __TMGNoteData = CD.TMGNoteDatas[j];
			GradationData& __Gradation = CD.TMGNoteDatas[j].Gradation;
			if (j != _index && __Gradation.Flag == GradationType::Start) { break; }
			if (__Gradation.Flag == GradationType::End) { break; }
			index = (int)GradationParamType::Scroll; if (GSIZE) { __TMGNoteData.Scroll = SUBSTITUTE; __TMGNoteData.Scroll += ADD; }
			index = (int)GradationParamType::Scrolli; if (GSIZE) { __TMGNoteData.Scrolli = SUBSTITUTE; __TMGNoteData.Scrolli += ADD; }
			index = (int)GradationParamType::ScrollRadius; if (GSIZE) { __TMGNoteData.ScrollRadius = SUBSTITUTE; __TMGNoteData.ScrollRadius += ADD; }
			index = (int)GradationParamType::ScrollDivValue; if (GSIZE) { __TMGNoteData.ScrollDivValue = SUBSTITUTE; __TMGNoteData.ScrollDivValue += ADD; }
			index = (int)GradationParamType::ScrollTheta; if (GSIZE) { __TMGNoteData.ScrollTheta = SUBSTITUTE; __TMGNoteData.ScrollTheta += ADD; }
			index = (int)GradationParamType::JudgeDelayMs; if (GSIZE) { __TMGNoteData.JudgeDelayMs = SUBSTITUTE; __TMGNoteData.JudgeDelayMs += ADD; }
			index = (int)GradationParamType::JudgeDelayX; if (GSIZE) { __TMGNoteData.JudgeDelayX = SUBSTITUTE; __TMGNoteData.JudgeDelayX += ADD; }
			index = (int)GradationParamType::JudgeDelayY; if (GSIZE) { __TMGNoteData.JudgeDelayY = SUBSTITUTE; __TMGNoteData.JudgeDelayY += ADD; }
			index = (int)GradationParamType::ColorR; if (GSIZE) { __TMGNoteData.ColorR = SUBSTITUTE; __TMGNoteData.ColorR += ADD; }
			index = (int)GradationParamType::ColorG; if (GSIZE) { __TMGNoteData.ColorG = SUBSTITUTE; __TMGNoteData.ColorG += ADD; }
			index = (int)GradationParamType::ColorB; if (GSIZE) { __TMGNoteData.ColorB = SUBSTITUTE; __TMGNoteData.ColorB += ADD; }
			index = (int)GradationParamType::Alpha; if (GSIZE) { __TMGNoteData.Alpha = SUBSTITUTE; __TMGNoteData.Alpha += ADD; }
			index = (int)GradationParamType::Size; if (GSIZE) { __TMGNoteData.Size = SUBSTITUTE; __TMGNoteData.Size += ADD; }
			index = (int)GradationParamType::Angle; if (GSIZE) { __TMGNoteData.Angle = SUBSTITUTE; __TMGNoteData.Angle += ADD; }
			index = (int)GradationParamType::BarlineSizeW; if (GSIZE) { __TMGNoteData.BarlineSizeW = SUBSTITUTE; __TMGNoteData.BarlineSizeW += ADD; }
			index = (int)GradationParamType::BarlineSizeH; if (GSIZE) { __TMGNoteData.BarlineSizeH = SUBSTITUTE; __TMGNoteData.BarlineSizeH += ADD; }
		}
	}

	
	if (item.AbsTime < CD.NowTime && NoteBranch) {
		_NowBPM = item.BPM;
		_NowLyric = item.Lyric;
		_NowHiSpeed = item.HiSpeed;
		if (item.LevelHold) {
			CD.LevelHold = true;
		}
		if (item.GOGOFlag == GOGOTime::GOGOStart) {
			item.GOGOFlag = GOGOTime::Null;
			AnimationTime[(int)Animation::GOGOTime] = CD.NowTime;
			CD.GOGOFlag = true;
			CD.GOGOFlame = 0;
		}
		if (item.GOGOFlag == GOGOTime::GOGOEnd) {
			item.GOGOFlag = GOGOTime::Null;
			CD.GOGOFlag = false;
		}
	}
}

void ChartSubProc(TJANoteData& item, int& _ActiveRollNum, int& _ActiveBalloonNum) {
	bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;
	if (item.AbsTime < CD.NowTime - FL.JudgeBad && item.Type >= '1' && item.Type <= '4' && !item.HitFlag && NoteBranch) {
		CD.Combo = 0;
		++CD.Bad; ++CD.BranchBad;
		++CD.HitNote; ++CD.BranchHitNote;
		CD.SoulProgress -= CD.SoulAddProgress * 2;
		item.HitFlag = true;
	}
	if (item.AbsTime < CD.NowTime && !item.HitFlag && NoteBranch) {
		if (item.Type == '5' || item.Type == '6') {
			bool NotRollFlag = false;
			if (CD.TJANoteDatas[item.RollEndIndex].AbsTime < CD.NowTime) {
				NotRollFlag = true;
				item.RollFlag = false;
			}
			else {
				item.RollFlag = true;
			}
			if (!NotRollFlag) {
				_ActiveRollNum++;
			}
		}
		if (item.Type == '7' || item.Type == '9') {
			bool NotBalloonFlag = false;
			if (CD.TJANoteDatas[item.RollEndIndex].AbsTime < CD.NowTime) {
				NotBalloonFlag = true;
				item.BalloonFlag = 2;
			}
			else {
				item.BalloonFlag = 1;
			}
			if (!NotBalloonFlag) {
				_ActiveBalloonNum++;
			}
		}
	}
	if (item.AbsTime < CD.NowTime) {
		item.BMFlag = true;
		if (item.Section) {
			item.Section = false;
			CD.BranchHitNote = 0;
			CD.BranchScore = 0;
			CD.BranchGood = 0;
			CD.BranchOk = 0;
			CD.BranchBad = 0;
			CD.BranchRoll = 0;
			CD.BranchRate = 0;
		}
	}
}

void ChartSubProc(TMGNoteData& item, int& _ActiveRollNum, int& _ActiveBalloonNum) {
	bool NoteBranch = item.IsBranch == CD.NowBranchFlag || item.IsBranch == BranchType::Null;
	if (item.AbsTime < CD.NowTime - FL.JudgeBad && item.Type >= '1' && item.Type <= '4' && !item.HitFlag && !item.DummyFlag && NoteBranch) {
		CD.Combo = 0;
		++CD.Bad; ++CD.BranchBad;
		++CD.HitNote; ++CD.BranchHitNote;
		CD.SoulProgress -= CD.SoulAddProgress * 2;
		item.HitFlag = true;
	}
	if (item.AbsTime < CD.NowTime && !item.HitFlag && !item.DummyFlag && NoteBranch) {
		if (item.Type == '5' || item.Type == '6') {
			bool NotRollFlag = false;
			if (CD.TMGNoteDatas[item.RollEndIndex].AbsTime < CD.NowTime) {
				NotRollFlag = true;
				item.RollFlag = false;
			}
			else {
				item.RollFlag = true;
			}
			if (!NotRollFlag) {
				_ActiveRollNum++;
			}
		}
		if (item.Type == '7' || item.Type == '9') {
			bool NotBalloonFlag = false;
			if (CD.TMGNoteDatas[item.RollEndIndex].AbsTime < CD.NowTime) {
				NotBalloonFlag = true;
				item.BalloonFlag = 2;
			}
			else {
				item.BalloonFlag = 1;
			}
			if (!NotBalloonFlag) {
				_ActiveBalloonNum++;
			}
		}
	}
	if (item.AbsTime < CD.NowTime) {
		item.BMFlag = true;
		if (item.Section) {
			item.Section = false;
			CD.BranchHitNote = 0;
			CD.BranchScore = 0;
			CD.BranchGood = 0;
			CD.BranchOk = 0;
			CD.BranchBad = 0;
			CD.BranchRoll = 0;
			CD.BranchRate = 0;
		}
	}

}