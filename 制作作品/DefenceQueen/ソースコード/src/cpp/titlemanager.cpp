//=========================================================
//
// タイトル管理処理 [ titlemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "titlemanager.h"
#include "manager.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include <memory>

//=========================================================
// コンストラクタ
//=========================================================
CTitleManager::CTitleManager()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTitleManager::~CTitleManager()
{

}
//==========================================================
//インスタンス取得
//==========================================================
CTitleManager* CTitleManager::GetInstance(void)
{
	// インスタンスを返す
	static CTitleManager pTitleManager;
	return &pTitleManager;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_TITLE_BGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleManager::Uninit(void)
{
	// 無し
}
//=========================================================
// 更新処理
//=========================================================
void CTitleManager::Update(void)
{
	// 入力デバイス取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();

	// 取得失敗時
	if (pKey == nullptr) return;
	if (pJoyPad == nullptr) return;

	// フェード取得
	CFade* pFade = CManager::GetInstance()->GetFade();
	if (pFade == nullptr) return;

	// キー入力時の遷移
	if ((pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_START)))
	{
		// サウンド取得
		CSound* pSound = CManager::GetInstance()->GetSound();
		if (pSound == nullptr) return;

		// チュートリアルシーンに遷移
		pFade->SetFade(std::make_unique<CTutorial>());	

		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_TITLEENTER);
		return;
	}
}
//==========================================================
// 描画処理
//==========================================================
void CTitleManager::Draw(void)
{

}