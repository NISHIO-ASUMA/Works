//=========================================================
//
// 防衛対象アリの処理 [ queen.h ]
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
#include "nomovecharactor.h"
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CParameter;
class CStateMachine;
class CSphereCollider;
class CMotion;
class CLoad;

//*********************************************************
// 女王キャラクタークラスを定義
//*********************************************************
class CQueen : public CNoMoveCharactor
{
public:

	//***********************************
	// モーション列挙型
	//***********************************
	enum MOTION
	{
		MOTION_NEUTRAL,
		MOTION_MAX
	};

	CQueen(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CQueen();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CSphereCollider * pOther);
	void Hit(const int nDamage);
	void SaveHp(void);

	inline void SetIsUse(const bool isUse) { m_isUse = isUse; }
	inline bool GetIsUse(void) { return m_isUse; }
	inline CSphereCollider* GetCollider(void) { return m_pSphereCollider.get(); }

	static CQueen* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:

	//***********************************
	// 定数構造体宣言
	//***********************************
	struct QueenInfo
	{
		static constexpr int Hp = 100;			 // 体力値
		static constexpr float HitRange = 80.0f; // コリジョン半径
		static constexpr const char* SCRIPT = "data/MOTION/Queen/Queen_Motion.txt"; // ロードファイル名
	};

	std::unique_ptr<CParameter>m_pParameter;			// パラメータークラスポインタ
	std::unique_ptr<CLoad>m_pLoad;						// 情報外部書き出しクラスのポインタ
	std::unique_ptr<CStateMachine>m_pStateMachine;		// ステート基底クラスのポインタ
	std::unique_ptr<CSphereCollider>m_pSphereCollider;	// 円形のコライダー

	CMotion* m_pMotion;	// モーションポインタ
	bool m_isUse;		// 使うかどうかのフラグ
};