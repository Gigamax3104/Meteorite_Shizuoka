#pragma once
#include "../../Matsunaga/Vector/Vector.h"
#include "../../Matsunaga/Meteorite/Meteorite.h"

#include <vector>

class Bullet;

class System {
private:
    std::vector<Meteorite>/*型を「Meteorite」に変更*/ meteors;  // 隕石の保管

public:
    void Init(Vector2 pos, double size, int health); // 初期化
    void Update();                                   // アップデート
    void LoadStage();                                // ステージ読み込み

    //追加部分
    std::vector<Meteorite>& GetMeteorite() { return meteors; } //隕石全体を取得する
};