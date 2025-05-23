#pragma once

class Player;
class Bullet;
class GameCamera;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void CameraVector();


	//void GetCameraForward();
	//void Render(RenderContext& rc);

	//メンバ変数
	GameCamera* m_gameCamera;
	Vector3 m_velocity;
	Player* m_player;//プレイヤー
	Vector3 m_toCameraPos;//注視点から視点に向かうベクトル
	ModelRender modelRender;//モデルレンダー

};

