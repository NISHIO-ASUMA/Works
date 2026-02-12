//=========================================================
//
// 仲間クラス全体の管理処理 [ arraymanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "arraymanager.h"
#include "array.h"
#include "manager.h"
#include "input.h"
#include "template.h"
#include "debugproc.h"
#include "topant.h"
#include "gamesceneobject.h"
#include "motioninstancing.h"
#include "extractantsignal.h"

//=========================================================
// コンストラクタ
//=========================================================
CArrayManager::CArrayManager() : m_pArrays{},
m_nActiveAll(NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CArrayManager::~CArrayManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CArrayManager::Init(CTopAnt* pTop)
{
	// 配列初期化
	m_pArrays.clear();

	// 構造体変数
	ArrayConfig config = {};

	// メモリの箱だけ先に確保
	m_pArrays.reserve(config.ALLARRAYS);

	// ポインタのセット
	for (int nCnt = 0; nCnt < config.ALLARRAYS; nCnt++)
	{
		// 仲間のアリを生成
		auto pArray = CArray::Create(VECTOR3_NULL,VECTOR3_NULL, config.LIFE);

		// トップアリのポインタをセット
		pArray->SetTopAntPointer(pTop);

		// ノードを設定
		pArray->NodeSetting();

		// 配列に追加
		m_pArrays.push_back(pArray);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CArrayManager::Uninit(void)
{
	// 動的配列の破棄
	m_pArrays.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CArrayManager::Update(void)
{

}
//=========================================================
// 描画処理
//=========================================================
void CArrayManager::Draw(void)
{
#ifdef _DEBUG
	// デバッグ表示
	CDebugproc::Print("最大アリ数 : [ %d ]", ArrayConfig::ALLARRAYS);
	CDebugproc::Draw(0, 240);
#endif // _DEBUG
}
//=========================================================
// 各スポナーに渡す関数
//=========================================================
std::vector<CArray*> CArrayManager::Allocate(const int& nStock,const int &nSatrtIdx)
{
	// ローカル配列
	std::vector<CArray*> result;
	result.reserve(nStock);

	// 生成カウント数
	int nCreateNum = 0;

	// 要素の開始をインデックス参照にする
	for (int nCnt = nSatrtIdx; nCnt < static_cast<int>(m_pArrays.size());nCnt++)
	{
		// 変数格納
		auto pArray = m_pArrays[nCnt];

		// 未使用なら
		if (!pArray->GetActive())
		{
			// スポナーに割り当てる
			pArray->SetActive(true);
			result.push_back(pArray);
			nCreateNum++;
		}
		
		// 一定数生成したら抜ける
		if (nCreateNum >= nStock) break;
	}

	// 要素数を返す
	return result;
}
//=========================================================
// 切り離し可能を通知する
//=========================================================
void CArrayManager::MessageSepalation(const D3DXVECTOR3& CenterPos, float fRadius)
{
	// 0以下なら
	if (fRadius <= 0.0f) return;

	for (auto pArray : m_pArrays)
	{
		// nullなら
		if (!pArray) continue;

		// 座標を取得
		D3DXVECTOR3 pos = pArray->GetPos();
		D3DXVECTOR3 posdiff = CenterPos - pos;

		// 長さを取得
		float fDistance = D3DXVec3Length(&posdiff);

		if (fDistance < fRadius)
		{
			if (pArray->GetIsTopOrder()) continue;

			// 座標設定	
			pArray->GetSignal()->SetPos(D3DXVECTOR3(pArray->GetPos().x, pArray->GetPos().y + 120.0f, pArray->GetPos().z));

			// 描画フラグを有効化する
			pArray->GetSignal()->SetisDraw(true);
		}
		else
		{
			// 描画フラグを無効化する
			pArray->GetSignal()->SetisDraw(false);
		}
	}
}
//=========================================================
// 仲間アリの切り離しを実行する
//=========================================================
void CArrayManager::ApplySeparation(const D3DXVECTOR3& CenterPos, float fRadius)
{
	// 0以下なら
	if (fRadius <= 0.0f) return;

	for (auto pArray : m_pArrays)
	{
		// nullなら
		if (!pArray) continue;

		// 座標を取得
		D3DXVECTOR3 pos = pArray->GetPos();
		D3DXVECTOR3 posdiff = CenterPos - pos;

		// 長さを取得
		float fDis = D3DXVec3Length(&posdiff);

		if (fDis < fRadius)
		{
			// 仲間アリに伝える
			pArray->SetIsTopOrder(true);

			// 描画をoffにする
			pArray->GetSignal()->SetisDraw(false);
		}
	}
}
//=========================================================
// エリア内に配置する関数
//=========================================================
void CArrayManager::PuttingArea(const D3DXVECTOR3& putpos)
{
	// 有効 かつ セットフラグが有効なものに通知
	for (auto pArray : m_pArrays)
	{
		// nullなら
		if (!pArray) continue;
		if (!pArray->GetIsTopOrder()) continue;
		if (!pArray->GetMove()) continue;

		// 対象のアリに命令を実行
		pArray->SetIsPointFlag(true);
		pArray->SetPos(putpos);
		pArray->SetRot(VECTOR3_NULL);
		pArray->SetIsMove(false);
	}
}
//=========================================================
// 配列のアクティブ数をセットする
//=========================================================
void CArrayManager::CountActiveArrays(const int& nCountArrays)
{
	m_nActiveAll += nCountArrays;
}
//=========================================================
// トップの命令を受けているアリの数を返す
//=========================================================
int CArrayManager::GetIsFollowtopArrays(void)
{
	// サイズカウント用変数
	int nCnt = 0;

	// 最大サイズから判別する
	for (auto Ant : m_pArrays)
	{
		// アクティブじゃないなら
		if (!Ant->GetActive()) continue;

		// オーダーを受けていなかったら
		if (!Ant->GetIsTopOrder()) continue;

		// カウントを加算
		nCnt++;
	}

	return nCnt;
}
//=========================================================
// フラグをセットする
//=========================================================
void CArrayManager::SetActiveAll(void)
{
	for (int nCnt = 0; nCnt < m_nActiveAll; nCnt++)
	{
		// すでにアクティブなら
		if (m_pArrays[nCnt]->GetActive()) continue;

		// 指定数だけアクティブ数に設定する
		m_pArrays[nCnt]->SetActive(true);
	}
}