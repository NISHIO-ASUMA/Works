//=========================================================
//
// 仲間アリの分割可能UI表示処理 [ extractantsignal.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "extractantsignal.h"

//=========================================================
// コンストラクタ
//=========================================================
CExtractAntSignal::CExtractAntSignal(int nPriority) : CBillboard(nPriority),
m_isDraw(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CExtractAntSignal::~CExtractAntSignal()
{

}
//=========================================================
// 生成処理
//=========================================================
CExtractAntSignal* CExtractAntSignal::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CExtractAntSignal* pAntSignal = new CExtractAntSignal;
	if (pAntSignal == nullptr) return nullptr;

	// オブジェクト設定
	pAntSignal->SetPos(pos);
	pAntSignal->SetSize(Config::WIDTH, Config::HEIGHT);
	pAntSignal->SetTexture(Config::TEXNAME);

	// 初期化失敗時
	if (FAILED(pAntSignal->Init())) return nullptr;

	return pAntSignal;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CExtractAntSignal::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CExtractAntSignal::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CExtractAntSignal::Update(void)
{
	// 描画フラグがfalseなら
	if (!m_isDraw) return;

	// 親クラスの更新処理
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CExtractAntSignal::Draw(void)
{
	// 描画フラグがfalseなら
	if (!m_isDraw) return;

	// 親クラスの描画処理
	CBillboard::Draw();
}