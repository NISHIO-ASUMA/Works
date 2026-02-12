//=========================================================
//
// ゲームのオブジェクト管理処理 [ gamesceneobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "gamesceneobject.h"
#include "ui.h"
#include "meshfield.h"
#include "blockmanager.h"
#include "feedmanager.h"
#include "feed.h"
#include "gametime.h"
#include "enemy.h"
#include "array.h"
#include "workeruimanager.h"
#include "worker.h"
#include "workermanager.h"
#include "arraymanager.h"
#include "enemyspawner.h"
#include "arrayspawnmanager.h"
#include "topant.h"
#include "score.h"
#include "manager.h"
#include "input.h"
#include "queen.h"
#include "enemyspawnmanager.h"
#include "meshdome.h"
#include "enemymanager.h"
#include "gamemanager.h"
#include "gamewallmanager.h"
#include "meshcylinder.h"
#include "basemapfeed.h"
#include "eventarea.h"
#include "eventareamanager.h"
#include "jsonmanager.h"

//*********************************************************
// 静的メンバ変数
//*********************************************************
CGameSceneObject* CGameSceneObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace GAMEOBJECT
{
	const D3DXVECTOR3 TimerPos = { 1020.0f,60.0f,0.0f };	// タイマーの座標
	const D3DXVECTOR3 TopAntPos = { 0.0f, 0.0f, -450.0f };	// 操作アリの座標
	const D3DXVECTOR3 QueenPos = { 0.0f, 55.0f, 0.0f };		// 女王アリの座標
	constexpr const char* LoadName = "data/JSON/Gameobject.json"; // 読み込みjsonファイル名
};

//=========================================================
// コンストラクタ
//=========================================================
CGameSceneObject::CGameSceneObject() : 
m_pBlocks(nullptr),
m_pTimer(nullptr),
m_pScore(nullptr),
m_pArrayManager(nullptr),
m_pWorkUi(nullptr),
m_pQueen(nullptr),
m_pSpawn(nullptr),
m_pArraySpawn(nullptr),
m_pTopAnt(nullptr),
m_pEnemySpawnManager(nullptr),
m_pEnemyManager(nullptr),
m_pWallManager(nullptr)
{
}
//=========================================================
// デストラクタ
//=========================================================
CGameSceneObject::~CGameSceneObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameSceneObject::Init(void)
{
	// ゲームで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(GAMEOBJECT::LoadName);

	// イベント生成
	CEventAreaManager::GetInstance()->Init();

	// 各種ポインタクラスの生成
	CreatePointer();

	// スコア初期化
	m_pScore->DeleteScore();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
	// 餌管理クラスの破棄
	m_pFeed.reset();

	// 配置オブジェクトクラスの破棄
	m_pBlocks.reset();

	// 仲間アリの破棄
	m_pArrayManager.reset();

	// 働き司令塔アリの破棄
	m_pWorkerManager.reset();

	// 敵スポナー破棄
	m_pSpawn.reset();

	// 仲間スポナー破棄
	m_pArraySpawn.reset();

	// 敵のスポナー管理クラス
	m_pEnemySpawnManager.reset();

	// 敵管理の破棄
	m_pEnemyManager.reset();

	// 世界の壁の破棄
	m_pWallManager.reset();

	// ui処理
	m_pWorkUi.reset();

	// マップの餌
	m_pBasemapFeed.reset();

	// イベント破棄
	CEventAreaManager::GetInstance()->Uninit();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CGameSceneObject::Update(void)
{
	// 仲間アリ管理クラスの更新
	if (m_pArrayManager)
	{
		m_pArrayManager->Update();
	}

	// 働きアリuiクラスの更新
	if (m_pWorkUi)
	{
		m_pWorkUi->Update();
	}

	// 仲間アリのスポナー情報更新
	if (m_pArraySpawn)
	{
		m_pArraySpawn->Update();
	}

	// 働きアリの更新
	if (m_pWorkerManager)
	{
		m_pWorkerManager->Update();
	}

	// 敵の更新
	if (m_pEnemyManager)
	{
		m_pEnemyManager->Update();
	}

	// イベント更新
	CEventAreaManager::GetInstance()->Update();
}
//=========================================================
// 描画処理
//=========================================================
void CGameSceneObject::Draw(void)
{
#ifdef _DEBUG
	// アリ管理クラスのデバッグ情報
	if (m_pArrayManager)
	{
		m_pArrayManager->Draw();
	}

	// スポナー管理クラスのデバッグ情報
	if (m_pArraySpawn)
	{
		m_pArraySpawn->Draw();
	}
#endif // _DEBUG
}
//=========================================================
// 対象オブジェクトの破棄
//=========================================================
void CGameSceneObject::DeleteQueen(void)
{
	// nullじゃなかったら
	if (m_pQueen)
	{
		m_pQueen->Uninit();
		m_pQueen = nullptr;
	}

	// ゲームフラグを有効化し、進行を停止する
	CGameManager::GetInstance()->SetIsGameEnd(true);
}
//=========================================================
// インスタンス取得処理
//=========================================================
CGameSceneObject* CGameSceneObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameSceneObject;
	}

	// インスタンスを返す
	return m_pInstance;
}
//=========================================================
// ポインタの生成を行う関数
//=========================================================
void CGameSceneObject::CreatePointer(void)
{
	// トップアリ生成
	m_pTopAnt = CTopAnt::Create(GAMEOBJECT::TopAntPos, VECTOR3_NULL);

	// ブロックマネージャー生成
	m_pBlocks = std::make_unique<CBlockManager>();
	m_pBlocks->Init();

	// タイマー生成
	m_pTimer = CGameTime::Create(GAMEOBJECT::TimerPos);

	// マップの標準の餌生成
	m_pBasemapFeed = std::make_unique<CBaseMapFeed>();
	m_pBasemapFeed->Init();

	// 餌の管理クラスを生成
	m_pFeed = std::make_unique<CFeedManager>();
	m_pFeed->Init();

	// 仲間アリの大軍を生成
	m_pArrayManager = std::make_unique<CArrayManager>();
	m_pArrayManager->Init(m_pTopAnt);

	// 出現場所生成
	m_pArraySpawn = std::make_unique<CArraySpawnManager>();
	m_pArraySpawn->Init(m_pArrayManager.get());

	// 防衛対象のクイーン生成
	m_pQueen = CQueen::Create(GAMEOBJECT::QueenPos, VECTOR3_NULL);

	// 敵場所生成
	m_pEnemySpawnManager = std::make_unique<CEnemySpawnManager>();
	m_pEnemySpawnManager->Init();

	// スコア生成
	m_pScore = CScore::Create(VECTOR3_NULL);

	// 敵管理クラスの生成
	m_pEnemyManager = std::make_unique<CEnemyManager>();
	m_pEnemyManager->Init();

	// 世界の壁管理クラスの生成
	m_pWallManager = std::make_unique<CGameWallManager>();
	m_pWallManager->Init();
}