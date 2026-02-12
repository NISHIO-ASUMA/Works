//=========================================================
//
// レンダリング処理 [ renderer.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "instancing.h"
#include "modelmanager.h"
#include "model.h"
#include "instancemodel.h"
#include "instancemodelmanager.h"
#include "texture.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CDebugproc* CRenderer::m_pDebug = nullptr;	// デバッグプロセスへのポインタ

//=========================================================
// コンストラクタ
//=========================================================
CRenderer::CRenderer() : 
m_pD3D(nullptr),
m_pD3DDevice(nullptr),
m_pZBuffMT(nullptr),
m_pRenderDef(nullptr),
m_pZBuffDef(nullptr),
m_pVtxMT(nullptr),
m_isbuller(false),
m_apRenderMT{},
m_apTextureMT{},
m_nBullerTime(NULL),
m_fps(NULL)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CRenderer::~CRenderer()
{
	// 無し
}
//=========================================================
// レンダラー初期化処理
//=========================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// ディスプレイモード
	D3DDISPLAYMODE d3ddm;			

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));						//パラメーターの0クリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;					// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;					// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;								// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// デプスバッファとして16bitを使用
	m_d3dpp.Windowed = bWindow;									// ウインドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#if 1
	// マルチターゲット用変数
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	for (int nTex = 0; nTex < NUM_FEEDBACKPOLYGON; nTex++)
	{
		// レンダリングターゲットテクスチャ生成
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_apTextureMT[nTex], NULL);
	}

	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		// レンダリング用インターフェース作成 
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);
	}

	// Zバッファ生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZBuffMT, NULL);

	// 現在のレンダリングターゲットを取得
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		// レンダリングターゲットを設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);
	}

	// Zバッファを設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャのクリア処理
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 設定したレンダーターゲットを戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// 設定したZバッファを戻す
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// ビューポート設定
	m_viewportMT.X = NULL;
	m_viewportMT.Y = NULL;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// フィードバック用ポリゴン生成
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BASEVERTEX * NUM_FEEDBACKPOLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxMT,
		NULL);

	// 幅,高さを設定
	float fWidth = SCREEN_WIDTH * 0.5f;
	float fHeight = SCREEN_HEIGHT * 0.5f;

	// 頂点座標の設定
	VERTEX_2D* pVtx = {};

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxMT->Lock(0, 0, (void**)&pVtx, 0);

	// 基準値
	D3DXVECTOR3 pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);

	// 各ポリゴンの設定
	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

		// rhwの設定(1.0fで固定)
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 透明度を設定
		float fAlpha = nCnt == 0 ? 1.0f : 0.65f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点座標を更新
		pVtx += BASEVERTEX;
	}

	//頂点バッファをアンロック
	m_pVtxMT->Unlock();

