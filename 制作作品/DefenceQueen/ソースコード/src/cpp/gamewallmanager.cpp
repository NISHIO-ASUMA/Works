//=========================================================
//
// ゲーム画面の壁複数管理クラス [ gamewallmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "gamewallmanager.h"
#include "gamewallmodel.h"
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
CGameWallManager::CGameWallManager() : m_pWall{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CGameWallManager::~CGameWallManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameWallManager::Init(void)
{
	// 外部ファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameWallManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CGameWallManager::Update(void)
{

}
//=========================================================
// 読み込み処理
//=========================================================
void CGameWallManager::LoadJson(void)
{
	// 開くファイルを指定
	std::ifstream file(Config::FILE_NAME);

	// 開け無かった
	if (!file.is_open())
	{
		// 例外メッセージボックス
		MessageBox(GetActiveWindow(), "ファイルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONERROR);

		// 終了
		return;
	}

	// jsonデータ読み込み
	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// SetObjectsの配列を回す
	for (const auto& b : j["SetObjects"])
	{
		// ファイルパス
		std::string filepath = b["filepath"];
		int nIdx = b["idx"];

		// 座標
		D3DXVECTOR3 pos
		(
			b["pos"][0],
			b["pos"][1],
			b["pos"][2]
		);

		// 角度
		D3DXVECTOR3 rot
		(
			b["rot"][0],
			b["rot"][1],
			b["rot"][2]
		);

		// サイズ
		D3DXVECTOR3 scale
		(
			b["scale"][0],
			b["scale"][1],
			b["scale"][2]
		);

		// 用意している配列に追加
		m_pWall[nIdx] = CGameWallModel::Create(pos,rot, scale,filepath.c_str());
	}
}