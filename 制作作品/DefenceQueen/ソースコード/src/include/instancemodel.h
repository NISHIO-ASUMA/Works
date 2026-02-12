//=========================================================
//
// インスタンシング適用モデル処理 [ instancingmodel.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インスタンシング適用するモデルオブジェクトクラスを定義
//*********************************************************
class CInstanceModel
{
public:

	CInstanceModel();
	~CInstanceModel();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename, const bool isShadow);
	void Uninit(void);
	void Update(const D3DXMATRIX& mtx);
	void Draw(const D3DXMATRIX& mtx);

	void DrawMtxShadow(void);
	void SetModelPass(const char* pModelName);
	void SetParent(CInstanceModel* pModel);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_scale = scale; }
	inline void SetMtxWorld(const D3DXMATRIX& mtx) { m_mtxworld = mtx; }
	inline void setLocalMtx(const D3DXMATRIX& mtxlocal) { m_Locla = mtxlocal; }

	inline void OffSetPos(const D3DXVECTOR3& offpos) { m_offPos = offpos; }
	inline void OffSetRot(const D3DXVECTOR3& offrot) { m_offRot = offrot; }
	inline void SetMtxShadow(bool isShadow) { m_isShadow = isShadow; }

	inline D3DXMATRIX GetMtxWorld(void) { return m_mtxworld; }
	inline int GetModelIdx(void) const { return m_nModelIdx; }

	static CInstanceModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename, const bool isShadow);

	void SetTargetPos(const D3DXVECTOR3& pos)
	{
		m_prevPos = m_nextPos;
		m_nextPos = pos;
		m_interp = 0.0f;
	}

	void SetTargetRot(const D3DXVECTOR3& rot)
	{
		m_prevRot = m_nextRot;
		m_nextRot = rot;
		m_interp = 0.0f;
	}

private:

	D3DXVECTOR3 m_pos, m_rot, m_offPos, m_offRot, m_scale; // 座標,角度,大きさ
	D3DXMATRIX m_mtxworld;			// ワールドマトリックス
	D3DXMATRIX m_Locla;				// ローカルマトリックス
	CInstanceModel* m_pParent;		// 親モデル

	int* m_pTexture;	// テクスチャポインタ
	bool m_isShadow;	// 影を設定するかどうか
	int m_nModelIdx;	// モデルインデックス
	bool m_isInst;		

	int m_nUpdateCount; // 更新フレームカウント

	static constexpr int MAX_UPDATECOUNT = 2;

	D3DXVECTOR3 m_prevPos, m_nextPos = { VECTOR3_NULL };
	D3DXVECTOR3 m_prevRot, m_nextRot = { VECTOR3_NULL };
	float m_interp = 0.0f;
};