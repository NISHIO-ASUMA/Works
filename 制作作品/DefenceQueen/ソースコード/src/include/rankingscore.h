//=========================================================
//
// ランキングスコア処理 [ rankingscore.h ]
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
#include "object.h"
#include <array>
#include <algorithm>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;
class CLoad;

//*********************************************************
// ランキングスコアクラス定義
//*********************************************************
class CRankingScore : public CObject
{
public:

	CRankingScore(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CRankingScore();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void);

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CRankingScore* Create(const D3DXVECTOR3& pos, float fWidth, float fHeight);

private:

	static constexpr int RANKSCOREDIGIT = 8; // 表示桁数
	static constexpr int RANKING_MAX = 5;	// ランキング数

	std::array<int, RANKING_MAX>m_aRankData;	// スコア配列
	std::unique_ptr<CLoad>m_pLoad;	// 配列スコア読み込みクラス

	D3DXVECTOR3 m_pos;		// 座標
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ

	CNumber* m_apNumber[RANKING_MAX][RANKSCOREDIGIT]; // 各スコアと桁数
};