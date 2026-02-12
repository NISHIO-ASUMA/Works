//=========================================================
//
// 敵のスポナー複数管理処理 [ enemyspawnmanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include <memory>
#include <vector>
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CEnemySpawner;

//*********************************************************
// 名前空間
//*********************************************************
namespace Enemy_Spawn
{
	static constexpr int CREATE_NUM = 4;		// 最大スポナー生成数
	static constexpr int SPAWN_MIN = 15;		// 最小湧き数
	static constexpr int SPAWN_MAX = 30;		// 最大湧き数

	const D3DXVECTOR3 SpawnPos[CREATE_NUM]		// スポナーの生成座標
	{
		{ 1175.0f,0.0f,308.0f },
		{ 1175.0f,0.0f,-308.0f },
		{ -1175.0f,0.0f,320.0f },
		{ -1175.0f,0.0f,-320.0f }
	};
};

//*********************************************************
// 複数スポナーの管理クラスを定義
//*********************************************************
class CEnemySpawnManager
{
public:

	CEnemySpawnManager();
	~CEnemySpawnManager();

	HRESULT Init(void);
	void Uninit(void);

	/// <summary>
	/// 各所のスポナーを取得する
	/// </summary>
	/// <param name="nIdx">取得するスポナーのインデックス番号</param>
	/// <returns></returns>
	inline CEnemySpawner* GetSpawner(const int& nIdx) { return m_pSpawners[nIdx]; }

private:

	std::array<CEnemySpawner*, Enemy_Spawn::CREATE_NUM>m_pSpawners; // 敵のスポナー配列
};