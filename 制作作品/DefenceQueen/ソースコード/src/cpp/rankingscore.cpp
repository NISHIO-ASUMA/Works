//=========================================================
//
// ランキングスコア処理 [ rankingscore.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "rankingscore.h"
#include "number.h"
#include <fstream>
#include "load.h"
#include "network.h"
#include "manager.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CRankingScore::CRankingScore(int nPriority) : CObject(nPriority),
m_fHeight(NULL),
m_fWidth(NULL),
m_pos(VECTOR3_NULL),
m_pLoad(nullptr)
{
	for (int nCntData = 0; nCntData < RANKING_MAX; nCntData++)
	{
		// スコアを格納
		m_aRankData[nCntData] = NULL;

		for (int nCnt = 0; nCnt < RANKSCOREDIGIT; nCnt++)
		{
			// ナンバーポインタ
			m_apNumber[nCntData][nCnt] = nullptr;
		}
	}
}
//=========================================================
// デストラクタ
//=========================================================
CRankingScore::~CRankingScore()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CRankingScore* CRankingScore::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CRankingScore* pRankScore = new CRankingScore;
	if (pRankScore == nullptr) return nullptr;

	// オブジェクト設定
	pRankScore->m_pos = pos; 
	pRankScore->m_fWidth = fWidth;
	pRankScore->m_fHeight = fHeight;

	// 初期化失敗時
	if (FAILED(pRankScore->Init())) return nullptr;

	// 生成されたポインタを返す
	return pRankScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingScore::Init(void)
{
	// スコアをロード
	Load();

	// 横幅計算
	float fTexPos = m_fWidth / RANKSCOREDIGIT;

	for (int nRank = 0; nRank < RANKING_MAX; nRank++)
	{
		// 桁数分
		for (int nDigit = 0; nDigit < RANKSCOREDIGIT; nDigit++)
		{
			// インスタンス生成
			m_apNumber[nRank][nDigit] = new CNumber;

			// Y座標をずらす
			float yOffset = m_pos.y + (m_fHeight + 65.0f) * nRank;

			// 初期化処理
			m_apNumber[nRank][nDigit]->Init
			(
				D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nDigit), yOffset, 0.0f),
				fTexPos,
				m_fHeight
			);

			// サイズ設定
			m_apNumber[nRank][nDigit]->SetSize(fTexPos, m_fHeight);

			// カラー設定
			m_apNumber[nRank][nDigit]->SetCol(D3DCOLOR_RGBA(0,193,232,255));

			// テクスチャ設定
			m_apNumber[nRank][nDigit]->SetTexture("time.png");
		}
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingScore::Uninit(void)
{
	// 使っている桁数分の破棄
	for (int nRankData = 0; nRankData < RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt < RANKSCOREDIGIT; nCnt++)
		{
			if (m_apNumber[nRankData][nCnt] != nullptr)
			{
				// ナンバークラスの終了処理
				m_apNumber[nRankData][nCnt]->Uninit();

				// 各配列のポインタの破棄
				delete m_apNumber[nRankData][nCnt];

				// null初期化
				m_apNumber[nRankData][nCnt] = nullptr;
			}
		}
	}

	// ロードクラスのポインタの破棄
	m_pLoad.reset();

	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CRankingScore::Update(void)
{
	// スコアの桁数更新
	for (int rank = 0; rank < RANKING_MAX; rank++)
	{
		// 現在のスコアを格納
		int nScore = m_aRankData[rank];

		for (int digit = 0; digit < RANKSCOREDIGIT; digit++)
		{
			// 1桁ずつ取り出す
			int nNum = nScore % 10; 
			nScore /= 10;

			// 桁更新
			m_apNumber[rank][digit]->SetDigit(nNum);
		}
	}
}
//=========================================================
// 描画処理
//=========================================================
void CRankingScore::Draw(void)
{
	// 使っている桁数分の描画
	for (int nRankData = 0; nRankData < RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt < RANKSCOREDIGIT; nCnt++)
		{
			// ナンバー描画
			m_apNumber[nRankData][nCnt]->Draw();
		}
	}
}
//=========================================================
// ランキングスコア読み込み
//=========================================================
void CRankingScore::Load(void)
{
	// Networkクラス取得
	CNetWork* pNet = CManager::GetInstance()->GetNetWork();
	if (!pNet) return;

	// サーバー接続チェック
	if (pNet->GetIsConnect())
	{
		// 受信用配列
		int recvData[RANKING_MAX] = {};

		// ランキング受信
		if (pNet->RecvInt(recvData))
		{
			// メンバ変数へコピー
			for (int nRecvScore = 0; nRecvScore < RANKING_MAX; nRecvScore++)
			{
				m_aRankData[nRecvScore] = recvData[nRecvScore];
			}
		}
	}
}