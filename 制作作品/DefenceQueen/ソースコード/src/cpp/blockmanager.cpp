//=========================================================
//
// ブロック管理処理 [ blockmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "blockmanager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json; // jsonオブジェクト

//=========================================================
// コンストラクタ
//=========================================================
CBlockManager::CBlockManager() : m_pBlocks{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CBlockManager::~CBlockManager()
{
	Uninit();
}
//=========================================================
// ブロック生成関数
//=========================================================
CBlock* CBlockManager::CreateManager(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, const char* pModelName)
{
	// インスタンス生成
	CBlock* pNewBlock = CBlock::Create(pos, rot, scale, pModelName);

	if (pNewBlock)
	{
		// 配列に追加
		m_pBlocks.push_back(pNewBlock);
	}

	return pNewBlock;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBlockManager::Init(void)
{
	// 配列のクリア
	m_pBlocks.clear();

	// ファイル読み込み
	Load();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBlockManager::Uninit(void)
{
	// 配列のクリア
	m_pBlocks.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CBlockManager::Update(void)
{
	// 無し
}
//=========================================================
// json読み込み
//=========================================================
HRESULT CBlockManager::Load(void)
{
	// 開くファイル
	std::ifstream file(FILE_NAME);

	// 開け無かった
	if (!file.is_open())
	{
		// 例外
		MessageBox(GetActiveWindow(), "ファイルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONERROR);

		// 終了
		return E_FAIL;
	}

	// jsonデータをセットする
	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// 既存のブロックを消す
	for (auto block : m_pBlocks)
	{
		if (block != nullptr)
		{
			// ブロックの終了処理
			block->Uninit();
		}
	}

	// 動的配列を空にする
	m_pBlocks.clear();

	// SetObjectsの配列を回す
	for (const auto& b : j["SetObjects"])
	{
		// ファイルパス
		std::string filepath = b["filepath"];
		int idx = b["idx"];

		// 座標
		D3DXVECTOR3 pos(
			b["pos"][0],
			b["pos"][1],
			b["pos"][2]
		);

		// 角度
		D3DXVECTOR3 rot(
			b["rot"][0],
			b["rot"][1],
			b["rot"][2]
		);

		// サイズ
		D3DXVECTOR3 size(
			b["scale"][0],
			b["scale"][1],
			b["scale"][2]
		);

		// 実際のブロック生成
		CreateManager(pos, rot, size, filepath.c_str());
	}

	return S_OK;
}