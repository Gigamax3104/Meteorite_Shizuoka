#include "Debug.h"
#include <vector> // std::vector用
#include <algorithm> // std::sort用
#include "Dxlib.h"

namespace
{

	struct DebugItem
	{
		std::string tag;    // 文字列にタグを付ける
		std::string text; // 表示する文字列
		std::function<std::string()> getter; // 値を文字列で取得する関数
		std::function<void()> drawFunc; // 図形
		unsigned int color;
		int priority;
		int mode;
		bool isDrawPriority;
		bool isDrawTag;
	};

	std::vector<DebugItem> list;
	bool isDirty = false;	// 追加されたときのフラグ

	void EraseInternal(int targetMode, const std::string& targetTag)
	{
		if (targetMode == -1 && targetTag.empty())
		{
			list.clear();
			return;
		}

		list.erase(
			std::remove_if(list.begin(), list.end(), [targetMode, &targetTag](const DebugItem& item) {
				bool matchMode = (targetMode == -1) || (item.mode == targetMode);
				bool matchTag = targetTag.empty() || (item.tag == targetTag);
				return matchMode && matchTag;
				}),
			list.end()
		);
	}

	void DrawInternal(const int targetMode, const std::string& targetTag = "", bool onlyEmptyTag = false)
	{
		if (isDirty)
		{
			std::sort(list.begin(), list.end(), [](const DebugItem& a, const DebugItem& b)
				{
					return a.priority < b.priority;
				});
			isDirty = false;
		}
		int y = 0;
		// Draw関数内のループ
		for (const auto& item : list)
		{
			if (targetMode != -1 && item.mode != targetMode)
			{
				continue;
			}
			if (onlyEmptyTag)
			{
				if (!item.tag.empty())
				{
					continue;
				}
			}
			if (!targetTag.empty() && item.tag != targetTag)
			{
				continue;
			}

			// 1. 図形が登録されていれば描画する
			if (item.drawFunc)
			{
				item.drawFunc();
			}

			// 2. 文字（またはgetter）が登録されていれば描画して改行する
			if (!item.text.empty() || item.getter)
			{
				std::string prefix;
				if (item.isDrawPriority)
				{
					prefix += "[" + std::to_string(item.priority) + "]";
				}
				if (item.isDrawTag && !item.tag.empty())
				{
					prefix += "[" + item.tag + "]";
				}

				if (item.getter)
				{
					std::string valStr = item.getter();
					DrawFormatString(0, y, item.color, "%s%s : %s", prefix.c_str(), item.text.c_str(), valStr.c_str());
				}
				else
				{
					DrawFormatString(0, y, item.color, "%s%s", prefix.c_str(), item.text.c_str());
				}

				y += 20; // 文字を描画したときだけY座標を下げる
			}
		}
	}

}

namespace Debug
{
	void AddInternal(const std::string& label, std::function<std::string()> getter, std::function<void()> drawFunc, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag)
	{
		list.emplace_back(DebugItem{ tag, label, getter, drawFunc, color, priority, mode, isDrawPriority, isDrawTag });
		isDirty = true;
	}


	void AddString(const std::string& label, std::function<std::string()> getter, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag)
	{
		AddInternal(label, getter, nullptr, color, priority, tag, mode, isDrawPriority, isDrawTag);
	}
	void AddString(const std::string& label, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag)
	{
		// 値がない場合は空文字を返す関数を登録
		AddInternal(label, nullptr, nullptr, color, priority, tag, mode, isDrawPriority, isDrawTag);
	}

