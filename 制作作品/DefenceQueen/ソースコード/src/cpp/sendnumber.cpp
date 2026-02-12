//=========================================================
//
// 送る数の数字処理 [ sendnumber.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "sendnumber.h"

//=========================================================
// コンストラクタ
//=========================================================
CSendNumber::CSendNumber(int nPriority) : CObject(nPriority),
m_nScore(NULL),
m_col(COLOR_WHITE),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_apNumber{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CSendNumber::~CSendNumber()
{

}
//=========================================================
// 生成処理
//=========================================================
CSendNumber* CSendNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// スコアインスタンス生成
	CSendNumber* pScore = new CSendNumber;
	if (pScore == nullptr) return nullptr;

	// 座標,サイズ設定
	pScore->m_pos = pos;
	pScore->m_fWidth = fWidth;
	pScore->m_fHeight = fHeight;

	// 初期化失敗時
	if (FAILED(pScore->Init())) return nullptr;

	return pScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSendNumber::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / NUM_SCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// インスタンス生成
		m_apNumber[nCnt] = new CNumber;

		// 初期化処理
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);

		// ナンバー変数のサイズ
		m_apNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// 座標設定
		m_apNumber[nCnt]->SetPos(m_pos);

		// カラー設定
		m_apNumber[nCnt]->SetCol(COLOR_GREEN);

		// テクスチャセット
		m_apNumber[nCnt]->SetTexture("time.png");
	}

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSendNumber::Uninit(void)
{
	// 使った分破棄
	for (auto number : m_apNumber)
	{
		// nullptrチェック
		if (number != nullptr)
		{
			number->Uninit();
			delete number;
			number = nullptr;
		}
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CSendNumber::Update(void)
{
	// スコア格納
	int nScore = m_nScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % NUM_DIGIT;

		nScore /= NUM_DIGIT;

		// ナンバー更新
		m_apNumber[nCntScore]->Update();

		// 桁更新
		m_apNumber[nCntScore]->SetDigit(nDigit);
	}
}
//=========================================================
// 描画処理
//=========================================================
void CSendNumber::Draw(void)
{
#if 0
	// 使っている桁数分の描画
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// ナンバー描画
		m_apNumber[nCnt]->Draw();
	}
#endif
}