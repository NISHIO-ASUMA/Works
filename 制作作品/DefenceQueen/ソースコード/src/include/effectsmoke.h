//=========================================================
//
// 煙エフェクト処理 [ effectsmoke.h ]
// Author : Asuma Nishi
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
// 煙エフェクトクラスを定義
//*********************************************************
class CEffectSmoke : public CBillboard
{
public:

	CEffectSmoke(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CEffectSmoke();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CEffectSmoke* Create(const D3DXVECTOR3& pos, const D3DXCOLOR& col, int nLife, float fRadius);

private:
	int m_nLife;		// 体力
	float m_fRadius;	// 半径
};