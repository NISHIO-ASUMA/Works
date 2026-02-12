//=========================================================
//
// タイム処理 [ gametime.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "gametime.h"
#include "number.h"
#include "manager.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CGameTime::CGameTime(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_nAllTime(NULL),
m_nCount(NULL),
m_nDecTime(NULL),
m_nMinute(NULL),
m_nSecond(NULL)
{
	// 値のクリア
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		m_pNumberMinute[nCnt] = nullptr;
		m_pNumberSecond[nCnt] = nullptr;
	}
}
//=========================================================
// デストラクタ
//=========================================================
CGameTime::~CGameTime()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CGameTime* CGameTime::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// インスタンス生成
	CGameTime* pTime = new CGameTime;
	if (pTime == nullptr) return nullptr;

	// オブジェクト設定
	pTime->m_fHeight = fHeight;
	pTime->m_fWidth = fWidth;
	pTime->m_pos = pos;

	// 初期化失敗時
	if (FAILED(pTime->Init())) return nullptr;

	// 結果を返す
	return pTime;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameTime::Init(void)
{
	// 最大時間をセット
	m_nAllTime = NUMTIME;

	// 分を計算
	m_nMinute = m_nAllTime / CARVETIME;

	// 秒を計算
	m_nSecond = m_nAllTime % CARVETIME;

	// 横幅計算
	float fTexPos = m_fWidth / DIGIT_TIME;

	// 分生成
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// インスタンス生成
		m_pNumberMinute[nCnt] = new CNumber;

		// ナンバー変数の初期化
		m_pNumberMinute[nCnt]->SetPos(m_pos);
		m_pNumberMinute[nCnt]->Init(D3DXVECTOR3(m_pos.x + (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);
		m_pNumberMinute[nCnt]->SetSize(fTexPos, m_fHeight);
		m_pNumberMinute[nCnt]->SetTexture("time.png");
	}

	// 横に座標をずらす
	m_pos.x += VALUE_WIDTH;

	// 秒生成
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// インスタンス生成
		m_pNumberSecond[nCnt] = new CNumber;

		// ナンバー変数の初期化
		m_pNumberSecond[nCnt]->SetPos(m_pos);
		m_pNumberSecond[nCnt]->Init(D3DXVECTOR3(m_pos.x + (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);
		m_pNumberSecond[nCnt]->SetSize(fTexPos, m_fHeight);
		m_pNumberSecond[nCnt]->SetTexture("time.png");
	}

	// 結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameTime::Uninit(void)
{
	// 使った分破棄
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// nullptrチェック
		if (m_pNumberMinute[nCnt] != nullptr)
		{
			// 終了処理
			m_pNumberMinute[nCnt]->Uninit();

			// ポインタの破棄
			delete m_pNumberMinute[nCnt];

			// null初期化
			m_pNumberMinute[nCnt] = nullptr;
		}

		// nullptrチェック
		if (m_pNumberSecond[nCnt] != nullptr)
		{
			// 終了処理
			m_pNumberSecond[nCnt]->Uninit();

			// ポインタの破棄
			delete m_pNumberSecond[nCnt];

			// null初期化
			m_pNumberSecond[nCnt] = nullptr;
		}
	}

	// オブジェクトの破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CGameTime::Update(void)
{
	if (m_nAllTime <= 0)
	{
		m_nAllTime = 0;
		return;
	}

	// カウンターを加算
	m_nCount++;

	// 1秒経過後 ( 秒減少 )
	if (m_nCount >= CARVETIME)
	{
		// カウンターを初期化する
		m_nCount = 0;

		// 全体時間を減らす
		m_nAllTime--;

		// 分
		m_nMinute = m_nAllTime / CARVETIME;

		// 秒を減らす
		m_nSecond = m_nAllTime % CARVETIME;

		// 0以下なら
		if (m_nAllTime <= 0)
		{
			m_nAllTime = 0;
		}

		// 減少していった時間を加算
		m_nDecTime++;
	}

	// 桁計算
	Second();
	Minute();
}
//=========================================================
// 描画処理
//=========================================================
void CGameTime::Draw(void)
{
	// 桁数分描画
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		m_pNumberMinute[nCnt]->Draw();
		m_pNumberSecond[nCnt]->Draw();
	}
}
//=========================================================
// 秒計算
//=========================================================
void CGameTime::Second(void)
{
	int aData = DIVIDE * DIVIDE;
	int aData2 = DIVIDE;

	// 分計算
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// 桁数計算
		int aPosTexU = m_nMinute % aData / aData2;
		aData /= DIVIDE;
		aData2 /= DIVIDE;

		// 分の桁数を更新する
		if (m_pNumberMinute[nCnt] != nullptr)
		{
			m_pNumberMinute[nCnt]->Update();
			m_pNumberMinute[nCnt]->SetDigit(aPosTexU);
		}
	}
}
//=========================================================
// 分計算
//=========================================================
void CGameTime::Minute(void)
{
	int aData = DIVIDE * DIVIDE;
	int aData2 = DIVIDE;

	// 秒計算
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// 桁数計算
		int aPosTexU = m_nSecond % aData / aData2;
		aData /= DIVIDE;
		aData2 /= DIVIDE;

		// 秒の桁数を更新する
		if (m_pNumberSecond[nCnt] != nullptr)
		{
			m_pNumberSecond[nCnt]->Update();
			m_pNumberSecond[nCnt]->SetDigit(aPosTexU);
		}
	}
}