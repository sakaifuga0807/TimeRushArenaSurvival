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

	//�����o�ϐ�
	GameCamera* m_gameCamera;
	Vector3 m_velocity;
	Player* m_player;//�v���C���[
	Vector3 m_toCameraPos;//�����_���王�_�Ɍ������x�N�g��
	ModelRender modelRender;//���f�������_�[

};

