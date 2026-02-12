//=========================================================
//
// 複数の餌を管理する処理 [ feedmanager.h ]
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
class CFeed;

//*********************************************************
// 餌を管理するクラスを定義
//*********************************************************
class CFeedManager
{
public:

	CFeedManager();
	~CFeedManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 配列の指定番号から取得
	/// </summary>
	/// <param name="nIdx">取得したいインデックス番号</param>
	/// <returns></returns>
	CFeed* GetFeed(const int nIdx) { return m_pFeed[nIdx]; }

	/// <summary>
	/// 配列の要素数を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetSize(void) const { return static_cast<int>(m_pFeed.size()); }

	/// <summary>
	/// 配列から要素を削除する
	/// </summary>
	/// <param name="pFeed">餌自身のポインタ</param>
	void Erase(CFeed* pFeed);

	/// <summary>
	/// 割り当てられてない餌を探して割り当てる関数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	CFeed* FindFreeFeed(void);

private:

	//****************************
	// 定数構造体
	//****************************
	struct Config
	{
		static constexpr int ALLFEED = 3;			 // 最大で確保するメモリ数
		static constexpr int ACTIVE_TIME = 10;		 // 10秒ごとに生成する
		static constexpr float CreateRadius = 85.0f; // 判定をする半径
	};

	std::vector<CFeed*>m_pFeed; // 使用する餌の動的配列
	int m_nCreateLastTime;		// 最後に生成した時間
	int m_nStopCount;			// 停止時間
};