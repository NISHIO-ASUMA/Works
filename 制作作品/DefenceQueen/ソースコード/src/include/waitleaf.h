//=========================================================
//
// 待機状態の末端ノード処理 [ waitleaf.h ]
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
#include "leafnodebase.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// 待機状態の処理をするノードクラスを定義
//*********************************************************
class CWaitLeaf : public CLeafNodeBase
{

public:

	CWaitLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) 
	{
		m_fWaitTime = 60.0f;
		m_fWaitCount = 60.0f;
	}

	~CWaitLeaf() = default;

	void Update() override
	{
		// 時間が0以下なら
		if (m_fWaitCount <= 0.0f)
		{
			// 成功結果に変更
			m_NodeResult = NodeInfo::NodeResult::Re_SUCCESS;
			return;
		}

		// カウントを減算
		m_fWaitCount = -1.0f;
	}

	void Exit() override
	{
		// 親クラスの終了処理
		CNodeBase::Exit();

		// 時間のカウントをもとに戻す
		m_fWaitCount = m_fWaitTime;
	}

private:
	float m_fWaitTime = 0.0f;
	float m_fWaitCount = 0.0f;
};