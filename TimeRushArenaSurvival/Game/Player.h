#pragma once
//プレイヤークラス
class Enemy;

class Player :public IGameObject
{
public:

	enum EnPlayerState
	{
		enPlayerState_GameOver,//ゲームクリアー
		enPlayerState_GameClear,//ゲームオーバー
		enPlayerState_GameClear_Idle//ゲームクリアー（待機中）
	};

	Player();
	~Player();
	//更新処理
	void Update();
	//回転処理
	void Rotation();
	//ステート管理
	const void ManageState();
	//描画処理
	void Render(RenderContext& rc);
	//移動処理
	void Move();
	//弾を作成
	void MakeBullet();
	//攻撃
	void Attack();

	//座標を取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//メンバ変数
	SoundSource* m_bgm=nullptr;
	Vector3 EnemyVec;

	Enemy*m_enemy;
	ModelRender m_modelRender;//モデルレンダー
	Vector3 m_moveSpeed;//移動速度
	Quaternion m_rotation;//回転
	Vector3 m_position = Vector3::Zero;//座標
	Vector3 m_forward = Vector3::AxisZ;
	CharacterController m_characterController;//キャラコン

	int m_MaxHP =10;
	int playerState = 0;//プレイヤーの状況
	int m_playerHP=10;//プレイヤーのHP
	float m_characonR = 0.0f; //キャラコンの半径

private:
	
};
