//=========================================================
//
// 仲間に指示を出すアリのクラス [ topant.h ]
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
class CInputKeyboard;
class CJoyPad;
class CSphereCollider;
class CBoxCollider;
class CCollisionSphere;
class CCollisionBox;
class CBoxToSphereCollision;
class CSepalationSign;
class CArrayManager;
class CPointObj;
class CCircle3D;

//*********************************************************
// 各スポナーのトップのアリのクラスを定義
//*********************************************************
class CTopAnt : public CMoveCharactor
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

	CTopAnt(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CTopAnt();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Moving(CJoyPad* pPad, CInputKeyboard* pKey);
	void MovePad(CJoyPad* pPad);
	void Separation(void);

	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos);
	bool CollisionSphere(CSphereCollider* pOther);
	bool CollisonT(CSphereCollider* pOther);
	bool CollisionArea(CArrayManager* pManager);

	void SetDestMovePos(const D3DXVECTOR3& pos) { m_DestPos = pos; }
	void SetSeparationRadius(const float& Radius) { m_fSeparationRadius = Radius; }
	void SetIsReturnPos(const bool& isflags) { m_isReturnNumber = isflags; }

	D3DXVECTOR3 GetDestPos(void) { return m_DestPos; }
	bool GetIsReturnPos(void) const { return m_isReturnNumber; }
	float GetSeparationRadius(void) { return m_fSeparationRadius; }

	static CTopAnt* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:

	//**********************
	// 定数構造体宣言
	//**********************
	struct Config
	{
		static constexpr float SPEED = 3.0f;					 // 移動速度
		static constexpr float MAX_RADIUS = 150.0f;				 // 最大範囲
		static constexpr float NorRot = D3DX_PI * 2.0f;			 // 回転補正
		static constexpr float Separation = 5.0f;				 // 範囲拡大
		static constexpr float AddPosY = 180.0f;				 // 高さのオフセット
		static constexpr float OffPosY = 240.0f;				 // 矢印の高さのオフセット
		static constexpr const char* MOTION_NAME = "data/MOTION/Top/Top_Motion.txt";	 // モーションパス
	};

	std::unique_ptr<CSphereCollider>m_pColliderSphere; // 球形コライダー
	std::unique_ptr<CBoxCollider>m_pColliderBox;	   // 矩形コライダー

	CCircle3D* m_pCircleObj;			// 円形オブジェクト
	CSepalationSign* m_pSeparationSign; // 切り離し表示サイン
	CSepalationSign* m_pPutSign;		// 置いていく表示
	CPointObj* m_pPoint;				// 矢印オブジェクト

	D3DXVECTOR3 m_DestPos;				// 座標設置用
	bool m_isSetPostion;				// アリをおいていくかどうか
	bool m_isHPressing;					// キーを押している間
	bool m_isReturnNumber;				// 数字を設定できるかどうか
	float m_fSeparationRadius;			// 切り離し範囲
};