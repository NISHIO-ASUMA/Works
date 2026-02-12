//=========================================================
//
// アリのシグナル状態を表すUI表示クラス処理 [ signalui.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "signalui.h"

//=========================================================
// コンストラクタ
//=========================================================
CSignalUi::CSignalUi(int nPriority) : CBillboard(nPriority),
m_isDraw(true)
{

}
//=========================================================
// デストラクタ
//=========================================================
CSignalUi::~CSignalUi()
{

}
//=========================================================
// 生成処理
//=========================================================
CSignalUi* CSignalUi::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const float fWidth, 
	const float fHeight, 
	const D3DXCOLOR& color
)
{
	// インスタンス生成
	CSignalUi* pSignal = new CSignalUi;
	if (pSignal == nullptr) return nullptr;

	// オブジェクト設定
	pSignal->SetPos(pos);
	pSignal->SetRot(rot);
	pSignal->SetSize(fWidth,fHeight);
	pSignal->SetCol(color);
	pSignal->SetTexture(Config::TEX_NAME);

	// 初期化失敗時
	if (FAILED(pSignal->Init())) return nullptr;

	return pSignal;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSignalUi::Init(void)
{
	// 親クラスの初期化
	CBillboard::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSignalUi::Uninit(void)
{
	// 親クラスの終了
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CSignalUi::Update(void)
{
	// 描画offなら
	if (!m_isDraw) return;

	// 親クラスの更新
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CSignalUi::Draw(void)
{
	// 描画offなら
	if (!m_isDraw) return;

	// 親クラスの描画
	CBillboard::Draw();
}