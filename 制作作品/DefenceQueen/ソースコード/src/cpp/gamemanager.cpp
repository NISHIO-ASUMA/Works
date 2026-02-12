//=========================================================
//
// ゲーム管理処理 [ gamemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "gamemanager.h"
#include "manager.h"
#include "debugproc.h"
#include "result.h"
#include "sound.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "blockmanager.h"
#include "gamesceneobject.h"
#include "loseresult.h"

//=========================================================
// インスタンス取得
//=========================================================
CGameManager* CGameManager::GetInstance(void)
{
	static CGameManager pInstance;
	return &pInstance;
}
//=========================================================
// コンストラクタ
//=========================================================
CGameManager::CGameManager() : m_isGameEnd(false)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CGameManager::~CGameManager()
{
	// 終了処理
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_GAMEBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CGameManager::Update(void)
{
#ifdef NDEBUG
	// 画面遷移
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_2))
	{
		// リザルト画面に遷移
		CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CResult>());
		return;
	}
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CGameManager::Draw(void)
{

}