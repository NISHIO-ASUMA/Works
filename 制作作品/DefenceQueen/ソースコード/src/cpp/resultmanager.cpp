//=========================================================
//
// リザルトシーン管理処理 [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "resultmanager.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "ranking.h"
#include "fade.h"
#include <fstream>
#include <memory>

//=========================================================
// コンストラクタ
//=========================================================
CResultManager::CResultManager()
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CResultManager::~CResultManager()
{
	// 無し
}
//=========================================================
// インスタンス取得
//=========================================================
CResultManager* CResultManager::GetInstance(void)
{
	static CResultManager pResultManager;
	return &pResultManager;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultManager::Init(void)
{	
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_RESULTBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultManager::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CResultManager::Update(void)
{
	// 入力デバイスを取得
	CInputKeyboard* pInput = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJyoPad = CManager::GetInstance()->GetJoyPad();

	if (pInput == nullptr) return;
	if (pJyoPad == nullptr) return;

	// 決定キーが押された
	if ((pInput->GetTrigger(DIK_RETURN) || pJyoPad->GetTrigger(pJyoPad->JOYKEY_A)))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetInstance()->GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// ランキングシーン遷移
			pFade->SetFade(std::make_unique <CRanking>());
			return;
		}
	}
}
//=========================================================
// 描画処理
//=========================================================
void CResultManager::Draw(void)
{

}