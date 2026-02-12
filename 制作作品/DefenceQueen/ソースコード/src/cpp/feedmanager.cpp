//=========================================================
//
// 複数の餌を管理する処理 [ feedmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "feedmanager.h"
#include "feed.h"
#include "meshcylinder.h"
#include "gamesceneobject.h"
#include "gametime.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace FEED_INFO
{
	constexpr int NUM_MODEL = 3;	 // モデルの最大種類数
	constexpr int NUM_ACTIVEPOS = 4; // 座標数

	const char* MODEL_NAME[NUM_MODEL] // パス配列
	{
		"FEED/Lemon.x",
		"FEED/Suger.x",
		"FEED/Solt.x"
	};

	D3DXVECTOR3 RandomActivePos[NUM_ACTIVEPOS] // 座標配列
	{
		{ -450.0f,0.0f, 180.0f},
		{ 750.0f, 0.0f, -190.0f},
		{ 255.0f, 0.0f, -630.0f},
		{ 935.0f, 0.0f, 10.0f},
	};
};

//=========================================================
// コンストラクタ
//=========================================================
CFeedManager::CFeedManager() :m_pFeed{}, m_nCreateLastTime(-1), m_nStopCount(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CFeedManager::~CFeedManager()
{
	// 破棄
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CFeedManager::Init(void)
{
	// 配列クリア
	m_pFeed.clear();

	// 初期生成の餌
	m_pFeed.push_back(CFeed::Create(D3DXVECTOR3(-1500.0f, 0.0f,-600.0f), VECTOR3_NULL, INITSCALE, "FEED/Suger.x",85.0f));
	//m_pFeed.push_back(CFeed::Create(D3DXVECTOR3(700.0f, 0.0f, -600.0f), VECTOR3_NULL, INITSCALE, "FEED/Suger.x", 85.0f));
	//m_pFeed.push_back(CFeed::Create(D3DXVECTOR3(0.0f, 0.0f, 570.0f), VECTOR3_NULL, INITSCALE, "FEED/Lemon.x", 85.0f));

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CFeedManager::Uninit(void)
{
	m_pFeed.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CFeedManager::Update(void)
{
	// 経過時間によって出現する
	 auto GameSceneObject = CGameSceneObject::GetInstance();
	 auto time = GameSceneObject->GetTime()->GetToAll();

	 // ランダム数
	 int nRand = rand() % FEED_INFO::NUM_MODEL;
	 int nRandPos = rand() % FEED_INFO::NUM_ACTIVEPOS;

	 // 餌が0なら即出す
	 if (m_pFeed.empty())
	 {
		 // ランダムな座標
		 D3DXVECTOR3 pos = FEED_INFO::RandomActivePos[nRandPos];

		 // 配列に追加
		 m_pFeed.push_back(CFeed::Create(pos,VECTOR3_NULL,INITSCALE,FEED_INFO::MODEL_NAME[nRand],Config::CreateRadius));

		 m_nCreateLastTime = time;

		 return;
	 }

	 // 総ゲーム時間から計算
	 if (time != 120 && time % Config::ACTIVE_TIME == NULL && time != m_nCreateLastTime)
	 {
		 // ランダムな座標
		 D3DXVECTOR3 ActivePos = FEED_INFO::RandomActivePos[nRandPos];

		 // 配列に追加
		 m_pFeed.push_back(CFeed::Create(ActivePos,VECTOR3_NULL,INITSCALE,FEED_INFO::MODEL_NAME[nRand],Config::CreateRadius));

		 // 時間変更
		 m_nCreateLastTime = time;
	 }
}
//=========================================================
// 描画処理
//=========================================================
void CFeedManager::Draw(void)
{
	// 無し
}
//=========================================================
// 要素を消す
//=========================================================
void CFeedManager::Erase(CFeed* pFeed)
{
	// 削除処理
	auto DeleteDestObj = std::find(m_pFeed.begin(), m_pFeed.end(), pFeed);

	// もし最後尾なら
	if (DeleteDestObj == m_pFeed.end())
	{
		return;
	}

	// 自身の削除
	(*DeleteDestObj)->Uninit();

	// 配列の要素を消す
	DeleteDestObj = m_pFeed.erase(DeleteDestObj);
}
//=========================================================
// 未登録の餌を探す
//=========================================================
CFeed* CFeedManager::FindFreeFeed(void)
{
	// 最大数を見る
	for (auto& feed : m_pFeed)
	{
		// もし割り当てられてなかったら
		if (!feed->IsAssigned())
			return feed;
	}

	return nullptr;
}