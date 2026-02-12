//=========================================================
//
// マネージャー処理 [ manager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 前方宣言
//*********************************************************
class CInputKeyboard;
class CJoyPad;
class CInputMouse;
class CSound;
class CTexture;
class CCamera;
class CLight;
class CFade;
class CXfileManager;
class CScene;
class CMotionManager;
class CModelManager;
class CNetWork;
class CInstanceModelManager;
class CInstanceMotionManager;
class CJsonManager;

//*********************************************************
// インクルードファイル
//*********************************************************
#include <memory>
#include "scene.h"
#include "renderer.h"

//*********************************************************
// マネージャークラスを定義
//*********************************************************
class CManager
{
public:

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScene(std::unique_ptr<CScene> pNewScene);
	CScene::MODE GetScene(void);

	CInstanceMotionManager* GetInstMotionManager(void) { return m_pInstMotionManager.get(); }
	CInstanceModelManager* GetInstanceModelM(void) { return m_pInstanceModel.get(); }
	CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard.get(); }
	CMotionManager* GetMotionManager(void) { return m_pMotionManager.get(); }
	CModelManager* GetModelManagere(void) { return m_pModelManager.get(); }
	CXfileManager* GetXManager(void) { return m_pXfileManager.get(); }
	CRenderer* GetRenderer(void) { return m_pRenderer.get(); }
	CInputMouse* GetMouse(void) { return m_pInputMouse.get(); }
	CTexture* GetTexture(void) { return m_pTexture.get(); }
	CNetWork* GetNetWork(void) { return m_pNetWork.get(); }
	CJoyPad* GetJoyPad(void) { return m_pJoyPad.get(); }
	CCamera* GetCamera(void) { return m_pCamera.get(); }
	CSound* GetSound(void) { return m_pSound.get(); }
	CLight* GetLight(void) { return m_pLight.get(); }
	CFade* GetFade(void) { return m_pFade.get(); }
	CScene* GetSceneRaw(void) { return m_pScene.get(); }

	CJsonManager* GetJsonManager(void) { return m_pJsonManager.get(); }

	// マネージャークラスのインスタンス取得
	static CManager* GetInstance(void)
	{
		static CManager pManeger;
		return &pManeger;
	};

private:

	CManager();
	~CManager();

	std::unique_ptr<CRenderer>m_pRenderer;			// レンダラークラスのポインタ
	std::unique_ptr<CInputKeyboard>m_pInputKeyboard;// キーボードクラスのポインタ
	std::unique_ptr<CJoyPad>m_pJoyPad;				// ジョイパッドクラスのポインタ
	std::unique_ptr<CSound>m_pSound;				// サウンドオブジェクトのポインタ
	std::unique_ptr<CInputMouse>m_pInputMouse;		// マウスクラスのポインタ
	std::unique_ptr<CTexture>m_pTexture;			// テクスチャクラスのポインタ
	std::unique_ptr<CCamera>m_pCamera;				// カメラクラスのポインタ
	std::unique_ptr<CLight>m_pLight;				// ライトクラスのポインタ	
	std::unique_ptr<CScene>m_pScene;;				// シーン管理クラスのポインタ
	std::unique_ptr<CFade>m_pFade;					// フェードクラスのポインタ
	std::unique_ptr<CXfileManager>m_pXfileManager;	// Xファイルマネージャークラス
	std::unique_ptr<CMotionManager>m_pMotionManager;// モーションマネージャークラス
	std::unique_ptr<CModelManager>m_pModelManager;	// キャラクターモデル管理クラス
	std::unique_ptr<CNetWork>m_pNetWork;			// ネットワーククラスのポインタ
	std::unique_ptr<CInstanceModelManager>m_pInstanceModel; // インスタンシング適用モデル管理クラス
	std::unique_ptr<CInstanceMotionManager>m_pInstMotionManager; // インスタンシングモデルのモーション管理クラス
	std::unique_ptr<CJsonManager>m_pJsonManager;

};