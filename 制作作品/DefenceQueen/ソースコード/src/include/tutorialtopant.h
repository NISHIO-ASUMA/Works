//=========================================================
//
// チュートリアルの指示アリの処理 [ tutorialtopant.h ]
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

//*********************************************************
// 前方宣言
//*********************************************************
class CInputKeyboard;
class CJoyPad;
class CBoxCollider;
class CCollisionBox;
class CSphereCollider;
class CSepalationSign;
class CPointObj;
class CCircle3D;

//*********************************************************
// チュートリアルの指示アリのクラスを定義
//*********************************************************
class CTutoTopAnt : public CMoveCharactor
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

	CTutoTopAnt(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CTutoTopAnt();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos);

	void Moving(CJoyPad* pPad, CInputKeyboard* pKey);
	void MovePad(CJoyPad* pPad);
	void Separation(void);

	void SetDestMovePos(const D3DXVECTOR3& pos) { m_DestPos = pos; }
	void SetSeparationRadius(const float& Radius) { m_fSeparationRadius = Radius; }

	D3DXVECTOR3 GetDestPos(void) { return m_DestPos; }
	float GetSeparationRadius(void) { return m_fSeparationRadius; }

	static CTutoTopAnt* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot = VECTOR3_NULL);

private:

	//**********************
	// 定数構造体宣言
	//**********************
	struct Config
	{
		static constexpr float SPEED = 3.0f;											 // 移動速度
		static constexpr float MAX_RADIUS = 100.0f;										 // 最大範囲
		static constexpr float NorRot = D3DX_PI * 2.0f;									 // 回転補正
		static constexpr float Separation = 3.0f;										 // 範囲拡大
		static constexpr float OffPosY = 240.0f;										// オフセット
		static constexpr const char* MOTION_NAME = "data/MOTION/Top/Top_Motion.txt";	 // モーションパス
	};

	std::unique_ptr<CSphereCollider>m_pSphereCollider; // 球形コライダー
	std::unique_ptr<CBoxCollider>m_pColliderBox;		// 矩形コライダー

	CCircle3D* m_pCircleObj;			// 円形範囲選択オブジェクト
	CSepalationSign* m_pSeparationSign; // 切り離し表示サイン
	CSepalationSign* m_pPutSign;		// 置いていく表示
	CPointObj* m_pPoint;				// 矢印オブジェクト

	D3DXVECTOR3 m_DestPos;				// 座標設置用
	bool m_isBranchSet;					// 切り離し中かどうか
	bool m_isHPressing;					// キーを押している間
	float m_fSeparationRadius;			// 切り離し範囲
};