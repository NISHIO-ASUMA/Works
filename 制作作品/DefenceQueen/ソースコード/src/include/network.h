//=========================================================
//
// ネットワーク管理クラス処理 [ network.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// オンライン通信ネットワークを管理するクラスを定義
//*********************************************************
class CNetWork
{
public:

	CNetWork();
	~CNetWork();

	HRESULT Init(void);
	void Uninit(void);

	bool Connect(const char* ip, int port);
	void Disconnect(void);

	bool SendInt(int nSendNumber);
	bool RecvInt(int* pOutData);

	inline bool GetIsConnect(void) const { return m_isConnected; }

private:

	static constexpr int RECVSIZE = 5;

	SOCKET m_sock;		// ソケット変数
	bool m_isConnected;	// 接続したか
};