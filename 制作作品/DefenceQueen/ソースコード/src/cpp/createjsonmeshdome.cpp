//===================================================================
//
// メッシュドームをJsonから生成する処理 [ createjsonmeshdome.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "createjsonmeshdome.h"
#include "meshdome.h"
#include "jsonconverter.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateMeshDome::Create(const json& Jsondata)
{
	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "Pos");

	// 半径の設定
	float fRadius = CJsonConverter::ConverterFloat(Jsondata, "Radius");

	// 実際のオブジェクト生成
	CMeshDome::Create(pos, fRadius);
}