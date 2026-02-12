//=========================================================
//
// ブロック管理処理 [ blockmanager.h ]
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
#include "block.h"
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// ブロック一括管理クラスを定義
//*********************************************************
class CBlockManager
{
public:

	CBlockManager();
	~CBlockManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pBlocks.size()); }

	/// <summary>
	/// 配列の中のブロックを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CBlock* GetBlock(const int nIdx) { return m_pBlocks[nIdx]; }

private:

	static constexpr const char* FILE_NAME = "data/JSON/Map.json"; // ファイル名
	static constexpr const char* RESULTFILE_NAME = "data/JSON/ResultMap.json"; // ファイル名

	HRESULT Load(void);
	CBlock* CreateManager(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, const char* pModelName);

	std::vector<CBlock*>m_pBlocks; // 配置するブロックの動的配列
};