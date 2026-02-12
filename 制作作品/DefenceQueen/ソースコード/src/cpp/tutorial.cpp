//=========================================================
//
// チュートリアル処理 [ tutorial.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "tutorial.h"
#include "tutorialmanager.h"
#include "manager.h"
#include "sound.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "tutorialobject.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CTutorial::CTutorial() : CScene(CScene::MODE_TUTORIAL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTutorial::~CTutorial()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorial::Init(void)
{	
	// カメラ初期化
	auto Camera = CManager::GetInstance()->GetCamera();
	Camera->Init();

	// チュートリアルマネージャー初期化
	CTutorialManager::GetInstance()->Init();

	// チュートリアルオブジェクト初期化
	CTutorialObject::GetInstance()->Init();

	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_TUTORIALBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorial::Uninit(void)
{
	// チュートリアルマネージャー終了
	CTutorialManager::GetInstance()->Uninit();

	// チュートリアルオブジェクト終了
	CTutorialObject::GetInstance()->Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CTutorial::Update(void)
{
	// チュートリアルマネージャー更新
	CTutorialManager::GetInstance()->Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutorial::Draw(void)
{
	
}