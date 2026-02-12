//===================================================================
//
// 順番にタスクをこなすシーケンスノードクラス [ sequence.h ]
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
#include "compositenodebase.h"

//*******************************************************************
// 前方宣言
//*******************************************************************
class CBlackBoard;

//*******************************************************************
// シーケンスクラスを定義
//*******************************************************************
class CSequence : public CCompositeNodeBase
{
public:

	CSequence(CBlackBoard* blackboard) : CCompositeNodeBase(blackboard) {};
	~CSequence() override = default; 

	void Update() override
	{
		// 現在の子ノードの更新を実行する
		m_ChildeNode[m_RanningIndex]->Update();

		// ノードの結果を取得する
		auto noderesult = m_ChildeNode[m_RanningIndex]->get_node_result();

		// ノード状態が"成功"なら
		if (noderesult == NodeInfo::NodeResult::Re_SUCCESS)
		{
			// 次のシーケンスのため番号を加算し、処理を返す
			NodeIncrement();
			return;
		}

		// ノード状態が"失敗"なら
		if (noderesult == NodeInfo::NodeResult::Re_FAIL)
		{
			// 終了処理実行
			Exit();
		}

		// ノードの状態に代入する
		m_NodeResult = noderesult;
	}

private:
	
	// 次のノードのインデックス番号取得関数
	const int GetNextIdx() const override
	{
		return m_RanningIndex + 1;
	};
};