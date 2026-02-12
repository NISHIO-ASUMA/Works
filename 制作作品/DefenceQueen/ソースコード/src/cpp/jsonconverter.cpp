//=========================================================
//
// Jsonデータ変換用ユーティリティ関数クラス [ jsonconverter.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "jsonconverter.h"

//=========================================================
// VECTOR3型の変数情報を返す
//=========================================================
D3DXVECTOR3 CJsonConverter::ConverterVec3(const JsonUtil::json& JsonData, const char* KeyName)
{
	D3DXVECTOR3 Localvec3 = VECTOR3_NULL;

	// jsonから読み取った値を反映
	Localvec3.x = JsonData[KeyName][0];
	Localvec3.y = JsonData[KeyName][1];
	Localvec3.z = JsonData[KeyName][2];

	return Localvec3;
}
//=========================================================
// int型の変数情報を返す
//=========================================================
int CJsonConverter::ConverterInt(const JsonUtil::json& JsonData, const char* KeyName)
{
	int nLocalint = NULL;

	// jsonから読み取った値を反映
	nLocalint = JsonData[KeyName];

	return nLocalint;
}
//=========================================================
// float型の変数情報を返す
//=========================================================
float CJsonConverter::ConverterFloat(const JsonUtil::json& JsonData, const char* KeyName)
{
	float fLocalfloat = 0.0f;

	// jsonから読み取った値を反映
	fLocalfloat = JsonData[KeyName];

	return fLocalfloat;
}
//=========================================================
// bool型の変数情報を返す
//=========================================================
bool CJsonConverter::ConverterBool(const JsonUtil::json& JsonData, const char* KeyName)
{
	bool isLocalFlag = false;

	// jsonから読み取った値を反映
	isLocalFlag = JsonData[KeyName];

	return isLocalFlag;
}
//=========================================================
// std::string型の変数情報を返す
//=========================================================
std::string CJsonConverter::ConverterPath(const JsonUtil::json& JsonData, const char* KeyName)
{
	std::string localString = {};

	// jsonから読み取った値を反映
	localString = JsonData[KeyName];

	return localString;
}