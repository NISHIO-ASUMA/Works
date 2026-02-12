//=========================================================
//
// 柱状エフェクト処理 [ effectpiler.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "billboard.h"

//*********************************************************
// エフェクトクラスを定義
//*********************************************************
class CEffectPiler : public CBillboard
{
public:

	CEffectPiler(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CEffectPiler();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CEffectPiler* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nLife);

private:
	int m_nLife;		// 体力
	float m_fRadius;	// 半径
};