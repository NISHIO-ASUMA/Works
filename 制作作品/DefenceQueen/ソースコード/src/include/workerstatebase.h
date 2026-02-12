//===================================================================
//
// 指示アリに関する状態管理処理基底クラス [ workerstatebase.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "state.h"
#include "worker.h"

//*******************************************************************
// 指示アリに対する状態管理基底クラスを定義
//*******************************************************************
class CWorkerStateBase : public CStateBase
{
public:

	//***************************
	// 状態管理ID列挙型
	//***************************
	enum ID
	{
		ID_NEUTRAL, // 待機
		ID_MOVE,	// 移動
		ID_ACTION,	// アクション
		ID_MAX
	};

	CWorkerStateBase();
	~CWorkerStateBase() = default;

	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnExit() {}

	int GetID() const override { return m_ID; }

	void SetID(ID id) { m_ID = id; }
	void SetOwner(CWorker* pWorker) { m_pWorker = pWorker; }

protected:
	CWorker* m_pWorker;	// 指示アリのポインタ

private:
	ID m_ID;		// 列挙型IDのポインタ
};