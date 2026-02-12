//=========================================================
//
// 各ポイントごとのスポーン処理 [ arrayspawner.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "arrayspawner.h"
#include "array.h"
#include "arraymanager.h"
#include "topant.h"
#include "separationant.h"
#include "debugproc.h"

//=========================================================
// コンストラクタ
//=========================================================
CArraySpawner::CArraySpawner() :
m_SpawnBasePos(VECTOR3_NULL),
m_nStockArrays(NULL), 
m_nActiveCount(NULL),
m_nGetSepaltioncnt(NULL),
m_AssignedArrays{},
m_ReturnAntList{},
m_nMySpawnIndexList(-1)
{

}
//=========================================================
// デストラクタ
//=========================================================
CArraySpawner::~CArraySpawner()
{

}
//=========================================================
// 生成関数
//=========================================================
CArraySpawner* CArraySpawner::Create(const D3DXVECTOR3& pos, const int nMaxArray, CArrayManager* pManager, const D3DXVECTOR3& MovePos,const int& nSepalations)
{
	// インスタンス生成
	CArraySpawner* pSpawner = new CArraySpawner;
	if (pSpawner == nullptr) return nullptr;

	// 値のセット
	pSpawner->SetMaxArray(nMaxArray);
	pSpawner->SetPos(pos);
	pSpawner->SetSepaltionCount(nSepalations);

	// 初期化失敗時
	if (FAILED(pSpawner->Init(pManager,MovePos))) return nullptr;

	return pSpawner;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CArraySpawner::Init(CArrayManager* pManager,const D3DXVECTOR3& MovePos)
{
	// Managerから渡された最大配列のうちのストック数分だけ持つ
	m_AssignedArrays = pManager->Allocate(m_nStockArrays, m_nGetSepaltioncnt);

	// セットされた数だけアクティブにする
	for (int nCnt = 0; nCnt < m_nStockArrays; nCnt++)
	{
		CArray* pArray = m_AssignedArrays[nCnt];

		// 座標セット
		pArray->SetPos(m_SpawnBasePos);
		pArray->SetActivePos(m_SpawnBasePos);
		pArray->SetDestPos(MovePos);
		pArray->SetSaveDestPos(MovePos);
		
		if (nCnt != 0)
		{
			// 先頭以外を未使用状態にする
			pArray->SetActive(false);
		}
	}
	
	// 分隊用リスト生成
	m_pSeparationListAnt = std::make_unique<CSeparationAnt>();
	m_pSeparationListAnt->Init(200);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CArraySpawner::Uninit(void)
{
	// ポインタの破棄
	m_pSeparationListAnt.reset();
}
//=========================================================
// 更新処理
//=========================================================
void CArraySpawner::Update(void)
{
	// カウント加算
	m_nActiveCount++;

	// 一定カウント後に配列内のアリをアクティブに変更する
	if (m_nActiveCount >= Config::ACTIVE_COUNT)
	{
		for (int nCnt = 0; nCnt < m_nStockArrays; nCnt++)
		{
			// ローカルポインタに格納
			CArray* pArray = m_AssignedArrays[nCnt];
			if (pArray->GetActive()) continue;

			// 有効化する
			pArray->SetActive(true);

			// 開始初期値に変更する
			m_nActiveCount = NULL;
			break;
		}
	}
}
//=========================================================
// 移動先設定関数
//=========================================================
void CArraySpawner::OrderMove(int nNum, const D3DXVECTOR3& destPos)
{
	int nSend = 0;

	for (int nCnt = 0; nCnt < static_cast<int>(m_AssignedArrays.size()); nCnt++)
	{
		// 最大なら
		if (nSend >= nNum) break;

		// 配列取得
		auto pArray = m_AssignedArrays[nCnt];
		if (!pArray) continue;

		// 使われてない物はスキップ
		if (!pArray->GetActive()) continue;
		if (!pArray->GetIsAtBase()) continue;
		if (pArray->GetMove()) continue;
		if (pArray->GetisStop()) continue;

		// 移動フラグをセット
		pArray->SetIsMove(true);

		// 基地から出た判定にする
		pArray->SetAtBase(false);

		// 向かう目的地をセット
		pArray->SetDestPos(destPos);

		// 仲間自身の登録番号を設定する
		pArray->SetMyListId(m_nMySpawnIndexList);

		// 分隊のリストに追加する
		m_pSeparationListAnt->AddToListGroup(m_nMySpawnIndexList, pArray);

		nSend++;
	}
}
//=========================================================
// 格納数設定処理
//=========================================================
void CArraySpawner::SetMaxArray(const int& nMaxArray)
{
	// スポナー一個の要素数をセットする
	m_nStockArrays = nMaxArray;
}

//=========================================================
// 配列のトップの仲間アリを返す
//=========================================================
CArray* CArraySpawner::GetTopArray() const
{
	return m_AssignedArrays[0]; // 先頭の配列
}
//=========================================================
// 動いているアリの最後尾を返す
//=========================================================
CArray* CArraySpawner::GetLastActiveAnt(void)
{
	// 最後尾配列から回す
	for (int nAnt = m_nStockArrays -1;nAnt >= 0;nAnt--)
	{
		if (!m_AssignedArrays[nAnt]->GetActive()) continue;
		if (!m_AssignedArrays[nAnt]->GetMove()) continue;

		// 移動かつアクティブなら
		if (m_AssignedArrays[nAnt]->GetActive() && m_AssignedArrays[nAnt]->GetMove())
		{
			// そのアリの番号を取得
			return m_AssignedArrays[nAnt];
		}
	}

	// 一匹のアリもついていない
	return nullptr;
}
//=========================================================
// 移動命令がonのアリの数を返す
//=========================================================
int CArraySpawner::GetIsMoveArrays(void)
{
	// カウント数
	int nActiveNum = 0;

	for (int nCnt = 0; nCnt < static_cast<int>(m_AssignedArrays.size()); nCnt++)
	{
		// アクティブなら加算
		if (m_AssignedArrays[nCnt]->GetMove()) nActiveNum++;
	}

	// 総カウント数を返す
	return nActiveNum;
}
//=========================================================
// 帰還命令がonのアリの数を返す
//=========================================================
int CArraySpawner::GetIsReturn(void)
{
	// カウント数
	int nActiveNum = 0;

	for (int nCnt = 0; nCnt < static_cast<int>(m_AssignedArrays.size()); nCnt++)
	{
		// 停止なら加算
		if (m_AssignedArrays[nCnt]->GetReturn()) nActiveNum++;
	}

	// 総カウント数を返す
	return nActiveNum;
}
//=========================================================
// 停止命令がonのアリの数を返す
//=========================================================
int CArraySpawner::GetIsStop(void)
{
	// カウント数
	int nActiveNum = 0;

	for (int nCnt = 0; nCnt < static_cast<int>(m_AssignedArrays.size()); nCnt++)
	{
		// 停止なら加算
		if (m_AssignedArrays[nCnt]->GetisStop()) nActiveNum++;
	}

	// 総カウント数を返す
	return nActiveNum;
}
//=========================================================
// インデックスを取得する
//=========================================================
int CArraySpawner::GetArrayIndex(CArray* pArray)
{
	// アリの格納配列で回す
	for (int nCnt = 0; nCnt < static_cast<int>(m_AssignedArrays.size()); nCnt++)
	{
		// 一致したら
		if (m_AssignedArrays[nCnt] == pArray) return nCnt;
	}

	return -1;
}
//=========================================================
// 配列の中からインデックスを指定して取得
//=========================================================
CArray* CArraySpawner::GetArray(int index)
{
	if (index < NULL || index >= static_cast<int>(m_AssignedArrays.size())) return nullptr;
	return m_AssignedArrays[index];
}