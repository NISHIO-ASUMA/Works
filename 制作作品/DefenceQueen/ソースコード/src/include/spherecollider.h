//=========================================================
//
// 球形コライダー処理 [ spherecollider.h ]
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
#include "collider.h"
#include <memory>

//*********************************************************
// 球形コライダークラスを定義
//*********************************************************
class CSphereCollider : public CCollider
{
public:

	CSphereCollider();
	~CSphereCollider();

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius(void) { return m_fRadius; }

	static std::unique_ptr<CSphereCollider>Create(D3DXVECTOR3 pos, float fRadius);

private:

	float m_fRadius; // 当たる半径
};