#endif
	// デバッグフォント初期化
	m_pDebug = new CDebugproc;
	m_pDebug->Init();

	// メンバ変数
	m_nBullerTime = NULL;

	// 最大インスタンス数
	const UINT MAX_INSTANCE = 2048;

	// インスタンシング用頂点バッファ生成
	HRESULT hr = m_pD3DDevice->CreateVertexBuffer
	(
		sizeof(InstanceData) * MAX_INSTANCE,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&m_instanceVB,
		nullptr
	);

	// 生成失敗時
	if (FAILED(hr))
		return hr;

	return S_OK;
}
//=========================================================
// レンダラー終了処理
//=========================================================
void CRenderer::Uninit(void)
{
	if (m_pDebug != nullptr)
	{
		// デバッグプロセスの終了
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	// 全オブジェクト破棄
	CObject::ReleaseAll();

	// マルチレンダリングターゲット用Zバッファの破棄
	if (m_pZBuffMT != nullptr)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxMT != nullptr)
	{
		m_pVtxMT->Release();
		m_pVtxMT = nullptr;
	}

	// 作成数分破棄
	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		// テクスチャ破棄
		if (m_apTextureMT[nCnt] != nullptr)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = nullptr;
		}

		// レンダー破棄
		if (m_apRenderMT[nCnt] != nullptr)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = nullptr;
		}
	}

	// インスタンシング頂点バッファの破棄
	if (m_instanceVB)
	{
		m_instanceVB->Release();
		m_instanceVB = nullptr;
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}
//=========================================================
// レンダラー更新処理
//=========================================================
void CRenderer::Update(void)
{
	// デバッグ情報の更新処理
	m_pDebug->Update();

	// 全オブジェクト更新処理
	CObject::UpdateAll();

	// ブラータイムを減算
	m_nBullerTime--;

	// 0以下の時に未使用
	if (m_nBullerTime <= 0)
	{
		m_isbuller = false;
	}

#ifdef _DEBUG

	// キー入力情報取得
	CInputKeyboard* pInput = CManager::GetInstance()->GetInputKeyboard();

	// ワイヤーフレーム設定
	if (pInput->GetTrigger(DIK_F3))
	{
		// ワイヤーフレームON
		OnWireFrame();
	}
	else if (pInput->GetTrigger(DIK_F4))
	{
		// ワイヤーフレームOFF
		OffWireFrame();
	}

	// ブラー起動
	if (pInput->GetTrigger(DIK_F6))
	{
		SetBuller(true,300);
	}

#endif // _DEBUG
}
//=========================================================
// レンダラー描画処理
//=========================================================
void CRenderer::Draw(void)
{
	// 画面クリア (バックバッファ&Zバッファ&ステンシルバッファのクリア)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 保存用変数宣言
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;

	// 変数
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	D3DVIEWPORT9 viewport;
	D3DXMATRIX mtxview, mtxprojection;

	// ゲット
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);
	m_pD3DDevice->GetViewport(&viewport);

	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);
	m_pD3DDevice->SetViewport(&m_viewportMT);

	m_pD3DDevice->SetRenderTarget(0, pRenderDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	m_pD3DDevice->SetViewport(&viewport);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画成功時

		// ブラー有効時
		if (m_isbuller)
		{
			// ターゲット設定
			ChangeTarget(CManager::GetInstance()->GetCamera()->GetPos(),CManager::GetInstance()->GetCamera()->GetPosR(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			// 画面クリア
			m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 225), 1.0f, 0);
		}

		// 全オブジェクト描画
		CObject::DrawAll();

		//************************
		// インスタンシングのDrawCall
		//************************
		DrawInstancingAll();

		// シーン取得
		CScene*pScene = CManager::GetInstance()->GetSceneRaw();

		// シーンの描画
		if (pScene != nullptr) pScene->Draw();

#if 0
		// ブラー有効時
		if (m_isbuller)
		{
			// Texture[1]番のポリゴンを描画
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
			m_pD3DDevice->SetStreamSource(0, m_pVtxMT, 0, sizeof(VERTEX_2D));
			m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

			// レンダー設定を戻す
			m_pD3DDevice->SetRenderTarget(0, pRenderDef);
			m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
			m_pD3DDevice->SetViewport(&viewport);
			m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);
			m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxprojection);

			// Texture[0]のポリゴン描画
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
			m_pD3DDevice->SetStreamSource(0, m_pVtxMT, 0, sizeof(VERTEX_2D));
			m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// テクスチャをワーカーに保存する
			pTextureWk = m_apTextureMT[0];
			m_apTextureMT[0] = m_apTextureMT[1];
			m_apTextureMT[1] = pTextureWk;

			// レンダー設定をワーカーに保存する
			pRenderWk = m_apRenderMT[0];
			m_apRenderMT[0] = m_apRenderMT[1];
			m_apRenderMT[1] = pRenderWk;
		}
#endif
		// フォントセット
		m_pDebug->Print("FPS : %d\n", m_fps);

		// デバッグフォントの描画
		m_pDebug->Draw(0, 0);

		// フェード描画
		CManager::GetInstance()->GetFade()->Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
