//=========================================================
//
// プレイヤー状態管理処理 [ playerstate.h ]
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
#include "state.h"
#include "player.h"

//*********************************************************
// プレイヤーのベース状態管理
//*********************************************************
class CPlayerStateBase : public CStateBase
{
public:

	//***************************
	// 状態管理ID列挙型
	//***************************
	enum ID
	{
		ID_NEUTRAL, // 待機
		ID_MOVE, // 移動
		ID_ACTION, // 攻撃
		ID_DAMAGE,// ダメージ
		ID_MAX
	};

	CPlayerStateBase();
	~CPlayerStateBase();

	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnExit() {}

	int GetID() const override { return m_ID; }
	
	void SetID(ID id) { m_ID = id; }
	void SetOwner(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

protected:

	CPlayer* m_pPlayer;		// プレイヤーのポインタ

private:
	ID m_ID;		// 列挙型のポインタ
};