#pragma once
#include "System/include.h"
#include "sub.h"

bool ChartNoteHitProc(TJANoteData& item, ChartData& cditem, bool& DonOrKaFlag);

bool ChartNoteHitProc(TMGNoteData& item, ChartData& cditem, bool& DonOrKaFlag);

void HitNoteProcDisp_DownScore(HitNoteData& item);

void HitNoteProcDisp_HitNoteUpJudge(HitNoteData& item);

void ChartDrawProc(TJANoteData& item, double& _NowBPM, double& _JPOSAllpx, string& _NowLyric);

void ChartDrawProc(TMGNoteData& item, double& _NowBPM, double& _JPOSAllpx_x, double& _JPOSAllpx_y, string& _NowLyric, double& _NowHiSpeed, const int& _index);

void ChartSubProc(TJANoteData& item, int& _ActiveRollNum, int& _ActiveBalloonNum);

void ChartSubProc(TMGNoteData& item, int& _ActiveRollNum, int& _ActiveBalloonNum);

