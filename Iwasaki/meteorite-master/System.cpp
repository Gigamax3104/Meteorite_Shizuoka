#include "DxLib.h"
#include "System.h"
#include "../Window.h"
#include <cstdlib>
#include <cmath> // 角度計算用（cos, sin）

void System::Init(Vector2 pos, double size, int health) {
    // 初期化処理
}

void System::Update() {
    Vector2 centerPos(Window::GetPos().x / 2, Window::GetPos().y / 2); // 変更部分:画面の中心座標（例：幅1080×高さ720の中央）

    // 約2%の確率で、四方八方から隕石を生成する
    if (GetRand(100) < 2) {
        double angle = GetRand(360) * 3.14159265 / 180.0; // 1. 周囲のどこから出すか（0〜360度をランダムに決める）
        double radius = 400.0; // 画面の中心から少し離れた外側から発生させる

        Vector2 spawnPos(
            centerPos.x + cos(angle) * radius, // 2. 角度と半径から、出現する座標（画面の外）を計算する
            centerPos.y + sin(angle) * radius
        );

        double speed = 45 + (double)(GetRand(30)); // 3. スピードを決める

        Meteorite newMeteor(spawnPos, speed, 30, centerPos); // 4. 生成してリスト（meteors）に追加する（中心に向かうように centerPos を渡す）
                                                             //変更部分:Meteoriteのコンストラクタの引数に合わせて変数を設定
                                                             //詳しくは松永の「Meteorite」ヘッダー内にある「Meteorite.h」を参考
        meteors.push_back(newMeteor);
    }

    for (auto meteoriteIt = meteors.begin(); meteoriteIt != meteors.end(); meteoriteIt++)
    {
        (meteoriteIt)->Update();
        (meteoriteIt)->Draw();
    }
}

void System::LoadStage() {
    // ステージ読み込み処理
}