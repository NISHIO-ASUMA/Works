//=========================================================
//
// あらかじめマップに配置されている餌の管理処理 [ basemapfeed.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "basemapfeed.h"
#include "feed.h"

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace BaseFeedInfo
{
	constexpr int Create_Pos = 4;

	// 生成する座標の設定
	const D3DXVECTOR3 CreateBasePos[Create_Pos]
	{
		{-885.0f,0.0f,-700.0f},
		{-885.0f,0.0f, 545.0f},
		{ 885.0f,0.0f, 545.0f},
		{ 885.0f,0.0f,-680.0f}
	};

	// モデルの種類数
	constexpr int MODELTYPE = 3;

	// 生成するモデルのパス
	const char * CreatePathName[MODELTYPE]
	{
		"FEED/Lemon.x",		// レモンモデル
		"FEED/Solt.x",		// 塩モデル
		"FEED/Suger.x"		// 砂糖モデル
	};

	constexpr int LIFE = 10000; // 体力値
	constexpr float HitRange = 85.0f; // コリジョン値
};

//=========================================================
// コンストラクタ
//=========================================================
CBaseMapFeed::CBaseMapFeed() : m_pBaseFeeds{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CBaseMapFeed::~CBaseMapFeed()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBaseMapFeed::Init(void)
{
	// 名前空間を使用
	using namespace BaseFeedInfo;

	// マップのベースの餌を生成する
	for (int nCnt = 0; nCnt < NUM_BASEFEED; nCnt++)
	{
		// ランダムパスを設定する
		int nRand = rand() % MODELTYPE;

		// マップに配置する餌を生成
		m_pBaseFeeds[nCnt] = CFeed::Create(CreateBasePos[nCnt],VECTOR3_NULL,INITSCALE, CreatePathName[nRand], HitRange,LIFE);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBaseMapFeed::Uninit(void)
{

}