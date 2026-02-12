//=========================================================
//
// チュートリアルの仲間アリの処理クラス [ tutoarrayant.h ]
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
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CCollisionBox;
class CSphereCollider;

//*********************************************************
// チュートリアル用の仲間アリクラスを定義
//*********************************************************
class CTutoArrayAnt : public CMoveCharactor
{
public:

	//********************
	// モーションの種類
	//********************
	enum MOTION
	{
		NEUTRAL,
		MOVE,
		MOTION_MAX
	};

	CTutoArrayAnt(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CTutoArrayAnt();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void FollowTopAnt(void);
	void ReturnBase(void);
	bool Collision(CSphereCollider* other);

	inline void SetIsTopFollow(const bool& isFollow) { m_isTopFollow = isFollow; }
	inline void SetDestPos(const D3DXVECTOR3& pos) { m_DestPos = pos; }

	static CTutoArrayAnt* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot = VECTOR3_NULL);

private:

	//**********************
	// 定数構造体宣言
	//**********************
	struct Config
	{
		static constexpr float SPEED = 2.0f;											 // 移動速度
		static constexpr float MAX_RADIUS = 40.0f;										 // 最大範囲
		static constexpr float MAX_DISTANCE = 60.0f;									 // 近づける距離
		static constexpr float NorRot = D3DX_PI * 2.0f;									 // 回転補正
		static constexpr const char* MOTION_NAME = "data/MOTION/Array/Array_Motion.txt"; // モーションパス
	};

	std::unique_ptr<CSphereCollider>m_pSphereCollider; // 球形コライダー
	D3DXVECTOR3 m_DestPos;				// 座標設置用

	bool m_isTopFollow;					// トップを追従するかどうか
	bool m_isReturnBase;				// 元の位置にもどってくるかどうか
};