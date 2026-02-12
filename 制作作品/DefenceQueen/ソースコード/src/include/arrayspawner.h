//=========================================================
//
// 各ポイントごとのスポーン処理 [ arrayspawner.h ]
// Author: Asuma Nishio
// 
// TODO : トップをどう持つか考える
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include <vector>
#include <array>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CArray;
class CArrayManager;
class CSeparationAnt;

//*********************************************************
// 一個のスポナーに対する処理のクラスを定義
//*********************************************************
class CArraySpawner
{	
public:

	CArraySpawner();
	~CArraySpawner();

	HRESULT Init(CArrayManager* pManager, const D3DXVECTOR3& MovePos);
	void Uninit(void);
	void Update(void);
	
	void SetMaxArray(const int& nMaxArray);
	inline void SetPos(const D3DXVECTOR3& pos) { m_SpawnBasePos = pos; }
	inline D3DXVECTOR3 GetPos(void) const { return m_SpawnBasePos; }

	/// <summary>
	/// 移動命令を出す関数
	/// </summary>
	/// <param name="nNum">移動させる数</param>
	/// <param name="destPos">移動先の座標</param>
	void OrderMove(int nNum, const D3DXVECTOR3& destPos);

	/// <summary>
	/// インデックス番号の加算
	/// </summary>
	/// <param name=""></param>
	void IncrementIdx(void) { m_nMySpawnIndexList++; }

	/// <summary>
	/// アリのインデックスを設定する
	/// </summary>
	/// <param name="pArray">仲間アリのポインタ</param>
	/// <returns></returns>
	int GetArrayIndex(CArray* pArray);

	/// <summary>
	/// 配列を分割する切れ目のカウントを設定する
	/// </summary>
	/// <param name="nCount"></param>
	/// <returns></returns>
	void SetSepaltionCount(const int& nCount) { m_nGetSepaltioncnt = nCount; }

	/// <summary>
	/// スポナーに格納されているアリをindexで取得
	/// </summary>
	/// <param name="index">指定番号</param>
	/// <returns></returns>
	CArray* GetArray(int index);

	/// <summary>
	/// ストックされているアリの数を取得する関数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetStockArray(void) const { return m_nStockArrays; }

	/// <summary>
	/// スポーンのインデックスリスト番号を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetIdx(void) const { return m_nMySpawnIndexList; }

	/// <summary>
	/// 先頭の仲間のアリを取得
	/// </summary>
	/// <returns></returns>
	CArray* GetTopArray() const;

	/// <summary>
	/// 最後尾のアリを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	CArray* GetLastActiveAnt(void);

	/// <summary>
	/// 移動命令を受けたアリの数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetIsMoveArrays(void);

	/// <summary>
	/// 帰還命令を受けたアリの数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetIsReturn(void);

	/// <summary>
	/// 停止命令を受けたアリの数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetIsStop(void);

	/// <summary>
	/// 配列に要素を登録する
	/// </summary>
	/// <param name="peturnArray">基地に帰ったアリのポインタ</param>
	void PushBack(CArray* peturnArray) { return m_ReturnAntList.push_back(peturnArray); }

	/// <summary>
	/// 分隊管理クラスのポインタを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline CSeparationAnt* GetSeparationAnt(void) { return m_pSeparationListAnt.get(); }

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">出現座標</param>
	/// <param name="nMaxArray">最大保持数</param>
	/// <param name="pManager">管理クラスのポインタ</param>
	/// <param name="MovePos">アリの移動先座標</param>
	/// <returns></returns>
	static CArraySpawner* Create
	(
		const D3DXVECTOR3& pos,
		const int nMaxArray,
		CArrayManager* pManager,
		const D3DXVECTOR3& MovePos,
		const int& nSepalations
	);

private:

	//*****************************
	// 定数構造体宣言
	//*****************************
	struct Config
	{
		static constexpr int ACTIVE_COUNT = 40; // アリをアクティブにする更新カウント
	};

	D3DXVECTOR3 m_SpawnBasePos;				// スポーンのベース座標
	std::vector<CArray*> m_AssignedArrays;	// 自分の担当アリ
	std::vector<CArray*>m_ReturnAntList;	// 帰ってきたアリたちの配列
	std::unique_ptr<CSeparationAnt>m_pSeparationListAnt;	// 分隊を管理するポインタ
	int m_nStockArrays;						// 1スポナーが保持できるアリの数
	int m_nMySpawnIndexList;				// 自分のスポナーのインデックス
	int m_nActiveCount;						// アクティブになるカウント
	int m_nGetSepaltioncnt;					// 配列分割カウント
};