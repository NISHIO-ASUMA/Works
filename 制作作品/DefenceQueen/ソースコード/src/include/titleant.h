//=========================================================
//
// タイトルのキャラクター処理 [ titleant.h ]
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
#include "movecharactor.h"
#include "instancingcharactor.h"
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;
class CBoxCollider;

//*********************************************************
// タイトルのアリのクラスを定義
//*********************************************************
class CTitleAnt : public CInstancingCharactor
{
public:

	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		MOTION_NEUTRAL,
		MOTION_MOVE,
		MOTION_MAX
	};

	CTitleAnt(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CTitleAnt();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther);

	static CTitleAnt* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& destpos,const D3DXVECTOR3& rot);

	inline void SetDestPos(const D3DXVECTOR3& pos) { m_DestPos = pos; }
	inline D3DXVECTOR3 GetDestPos(void) const { return m_DestPos; }

private:

	//**************************************
	// 定数構造体宣言
	//**************************************
	struct Config
	{
		static constexpr float MOVE = 3.0f; // 移動量
		static constexpr const char* SCRIPT = "data/MOTION/Array/Title_Array.txt";
	};

	D3DXVECTOR3 m_DestPos;			// 目的座標
	std::unique_ptr<CSphereCollider>m_pCollider;	// 球形コライダー
};