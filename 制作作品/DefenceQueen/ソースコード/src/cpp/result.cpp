//=========================================================
//
// リザルトシーン処理 [ result.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "result.h"
#include "resultmanager.h"
#include "manager.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "resultobject.h"
#include "sound.h"
#include <memory>

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{

}
//=========================================================
// デストラクタ
//=========================================================
CResult::~CResult()
{
	// 無し
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResult::Init(void)
{
	// オブジェクト初期化
	CResultObject::GetInstance()->Init();

	// サウンド取得
	auto Sound = CManager::GetInstance()->GetSound();
	if (Sound == nullptr) return E_FAIL;

	// サウンド再生
	Sound->Play(CSound::SOUND_LABEL_RESULTBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResult::Uninit(void)
{
	// インスタンス破棄
	CResultObject::GetInstance()->Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CResult::Update(void)
{
	// キー入力時
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) || 
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_A) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// ランキング画面遷移
		auto fade = CManager::GetInstance()->GetFade();
		fade->SetFade(std::make_unique<CRanking>());
		return;
	}
}
//=========================================================
// 描画処理
//=========================================================
void CResult::Draw(void)
{
	
}