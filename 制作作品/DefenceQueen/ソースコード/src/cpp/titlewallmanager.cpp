//=========================================================
//
// ゲーム画面の壁複数管理クラス [ gamewallmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "titlewallmanager.h"
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
CTitleWallManager::CTitleWallManager() : m_pWall{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CTitleWallManager::~CTitleWallManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleWallManager::Init(void)
{
	// jsonデータ取得
	LoadJson();

    return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleWallManager::Uninit(void)
{
	
}
//=========================================================
// jsonファイル読み込み
//=========================================================
void CTitleWallManager::LoadJson(void)
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
		m_pWall[nIdx] = CGameWallModel::Create(pos, rot, scale, filepath.c_str());
	}
}