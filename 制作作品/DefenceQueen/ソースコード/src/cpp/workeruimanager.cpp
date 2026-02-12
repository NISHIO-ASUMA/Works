//=========================================================
//
// アリで出すUIの複数管理処理 [ workeruimanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "workeruimanager.h"
#include "workerui.h"
#include "gamesceneobject.h"
#include "workermanager.h"
#include "worker.h"

//*********************************************************
// 定数宣言
//*********************************************************
namespace WorkUi
{
	const D3DXVECTOR3 Bacepos = D3DXVECTOR3(520.0f, 60.0f, 0.0f);	// 基準座標
	constexpr float WIDTH = 40.0f;									// 横幅
	constexpr float HEIGHT = 40.0f;									// 高さ
	constexpr float SPACE_WIDTH = 140.0f;							// 間隔の横幅
}

//=========================================================
// コンストラクタ
//=========================================================
CWorkerUiManager::CWorkerUiManager()
{
	// 値のクリア
	for (auto worker : m_pWorkUi)
	{
		worker = nullptr;
	}
}
//=========================================================
// デストラクタ
//=========================================================
CWorkerUiManager::~CWorkerUiManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorkerUiManager::Init(void)
{
	// 名前空間
	using namespace WorkUi;

	for (int nCnt = 0; nCnt < NUM_UI; nCnt++)
	{
		// 基準座標セット
		D3DXVECTOR3 pos = Bacepos;

		// 横幅を空ける
		pos.x += nCnt * SPACE_WIDTH;

		// UIを配置する
		m_pWorkUi[nCnt] = CWorkerUi::Create(pos, WIDTH, HEIGHT);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWorkerUiManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CWorkerUiManager::Update(void)
{
	// 司令塔アリのワーカーフラグを取得
	auto WorkAnt = CGameSceneObject::GetInstance()->GetWorkerM();
	if (WorkAnt == nullptr) return;

	// 各配列に応じて有効状態を取得しカラーを変更させる
	for (int nCnt = 0; nCnt < WorkAnt->GetSize(); nCnt++)
	{
		// 有効なら
		if (WorkAnt->GetWorker(nCnt)->GetIsWork())
		{
			// カラー変更
			m_pWorkUi[nCnt]->SetCol(COLOR_RED);
		}
		else
		{
			// カラーはそのまま
			m_pWorkUi[nCnt]->SetCol(COLOR_WHITE);
		}
	}
}