//=========================================================
//
// イベントエリア複数管理処理 [ eventareamanager.h ]
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
class CEventArea;
class CFeed;

//*********************************************************
// イベント中のエリアを複数管理するクラスを定義
//*********************************************************
class CEventAreaManager
{
public:

	~CEventAreaManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列から要素を削除する
	/// </summary>
	/// <param name="pArea">消去対象のポインタ</param>
	void Erase(CEventArea* pArea);

	/// <summary>
	/// 配列から要素を削除する
	/// </summary>
	/// <param name="pFeed">消去対象のポインタ</param>
	void EraseFeed(CFeed* pFeed);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetSize(void) const { return static_cast<int>(m_pAreas.size()); }

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetFeedSize(void) const { return static_cast<int>(m_pFeeds.size()); }

	/// <summary>
	/// 配列から判定したいイベントエリアを取得
	/// </summary>
	/// <param name="nIdx">取得先の番号</param>
	/// <returns></returns>
	CEventArea* GetIdx(const int& nIdx) { return m_pAreas[nIdx]; }

	/// <summary>
	/// 配列から判別したい餌を取得
	/// </summary>
	/// <param name="nIdx">取得先の番号</param>
	/// <returns></returns>
	CFeed * GetFeedIdx(const int& nIdx) { return m_pFeeds[nIdx]; }

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CEventAreaManager* GetInstance(void)
	{
		static CEventAreaManager Instance;
		return &Instance;
	};

private:

	CEventAreaManager();				// 格納コンストラクタ

	std::vector<CEventArea*>m_pAreas;	// ポイントエリアの管理配列
	std::vector<CFeed*>m_pFeeds;		// イベント内の餌配列
	int m_nLastEventTime;				// イベント時間
	int m_nCreateidx;					// 生成インデックス
};