//=========================================================
//
// 開始シーンの管理処理 [ startscenemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "startscenemanager.h"
#include "manager.h"
#include "fade.h"
#include "title.h"
#include "sound.h"
#include <memory>
#include "ui.h"

//=========================================================
// コンストラクタ
//=========================================================
CStartSceneManager::CStartSceneManager() : m_nChangeCount(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CStartSceneManager::~CStartSceneManager()
{
	Uninit();
}
//=========================================================
// シングルトン取得
//=========================================================
CStartSceneManager* CStartSceneManager::GetInstance()
{
	static CStartSceneManager Instance;
	return &Instance;
};
//=========================================================
// 初期化処理
//=========================================================
HRESULT CStartSceneManager::Init(void)
{
	// ロゴUI生成
	CUi::Create(CENTERPOS, 0, 300, 300, "startgameicon.png", false,true,170);

	// サウンド再生
	auto Sound = CManager::GetInstance()->GetSound();
	if (Sound) Sound->Play(CSound::SOUND_LABEL_START);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CStartSceneManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CStartSceneManager::Update(void)
{
	// カウントを加算
	m_nChangeCount++;

	// 最大値より大きくなったら
	if (m_nChangeCount >= Config::MAX_CHANGECOUNT)
	{
		// フェードインスタンス取得
		auto Fade = CManager::GetInstance()->GetFade();

		if (Fade != nullptr)
		{
			// タイトルにシーンを切り替える
			Fade->SetFade(std::make_unique<CTitle>());
			return;
		}
	}
}