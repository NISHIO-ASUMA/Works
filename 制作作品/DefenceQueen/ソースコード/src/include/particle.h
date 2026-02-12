//=========================================================
//
// パーティクル処理 [ particle.h ]
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
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CEffect;

//*********************************************************
// パーティクルクラスを定義
//*********************************************************
class CParticle : public CObject
{
public:

	CParticle(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CParticle();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CParticle* Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nMaxParticle, int nRadius, int nLength, int nLife);

private:
	std::vector<CEffect*> m_pEffect; // エフェクト配列

	D3DXVECTOR3 m_pos;	// 座標
	D3DXCOLOR m_col;	// 色
	int m_nMaxParticle; // 最大数
	int m_nLife;		// 寿命
	int m_nLength;		// 飛散する距離
	int m_nRadius;		// 粒の大きさ
};