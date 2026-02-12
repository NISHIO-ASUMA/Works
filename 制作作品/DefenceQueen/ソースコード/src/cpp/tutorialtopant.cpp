//=========================================================
//
// チュートリアルの指示アリの処理 [ tutorialtopant.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "tutorialtopant.h"
#include "boxcollider.h"
#include "collisionbox.h"
#include "blockmanager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "effect.h"
#include "arraymanager.h"
#include "boxtospherecollision.h"
#include "tutorialobject.h"
#include "fade.h"
#include "game.h"
#include "tutoarrayant.h"
#include "spherecollider.h"
#include "eventarea.h"
#include "sepalationsign.h"
#include "sound.h"
#include "pointobj.h"
#include "circle3d.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutoTopAnt::CTutoTopAnt(int nPriority) : CMoveCharactor(nPriority),m_pColliderBox(nullptr),m_isBranchSet(false),
m_fSeparationRadius(NULL), m_isHPressing(false), m_DestPos(VECTOR3_NULL),m_pPutSign(nullptr),m_pSeparationSign(nullptr),
m_pPoint(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTutoTopAnt::~CTutoTopAnt()
{

}
//=========================================================
// 生成処理
//=========================================================
CTutoTopAnt* CTutoTopAnt::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CTutoTopAnt* pTop = new CTutoTopAnt;
	if (pTop == nullptr) return nullptr;

	// オブジェクト設定
	pTop->SetPos(pos);
	pTop->SetRot(rot);
	pTop->SetUseStencil(false);
	pTop->SetUseOutLine(true);

	// 初期化失敗時
	if FAILED(pTop->Init()) return nullptr;

	return pTop;
}
//=========================================================
// チュートリアルトップアリの初期化処理
//=========================================================
HRESULT CTutoTopAnt::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad(Config::MOTION_NAME, MOTION_MAX, true);

	// コライダー生成
	m_pColliderBox = CBoxCollider::Create(GetPos(), GetOldPos(), D3DXVECTOR3(20.0f, 20.0f, 20.0f));
	m_pSphereCollider = CSphereCollider::Create(GetPos(), 80.0f);

	// 円形生成
	m_pCircleObj = CCircle3D::Create(GetPos(), VECTOR3_NULL, m_fSeparationRadius, 3.0f, 0.0f);

	// 切り離しui生成
	m_pSeparationSign = CSepalationSign::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 240.0f, GetPos().z), "Sepalation.png");

	// 置き配置UI生成
	m_pPutSign = CSepalationSign::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 240.0f, GetPos().z), "PutAnt.png");

	// 矢印生成
	m_pPoint = CPointObj::Create(D3DXVECTOR3(GetPos().x, GetPos().y + Config::OffPosY, GetPos().z), D3DXVECTOR3(-90.0f, 0.0f, 0.0f));

	return S_OK;
}
//=========================================================
// チュートリアルトップアリの終了処理
//=========================================================
void CTutoTopAnt::Uninit(void)
{
	// 矩形コライダー破棄
	m_pColliderBox.reset();

	// 球形コライダ―の破棄
	m_pSphereCollider.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//=========================================================
// チュートリアルのトップアリの更新処理
//=========================================================
void CTutoTopAnt::Update(void)
{
	// 座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 oldPos = GetOldPos();

	// 入力デバイス取得
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	// 移動関数
	Moving(pPad, pKey);
	MovePad(pPad);

	// キー入力で選択範囲拡大
	if (pKey->GetPress(DIK_SPACE) || pPad->GetPress(CJoyPad::JOYKEY_X))
	{
		// 切り離しの加算
		if (!m_isHPressing)
		{
			m_isHPressing = true;
			m_fSeparationRadius = NULL;
		}

		// 描画オン
		m_pSeparationSign->SetIsDraw(true);

		// 押している間
		Separation();
	}
	else
	{
		// 離した瞬間
		if (m_isHPressing)
		{
			// 無効化
			m_isHPressing = false;

			// 描画オフ
			m_pSeparationSign->SetIsDraw(false);

			// サイズ初期化
			m_pCircleObj->SetSize(0.0f, 3.0f);

			// 仲間の黒アリを取得する
			auto ArrayAnt = CTutorialObject::GetInstance()->GetArrayAnt();
			auto ArrayPos = ArrayAnt->GetPos();

			auto DestRage = ArrayAnt->GetPos() - pos;
			float fDis = D3DXVec3Length(&DestRage);

			// 円形範囲判定
			if (fDis <= m_fSeparationRadius)
			{
				ArrayAnt->SetIsTopFollow(true);
			}
		}
	}

	// オブジェクトの座標更新
	CMoveCharactor::UpdatePosition();

	// 更新された座標を取得
	D3DXVECTOR3 UpdatePos = GetPos();
	
	// 矢印の座標の更新
	m_pPoint->SetPos(D3DXVECTOR3(UpdatePos.x, UpdatePos.y + Config::OffPosY, UpdatePos.z));

	// 球形コライダーの位置更新
	if (m_pSphereCollider) m_pSphereCollider->SetPos(UpdatePos);

	// 矩形コライダーの位置更新
	if (m_pColliderBox)
	{
		m_pColliderBox->SetPos(UpdatePos);
		m_pColliderBox->SetPosOld(oldPos);
	}

	// 配置されているブロックを取得
	auto Block = CTutorialObject::GetInstance()->GetBlockManager();
	if (Block == nullptr) return;

	// ブロックオブジェクトとの当たり判定
	for (int nBlock = 0; nBlock < Block->GetAll(); nBlock++)
	{
		// コライダー取得
		CBoxCollider* pOtherCollider = Block->GetBlock(nBlock)->GetCollider();
		if (pOtherCollider == nullptr) continue;

		// 矩形で当たる
		if (Collision(pOtherCollider, &UpdatePos))
		{
			// 当たった点の座標セット
			SetPos(UpdatePos);

			// 矩形コライダー座標更新
			m_pColliderBox->SetPos(UpdatePos);
		}
	}

	//// エリアと判定
	//auto Area = CTutorialObject::GetInstance()->GetEventArea();
	//if (!Area) return;

	//// 当たっていたら
	//if (Area->Collision(m_pSphereCollider.get()))
	//{
	//	// コライダー座標更新
	//	m_pSphereCollider->SetPos(UpdatePos);

	//	// UI表示をする
	//	m_pPutSign->SetIsDraw(true);

	//	// サインの座標設定
	//	m_pPutSign->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + 240.0f, GetPos().z));

	//	// 仲間アリの位置を変更
	//	if (pKey->GetPress(DIK_RETURN) || pPad->GetPress(CJoyPad::JOYKEY_A))
	//	{
	//		auto ArrayAnt = CTutorialObject::GetInstance()->GetArrayAnt();
	//		ArrayAnt->SetPos(Area->GetPos());
	//		ArrayAnt->SetIsTopFollow(false);
	//	}
	//}
	//else
	//{
	//	// UI表示をしない
	//	m_pPutSign->SetIsDraw(false);
	//}

	// 親クラスの更新
	CMoveCharactor::Update();
}
//=========================================================
// チュートリアルトップアリの描画処理
//=========================================================
void CTutoTopAnt::Draw(void)
{
	// 親クラスの描画
	CMoveCharactor::Draw();
}
//=========================================================
// キー入力移動関数
//=========================================================
void CTutoTopAnt::Moving(CJoyPad * pPad,CInputKeyboard * pKey)
{
	// 移動量を取得
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 rotdest = GetRotDest();

	// パッドのスティック入力があったら
	if (pPad->GetLeftStick()) return;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera == nullptr) return;

	// 移動フラグ
	bool isMove = false;

	if (pKey->GetPress(DIK_A) || pPad->GetPress(CJoyPad::JOYKEY_LEFT))
	{// Aキー

		if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_RIGHT))
		{// 左斜め上

			move.x += sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::SPEED;
			move.z += cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
			isMove = true;
		}
		else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
		{// 右斜め下

			move.x -= sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::SPEED;
			move.z -= cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
			isMove = true;

		}
		else
		{// 単体
			move.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::SPEED;
			move.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
			isMove = true;
		}

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_D) || pPad->GetPress(CJoyPad::JOYKEY_RIGHT))
	{// Dキーを押した

		if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_UP))
		{// Wキーを押した
			move.x += sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::SPEED;
			move.z += cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.75f);
			isMove = true;
		}
		else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
		{// Sキーを押した
			move.x -= sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::SPEED;
			move.z -= cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.25f);
			isMove = true;
		}
		else
		{// Dキーのみ押した
			move.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::SPEED;
			move.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::SPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.5f);
			isMove = true;
		}

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_UP))
	{// Wキーを押した

		move.x += sinf(pCamera->GetRot().y) * Config::SPEED;
		move.z += cosf(pCamera->GetRot().y) * Config::SPEED;
		rotdest.y = pCamera->GetRot().y - D3DX_PI;
		isMove = true;

		// 角度を正規化
		if (rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			rot.y += Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
	{// Sキーを押した

		move.x -= sinf(pCamera->GetRot().y) * Config::SPEED;
		move.z -= cosf(pCamera->GetRot().y) * Config::SPEED;
		rotdest.y = pCamera->GetRot().y;
		isMove = true;

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}

	if (isMove)
	{
		// MOVEじゃなかったら切り替え
		if (GetMotion()->GetMotionType() != MOVE)
		{
			GetMotion()->SetMotion(MOVE);
		}
	}
	else
	{
		// NEUTRALに遷移する
		if (GetMotion()->GetMotionType() == MOVE)
		{
			//　タイプ切り替え
			GetMotion()->SetMotion(NEUTRAL, true, 10);
		}
	}

	if (rotdest.y - rot.y > D3DX_PI)
	{// 左回転
		// 角度
		rot.y = rot.y + Config::NorRot;
	}
	else if (rot.y - rotdest.y > D3DX_PI)
	{// 右回転
		// 角度
		rot.y = rot.y - Config::NorRot;
	}

	// 現在の角度
	rot.y += (rotdest.y - rot.y) * 0.25f;

	// キャラクターに適用する
	SetRot(rot);
	SetRotDest(rotdest);
	SetMove(move);
 }
//=========================================================
// パッド入力移動関数
//=========================================================
void CTutoTopAnt::MovePad(CJoyPad * pPad)
{
	// 移動量を取得
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rotdest = GetRotDest();

	// 入力デバイス取得
	XINPUT_STATE* pStick = pPad->GetStickAngle();

	// 入力受け取り失敗時
	if (!pPad->GetLeftStick()) return;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera == nullptr) return;

	// フラグ
	bool isMoving = false;
	static bool wasStick = false;

	// 取得できたら
	if (pPad->GetLeftStick())
	{
		// 左スティックの角度
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// デッドゾーンを設定
		float DeadZone = 32767.0f * 0.25f;
		float fMag = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

		if (fMag > DeadZone)
		{
			// 正規化
			float normalizeX = (LStickAngleX / fMag);
			float normalizeY = (LStickAngleY / fMag);

			// 移動量を計算する
			float MoveX = normalizeX * cosf(-pCamera->GetRot().y) - normalizeY * sinf(-pCamera->GetRot().y);
			float MoveZ = normalizeX * sinf(-pCamera->GetRot().y) + normalizeY * cosf(-pCamera->GetRot().y);

			// 移動量を設定
			move.x += MoveX * Config::SPEED;
			move.z += MoveZ * Config::SPEED;
			rotdest.y = atan2f(-MoveX, -MoveZ);
			isMoving = true;
		}
	}

	if (isMoving)
	{
		// MOVEじゃなかったら
		if (GetMotion()->GetMotionType() != MOVE)
		{
			// モーション変更
			GetMotion()->SetMotion(MOVE);
		}
	}
	else if (!isMoving && wasStick)
	{
		// 離した瞬間の判定
		if (GetMotion()->GetMotionType() == MOVE)
			GetMotion()->SetMotion(NEUTRAL, true, 10);
	}

	// フラグを変更する
	wasStick = isMoving;

	// 適用
	SetMove(move);
	SetRotDest(rotdest);
}
//=========================================================
// 指示を分ける関数
//=========================================================
void CTutoTopAnt::Separation(void)
{
	// 自身の体から半径を作成
	D3DXVECTOR3 pos = GetPos();

	// 半径加算処理
	m_fSeparationRadius += Config::Separation;

	if (m_fSeparationRadius >= Config::MAX_RADIUS)
		m_fSeparationRadius = Config::MAX_RADIUS;

	// 設定する
	SetSeparationRadius(m_fSeparationRadius);

	// オブジェクトのサイズ更新
	m_pCircleObj->SetPos(pos);
	m_pCircleObj->SetSize(m_fSeparationRadius, 3.0f);

	// サインの座標設定
	m_pSeparationSign->SetPos(D3DXVECTOR3(pos.x, pos.y + 240.0f, pos.z));
}
//=========================================================
// 矩形の当たり判定処理
//=========================================================
bool CTutoTopAnt::Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos)
{
	// nullなら
	if (m_pColliderBox == nullptr) return false;

	return CCollisionBox::Collision(m_pColliderBox.get(), pOther, pOutPos);
}