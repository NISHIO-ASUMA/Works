//========================================================
//
// レンダリング処理 [ renderer.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "debugproc.h"
#include <unordered_map>
#include <vector>
#include <string>

//*********************************************************
// 前方宣言
//*********************************************************
class CInstanceModel;

//*********************************************************
// レンダラークラスを定義
//*********************************************************
class CRenderer
{
public:

	//****************************
	// インスタンシング構造体
	//****************************
	struct InstanceData
	{
		D3DXMATRIX mtxworld; // ワールドマトリックス
		int materialID;		 // マテリアルID
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void OnWireFrame();
	void OffWireFrame();
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	void SetSize(UINT uWidth, UINT uHeight) { m_Width = uWidth, m_Height = uHeight; }
	void SetBuller(bool isBuller, const int nMaxbullerTime);
	void GetFps(const int& nFps) { m_fps = nFps; }

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	UINT GetWidth(void) { return m_Width; }
	UINT GetHeight(void) { return m_Height; }
	bool GetBuller(void) { return m_isbuller; }
	static CDebugproc* GetDebug(void) { return m_pDebug; }

	/// <summary>
	/// インスタンシング登録関数
	/// </summary>
	/// <param name="nIdxModel">モデルインデックス</param>
	/// <param name="pModel">モデルポインタ</param>
	void AddInstanceObject(const int nIdxModel, CInstanceModel* pModel);

	/// <summary>
	/// 全インスタンシングオブジェクト描画関数
	/// </summary>
	/// <param name=""></param>
	void DrawInstancingAll(void);

	/// <summary>
	/// インスタンス配列内オブジェクトクリア関数
	/// </summary>
	/// <param name=""></param>
	void ClearDrawInstance(void) { m_RegisterInstObject.clear(); };

private:

	static inline constexpr int NUM_FEEDBACKPOLYGON = 2; // フィードバック用ポリゴン
	static inline constexpr int MAX_MATERIAL = 512;		 // 最大マテリアル数

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	static CDebugproc* m_pDebug;		// デバッグ
	int m_fps;							// fps
	int m_nBullerTime;					// 継続時間
	bool m_isDeviceLost;				// デバイスロスト対処
	bool m_isbuller;					// ブラーかどうか

	D3DPRESENT_PARAMETERS m_d3dpp;		// プレゼンテーションパラメータ
	UINT m_Width;						// 横幅
	UINT m_Height;						// 高さ

	LPDIRECT3DVERTEXBUFFER9 m_instanceVB;	// インスタンシング用頂点バッファ
	std::unordered_map<int, std::vector<CInstanceModel*>> m_RegisterInstObject = {}; // インスタンシング登録配列
	std::vector<InstanceData>m_MaterialID = {};					// マテリアルID配列

	LPDIRECT3DSURFACE9 m_pRenderDef; // マルチターゲットレンダリング用インターフェース保存用
	LPDIRECT3DSURFACE9 m_pZBuffDef;	 // マルチターゲットレンダリング用Zバッファ保存用
	LPDIRECT3DTEXTURE9 m_apTextureMT[NUM_FEEDBACKPOLYGON]; // レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[NUM_FEEDBACKPOLYGON];	 // テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;	 // テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;		 // テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_pVtxMT;  // ポリゴン用頂点バッファ
};