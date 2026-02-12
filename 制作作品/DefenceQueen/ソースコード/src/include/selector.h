//=========================================================
//
// 行動を選択するノードのクラス [ selector.h ]
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
#include "compositenodebase.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// 選択ノードクラスを定義
//*********************************************************
class CSelector : public CCompositeNodeBase
{
public:

	CSelector(CBlackBoard* blackboard) : CCompositeNodeBase(blackboard) {};
	~CSelector() override = default;

	void Update() override
	{
		// 現在ノードの更新処理
		m_ChildeNode[m_RanningIndex]->Update();

		// ノードの状態取得
		auto noderesult = m_ChildeNode[m_RanningIndex]->get_node_result();

		// 結果が"失敗"なら
		if (noderesult == NodeInfo::NodeResult::Re_FAIL)
		{
			// 次のノード番号のために加算する
			NodeIncrement();
		}

		// 結果が"成功"なら
		if (noderesult == NodeInfo::NodeResult::Re_SUCCESS)
		{
			// ノードを終了する
			Exit();
		}

		// ノードの状態をセットする
		m_NodeResult = noderesult;
	}

private:

	// 次のノードのインデックス番号取得関数
	const int GetNextIdx() const override
	{
		return m_RanningIndex + 1;
	};
};