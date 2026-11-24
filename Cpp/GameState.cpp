#include	"../h/ResultState.h"
#include	"../h/GameState.h"

#include	"../Iwasaki/Time/Time.h"

#include	"../Iwasaki/Collision/Collision.h"
#include	"../Iwasaki/Window.h"

#include	"../Matsunaga/Debug/Debug.h"

using namespace std;

void GameState::Enter() {
	m_window.Init(Vector2(1024, 720));
	SetGraphMode(m_window.GetSize().x, m_window.GetSize().y, m_window.GetColorBitDepth());

	m_system.Init();

	m_turret.InitTurretPosition(m_window.GetSize() / 2); //砲台の位置の初期化
	m_earth.Initialize(Vector2(m_window.GetSize() / 2), 50.0, 5);

	Debug::AddString("Turret_Pos", [this]() { return m_turret.GiveTurretPosition(); }, 0xffffff, 0, "Turret", 1, true, true);
	Debug::AddString("Window_Pos", [this]() { return m_window.GetSize();}, 0xffffff, 0, "Window", 0, true, true);
}

StateBase* GameState::Loop() {
	Debug::Draw(Input::GetDebugValue());

	Time::Update();

	DrawString(0, 20, "Game", 0xffffff);

	m_system.Update(); //システムの更新
	if (m_system.IsTimeUp()) return new ResultState(true);

	m_earth.Update();

	std::vector<Meteorite>& meteorites = m_system.GetMeteorite(); //隕石の取得

	for (auto meteoriteIt = meteorites.begin(); meteoriteIt != meteorites.end(); ) {
		if (Collision::CircleAndCircle(m_earth.GetPos(), m_earth.GetSize(), meteoriteIt->Position(), meteoriteIt->Radius())) {
			m_earth.OnHit(meteoriteIt->GetDamage());
			meteoriteIt = meteorites.erase(meteoriteIt);

			if (m_earth.IsDestroyed()) return new ResultState(false);
		}
		else
		{
			meteoriteIt++;
		}
	}
	m_turret.InputTurretPosition(); //プレイヤーの入力処理による移動

	for (auto meteoriteIt = meteorites.begin(); meteoriteIt != meteorites.end(); ) {
		if (Collision::CircleAndCircle(m_turret.GiveTurretPosition(), m_turret.GetTurretHitRadius(), meteoriteIt->Position(), meteoriteIt->Radius())) {
			m_turret.SetTurretBroken(); break;
		}
		else
		{
			meteoriteIt++;
		}
	}

	m_turret.TurretBrokenProcessTime();
	m_turret.TurretReloadProcessTime();

	m_turret.InputTurretShoot(); //プレイヤーの入力処理による弾発射
	m_turret.UpdateTurretPosition(m_window.GetSize()); //砲台と弾の移動更新
	m_turret.RenderTurretAndTurretCenterPoint(); //砲台と弾の描画処理

	std::vector<Ammo>& ammos = m_turret.GiveAmmo(); //弾の取得

	for (auto ammo = ammos.begin(); ammo != ammos.end(); )
	{
		bool isCollision = false;

		for (auto meteoriteIt = meteorites.begin(); meteoriteIt != meteorites.end(); )
		{
			//弾と隕石の当たり判定
			if (Collision::CircleAndCircle((ammo)->Position(), (ammo)->Radius(), (meteoriteIt)->Position(), (meteoriteIt)->Radius()))
			{
				meteoriteIt = meteorites.erase(meteoriteIt);

				ammo->DestroyAmmo();
				ammo = ammos.erase(ammo);

				isCollision = true;

				break;
			}
			else
			{
				meteoriteIt++;
			}
		}

		if (!isCollision)
		{
			ammo->AmmoAreaDestroy(m_window);
			if (!ammo->Flag()) { ammo = ammos.erase(ammo); continue; }

			ammo++;
		}
	}

	if (m_turret.GiveInput().GetKeyDown(KEY_INPUT_RETURN)) return new ResultState(false);

	return this;
}

void GameState::Exit() {

}