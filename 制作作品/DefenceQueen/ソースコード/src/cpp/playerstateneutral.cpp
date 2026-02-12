//=========================================================
//
// ニュートラル状態管理処理 [ playerstateneutral.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "playerstateneutral.h"
#include "manager.h"
#include "input.h"
#include "playerstatemove.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayerStateNeutral::CPlayerStateNeutral()
{
	// IDセット
	SetID(ID_NEUTRAL);
}
//=========================================================
// デストラクタ
//=========================================================
CPlayerStateNeutral::~CPlayerStateNeutral()
{

}
//==========================================================
// 開始関数
//==========================================================
void CPlayerStateNeutral::OnStart()
{
	// モーションセット
	// m_pPlayer->GetMotion()->SetMotion(CPlayer::MOTION_NEUTRAL, true, 10);
}
//==========================================================
// 更新関数
//==========================================================
void CPlayerStateNeutral::OnUpdate()
{
	//// キー入力情報を取得
	//auto pKey = CManager::GetInstance()->GetInputKeyboard();
	//auto pPad = CManager::GetInstance()->GetJoyPad();

	//// キー入力があったなら
	//if (m_pPlayer->isMoveInputKey(pKey) || m_pPlayer->isMovePadButton(pPad))
	//{
	//	// 移動状態に変更
	//	// m_pPlayer->ChangeState(new CPlayerStateMove(), ID_MOVE);
	//	return;
	//}
}
//==========================================================
// 終了関数
//==========================================================
void CPlayerStateNeutral::OnExit()
{

}