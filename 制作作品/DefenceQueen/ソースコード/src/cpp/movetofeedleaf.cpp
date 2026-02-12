//===================================================================
//
// 餌に向かって進み続ける末端ノード処理 [ movetofeedleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "movetofeedleaf.h"
#include "array.h"

//===================================================================
// 更新関数
//===================================================================
void CMoveToFeedLeaf::Update(void)
{
	// アリを取得
	const auto& Array = m_pBlackBoard->GetValue<CArray*>("Array");
	if (!Array) m_Result = NodeInfo::NodeResult::Re_FAIL;

	// 目的地の座標を取得
	auto DestPos = m_pBlackBoard->GetValue<D3DXVECTOR3>("ArrayDestPos");

	// 追従関数
	Array->FollowDestination(DestPos);

	// 判別フラグ取得
	bool IsMove = m_pBlackBoard->GetValue<bool>("CheckNearFeed");

	if (IsMove)
		m_Result = NodeInfo::NodeResult::Re_SUCCESS;
	else
		m_Result = NodeInfo::NodeResult::Re_RUNING;
}