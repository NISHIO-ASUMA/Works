//=========================================================
//
// タイトルのアリを管理する処理 [ titleantmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "titleantmanager.h"
#include "titleant.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SPAWNINFO
{
	// 出現択数
	constexpr int NUM_POS = 2;

	constexpr int MAX_COUNT = 120;

	// 出現座標
	const D3DXVECTOR3 ActivePos[NUM_POS]
	{
		{-1015.0f,0.0f,200.0f},
		{1000.0f,0.0f,-200.0f}
	};

	// 目的地の座標
	const D3DXVECTOR3 MoveDestPos[NUM_POS]
	{
		{1080.0f,0.0f,200.0f},
		{-1080.0f,0.0f,-200.0f}
	};
};

//=========================================================
// コンストラクタ
//=========================================================
CTitleAntManager::CTitleAntManager() : m_pAnts{}, m_nCreateCount(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CTitleAntManager::~CTitleAntManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleAntManager::Init(void)
{
	// 配列の要素をクリアする
	m_pAnts.clear();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleAntManager::Uninit(void)
{
	// 配列の要素をクリアする
	m_pAnts.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CTitleAntManager::Update(void)
{
	// 配列に要素を追加していく
	m_nCreateCount++;

#if 1
	if (m_nCreateCount >= SPAWNINFO::MAX_COUNT)
	{
		// 配列に追加する
		m_pAnts.push_back(CTitleAnt::Create(SPAWNINFO::ActivePos[0], SPAWNINFO::MoveDestPos[0], VECTOR3_NULL));
		m_pAnts.push_back(CTitleAnt::Create(SPAWNINFO::ActivePos[1], SPAWNINFO::MoveDestPos[1], VECTOR3_NULL));

		// カウント初期化
		m_nCreateCount = 0;
	}
#endif
}