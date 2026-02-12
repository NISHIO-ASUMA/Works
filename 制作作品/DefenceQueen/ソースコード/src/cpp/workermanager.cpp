//=========================================================
//
// 働きアリの複数管理処理 [ workermanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "workermanager.h"
#include "worker.h"
#include "gamesceneobject.h"
#include "feedmanager.h"
#include "feed.h"

//*********************************************************
// 定数宣言空間
//*********************************************************
//namespace BASEDATA
//{
//	const D3DXVECTOR3 CreatePos[3]
//	{
//		{VECTOR3_NULL},
//		{-60.0f,0.0f,10.0f},
//		{60.0f,0.0f,10.0f},
//	};
//};

//=========================================================
// コンストラクタ
//=========================================================
CWorkerManager::CWorkerManager() : m_pWorker{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CWorkerManager::~CWorkerManager()
{
	// 終了処理
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorkerManager::Init(void)
{
	// 配列のクリア
	m_pWorker.clear();

	// 構造体変数を使用
	Config config = {};

	// 生成するサイズのメモリをあらかじめ確保する
	m_pWorker.reserve(config.NUM_WOKER);

	// 動的ポインタを生成
	for (int nCnt = 0; nCnt < config.NUM_WOKER; nCnt++)
	{
		// 生成
		auto pWork = CWorker::Create(VECTOR3_NULL, VECTOR3_NULL);

		// 配列に追加
		m_pWorker.push_back(pWork);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWorkerManager::Uninit(void)
{
	// 配列クリア
	m_pWorker.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CWorkerManager::Update(void)
{
#if 1
	// 餌取得
	auto pFeedMgr = CGameSceneObject::GetInstance()->GetFeedManager();

	// Workerを見る
	for (auto& worker : m_pWorker)
	{
		// 動いていない働きアリ
		if (!worker->GetMoveState()) continue;

		// 未割当の餌を探す
		CFeed* pFeed = pFeedMgr->FindFreeFeed();
		if (!pFeed) break;

		// Workerに指示
		worker->AssignFeed(pFeed);

		// セットするにする
		pFeed->SetAssigned(true);
	}
#endif
}