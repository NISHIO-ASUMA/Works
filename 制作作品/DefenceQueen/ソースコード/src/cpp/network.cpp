//=========================================================
//
// ネットワーク管理クラス処理 [ network.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#include "network.h"

//=========================================================
// コンストラクタ
//=========================================================
CNetWork::CNetWork() : m_sock(INVALID_SOCKET),m_isConnected(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CNetWork::~CNetWork()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================a
HRESULT CNetWork::Init(void)
{
	// セットアップ
	WSADATA wsaData;

	// スタートアップに失敗したら
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		return E_FAIL;
	}

	// サーバーと接続開始
	Connect("127.0.0.1", 22333);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CNetWork::Uninit(void)
{
	// 接続消去
	Disconnect();

	// WinSockクリーンアップ
	WSACleanup();
}
//=========================================================
// 接続処理
//=========================================================
bool CNetWork::Connect(const char* ip, int port)
{
	// もし接続されているなら
	if (m_isConnected) return true;

	// ソケット作成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	// 生成に失敗したら
	if (m_sock == INVALID_SOCKET) return false;

	// ソケット設定
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	// 例外設定
	if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
	{
		// 接続終了
		Disconnect();
		return false;
	}

	// 接続が失敗なら
	if (connect(m_sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		Disconnect();
		return false;
	}

	// 接続済みに設定
	m_isConnected = true;

	// 結果を返す
	return true;
}
//=========================================================
// 接続終了処理
//=========================================================
void CNetWork::Disconnect(void)
{
	// NULLチェック
	if (m_sock == INVALID_SOCKET)
	{
		// 接続されていない時
		return;
	}

	// クライアント切断
	closesocket(m_sock);

	// ソケットをNULL化する
	m_sock = INVALID_SOCKET;

	// フラグ初期化
	m_isConnected = false;
}
//=========================================================
// int型の数値をサーバーに送る
//=========================================================
bool CNetWork::SendInt(int nSendNumber)
{
	// 接続されていない または ソケットがnull値なら
	if (!m_isConnected || m_sock == INVALID_SOCKET)
		return false;

	// データ格納変数
	int nSent = 0;
	const int nSendSize = sizeof(int);

	// 送るサイズがint以下なら
	while (nSent < nSendSize)
	{
		// データをサーバーに送信
		int nData = send(m_sock,(char*)&nSendNumber + nSent,nSendSize - nSent,0);

		// データがないなら
		if (nData <= 0)
		{
			return false;
		}

		// 加算する
		nSent += nData;
	}

	// 完了フラグを返す
	return true;
}
//=========================================================
// int型の数値をサーバーから受け取る
//=========================================================
bool CNetWork::RecvInt(int* pOutData)
{
	// 接続されていない または ソケットがnull値
	if (!m_isConnected || m_sock == INVALID_SOCKET)
		return false;

	// 受け取り用変数
	int nReceived = 0;
	const int nRecvSize = 20; // 固定長

	while (nReceived < nRecvSize)
	{
		// 実際の受け取り
		int nRecvData = recv(m_sock,(char*)pOutData + nReceived, nRecvSize - nReceived,0);

		// 受け取り終わったら
		if (nRecvData <= 0)
		{
			// 最終エラーを取得
			int nErrerId = WSAGetLastError();

			// 例外メッセージ
			char buf[256];
			sprintf_s(buf, "RecvInt recv error : %d\n", nErrerId);
			OutputDebugStringA(buf);

			return false;
		}

		// 要素を更新
		nReceived += nRecvData;
	}

	//m_isConnected = false;

	return true;
}