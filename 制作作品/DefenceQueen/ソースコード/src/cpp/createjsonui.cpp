//=========================================================
//
// UIオブジェクトをJsonから生成する処理 [ createjsonui.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//**********************************************************
// インクルードファイル
//**********************************************************
#include "createjsonui.h"
#include "jsonconverter.h"
#include "ui.h"

//==========================================================
// 生成処理
//==========================================================
void CJsonCreateUi::Create(const json& Jsondata)
{
	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "Pos");

	// 点滅間隔の設定
	int nFrashFrame = CJsonConverter::ConverterInt(Jsondata, "FlashFrame");

	// サイズ情報の設定
	float fWidth = CJsonConverter::ConverterFloat(Jsondata,"Width");
	float fHeight = CJsonConverter::ConverterFloat(Jsondata, "Height");

	// テクスチャ情報の設定
	std::string Texture = CJsonConverter::ConverterPath(Jsondata, "Texture");

	// UIオブジェクトクラスの生成
	CUi::Create(pos, nFrashFrame,fWidth,fHeight,Texture.c_str());
}