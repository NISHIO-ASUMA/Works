//=========================================================
//
// 働きアリの複数管理処理 [ workermanager.h ]
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
class CWorker;

//*********************************************************
// 働きアリの管理クラスを定義
//*********************************************************
class CWorkerManager
{
public:

	CWorkerManager();
	~CWorkerManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列の指定番号から取得する
	/// </summary>
	/// <param name="nIdx">取得したい番号</param>
	/// <returns></returns>
	CWorker* GetWorker(const int nIdx) { return m_pWorker[nIdx]; }

	/// <summary>
	/// 動的配列そのものを取得
	/// </summary>
	/// <returns></returns>
	std::vector<CWorker*>& GetInfo() { return m_pWorker; }

	/// <summary>
	/// 配列の最大要素数を取得
	/// </summary>
	/// <returns></returns>
	int GetSize() const { return m_pWorker.size(); }

private:

	//*************************
	// 定数構造体
	//*************************
	struct Config
	{
		static constexpr int NUM_WOKER = 3;	// 最大数
		static constexpr float WORKER_INTERVAL = 70.0f; // 間隔
	};

	std::vector<CWorker*>m_pWorker; // クラス配列ポインタ
};