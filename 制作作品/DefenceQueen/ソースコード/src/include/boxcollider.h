//=========================================================
//
// 矩形のコライダー処理 [ boxcollider.h ]
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
// 矩形のコライダー設定処理
//*********************************************************
class CBoxCollider : public CCollider
{
public:

	//*********************************
	// コライダーで使用する変数構造体
	//*********************************
	struct BoxColliderInfo
	{
		D3DXVECTOR3 posOld;// 前回の座標
		D3DXVECTOR3 Size;  // コライダーの大きさ
	};
	
	CBoxCollider();
	~CBoxCollider();

	void SetPosOld(const D3DXVECTOR3 posold) { m_BoxInfo.posOld = posold; }
	BoxColliderInfo GetInfo(void) const { return m_BoxInfo; }

	static std::unique_ptr<CBoxCollider>Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posold, const D3DXVECTOR3 size);

private:

	BoxColliderInfo m_BoxInfo; // 構造体変数
};