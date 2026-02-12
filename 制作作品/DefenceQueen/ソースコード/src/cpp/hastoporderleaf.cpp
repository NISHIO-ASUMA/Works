//===================================================================
//
// 命令を受けているか判別する末端ノード処理 [ hastoporderleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#include "hastoporderleaf.h"
#include "array.h"

//===================================================================
// 更新処理
//===================================================================
void CHasTopOrderLeaf::Update()
{
	// キーが無ければ失敗
	if (!m_pBlackBoard->HasKeyData("GetTopOrder"))
	{
		m_Result = NodeInfo::NodeResult::Re_FAIL;
		return;
	}

	// 判別値を取得
	bool hasOrder = m_pBlackBoard->GetValue<bool>("GetTopOrder");

	// 判定
	if (hasOrder)
		m_Result = NodeInfo::NodeResult::Re_SUCCESS;
	else
		m_Result = NodeInfo::NodeResult::Re_FAIL;
}