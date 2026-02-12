//=========================================================
//
// あらかじめマップに配置されている餌の管理処理 [ basemapfeed.h ]
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
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CFeed;

//*********************************************************
// 配置されている餌を管理するクラスを定義
//*********************************************************
class CBaseMapFeed
{
public:

	CBaseMapFeed();
	~CBaseMapFeed();

	HRESULT Init(void);
	void Uninit(void);

	/// <summary>
	/// 配列中の指定番号の餌のポインタを返す
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CFeed* GetFeedIdx(const int& nIdx) const { return m_pBaseFeeds[nIdx]; }

	/// <summary>
	/// 配列の最大サイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetFeedArraySize(void) const { return static_cast<int>(m_pBaseFeeds.max_size()); }

private:

	static constexpr int NUM_BASEFEED = 4;			// 配置最大数
	std::array<CFeed*, NUM_BASEFEED>m_pBaseFeeds;	// 餌の配列
};