#include "Turret.h"
#include "Ammo.h"
#include "Dxlib.h"
#include "../../Iwasaki/Time/Time.h"

#include <cmath>

using namespace std;
//関数名、クラス名はアッパーキャメルケース、変数名はローワースネークケース、ただし、グローバル変数はアッパースネークケース
//プレイヤーに関するクラス定義

////////////////////////////////////////////////////////////////////
//コンストラクタ
Turret::Turret() :
	position_(0.0, 0.0), center_position_(0.0, 0.0), move_radius_(100.0), move_clockwise_(false),
	move_counter_clockwise_(false), degree_(0), speed_(1.0), input_shoot_(false), is_turret_broken_(false), turret_broken_process_time_(0.0),
	turret_repair_time_(3.0), turret_hit_radius_(10), turret_reloading_process_time_(0.0), turret_shoot_span_(0.7), is_turret_reloaded_(true) {}

////////////////////////////////////////////////////////////////////
void Turret::InitTurretPosition(const Vector2& window_pos) {
	center_position_ = Vector2(window_pos);
	position_ = Vector2(center_position_.x, center_position_.y - move_radius_); //タレットの初期座標設定

	degree_ = 90;
}

//プレイヤーのインプットを受け取る
void Turret::InputTurretPosition() {
	if (m_input.GetKey(KEY_INPUT_D) == 1) {
		move_clockwise_ = true;
	}
	if (m_input.GetKey(KEY_INPUT_A) == 1) {
		move_counter_clockwise_ = true;
	}
}

void Turret::UpdateTurretPosition(const Vector2& window_pos) {
	m_input.Update();

	if (move_clockwise_) {
		degree_ -= 72 * Time::delta_time;
		move_clockwise_ = false;
	}
	if (move_counter_clockwise_) {
		degree_ += 72 * Time::delta_time;
		move_counter_clockwise_ = false;
	}

	double radian = cos(degree_ / 180 * 3.141592 * speed_), radian2 = sin(degree_ / 180 * 3.141592 * speed_);

	position_.x = (radian * move_radius_ + center_position_.x);
	position_.y = (-radian2 * move_radius_ + center_position_.y);

	int size = ammoes_.size();

	Vector2 ammo_position_;
	double ammo_radius_;
	bool ammo_flag_;

	for (auto ammo = ammoes_.begin();ammo != ammoes_.end();) {
		bool is_area_destroy_ = false;
		ammo->SetAmmoPosition();
		ammo->GiveAmmoInfo(&ammo_position_, &ammo_radius_, &ammo_flag_);
		if (ammo_position_.x + ammo_radius_ < 0) {
			ammo->DestroyAmmo();
			ammo = ammoes_.erase(ammo);
			is_area_destroy_ = true;
		}
		else if (ammo_position_.x - ammo_radius_ > window_pos.x) {
			ammo->DestroyAmmo();
			ammo = ammoes_.erase(ammo);
			is_area_destroy_ = true;
		}
		else if (ammo_position_.y + ammo_radius_ < 0) {
			ammo->DestroyAmmo();
			ammo = ammoes_.erase(ammo);
			is_area_destroy_ = true;
		}
		else if (ammo_position_.y - ammo_radius_ > window_pos.y) {
			ammo->DestroyAmmo();
			ammo = ammoes_.erase(ammo);
			is_area_destroy_ = true;
		}
		else {
			ammo->RenderAmmo();
			ammo++;
		}
	}
}

//プレイヤーのX座標情報を渡す
double Turret::GiveTurretPositionX() {
	return position_.x;
}

//プレイヤーのY座標情報を渡す
double Turret::GiveTurretPositionY() {
	return position_.y;
}

Vector2 Turret::GiveTurretPosition() {
	return position_;
}

//プレイヤーの偏角を渡す
double Turret::GiveTurretDegree() {
	return degree_;
}

void Turret::InputTurretShoot() {
	//プレイヤーの発射入力を検知する
	if (m_input.GetKeyDown(KEY_INPUT_SPACE) == 1 && is_turret_broken_ == false && is_turret_reloaded_ == true) {
		is_turret_reloaded_ = false;
		Ammo ammo(position_, degree_);
		ammoes_.push_back(ammo);
	}
}

//temp
void Turret::RenderTurretAndTurretCenterPoint() {
	size_t size = ammoes_.size();

	//プレイヤーとプレイヤーの移動の中心を描画
	DrawPixel(center_position_.x, center_position_.y, 0xffffff);
	DrawPixel(position_.x, position_.y, 0xffffff);
	DrawCircle(center_position_.x, center_position_.y, 3, 0xffffff, true);
	DrawCircle(position_.x, position_.y, 3, 0xffffff, true);

	//砲台の見た目の代わりになる箱を描画
	//DrawBox(position_.x - turret_hit_radius_, position_.y - turret_hit_radius_, position_.x + turret_hit_radius_ +1, position_.y + turret_hit_radius_ +1, 0x00ff00, FALSE);
	//y座標について、本来はsinを使うべきですが、0が出力されているようなので、sinはcosを90度ずらしたものと同じという性質を使ってます
	left_up_position.x = position_.x + cos((180 - 45 + degree_) / 180 * 3.141592) * turret_hit_radius_;
	left_up_position.y = position_.y + cos((180 - 45 + 90 + degree_) / 180 * 3.141592) * turret_hit_radius_;

	left_down_position.x = position_.x + cos((180 + 45 + degree_) / 180 * 3.141592) * turret_hit_radius_;
	left_down_position.y = position_.y + cos((180 + 48 + 90 + degree_) / 180 * 3.141592) * turret_hit_radius_;

	right_up_position.x = position_.x + cos((45 + degree_) / 180 * 3.141592) * turret_hit_radius_;
	right_up_position.y = position_.y + cos((45 + 90 + degree_) / 180 * 3.141592) * turret_hit_radius_;

	right_down_position.x = position_.x + cos((-45 + degree_) / 180 * 3.141592) * turret_hit_radius_;
	right_down_position.y = position_.y + cos((-45 + 90 + degree_) / 180 * 3.141592) * turret_hit_radius_;

	DrawLine(left_up_position.x, left_up_position.y, right_up_position.x, right_up_position.y, 0x00ff00);
	DrawLine(right_up_position.x, right_up_position.y, right_down_position.x, right_down_position.y, 0x00ff00);
	DrawLine(right_down_position.x, right_down_position.y, left_down_position.x, left_down_position.y, 0x00ff00);
	DrawLine(left_down_position.x, left_down_position.y, left_up_position.x, left_up_position.y, 0x00ff00);
}

//タレットが被弾した時に呼ぶ関数、タレットを発射不能にする
void Turret::SetTurretBroken() {
	is_turret_broken_ = true;
}

//タレットが被弾してからの経過時間を計算する関数
void Turret::TurretBrokenProcessTime() {
	if (is_turret_broken_ == true) {
		turret_broken_process_time_ += Time::delta_time;
		if (turret_broken_process_time_ >= turret_repair_time_) {
			is_turret_broken_ = false;
			turret_broken_process_time_ = 0;
		}
	}
}

//タレットの半径を渡す関数
double Turret::GetTurretHitRadius() {
	return turret_hit_radius_;
}

//タレットが再装填を開始してからの経過時間を計算する関数
void Turret::TurretReloadProcessTime() {
	if (is_turret_reloaded_ == false) {
		turret_reloading_process_time_ += Time::delta_time;
		if (turret_reloading_process_time_ >= turret_shoot_span_) {
			is_turret_reloaded_ = true;
			turret_reloading_process_time_ = 0;
		}
	}
}
