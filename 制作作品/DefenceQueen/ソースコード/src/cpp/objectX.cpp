//=========================================================
//
// Xファイル処理 [ objectX.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "objectX.h"
#include "manager.h"
#include "xfilemanager.h"
#include "texture.h"
#include "outline.h"

//=========================================================
// コンストラクタ
//=========================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority),
m_mtxWorld{},
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_Scale(INITSCALE),
m_isShadow(false),
m_isOutLine(false),
m_col(V_COLOR_WHITE),
m_nIdxModel(-1),
m_OutLineColor(OUTLINE_COLOR)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CObjectX::~CObjectX()
{
	// 無し
}
//=========================================================
// アウトラインカラー設定関数
//=========================================================
inline void CObjectX::SetOutLineColor(const D3DXVECTOR4& color)
{
	m_OutLineColor = color;
}
//=========================================================
// 生成処理
//=========================================================
CObjectX* CObjectX::Create(const char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CObjectX* pObjX = new CObjectX;
	if (pObjX == nullptr) return nullptr;

	// オブジェクト設定
	pObjX->SetPos(pos);
	pObjX->SetRot(rot);
	pObjX->SetFilePass(pModelName);

	// 初期化処理
	if (FAILED(pObjX->Init())) return nullptr;

	// ポインタを返す
	return pObjX;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CObjectX::Init(void)
{
	// 結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CObjectX::Uninit(void)
{
	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CObjectX::Update(void)
{
	// 無し
}
//=========================================================
// 描画処理
//=========================================================
void CObjectX::Draw(void)
{
	// インデックスが-1なら
	if (m_nIdxModel == -1)
		return;

	// ファイルマネージャー取得
	CXfileManager* pXMgr = CManager::GetInstance()->GetXManager();
	if (!pXMgr) return;

	// 配列情報
	auto& fileData = pXMgr->GetList();
	if (m_nIdxModel >= static_cast<int>(fileData.size())) return;

	// 配列
	auto& model = fileData[m_nIdxModel];
	if (!model.pMesh) return;

	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale,mtxRot, mtxTrans;

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスの行列計算
	m_mtxWorld = mtxScale * mtxRot * mtxTrans;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマトリックスの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルが取得できたら
	if (model.pBuffMat)
	{
		// マテリアルデータのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

		// テクスチャ取得
		CTexture* pTex = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// カラー
			D3DXMATERIAL Col = pMat[nCnt];

			// カラーの乗算
			Col.MatD3D.Diffuse.a *= m_col.a;
			Col.MatD3D.Diffuse.r *= m_col.r;
			Col.MatD3D.Diffuse.g *= m_col.g;
			Col.MatD3D.Diffuse.b *= m_col.b;

			// マテリアルのセット
			pDevice->SetMaterial(&Col.MatD3D);

			// テクスチャ取得
			int texIdx = model.pTexture[nCnt];

			// テクスチャセット
			pDevice->SetTexture(0, (texIdx >= 0) ? pTex->GetAddress(texIdx) : nullptr);

			// モデルの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// マトリックスシャドウ描画
	if (m_isShadow) DrawShadow();

	// アウトラインがないなら
	if (!m_isOutLine) return;

	// カリング切る
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 開始
	COutLine::GetInstance()->Begin();
	COutLine::GetInstance()->BeginPass();

	// アウトラインを描画する
	DrawOutLine(m_OutLineColor);

	// 終了
	COutLine::GetInstance()->EndPass();
	COutLine::GetInstance()->End();

	// カリングを戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//=========================================================
// マトリックスシャドウ描画
//=========================================================
void CObjectX::DrawShadow(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return;

	// Xファイルマネージャー取得
	CXfileManager* pXMgr = CManager::GetInstance()->GetXManager();
	if (!pXMgr) return;

	// 配列情報の取得
	auto& fileData = pXMgr->GetList();
	if (m_nIdxModel < NULL || m_nIdxModel >= static_cast<int>(fileData.size())) return;

	// モデル要素を取得
	auto& model = fileData[m_nIdxModel];
	if (!model.pMesh) return;

	// ライト方向
	D3DXVECTOR4 lightDir(0.2f, -0.63f, 0.12f, 0.0f);

	// 平面投影座標を設定
	D3DXPLANE plane;
	D3DXVECTOR3 point = D3DXVECTOR3(0.0f,0.5f,0.0f);
	D3DXVECTOR3 normal = D3DXVECTOR3(0.0f,-0.5f,0.0f);
	D3DXPlaneFromPointNormal(&plane, &point, &normal);

	// 影マトリックス生成
	D3DXMATRIX mtxShadow;
	D3DXMatrixShadow(&mtxShadow, &lightDir, &plane);

	// 影をモデルの位置に合わせる
	D3DXMATRIX mtxWorldShadow;
	D3DXMatrixMultiply(&mtxWorldShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldShadow);

	// 半透明に設定
	D3DMATERIAL9 shadowMat = {};
	shadowMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.35f);
	shadowMat.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.35f);

	// マテリアルセット
	pDevice->SetMaterial(&shadowMat);

	// メッシュ描画
	for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
	{
		model.pMesh->DrawSubset(nCnt);
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&shadowMat);
}
//=========================================================
// アウトライン描画関数
//=========================================================
void CObjectX::DrawOutLine(const D3DXVECTOR4& color)
{
	// インデックスが-1なら
	if (m_nIdxModel == -1)
		return;

	// ファイルマネージャー取得
	CXfileManager* pXMgr = CManager::GetInstance()->GetXManager();
	if (!pXMgr) return;

	// 配列情報
	auto& fileData = pXMgr->GetList();
	if (m_nIdxModel >= static_cast<int>(fileData.size())) return;

	// 配列
	auto& model = fileData[m_nIdxModel];
	if (!model.pMesh) return;

	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルが取得できたら
	if (model.pBuffMat)
	{
		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// シェーダーパラメーター設定
			COutLine::GetInstance()->SetParameter(OUTLINE_SIZE, color, m_mtxWorld);

			// モデルの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}
}
//=========================================================
// 新規モデル登録
//=========================================================
void CObjectX::SetFilePass(const char* pFilePass)
{
	// マネージャー取得
	CXfileManager* pXMgr = CManager::GetInstance()->GetXManager();

	if (pXMgr != nullptr)
	{
		// パス連結
		std::string ModelName = "data/MODEL/";
		ModelName += pFilePass;

		// モデルを登録
		m_nIdxModel = pXMgr->Register(ModelName.c_str());
	}
}
