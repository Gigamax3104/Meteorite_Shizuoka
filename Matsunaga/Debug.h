#pragma once

#include <string> // std::string用
#include <functional> // std::function用
#include <ios> // std::boolalpha用
#include <sstream>
#include <iomanip>
#include "Vector/Vector.h"


inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
namespace Debug
{
	void AddInternal(const std::string& label, std::function<std::string()> getter, std::function<void()> drawFunc, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag);
	void AddString(const std::string& label, std::function<std::string()> getter, unsigned int color = 0xffffff, int priority = 0, const std::string& tag = "", int mode = 0, bool isDrawPriority = false, bool isDrawTag = false);
	void AddString(const std::string& label, std::function<Vector2()> getter, unsigned int color = 0xFFFFFF, int priority = 0, const std::string& tag = "", int mode = 0, bool isDrawPriority = false, bool isDrawTag = false);
	void AddString(const std::string& label, std::function<Vector3()> getter, unsigned int color = 0xFFFFFF, int priority = 0, const std::string& tag = "", int mode = 0, bool isDrawPriority = false, bool isDrawTag = false);
	void AddString(const std::string& label, unsigned int color = 0xffffff, int priority = 0, const std::string& tag = "", int mode = 0, bool isDrawPriority = false, bool isDrawTag = false);
	template <typename T>
	void AddString(const std::string& label, const T* valuePtr, unsigned int color = 0xffffff, int priority = 0, const std::string& tag = "", int mode = 0, bool isDrawPriority = false, bool isDrawTag = false)
	{
		// 描画時に呼び出される「最新の値を文字列化する処理」を作成	
		auto getter = [valuePtr]() -> std::string
			{
				if (!valuePtr)
				{
					return "null";
				}
				std::ostringstream oss;
				oss << std::boolalpha; // bool値を true / false にする
				oss << std::fixed << std::setprecision(2); // 少数第二位まで
				oss << *valuePtr;
				return oss.str();
			};
		AddInternal(label, getter, nullptr, color, priority, tag, mode, isDrawPriority, isDrawTag);
	}

	void AddCircle(const Vector2* pos, const int* r, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddBox(const Vector2* pos, const Vector2* size, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddLine(const Vector2* posA, const Vector2* posB, unsigned int color = 0xffffff, const std::string& tag = "", int mode = 0);

	void AddCircle(const Vector2 pos, int r, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddBox(const Vector2 pos, const Vector2 size, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddLine(const Vector2 posA, const Vector2 posB, unsigned int color = 0xffffff, const std::string& tag = "", int mode = 0);

	void AddCircle(std::function<Vector2()> posGetter, std::function<int()> rGetter, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddBox(std::function<Vector2()> posGetter, std::function<Vector2()> sizeGetter, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);
	void AddLine(std::function<Vector2()> posAGetter, std::function<Vector2()> posBGetter, unsigned int color = 0xffffff, bool fillFlag = false, const std::string& tag = "", int mode = 0);

	void Erase(const int targetMode); // モード指定削除
	void Erase(const std::string& targetTag); // タグ指定削除
	void Erase(const int targetMode, const std::string& targetTag); // モード、タグ指定で表示
	void Clear(); // 全て削除
	void Draw(const int targetMode); // モード指定で表示
	//void Draw(const std::string& targetTag); // タグ指定で表示
	//void Draw(const int targetMode, const std::string& targetTag); // モード、タグ指定で表示
	//void Draw(); // デフォルト表示(モードがタグを指定していないデバッグを表示（0, ""))
	void AllDraw(); // モード、タグ関係なくすべて表示



}