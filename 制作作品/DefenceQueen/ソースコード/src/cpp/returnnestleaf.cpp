//===================================================================
//
// 基地に帰還する末端ノード処理 [ returnnestleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "returnnestleaf.h"
#include "array.h"

//===================================================================
// 更新処理
//===================================================================
void CReturnNestLeaf::Update(void)
{
	// アリを取得
	const auto& Array = m_pBlackBoard->GetValue<CArray*>("Array");
	if (!Array) m_Result = NodeInfo::NodeResult::Re_FAIL;
	
	// 基地に帰る関数を設定
	Array->SpawnReturn();

	// 到着判定
	if (Array->GetIsAtBase())
		m_Result = NodeInfo::NodeResult::Re_SUCCESS;
	else
		m_Result = NodeInfo::NodeResult::Re_RUNING;
}