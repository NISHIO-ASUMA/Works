//=========================================================
//
// チュートリアル管理処理 [ tutorialmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//********************************************************
// インクルードファイル
//********************************************************
#include "tutorialmanager.h"
#include "tutorialui.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "ui.h"
#include "sound.h"
#include "input.h"
#include "game.h"

//=========================================================
// インスタンス取得
//=========================================================
CTutorialManager* CTutorialManager::GetInstance(void)
{
	// 生成されたインスタンス
	static CTutorialManager pTutoManager;
	return &pTutoManager;
}
//=========================================================
// コンストラクタ
//=========================================================
CTutorialManager::CTutorialManager()
{

}
//=========================================================
// デストラクタ
//=========================================================
CTutorialManager::~CTutorialManager()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialManager::Init(void)
{
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CTutorialManager::Update(void)
{
	// 画面遷移キー
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_TAB) || 
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// ゲームシーンに遷移する
		CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CGame>());

		return;
	}
}
//=========================================================
// 描画処理
//=========================================================
void CTutorialManager::Draw(void)
{

}