//=========================================================
// レンダーターゲット変更関数
//=========================================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	// マトリックス変数
	D3DXMATRIX mtxprojection, mtxview;

	// アスペクト比計算用
	float fAsepct = NULL;

	// レンダリングターゲットを設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	// Zバッファを設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// ビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxview);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxview,
		&posV,
		&posR,
		&vecU);

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxprojection);

	// アスペクト比の設定
	fAsepct = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxprojection,
		D3DXToRadian(45.0f),
		fAsepct,
		10.0f,
		3000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxprojection);
}
//=========================================================
// ブラーの設定
//=========================================================
void CRenderer::SetBuller(bool isBuller, const int nMaxbullerTime)
{
	// 値を設定
	m_isbuller = isBuller;
	m_nBullerTime = nMaxbullerTime;
}


//=========================================================
// インスタンシングオブジェクト登録関数
//=========================================================
void CRenderer::AddInstanceObject(const int nIdxModel,CInstanceModel* pModel)
{
	// 対象オブジェクトをインスタンシング配列に追加
	m_RegisterInstObject[nIdxModel].push_back(pModel);
}
//=========================================================
// インスタンシング描画関数
//=========================================================
void CRenderer::DrawInstancingAll(void)
{
	// モデルマネージャー取得
	auto* modelMgr = CManager::GetInstance()->GetInstanceModelM();
	auto& models = modelMgr->GetList();

	for (auto& pair : m_RegisterInstObject)
	{
		// 配列の最初と次を取得
		int nModelIdx = pair.first;
		auto& instances = pair.second;

		// 情報が無かったら
		if (instances.empty())
			continue;

		// 対象オブジェクトの情報を格納
		auto& modelInfo = models[nModelIdx];
		auto& modelData = modelInfo.InstanceData;

		//============================
		// インスタンシング頂点バッファ更新
		//============================
		InstanceData* pInst = nullptr;

		// 頂点バッファをロック
		m_instanceVB->Lock(0, 0, (void**)&pInst, D3DLOCK_DISCARD);

		for (int nCnt = 0; nCnt < static_cast<int>(instances.size()); nCnt++)
		{
			// ワールドマトリックス設定
			pInst[nCnt].mtxworld = instances[nCnt]->GetMtxWorld();
		}

		// 頂点バッファをアンロック
		m_instanceVB->Unlock();

		//=============================
		// マテリアル単位ループ
		//=============================
		for (DWORD matID = 0; matID < modelInfo.dwNumMat; matID++)
		{
			//=========================
			// マテリアルカラー取得
			//=========================
			D3DXVECTOR4 matColor = VECTOR4_NULL;

			if (modelInfo.pBuffMat)
			{
				// モデルのマテリアルを取得する
				auto& mat = modelInfo.Materials[matID];

				// D3DXVECTOR4に変換する
				matColor = D3DXVECTOR4
				(
					mat.Diffuse.r,
					mat.Diffuse.g,
					mat.Diffuse.b,
					mat.Diffuse.a
				);
			}

			//=========================
			// インスタンシング描画
			//=========================
			CInstancing::GetInstance()->Begin();
			CInstancing::GetInstance()->BeginPass();

			// インスタンシング開始関数
			CInstancing::GetInstance()->BeginInstancing
			(
				instances.size(),
				modelData.VtxBuffer,
				sizeof(MODEL_3D),
				modelData.IndexBuffer,
				m_instanceVB,
				sizeof(InstanceData)
			);

			// シェーダーパラメーターセット
			CInstancing::GetInstance()->SetInstancingParam(nullptr, matColor);

			// ポリゴン描画
			m_pD3DDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLELIST,
				0,
				0,
				modelData.vtxCount,
				0,
				modelData.PrimCount
			);

			// インスタンシングパラメーターリセット
			CInstancing::GetInstance()->EndInstancing();

			// インスタンシング終了
			CInstancing::GetInstance()->EndPass();
			CInstancing::GetInstance()->End();
		}
	}

	// セカンドの配列クリア
	for (auto& pair : m_RegisterInstObject)
	{
		pair.second.clear();
	}
}

//=========================================================
// ワイヤーフレーム起動
//=========================================================
void CRenderer::OnWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=========================================================
// ワイヤーフレーム終了
//=========================================================
void CRenderer::OffWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}