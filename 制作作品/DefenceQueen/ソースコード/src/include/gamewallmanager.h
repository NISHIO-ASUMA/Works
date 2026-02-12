//=========================================================
//
// ゲーム画面の壁複数管理クラス [ gamewallmanager.h ]
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
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CGameWallModel;

//*********************************************************
// ゲーム画面の壁一括管理クラスを定義
//*********************************************************
class CGameWallManager
{
public:

	CGameWallManager();
	~CGameWallManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配置されている壁の取得
	/// </summary>
	/// <param name="nIdx">取得する番号</param>
	/// <returns></returns>
	CGameWallModel* GetGameWall(const int nIdx) const { return m_pWall[nIdx]; }

	/// <summary>
	/// 配列のサイズを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetSize(void) const { return static_cast<int>(m_pWall.max_size()); }

private:

	//**********************************
	// 定数構造体宣言
	//**********************************
	struct Config
	{
		static constexpr int NUM_WALL = 4;									// 最大生成数
		static constexpr const char* FILE_NAME = "data/JSON/WallInfo.json"; // ファイルパス
	};


	void LoadJson(void); // json関数

	std::array<CGameWallModel*, Config::NUM_WALL>m_pWall;					// 確保する配列情報
};