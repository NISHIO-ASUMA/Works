//=========================================================
//
// タイトルで出現するオブジェクト処理 [ titleobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "titleobject.h"
#include "ui.h"
#include "meshfield.h"
#include "titleant.h"
#include "titlewallmanager.h"
#include "titleantmanager.h"
#include "jsonmanager.h"
#include "manager.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CTitleObject* CTitleObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace TITLEOBJECT
{
	constexpr const char* LoadName = "data/JSON/Titleobject.json"; // 読み込むjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CTitleObject::CTitleObject() : m_pWallManager(nullptr),
m_pTitleAntManager(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTitleObject::~CTitleObject()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleObject::Init(void)
{
	// 見えない壁生成
	m_pWallManager = std::make_unique<CTitleWallManager>();
	m_pWallManager->Init();

	// タイトルのアリ生成
	m_pTitleAntManager = std::make_unique<CTitleAntManager>();
	m_pTitleAntManager->Init();

	// タイトルで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(TITLEOBJECT::LoadName);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleObject::Uninit(void)
{
	// タイトルの壁の破棄
	m_pWallManager.reset();

	// タイトルのアリたちの破棄
	m_pTitleAntManager.reset();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CTitleObject::Update(void)
{
	// タイトルのアリ管理クラスの更新
	if (m_pTitleAntManager) m_pTitleAntManager->Update();
}
//=========================================================
// シングルトン取得処理
//=========================================================
CTitleObject* CTitleObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr)
	{
		// インスタンス生成
		m_pInstance = new CTitleObject;
	}

	// シングルトンを返す
	return m_pInstance;
}