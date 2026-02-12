//=========================================================
//
// エフェクト処理 [ effect.h ]
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
#include "billboard.h"

//*********************************************************
// エフェクトクラスを定義
//*********************************************************
class CEffect : public CBillboard
{
public:

	CEffect(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CEffect();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetLife(int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

	static CEffect* Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, int nLife, float fRadius);

private:
	int m_nLife;		// 寿命
	float m_fRadius;	// 半径
	D3DXVECTOR3 m_move;	// 移動量
};