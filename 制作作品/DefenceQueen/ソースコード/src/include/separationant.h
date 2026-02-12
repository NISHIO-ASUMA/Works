//=========================================================
//
// 仲間アリを分隊ごとに持つクラスの処理 [ separationant.h ]
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
#include <list>
#include <vector>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CArray;
class CArrayManager;

//*********************************************************
// 分隊ごとに管理できる仲間アリのリストクラスを定義
//*********************************************************
class CSeparationAnt
{
public:

	CSeparationAnt();
	~CSeparationAnt();

	HRESULT Init(const int &nSeparetionNum);
	void Uninit(void);

	/// <summary>
	/// メンバのリストに追加する関数
	/// </summary>
	/// <param name="nSquadIndex">インデックス</param>
	/// <param name="pAnt">仲間アリのポインタ</param>
	void AddListAnt(const int& nSquadIndex, CArray* pAnt);

	/// <summary>
	/// グループに分けて登録する
	/// </summary>
	/// <param name="nGroup">グループインデックス</param>
	/// <param name="pAnt">そのグループに属するアリ</param>
	void AddToListGroup(const int& nGroup, CArray* pAnt);

	/// <summary>
	/// 帰ってきたインデックスリストのクリアと再形成関数
	/// </summary>
	/// <param name="nGroup">対象のグループ</param>
	/// <param name="pAnt">グループ内のアリ</param>
	void ClearListGroup(const int& nGroup, CArray* pAnt);

	/// <summary>
	/// リストの参照返し
	/// </summary>
	/// <param name="nIdx">要素番号</param>
	/// <returns></returns>
	std::list<CArray*>& GetSeqaudList(const int& nIdx) { return m_pAntList[nIdx]; }

	/// <summary>
	/// 分隊アリの参照返し
	/// </summary>
	/// <param name="nIdx">要素番号</param>
	/// <returns></returns>
	std::list<CArray*>& GetGroupListAnt(const int& nIdx) { return m_pAntGroup[nIdx]; }

private:

	std::vector<std::list<CArray*>>m_pAntList;	// 全体を見る動的リスト
	std::vector<std::list<CArray*>>m_pAntGroup;	// 分隊ごと動的リスト
};