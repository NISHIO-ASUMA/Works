//=========================================================
//
// 餌獲得のシグナルを表すUI表示クラス処理 [ feedsignal.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "feedsignal.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CFeedSignal::CFeedSignal(int nPriority) : CBillboard(nPriority),
m_isDraw(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CFeedSignal::~CFeedSignal()
{

}
//=========================================================
// 生成処理
//=========================================================
CFeedSignal* CFeedSignal::Create
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const float fWidth,
	const float fHeight,
	const D3DXCOLOR& color)
{
	// インスタンス生成
	CFeedSignal* pSignal = new CFeedSignal;
	if (pSignal == nullptr) return nullptr;

	// オブジェクト設定
	pSignal->SetPos(pos);
	pSignal->SetRot(rot);
	pSignal->SetSize(fWidth, fHeight);
	pSignal->SetCol(color);
	pSignal->SetTexture(Config::TEX_NAME);

	// 初期化失敗時
	if (FAILED(pSignal->Init())) return nullptr;

	return pSignal;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CFeedSignal::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CFeedSignal::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CFeedSignal::Update(void)
{
	// 描画offなら
	if (!m_isDraw) return;

	// 親クラスの更新
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CFeedSignal::Draw(void)
{
	// 描画offなら
	if (!m_isDraw) return;

	// 親クラスの描画
	CBillboard::Draw();
}