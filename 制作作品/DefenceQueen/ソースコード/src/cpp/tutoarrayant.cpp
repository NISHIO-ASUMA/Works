//=========================================================
//
// チュートリアルの仲間アリの処理クラス [ tutoarrayant.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "tutoarrayant.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "tutorialobject.h"
#include "tutorialtopant.h"
#include "template.h"
#include "feed.h"
#include "eventarea.h"

//=========================================================
// コンストラクタ
//=========================================================
CTutoArrayAnt::CTutoArrayAnt(int nPriority) : CMoveCharactor(nPriority),
m_pSphereCollider(nullptr),
m_DestPos(VECTOR3_NULL),
m_isTopFollow(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CTutoArrayAnt::~CTutoArrayAnt()
{

}
//=========================================================
// 生成処理
//=========================================================
CTutoArrayAnt* CTutoArrayAnt::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CTutoArrayAnt* pTutoAnt = new CTutoArrayAnt;
	if (pTutoAnt == nullptr) return nullptr;

	// オブジェクト設定
	pTutoAnt->SetPos(pos);
	pTutoAnt->SetRot(rot);
	pTutoAnt->SetDestPos(pos);
	pTutoAnt->SetUseStencil(false);
	pTutoAnt->SetUseOutLine(false);

	// 初期化失敗時
	if (FAILED(pTutoAnt->Init())) return nullptr;

	return pTutoAnt;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutoArrayAnt::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// オブジェクトの種類をセット
	SetObjType(CObject::TYPE_ARRAY);

	// モーションセット
	MotionLoad("data/MOTION/Array/Array_Motion.txt", MOTION_MAX, true);

	// スフィアコライダー生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), Config::MAX_RADIUS);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutoArrayAnt::Uninit(void)
{
	// コライダーポインタの破棄
	m_pSphereCollider.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CTutoArrayAnt::Update(void)
{
	// 現在座標の取得
	auto Pos = GetPos();

	// 追従フラグが有効なら
	if (m_isTopFollow)
	{
		FollowTopAnt();
	}

	// 元の位置にもどってくるのが有効なら
	if (m_isReturnBase)
	{
		ReturnBase();
	}
	
	// 座標のみ更新
	CMoveCharactor::UpdatePosition();

	// 更新座標取得
	auto UpdatePos = GetPos();

	// コライダー座標の更新
	m_pSphereCollider->SetPos(UpdatePos);

	// チュートリアルの餌との当たり判定
	auto TutoFeed = CTutorialObject::GetInstance()->GetTutoFeed();
	if (TutoFeed == nullptr) return;

	// 当たり判定結果がtrueの時
	if (Collision(TutoFeed->GetCollider()))
	{
		// 餌にダメージ
		TutoFeed->DecLifeTuto(1);

		// 元居た場所にかえって来る
		m_isReturnBase = true;
		m_isTopFollow = false;
	}

	// 親クラス全体の更新処理
	CMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutoArrayAnt::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();
}
//=========================================================
// 球形の当たり判定
//=========================================================
bool CTutoArrayAnt::Collision(CSphereCollider* other)
{
	// nullなら
	if (m_pSphereCollider == nullptr) return false;

	// 球形クラスの判定関数を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(),other);
}
//=========================================================
// トップアリ追従関数
//=========================================================
void CTutoArrayAnt::FollowTopAnt(void)
{
	// 追従対象を取得
	auto Top = CTutorialObject::GetInstance()->GetTutoTopAnt();

	// 目的地までの距離を算出
	D3DXVECTOR3 moveVec = Top->GetPos() - GetPos();
	float distToDest = D3DXVec3Length(&moveVec);

	// 到着したら止まる
	if (distToDest < Config::MAX_DISTANCE)
	{
		// 移動量を0にする
		SetMove(VECTOR3_NULL);

		// 待機モーションに設定
		GetMotion()->SetMotion(NEUTRAL);
	}
	else
	{
		// 目的地へ直接移動
		D3DXVec3Normalize(&moveVec, &moveVec);
		moveVec *= Config::SPEED;

		// 目的角を計算
		float fAngle = atan2(-moveVec.x, -moveVec.z);
		D3DXVECTOR3 Rotdest = GetRotDest();

		// 正規化関数
		Rotdest.y = NormalAngle(fAngle);

		// 目的角の値をセット
		SetRotDest(Rotdest);

		// 移動量を加算
		SetMove(moveVec);

		// 移動モーションにセット
		GetMotion()->SetMotion(MOVE);
	}
}
//=========================================================
// 元の位置にもどってくる関数
//=========================================================
void CTutoArrayAnt::ReturnBase(void)
{
	// ベクトル生成
	D3DXVECTOR3 moveVec = m_DestPos - GetPos();
	float distToDest = D3DXVec3Length(&moveVec);

	// 到着したら止まる
	if (distToDest < 2.0f)
	{
		// 角度と移動量を初期化する
		SetMove(VECTOR3_NULL);
		SetRot(VECTOR3_NULL);

		// 待機モーションに設定
		GetMotion()->SetMotion(NEUTRAL);

		m_isReturnBase = false;
	}
	else
	{
		// 目的地へ直接移動
		D3DXVec3Normalize(&moveVec, &moveVec);
		moveVec *= Config::SPEED;

		// 目的角を計算
		float fAngle = atan2(-moveVec.x, -moveVec.z);
		D3DXVECTOR3 Rotdest = GetRotDest();

		// 正規化関数
		Rotdest.y = NormalAngle(fAngle);

		// 目的角の値をセット
		SetRotDest(Rotdest);

		// 移動量を加算
		SetMove(moveVec);

		// 移動モーションにセット
		GetMotion()->SetMotion(MOVE);
	}
}