	void AddString(const std::string& label, std::function<Vector2()> getter, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag)
	{
		auto strGetter = [getter]() -> std::string
			{
				if (!getter)
				{
					return "";
				}
				Vector2 v = getter();
				return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
			};

		AddInternal(label, strGetter, nullptr, color, priority, tag, mode, isDrawPriority, isDrawTag);
	}
	void AddString(const std::string& label, std::function<Vector3()> getter, unsigned int color, int priority, const std::string& tag, int mode, bool isDrawPriority, bool isDrawTag)
	{
		auto strGetter = [getter]() -> std::string
			{
				if (!getter)
				{
					return "";
				}
				Vector3 v = getter();
				return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
			};

		AddInternal(label, strGetter, nullptr, color, priority, tag, mode, isDrawPriority, isDrawTag);
	}
	void AddCircle(const Vector2* pos, const int* r, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [pos, r, color, fillFlag]()
			{
				if (pos && r)
				{
					DrawCircle((int)pos->x, (int)pos->y, *r, color, fillFlag);
				}

			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddBox(const Vector2* pos, const Vector2* size, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [pos, size, color, fillFlag]()
			{
				if (pos && size)
				{
					DrawBox((int)pos->x - ((int)size->x / 2), (int)pos->y - ((int)size->y / 2), (int)pos->x + ((int)size->x / 2), (int)pos->y + ((int)size->y / 2), color, fillFlag);
				}

			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddLine(const Vector2* posA, const Vector2* posB, unsigned int color, const std::string& tag, int mode)
	{
		auto draw = [posA, posB, color]()
			{
				if (posA && posB)
				{
					DrawLine((int)posA->x, (int)posA->y, (int)posB->x, (int)posB->y, color);
				}

			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}


	void AddCircle(const Vector2 pos, int r, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [pos, r, color, fillFlag]()
			{
				DrawCircle((int)pos.x, (int)pos.y, r, color, fillFlag);
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddBox(const Vector2 pos, const Vector2 size, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [pos, size, color, fillFlag]()
			{
				DrawBox((int)pos.x - ((int)size.x / 2), (int)pos.y - ((int)size.y / 2), (int)pos.x + ((int)size.x / 2), (int)pos.y + ((int)size.y / 2), color, fillFlag);
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddLine(const Vector2 posA, const Vector2 posB, unsigned int color, const std::string& tag, int mode)
	{
		auto draw = [posA, posB, color]()
			{
				DrawLine((int)posA.x, (int)posA.y, (int)posB.x, (int)posB.y, color);
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddCircle(std::function<Vector2()> posGetter, std::function<int()> rGetter, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [posGetter, rGetter, color, fillFlag]()
			{
				if (posGetter && rGetter)
				{
					Vector2 pos = posGetter();
					int r = rGetter();
					DrawCircle((int)pos.x, (int)pos.y, r, color, fillFlag);
				}
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddBox(std::function<Vector2()> posGetter, std::function<Vector2()> sizeGetter, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [posGetter, sizeGetter, color, fillFlag]()
			{
				if (posGetter && sizeGetter)
				{
					Vector2 pos = posGetter();
					Vector2 size = sizeGetter();
					DrawBox((int)pos.x - ((int)size.x / 2), (int)pos.y - ((int)size.y / 2), (int)pos.x + ((int)size.x / 2), (int)pos.y + ((int)size.y / 2), color, fillFlag);
				}
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}

	void AddLine(std::function<Vector2()> posAGetter, std::function<Vector2()> posBGetter, unsigned int color, bool fillFlag, const std::string& tag, int mode)
	{
		auto draw = [posAGetter, posBGetter, color, fillFlag]()
			{
				if (posAGetter && posBGetter)
				{
					Vector2 posA = posAGetter();
					Vector2 posB = posBGetter();
					DrawLine((int)posA.x, (int)posA.y, (int)posB.x, (int)posB.y, color);
				}
			};
		AddInternal("", nullptr, draw, color, 0, tag, mode, false, false);
	}
	void Erase(const int targetMode)
	{
		EraseInternal(targetMode, "");
	}

	void Erase(const std::string& targetTag)
	{
		if (targetTag.empty())
		{
			return;
		}
		EraseInternal(-1, targetTag);
	}

	void Erase(const int targetMode, const std::string& targetTag)
	{
		if (targetTag.empty())
		{
			return;
		}
		EraseInternal(targetMode, targetTag);
	}

	void Clear()
	{
		list.clear();
	}


	void Draw(const int targetMode)
	{
		DrawInternal(targetMode, "");
	}

	void Draw(const std::string& targetTag)
	{
		if (targetTag.empty())
		{
			return;
		}
		DrawInternal(-1, targetTag);
	}

	void Draw(const int targetMode, const std::string& targetTag)
	{
		if (targetTag.empty())
		{
			return;
		}
		DrawInternal(targetMode, targetTag);
	}

	void Draw()
	{
		DrawInternal(0, "", true);
	}

	void AllDraw()
	{
		DrawInternal(-1, "");
	}
}
