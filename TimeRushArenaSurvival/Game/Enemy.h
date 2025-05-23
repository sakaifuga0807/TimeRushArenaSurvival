#pragma once

//クラス宣言
class Player;
class Game;

class Enemy:public	IGameObject
{
public:
	//エネミーステート
	enum EnEnenemyState
	{
		enEnemyState_Idle,					//待機。
		enEnemyState_Chase,					//追跡。
		enEnemyState_Attack,				//攻撃。
		enEnemyState_ReceiveDamage,			//被ダメージ。
		enEnemyState_Down,					//ダウン。
	};
public:
	//メンバ関数
	Enemy();
	~Enemy();

	void Update();
	void Render(RenderContext&rc);
	bool Start();

	void Init(Vector3 initPos);
	//回転処理
	void Rotation();
	//攻撃処理
	void Attack();
	//追跡処理
	void Chase();
	//攻撃用のコリジョンを作る処理
	void MakeAttackCollision();
	//当たり判定
	void Collision();
	//追従処理
	const bool SerchPlayer() const;
	//アニメーション
	void PlayAnimation();
	//プレイヤーと敵の判定
	void IsHit();


	//HPを設定する
	void setHP(const int hp)
	{
		m_hp = hp;
	}

	//座標を設定
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//座標を取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//大きさを取得
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	////ステート関係////
	//ステート管理
	void ManageState();

	//待機ステート
	void ProcessIdleStateTransition();

	//追跡ステート
	void ProcessChaseStateTransition();

	//攻撃ステート
	void ProcessAttackStateTransition();

	//被ダメステート
	void ProcessReceiveDamageStateTransition();

	//ダウンステート
	void ProcessDownStateTransition();

	//ステートの移行処理
	void ProcessCommonStateTransition();

	//攻撃できる距離かどうかを調べる
	//攻撃できるならtrue
	const bool IsCanAttack() const;
	//アニメーションイベント用の関数
	//void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	//アニメーションのステート
	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Attack,
		enAnimationClip_Damage,
		enAnimationClip_Down,
		enAnimationClip_Num,
	};

	//メンバ変数
	bool m_gameClear = false;
	int m_stopTimer;
	Game*m_game;
	SoundSource* m_bgm = nullptr;
	bool m_GameClearFlag=false;
	int m_DefeatedEnemy = 50;
	AnimationClip m_animationClips[enAnimationClip_Num];//アニメーションクリップ
	EnEnenemyState m_enemyState = enEnemyState_Idle;//敵のステート
	Quaternion m_rotation;
	Vector3 m_moveVector;//移動方向ベクトル
	Vector3 m_position;//座標
	ModelRender m_modelRender;//モデルレンダー
	Vector3 m_moveSpeed;//移動速度
	CharacterController m_charaCon;//キャラコン

	int m_swordBoneId = -1;
	bool m_isUnderAttack = false;//攻撃中か？
	Vector3 m_forward = Vector3::AxisZ;//敵の正面ベクトル
	Player* m_player = nullptr;//プレイヤー
	Vector3 m_scale = Vector3(15.0f,15.0f,15.0f);//スケール
	int m_hp = 6;//HP
	float m_idleTimer=0.0f ;//追跡タイマー
	float m_chaseTimer=0.0f;//待機タイマー
	float m_characonR = 0.0f; //キャラコンの半径
	float m_hitCoolTime = 3.0f;//判定を取るクールタイム
	float m_GameClearDrawTime = 1.5f;//ゲームクリアを表示させるタイマー
	bool m_flag = true;//判定をtrueで動作させる
};

