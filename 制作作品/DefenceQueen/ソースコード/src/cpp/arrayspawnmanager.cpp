//=========================================================
//
// 仲間のスポナー管理処理 [ arrayspawnmanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "arrayspawnmanager.h"
#include "arraymanager.h"
#include "arrayspawner.h"
#include "debugproc.h"
#include "array.h"
#include "objectX.h"

//*********************************************************
// 使用名前空間
//*********************************************************
using namespace SpawnManager;

//=========================================================
// コンストラクタ
//=========================================================
CArraySpawnManager::CArraySpawnManager() : m_pArraySpawner{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CArraySpawnManager::~CArraySpawnManager()
{
	// 終了処理
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CArraySpawnManager::Init(CArrayManager* pManager)
{
	// 生成されたアリの最大数を取得
	int nUseAnt = pManager->GetAllUseArrays();

	// 配列の要素数分のスポナーを生成する
	for (int nCnt = 0; nCnt < NUM_ARRAYSPAWNER; nCnt++)
	{
		// 動かせる最大数から分割計算する
		int nSepalation = nUseAnt / NUM_ARRAYSPAWNER;
		int nIdx = nSepalation * nCnt;

		// スポナーを生成 ( スポナー座標,仲間のストック数,アリ管理マネージャーポインタ )
		m_pArraySpawner[nCnt] = CArraySpawner::Create(SpawnManager::POS[nCnt], NUM_CREATEARRAY, pManager, SpawnManager::BasePos[nCnt], nIdx);

		// スポーンポイントのオブジェクト生成
		CObjectX::Create("STAGEOBJ/SpawnBase.x", SpawnManager::POS[nCnt],SpawnManager::ROTVALUE[nCnt]);

		// アクティブなアリを加算
		pManager->CountActiveArrays(m_pArraySpawner[nCnt]->GetStockArray());
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CArraySpawnManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_ARRAYSPAWNER; nCnt++)
	{
		if (m_pArraySpawner[nCnt])
		{
			// ポインタの破棄とnull初期化
			delete m_pArraySpawner[nCnt];
			m_pArraySpawner[nCnt] = nullptr;
		}
	}
}
//=========================================================
// 更新処理
//=========================================================
void CArraySpawnManager::Update(void)
{
	for (int nCnt = 0; nCnt < NUM_ARRAYSPAWNER; nCnt++)
	{
		// 更新処理
		m_pArraySpawner[nCnt]->Update();
	}
}
//=========================================================
// 描画処理
//=========================================================
void CArraySpawnManager::Draw(void)
{
#if 0
	// デバッグ情報表示処理
	CDebugproc::Print("各スポナーのアリ情報\n[0]番目 : ストック数 [ %d ]\n[1]番目 : ストック数 [ %d ]\n[2]番目 : ストック数 [ %d ]\n",
		m_pArraySpawner[0]->GetStockArray(), m_pArraySpawner[1]->GetStockArray(), m_pArraySpawner[2]->GetStockArray());
	CDebugproc::Draw(1050, 20, COLOR_WHITE);

	// インデックス情報の管理
	CDebugproc::Print("各スポナーのIDX情報\n[0]番目 : %d\n[1]番目 : %d\n[2]番目 : %d\n",
		m_pArraySpawner[0]->GetIdx(), m_pArraySpawner[1]->GetIdx(), m_pArraySpawner[2]->GetIdx());
	CDebugproc::Draw(1050, 180, COLOR_WHITE);

	// 移動命令を受けたアリのフラグ情報の管理
	CDebugproc::Print("移動命令を受けたアリの数\n[0] : { %d / 50 }\n[1] : { %d / 50 }\n[2] : { %d / 50 }\n",
		m_pArraySpawner[0]->GetIsMoveArrays(), m_pArraySpawner[1]->GetIsMoveArrays(), m_pArraySpawner[2]->GetIsMoveArrays());
	CDebugproc::Draw(1050, 330, COLOR_WHITE);

	// 帰還命令を受けたアリのフラグ情報の管理
	CDebugproc::Print("帰還命令を受けたアリの数\n[0] : { %d / 50 }\n[1] : { %d / 50 }\n[2] : { %d / 50 }\n",
		m_pArraySpawner[0]->GetIsReturn(), m_pArraySpawner[1]->GetIsReturn(), m_pArraySpawner[2]->GetIsReturn());
	CDebugproc::Draw(1050, 400, COLOR_WHITE);

	// 停止命令を受けたアリのフラグ情報の管理
	CDebugproc::Print("停止命令を受けたアリの数\n[0] : { %d / 50 }\n[1] : { %d / 50 }\n[2] : { %d / 50 }\n",
		m_pArraySpawner[0]->GetIsStop(), m_pArraySpawner[1]->GetIsStop(), m_pArraySpawner[2]->GetIsStop());
	CDebugproc::Draw(1050, 480, COLOR_WHITE);
#endif // _DEBUG
}