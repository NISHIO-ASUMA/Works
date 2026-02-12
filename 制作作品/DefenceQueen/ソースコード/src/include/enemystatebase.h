//=========================================================
//
// 敵の状態管理基底処理 [ enemystatebase.h ]
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
#include "state.h"
#include "enemy.h"

//*********************************************************
// 敵の状態管理ベースクラス
//*********************************************************
class CEnemyStateBase : public CStateBase
{
public:

	//***************************
	// 状態管理ID列挙型
	//***************************
	enum ID
	{
		ID_NEUTRAL, // 待機
		ID_MOVE,	// 移動
		ID_ACTION,	// 攻撃
		ID_DAMAGE,	// ダメージ
		ID_MAX
	};

	CEnemyStateBase();
	~CEnemyStateBase();

	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnExit() {}

	int GetID() const override { return m_ID; }

	void SetID(ID id) { m_ID = id; }
	void SetOwner(CEnemy* pEnemy) { m_pEnemy = pEnemy; }

protected:
	CEnemy* m_pEnemy;	// 敵のポインタ

private:
	ID m_ID;		// 列挙型のポインタ
};