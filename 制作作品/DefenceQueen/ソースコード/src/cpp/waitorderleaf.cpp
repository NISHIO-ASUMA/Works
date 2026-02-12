//===================================================================
//
// 待機状態時で外部からの命令を待つ末端ノードの処理 [ waitorderleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "waitorderleaf.h"

//===================================================================
// 更新処理
//===================================================================
void CWaitOrderLeaf::Update(void)
{
	//int n = 0;
#if 0 // TODO : ここのフラグ変数を変更する
	// 現在のフラグ状態を取得
	bool IsUseOrder = false;

	if (m_pBlackBoard->HasKeyData("GetTopOrder"))
		IsUseOrder = m_pBlackBoard->GetValue<bool>("GetTopOrder");

	// もし有効なら次のノードに移行する
	if (IsUseOrder)
		m_Result = NodeInfo::NodeResult::Re_SUCCESS; // 成功
	else
		m_Result = NodeInfo::NodeResult::Re_RUNING; // 継続中
#endif
}