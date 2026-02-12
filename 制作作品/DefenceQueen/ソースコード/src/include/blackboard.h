//=========================================================
//
// ノードに関するブラックボード処理 [ blackboard.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include <string>
#include <map>
#include <any>
#include <unordered_map>
#include <stdexcept>

//*********************************************************
// ブラックボードのベースクラス定義
//*********************************************************
class CBlackBoard
{
public:

	/// <summary>
	/// 要素をセットするテンプレート関数
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="KeyData">キーの名前</param>
	/// <param name="value">そのキーの値</param>
	template<class T> void SetValue(const std::string& KeyData, const T& value)
	{
		// 値をセットするs
		m_Data[KeyData] = value;
	}

	/// <summary>
	/// 要素を取得するテンプレート関数
	/// </summary>
	/// <typeparam name="T">型情報</typeparam>
	/// <param name="KeyData">格納されている情報名</param>
	/// <returns></returns>
	template<class T> T GetValue(const std::string& KeyData) const
	{
		// 情報を見つける
		auto iter = m_Data.find(KeyData);

		// 情報があったら
		if (iter != m_Data.end())
		{
			// 任意の型変換
			return std::any_cast<T>(iter->second);
		}

		// 例外処理
		throw std::runtime_error("キーのデータがありません : " + KeyData);
	}

	/// <summary>
	/// キーの情報があるかどうかを返す関数
	/// </summary>
	/// <param name="Key">検索するキーの名前</param>
	/// <returns></returns>
	bool HasKeyData(const std::string& Key) const
	{
		return m_Data.find(Key) != m_Data.end();
	}

	/// <summary>
	/// 要素のクリアを行う関数
	/// </summary>
	/// <param name=""></param>
	void Clear(void)
	{
		m_Data.clear();
	}

private:

	// NOTE : 複数の情報を保持するためunordered_map型の変数を使う
	// 文字列を使ってキーを判別する
	std::unordered_map<std::string, std::any> m_Data;
};