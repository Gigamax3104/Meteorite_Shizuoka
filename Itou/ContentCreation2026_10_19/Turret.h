#pragma once
#include	<vector>
#include	"../../Matsunaga/Input/Input.h"
#include	"Ammo.h"
#include	"../../Matsunaga/Vector/Vector.h"

//クラス名、関数名はアッパーキャメルケース、変数名はローワースネークケース、ただし、グローバル変数はアッパースネークケース
//プレイヤーに関するクラス宣言

class Turret {
private:
	Vector2 position_;
	Vector2 center_position_;
	double move_radius_;
	bool move_clockwise_;
	bool move_counter_clockwise_;
	//turret_degreeは度数法であって弧度法ではない
	double degree_;
	double speed_;
	bool input_shoot_;

	Input m_input;

	std::vector<Ammo> ammoes_;
	//タレットが破損(発射不能になっているか)を管理するbool
	bool is_turret_broken_;
	//タレットが被弾してからの経過時間を計算する変数
	double turret_broken_process_time_;
	//タレットの修復にかかる時間、備考 : コンストラクタで設定する
	double turret_repair_time_;

	//タレットの半径、備考 : コンストラクタで設定する
	double turret_hit_radius_;
	//タレット描画の左上頂点の座標
	Vector2 left_up_position;
	//タレット描画の左下頂点の座標
	Vector2 left_down_position;
	//タレット描画の右上頂点の座標
	Vector2 right_up_position;
	//タレット描画の右下頂点の座標
	Vector2 right_down_position;

	//タレットの再装填時間の計算に使う変数
	double turret_reloading_process_time_;
	//タレットの再装填にかかる時間、コンストラクタで設定する
	double turret_shoot_span_;
	//タレットが装填済みかを管理する変数、発射検知も兼ねる
	bool is_turret_reloaded_;
public:
	//コンストラクタ
	Turret();
	//砲台の初期座標設定
	void InitTurretPosition(const Vector2& window_pos);
	//プレイヤーのインプットを受け取る
	void InputTurretPosition();
	//プレイヤーの位置更新
	void UpdateTurretPosition(const Vector2& window_pos);
	//プレイヤーのX座標情報を渡す
	double GiveTurretPositionX();
	//プレイヤーののY座標情報を渡す
	double GiveTurretPositionY();
	//プレイヤーの座標情報を渡す
	Vector2 GiveTurretPosition();
	//プレイヤーの偏角を渡す
	double GiveTurretDegree();

	//ここから追加・変更部分
	const Ammo& GiveAmmo(int ID) { return ammoes_[ID]; } /*弾を取得する*/
	std::vector<Ammo>& GiveAmmo() { return ammoes_; } /*弾全体を取得する*/

	Input GiveInput() { return m_input; } /*入力処理を取得する*/
	//ここまで

	//プレイヤーの発射入力を検知する
	void InputTurretShoot();
	//プレイヤーとプレイヤーの移動の中心を表示する
	void RenderTurretAndTurretCenterPoint();
	//タレットが被弾したときに呼ぶ関数、タレットを発射不能にする
	void SetTurretBroken();
	//タレットが被弾してから何秒経過したか計算する関数
	void TurretBrokenProcessTime();
	//タレットの半径を渡す関数
	double GetTurretHitRadius();
	//タレットが再装填を開始してからの経過時間を計算する関数
	void TurretReloadProcessTime();
};