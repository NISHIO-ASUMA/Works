//===================================================================
//
// 仲間アリを分隊ごとに持つクラスの処理 [ separationant.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "separationant.h"
#include "debugproc.h"
#include "array.h"

//===================================================================
// コンストラクタ
//===================================================================
CSeparationAnt::CSeparationAnt() : m_pAntList{},
m_pAntGroup{}
{

}
//===================================================================
// デストラクタ
//===================================================================
CSeparationAnt::~CSeparationAnt()
{

}
//===================================================================
// 初期化処理
//===================================================================
HRESULT CSeparationAnt::Init(const int& nSeparetionNum)
{
	// 格納するアリのリストの最大サイズを指定する
	m_pAntList.resize(nSeparetionNum);

	return S_OK;
}
//===================================================================
// 終了処理
//===================================================================
void CSeparationAnt::Uninit(void)
{
	// 配列のクリア
	m_pAntList.clear();
}
//===================================================================
// リストに隊列アリを追加する関数
//===================================================================
void CSeparationAnt::AddListAnt(const int& nSquadIndex, CArray* pAnt)
{
	// 0以下 または 配列オーバーなら
	if (nSquadIndex < 0 || nSquadIndex >= static_cast<int>(m_pAntList.size())) return;

	// リスト要素を格納
	auto& listRef = m_pAntList[nSquadIndex];

	if (!listRef.empty())
	{
		// 最後尾のアリを追従対象にする
		CArray* prev = listRef.back();

		// 前のアリを設定
		pAnt->SetPrevAnt(prev);
	}
	else
	{
		// 先頭のアリなし
		pAnt->SetPrevAnt(nullptr);
	}

	// 要素を配列に追加
	listRef.push_back(pAnt);
}
//===================================================================
// リスト群の追加を行う関数
//===================================================================
void CSeparationAnt::AddToListGroup(const int& nGroup, CArray* pAnt)
{
	// サイズチェック
	if (nGroup >= static_cast<int>(m_pAntGroup.size()))
		m_pAntGroup.resize(nGroup + 1);

	// 参照リスト変数
	auto& listRef = m_pAntGroup[nGroup];

	// 前後のアリの設定をする
	if (!listRef.empty())
	{
		// 最後尾に追加
		CArray* prev = listRef.back();

		// 追従対象を設定
		pAnt->SetPrevAnt(prev);
	}
	else
	{
		pAnt->SetPrevAnt(nullptr);
	}

	// 配列の要素に格納
	listRef.push_back(pAnt);
}
//===================================================================
// リスト群のクリアを行う関数
//===================================================================
void CSeparationAnt::ClearListGroup(const int& nGroup, CArray* pAnt)
{
	// 番号が範囲外なら
	if (nGroup < 0 || nGroup >= static_cast<int>(m_pAntGroup.size())) return;

	// 参照変数
	auto& listRef = m_pAntGroup[nGroup];

	// アリをリストの要素から削除
	listRef.remove(pAnt);

	// 再設定する
	CArray* prev = nullptr;

	for (auto* Ant : listRef)
	{
		// 追従対象をセット
		Ant->SetPrevAnt(prev);

		// 変更
		prev = Ant;
	}
}