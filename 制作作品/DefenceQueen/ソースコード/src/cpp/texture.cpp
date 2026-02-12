//=========================================================
//
// テクスチャ処理 [ texture.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "manager.h"
#include "json.hpp"
#include <fstream>

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json;

//*********************************************************
// 静的メンバ変数
//*********************************************************
int CTexture::m_nNumAll = NULL;	// 総数管理

//=========================================================
// コンストラクタ
//=========================================================
CTexture::CTexture() : m_pTextureData{}
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CTexture::~CTexture()
{
	// すべての破棄
	UnLoad();
}
//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CTexture::Load(void)
{
	// json適用
	LoadJson();

	return S_OK;
}
//=========================================================
// テクスチャ破棄
//=========================================================
void CTexture::UnLoad(void)
{
	// すべてのテクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		// テクスチャの破棄
		if (m_pTextureData[nCnt].TexData)
		{
			m_pTextureData[nCnt].TexData->Release();
			m_pTextureData[nCnt].TexData = nullptr;
		}

		// 配列のクリア
		m_pTextureData[nCnt].TexName.clear();
	}

	// 総数初期化
	m_nNumAll = 0;
}
//=========================================================
// テクスチャ番号を登録する
//=========================================================
int CTexture::Register(const char* pFileName)
{
	// もしテクスチャ名がnullなら
	if (pFileName == nullptr) return -1;

	// すでに登録済みならその番号を返す
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		if (m_pTextureData[nCnt].TexName == pFileName)
			return nCnt;
	}

	// なかった場合,新規登録
	if (m_nNumAll < NUM_TEXTURE)
	{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// テクスチャの作成に成功したら
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, pFileName, &m_pTextureData[m_nNumAll].TexData)))
		{
			// 配列に保存
			m_pTextureData[m_nNumAll].TexName = pFileName;

			// 加算インデックスを返す
			return m_nNumAll++;
		}
	}

	return -1;
}
//=========================================================
// テクスチャ番号取得
//=========================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	// 例外処理
	if (nIdx < NULL || nIdx >= NUM_TEXTURE) return nullptr;

	// テクスチャ番号を取得
	return m_pTextureData[nIdx].TexData;
}
//=========================================================
// jsonファイル読み込み
//=========================================================
HRESULT CTexture::LoadJson(void)
{
	// JSONファイルを開く
	std::ifstream ifs("data/JSON/Texture.json");

	// ファイルが開けなかったら
	if (!ifs.is_open())
	{
		MessageBox(GetActiveWindow(), "jsonファイルを開けません", "エラー", MB_OK);

		return E_FAIL;
	}

	// JSON読み込み
	json j;
	ifs >> j;

	// 文字列があるかチェック
	if (j.is_null() || j.empty())
	{
		MessageBox(GetActiveWindow(), "テクスチャファイルがありません", "エラー", MB_OK);

		return E_FAIL;
	}

	// 値の初期化
	m_nNumAll = 0;

	// テクスチャ読み込んで登録をする
	for (const auto& tex : j)
	{
		// "Texture"キーが存在しない または nullならスキップ
		if (!tex.contains("Texture") || tex["Texture"].is_null())
			continue;

		// ファイル名格納
		std::string file = tex["Texture"].get<std::string>();

		// 最大値を超えていたら または ファイル名がなかったら
		if (file.empty() || m_nNumAll >= NUM_TEXTURE)
			continue;

		// テクスチャ名を配列のデータに格納
		m_pTextureData[m_nNumAll].TexName = file;

		// 総数を加算
		m_nNumAll++;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return E_FAIL;

	// 登録されたファイルを読み込む
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// テクスチャを生成
		HRESULT hr = D3DXCreateTextureFromFile
		(
			pDevice,
			m_pTextureData[nCnt].TexName.c_str(),
			&m_pTextureData[nCnt].TexData
		);

		// 作成失敗
		if (FAILED(hr))
		{
			// nullにする
			m_pTextureData[nCnt].TexData = nullptr;
		}
	}

	return S_OK;
}