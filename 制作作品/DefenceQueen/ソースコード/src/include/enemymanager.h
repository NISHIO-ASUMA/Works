//=========================================================
//
// 敵の複数管理処理 [ enemymanager.h ]
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
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CEnemy;

//*********************************************************
// 敵の情報を一括管理するクラスを定義
//*********************************************************
class CEnemyManager
{
public:

	CEnemyManager();
	~CEnemyManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列番号情報の取得
	/// </summary>
	/// <param name="nIdx">取得するインデックス</param>
	/// <returns></returns>
	CEnemy * GetEnemyIdx(const int nIdx) { return m_pEnemys[nIdx]; }

	/// <summary>
	/// 動的配列本体を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CEnemy*>& GetEnemy(void) { return m_pEnemys; }

	/// <summary>
	/// 配列のサイズを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetSize(void) const { return static_cast<int>(m_pEnemys.size()); }

	/// <summary>
	/// 配列から要素を消す関数
	/// </summary>
	/// <param name="pEnemy">敵のポインタ</param>
	void Erase(CEnemy* pEnemy);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr int NUM_ENEMY = 100;		// 最大数
		static constexpr int LIFE = 1;				// 体力値
		static constexpr int SPAWM_INTERBAL = 10;	// 生成インターバル
		static constexpr int RANDOM_VALUE_MAX = 50; // 生成ランダム最大値
		static constexpr int RANDOM_VALUE_MIN = 10; // 生成ランダム最小値
		static constexpr float SPEED = 3.0f;		// 移動速度
	};

	std::vector<CEnemy*>m_pEnemys;	// 敵の動的配列
	int m_nCreateLastTime;			// 最後に生成した時間
};