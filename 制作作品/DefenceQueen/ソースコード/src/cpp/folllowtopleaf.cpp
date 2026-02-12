//===================================================================
//
// トップアリを追従する末端ノード処理 [ followtopleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "folllowtopleaf.h"
#include "array.h"
#include "topant.h"
#include "gamesceneobject.h"
#include "template.h"
#include "motioninstancing.h"

//===================================================================
// トップを追従する末端ノードの更新処理
//===================================================================
void CFolllowTopLeaf::Update(void)
{
	// アリを取得
	const auto& Array = m_pBlackBoard->GetValue<CArray*>("Array");

	// トップアリを取得
	const auto& TopAntPos = m_pBlackBoard->GetValue<D3DXVECTOR3>("TopPos");

	// 追従する関数を実行する
	Array->FollowTop(TopAntPos);

	// フラグを取得
	bool IsSetPoint = m_pBlackBoard->GetValue<bool>("SetPoint");

	if (IsSetPoint)
		m_Result = NodeInfo::NodeResult::Re_FAIL; // 次のノードへ移動
	else
		m_Result = NodeInfo::NodeResult::Re_RUNING; // 処理を継続
}