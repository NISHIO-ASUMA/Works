//=========================================================
//
// イベントエリア複数管理処理 [ eventareamanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "eventareamanager.h"
#include "gamesceneobject.h"
#include "gametime.h"
#include "eventarea.h"
#include "feed.h"

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace EventManager
{
	constexpr int NUMPATH = 3;				// モデルの種類
	constexpr int NUM_ACTIVEPOS = 4;		// イベントが起きる場所の数
	constexpr float COLLISIONRANGE = 65.0f; // 餌の衝突判定半径

	const char* MODEL_NAME[NUMPATH]			// モデルパス配列	
	{
		"FEED/Lemon.x",
		"FEED/Suger.x",
		"FEED/Solt.x"
	};

	const D3DXVECTOR3 EventPos[NUM_ACTIVEPOS] // 出現座標の値
	{
		{ 585.0f, 0.0f,-390.0f},
		{ 435.0f,0.0f, 30.0f},
		{-530.0f,0.0f, -45.0f},
		{ -5.0f, 0.0f, 400.0f}
	};

	const D3DXVECTOR3 EventFeedPos[NUM_ACTIVEPOS] // 出現餌の座標の値
	{
		{ 605.0f, 0.0f,-420.0f},
		{ 525.0f,0.0f,  30.0f},
		{-600.0f,0.0f, -45.0f},
		{ -35.0f,0.0f, 410.0f}
	};

	const int RANDOMHP[NUM_ACTIVEPOS] // 体力値配列
	{
		10,
		20,
		25,
		30
	};

};

//=========================================================
// コンストラクタ
//=========================================================
CEventAreaManager::CEventAreaManager() : m_pAreas{}, 
m_pFeeds{},
m_nLastEventTime(NULL), 
m_nCreateidx(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEventAreaManager::~CEventAreaManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEventAreaManager::Init(void)
{
	m_pAreas.clear();
	m_pFeeds.clear();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEventAreaManager::Uninit(void)
{
	// 配列をクリア
	m_pAreas.clear();

	m_pFeeds.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CEventAreaManager::Update(void)
{
	// 名前空間を使用する
	using namespace EventManager;

	// ゲーム時間から計測する
	const auto& Time = CGameSceneObject::GetInstance()->GetTime();
	int nTime = Time->GetToAll();

	// ランダム値を生成
	int nRand = rand() % NUM_ACTIVEPOS;
	int nPathRand = rand() % NUMPATH;

	// 総ゲーム時間から計算
	if (nTime % 5 == 0 && nTime != m_nLastEventTime)
	{
		// ランダムな座標
		D3DXVECTOR3 ActivePos = EventPos[nRand];

		// 判定エリアと餌を生成
		CEventArea * area = CEventArea::Create(ActivePos);
		CFeed * feed = CFeed::Create(EventFeedPos[nRand], VECTOR3_NULL, INITSCALE, MODEL_NAME[nPathRand], COLLISIONRANGE, RANDOMHP[nRand]);

		// オーナーに設定
		feed->SetOwnerArea(area);

		// 餌イベントの配列に追加
		m_pAreas.push_back(area);

		// 餌オブジェクトを配置する
		m_pFeeds.push_back(feed);

		// 出現時間変更
		m_nLastEventTime = nTime;
	}
}
//=========================================================
// エリア配列の要素をクリアする関数
//=========================================================
void CEventAreaManager::Erase(CEventArea* pArea)
{
	// 削除処理
	auto DeleteDestObj = std::find(m_pAreas.begin(), m_pAreas.end(), pArea);

	// もし最後尾なら
	if (DeleteDestObj == m_pAreas.end())
	{
		return;
	}

	// 自身の削除
	(*DeleteDestObj)->Uninit();

	// 配列の要素を消す
	DeleteDestObj = m_pAreas.erase(DeleteDestObj);
}
//=========================================================
// 餌配列の要素をクリアする関数
//=========================================================
void CEventAreaManager::EraseFeed(CFeed* pFeed)
{
	// 削除処理
	auto DeleteDestObj = std::find(m_pFeeds.begin(), m_pFeeds.end(), pFeed);

	// もし最後尾なら
	if (DeleteDestObj == m_pFeeds.end())
	{
		return;
	}

	// 自身の削除
	(*DeleteDestObj)->Uninit();

	// 配列の要素を消す
	DeleteDestObj = m_pFeeds.erase(DeleteDestObj);
}