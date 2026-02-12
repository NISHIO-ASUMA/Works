//=========================================================
//
// テクスチャ処理 [ texture.h ]
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
#include <string>

//*********************************************************
// テクスチャ管理クラスを定義
//*********************************************************
class CTexture
{
public:

	CTexture();
	~CTexture();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// テクスチャを配列に登録する
	/// </summary>
	/// <param name="pFileName">登録名</param>
	/// <returns></returns>
	int Register(const char* pFileName);

	/// <summary>
	/// 指定インデックスのアドレスを取得する
	/// </summary>
	/// <param name="nIdx">テクスチャ登録番号</param>
	/// <returns></returns>
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:

	HRESULT LoadJson(void);		// json読み込み関数

	static constexpr int NUM_TEXTURE = 128;	// 最大テクスチャ数
	
	//*************************
	// テクスチャデータ構造体
	//*************************
	struct TEXDATA
	{
		std::string TexName;		// テクスチャ名
		LPDIRECT3DTEXTURE9 TexData; // テクスチャポインタ
	};

	TEXDATA m_pTextureData[NUM_TEXTURE]; // テクスチャ管理配列
	static int m_nNumAll;				 // テクスチャカウント変数
};