#include "../pch.h"
#include "PakPakMonster.h"
#include "Color.h"

using namespace DirectX;

// コンストラクタ
PakPakMonster::PakPakMonster()
	: m_scene(SCENE::Title)
	, m_bgColor{}
	, m_score(0)
	, m_highscore{}
	, m_stage(0)
	, m_level(0)
	, m_rest(0)
	, m_timer(0)
	, m_pause(false)
	, m_pauseReason(PauseReason::None)
	, m_pauseTimer(0)
	, m_monsterEatCnt(0)
{
}

// シーンの初期化
void PakPakMonster::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// 各変数の初期化
	m_rest = LEVEL1_PLAYER_CNT;				// 残機数
	m_score = 0;							// 得点
	m_highscore[0] = m_highscore[1] = 0;	// ハイスコア
	m_scene = SCENE::Title;					// タイトル画面
	m_level = 0;							// ゲームモード（簡単）

	// 背景色（黒）
	m_bgColor = GetColor(s_color[COLOR_BLACK]);
}

// シーンの更新
void PakPakMonster::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	switch (m_scene)
	{
	case SCENE::Title:	// タイトル画面
		UpdateTitle();
		break;
	case SCENE::Result:	// 結果画面
		UpdateResult();
		break;
	case SCENE::Game:	// ゲーム中
		UpdateGame();
		break;
	default:
		break;
	}
}

// シーンの描画
void PakPakMonster::Render()
{
	auto* debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"PakPakMonster", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	auto states = GetUserResources()->GetCommonStates();

	// 背景色の描画
	SetBgColor(m_bgColor);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->AlphaBlend(), states->PointClamp());

	switch (m_scene)
	{
	case SCENE::Title:	// タイトル画面
		DrawTitle();
		break;
	case SCENE::Result:	// 結果画面
		DrawResult();
		break;
	case SCENE::Game:	// ゲーム中
		DrawGame();
		break;
	default:
		break;
	}

	m_spriteBatch->End();
}

void PakPakMonster::Finalize()
{
	m_monsterTex.Reset();
	m_food.Reset();
	m_playerTex.Reset();
	m_bg03.Reset();
	m_bg02.Reset();
	m_bg01.Reset();
	m_result.Reset();
	m_number.Reset();

	m_spriteBatch.reset();
	m_primitiveBatch.reset();
	m_basicEffect.reset();
}

void PakPakMonster::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// ベーシックエフェクト
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(false);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// テクスチャの読み込み
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, m_number.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/result.dds", nullptr, m_result.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_01.dds", nullptr, m_bg01.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_02.dds", nullptr, m_bg02.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_03.dds", nullptr, m_bg03.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/player.dds", nullptr, m_playerTex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/food.dds", nullptr, m_food.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/monster.dds", nullptr, m_monsterTex.ReleaseAndGetAddressOf()));
}

void PakPakMonster::CreateWindowSizeDependentResources()
{
	// 画面サイズを取得
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();

	// 正射影行列の作成
	m_proj = SimpleMath::Matrix::CreateOrthographicOffCenter(
		0.0f, static_cast<float>(rect.right), static_cast<float>(rect.bottom), 0.0f, 0.0f, 1.0f);
}

void PakPakMonster::OnDeviceLost()
{
	Finalize();
}

// 背景色の描画
void PakPakMonster::SetBgColor(DirectX::SimpleMath::Color color)
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	VertexPositionColor v[4] =
	{
		{ SimpleMath::Vector3(    0.0f,     0.0f, 0.0f), color },
		{ SimpleMath::Vector3(SCREEN_W,     0.0f, 0.0f), color },
		{ SimpleMath::Vector3(SCREEN_W, SCREEN_H, 0.0f), color },
		{ SimpleMath::Vector3(    0.0f, SCREEN_H, 0.0f), color },
	};

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());

	// エフェクトの設定
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(SimpleMath::Matrix::Identity);
	m_basicEffect->SetProjection(m_proj);
	m_basicEffect->Apply(context);

	// 入力レイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	// 背景を指定色で描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(v[0], v[1], v[2], v[3]);
	m_primitiveBatch->End();
}

// 数字の描画
void PakPakMonster::DrawNumber(int x, int y, int number, int keta, DirectX::SimpleMath::Color color, int dis)
{
	int max = 1;

	// 数字が桁数に入らない場合表示を修正する
	for (int i = 0; i < keta; i++)
	{
		max *= 10;
	}
	if (number >= max) number = max - 1;

	// 得点を桁ごとに描画していく
	for (int i = 0; i < keta; i++)
	{
		int val = number % 10;
		number /= 10;
		// テクスチャの切り出す矩形
		RECT srcRect = { 40 * val, 0, 40 * val + 5 * 8, 5 * 8 };
		m_spriteBatch->Draw(
			m_number.Get(),
			SimpleMath::Vector2(static_cast<float>(x - (32 + dis) * i + (keta - 1) * (32 + dis)), static_cast<float>(y)),
			&srcRect,
			color);
	}
}

bool PakPakMonster::Pause()
{
	if (m_pauseReason == PauseReason::None) return false;

	// タイマー減算
	if (m_pauseTimer > 0) m_pauseTimer--;

	// ポーズしている理由によって
	switch (m_pauseReason)
	{
	case PauseReason::Start:	// ゲームスタート時
		if (m_pauseTimer == 0)
		{
			// ポーズを解除してゲームスタート！
			m_pauseReason = PauseReason::None;
			// プレイヤーも動き出す
			m_player.SetState(Player::PlayerState::Normal);
		}
		break;
	case PauseReason::Eat:		// モンスターを食べた時
		// モンスターが目になるまではポーズする
		if ( (m_monster.ChangeBody(static_cast<int>(Monster::Type::Type1), m_stage, m_level) == true)
		  && (m_monster.ChangeBody(static_cast<int>(Monster::Type::Type2), m_stage, m_level) == true))
		{
			m_pauseReason = PauseReason::None;
		}
		break;
	case PauseReason::Clear:	// ゲームクリア時
		if (m_pauseTimer == 0)
		{
			m_pauseReason = PauseReason::None;
			m_stage++;
			m_map.InitiaizeFood();
			m_scene = SCENE::Result;
			InitializeResult();
		}
		break;
	case PauseReason::Dead:		// プレイヤーが死んだ時
		DeadPlayer();
		break;
	case PauseReason::Gameover:	// ゲームオーバー時
		GameOver();
		break;
	}
	return true;
}

// タイトル画面の更新
void PakPakMonster::UpdateTitle()
{
	auto tracker = GetUserResources()->GetKeyboardStateTracker();

	// Zキーでスタート
	if (tracker->pressed.Z)
	{
		// ゲームへ
		m_scene = SCENE::Game;
		// ゲーム開始時の初期化
		InitializeGame(true);
	}
	// Xキーで難易度変更 
	if (tracker->pressed.X)
	{
		if (m_level == 0)
		{
			m_level = 1;
			m_rest = LEVEL2_PLAYER_CNT;		// 残機数
		}
		else
		{
			m_level = 0;
			m_rest = LEVEL1_PLAYER_CNT;		// 残機数
		}
	}
}

// タイトル画面の描画
void PakPakMonster::DrawTitle()
{
	// 背景の描画
	m_spriteBatch->Draw(m_result.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// ゲームモードの表示
	DrawNumber(18 * 16 + 8, 5 * 16 + 8, m_level + 1, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// 残機数
	DrawNumber(16 * 16 + 8, 20 * 16 + 8, m_rest, 1,	GetColor(s_color[COLOR_CYAN]), 8);

	// ハイスコア
	DrawNumber(8 * 16, 15 * 16 + 4, m_highscore[m_level], 5, GetColor(s_color[COLOR_GRAY]), 8);
}

void PakPakMonster::InitializeResult()
{
	// 背景色（黒）
	m_bgColor = GetColor(s_color[COLOR_BLACK]);

	// 3秒
	m_timer = 60 * 3;
}

// 結果画面の更新
void PakPakMonster::UpdateResult()
{
	if (m_timer > 0) m_timer--;

	if (m_timer == 0)
	{
		// 次のステージへ
		m_scene = SCENE::Game;
		// ゲームの初期化
		InitializeGame(false);
	}
}

// 結果画面の描画
void PakPakMonster::DrawResult()
{
	// 背景の描画
	m_spriteBatch->Draw(m_result.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// ゲームモードの表示
	DrawNumber(18 * 16 + 8, 5 * 16 + 8, m_level + 1, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// 残機数
	DrawNumber(16 * 16 + 8, 20 * 16 + 8, m_rest, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// 得点
	DrawNumber(8 * 16, 15 * 16 + 4, m_score, 5, GetColor(s_color[COLOR_GRAY]), 8);
}

// ゲームの初期化
void PakPakMonster::InitializeGame(bool value)
{
	// 背景色（黒）
	m_bgColor = GetColor(s_color[COLOR_BLACK]);

	// ポーズ
	m_pause = true;

	// ポーズ理由
	m_pauseReason = PauseReason::Start;

	// ポーズ時間
	m_pauseTimer = START_WAIT;

	// マップの初期化
	m_map.InitializeMap();

	// ボーナスえさの初期化
	m_map.InitializeBonus();

	// プレイヤーの初期化
	m_player.Initialize();

	// モンスターの初期化
	m_monster.Initialize();

	// ゲーム開始時の初期化
	if (value)
	{
		m_stage = 0;			// ステージ初期化
		m_score = 0;			// スコア初期化
		m_map.InitiaizeFood();	// えさを初期化
	}
}

// ゲームの更新
void PakPakMonster::UpdateGame()
{
	// 画面が止まった時の処理
	if (Pause()) return;

	// プレイヤーの位置を取得
	int playerX, playerY;
	m_player.GetPlayerPos(&playerX, &playerY);

	// マップの更新
	m_map.Update();

	// モンスターの更新
	m_monster.Update(m_map, m_stage, m_level, playerX, playerY);

	// プレイヤーの更新
	bool eatPowerup = false;
	m_player.Update(m_map, &m_score, &eatPowerup);
	// パワーえさを食べたのでモンスターを弱らせる
	if (eatPowerup) m_monster.WeakMonster();
	// パワーアップ中でなければ連続で食べた数を初期化
	if (!m_player.IsPowerup()) m_monsterEatCnt = 0;

	// モンスターと接触した？
	int type;
	if (HitCheck(&type) == TRUE)
	{
		// 通常はプレイヤーは死亡
		if (m_monster.GetState(type) == Monster::State::Normal)
		{
			// プレイヤーは食べられた
			m_player.Dead();
			m_monster.SetState(type, Monster::State::Eat);
			m_pauseReason = PauseReason::Dead;
			m_pauseTimer = PAUSE_TIME_DEAD;
		}
		else
		{
			// モンスターが弱っている場合は食べる
			if (m_monster.GetState(type) == Monster::State::Weak)
			{
				// モンスターは食べられた
				m_monster.Dead(type);
				m_monsterEatCnt++;
				m_score += Player::SCORE_MONSTER * m_monsterEatCnt;
				m_pauseReason = PauseReason::Eat;
			}
		}
	}
	// ステージクリア？
	if (m_map.GetFoodCnt() == 0)
	{
		// 画面を止めて、結果画面へ
		m_pauseReason = PauseReason::Clear;
		m_pauseTimer = PAUSE_TIME_CLEAR;
		// プレイヤーはアニメーションを停止させる
		m_player.SetState(Player::PlayerState::Stop);
	}
}

// ゲームの描画
void PakPakMonster::DrawGame()
{
	DrawMap();
	DrawMonster();
	DrawPlayer();

	// デバッグ表示
//static char* mode[] =
//{
//	"PAUSE_NONE",
//	"PAUSE_START",
//	"PAUSE_CLEAR",
//	"PAUSE_EAT",
//	"PAUSE_DEAD",
//	"PAUSE_OVER",
//};

//DrawFormatString(0, 0, GetColor(255, 255, 255), "%s", mode[g_pauseReason]);
//DrawFormatString(0, 20, GetColor(255, 255, 255), "score = %d", g_score);
//DrawFormatString(0, 40, GetColor(255, 255, 255), "eat_score = %d", g_player.eat_score);

//DrawFormatString(0, 20, GetColor(255, 255, 255), "anime1 = %d", g_monster[0].animeTimer);
//DrawFormatString(0, 40, GetColor(255, 255, 255), "anime2 = %d", g_monster[1].animeTimer);

}

// マップの描画
void PakPakMonster::DrawMap()
{
	// ステージの色
	int stage_color[] = { COLOR_GREEN, COLOR_CYAN, COLOR_BLACK };

	// えさの色
	int food_color[] =
	{
		COLOR_BLACK, COLOR_GREEN, COLOR_PINK, COLOR_ORANGE, COLOR_GREEN, COLOR_PINK, COLOR_ORANGE
	};

	int id;

	// 難易度によってステージの色の変え方を変える
	if (m_level == 0)
	{
		id = m_stage % 2;
	}
	else
	{
		id = m_stage % 3;
	}

	int color = stage_color[id];

	// 背景の描画
	m_spriteBatch->Draw(m_bg01.Get(), SimpleMath::Vector2(0.0f, 0.0f));
	m_spriteBatch->Draw(m_bg02.Get(), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Color(GetColor(s_color[color])));

	// デバッグ用（通路の絵）
	//m_spriteBatch->Draw(m_bg03.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// えさの描画
	for (int i = 0; i < Map::FOOD_MAX; i++)
	{
		const Map::FoodInfo* food = m_map.GetFoodInfo(i);

		// 表示位置を設定
		float x = static_cast<float>(food->x * Map::CHIP_SIZE + 8);
		float y = static_cast<float>(food->y * Map::CHIP_SIZE);

		// えさの色を設定
		color = food_color[static_cast<int>(food->state)];

		// えさの矩形
		RECT srcRect = { 0, 0, 56, 56 };

		switch (food->state)
		{
			// 通常のえさ
		case Map::FoodState::Food_3:
		case Map::FoodState::Food_2:
		case Map::FoodState::Food_1:
			m_spriteBatch->Draw(m_food.Get(), SimpleMath::Vector2(x, y), &srcRect, SimpleMath::Color(GetColor(s_color[color])));
			break;
			// パワーえさ
		case Map::FoodState::Power_3:
		case Map::FoodState::Power_2:
		case Map::FoodState::Power_1:
			srcRect.left += 56;
			srcRect.right += 56;
			m_spriteBatch->Draw(m_food.Get(), SimpleMath::Vector2(x, y), &srcRect, SimpleMath::Color(GetColor(s_color[color])));
			break;
		}
	}

	const Map::BonusInfo* bonus = m_map.GetBonusInfo();

	// ボーナスえさの表示
	if (bonus->state == Map::BonusState::Display)
	{
		// えさの矩形
		RECT srcRect = { 56 * 2, 0, 56 * 3, 56 };
		// 色はチカチカさせる
		static int blinkColor = 0;
		blinkColor = (blinkColor + 1) % (COLOR_MAX - 1);
		m_spriteBatch->Draw(m_food.Get(),
			SimpleMath::Vector2(static_cast<float>(bonus->x + 8), static_cast<float>(bonus->y)),
			&srcRect, SimpleMath::Color(GetColor(s_color[blinkColor + 1])));
	}

	// ボーナスの得点の表示
	if (bonus->state == Map::BonusState::Score)
	{
		DrawNumber(9 * 16 + 8, 12 * 16 + 8, 100, 3, GetColor(s_color[COLOR_BLACK]), 16);
	}
}

void PakPakMonster::DrawMonster()
{
	m_monster.UpdateAnime();

	for (int i = 0; i < Monster::MONSTER_CNT; i++)
	{
		SimpleMath::Color color;
		int anime_y;
		if (m_monster.GetMonsterType(i) == Monster::Type::Type1)
		{
			// 追いかけモンスターの色
			anime_y = 0;
			color = GetColor(s_color[COLOR_PINK]);
		}
		else
		{
			// 気まぐれモンスターの色
			anime_y = 1;
			color = GetColor(s_color[COLOR_PURPLE]);
		}
		// モンスターが弱っている
		Monster::State state = m_monster.GetState(i);
		if ( state == Monster::State::Weak
		  || state == Monster::State::Dead
		  || state == Monster::State::Return
		  || state == Monster::State::Revive )
		{
			// 水色
			color = GetColor(s_color[COLOR_CYAN]);
			// 弱り状態が終わる２秒前から点滅させる
			int weakTime = m_monster.GetWeakTime(i);
			if (weakTime < 2 * 60 && (weakTime >> 2) % 2)
			{
				// 灰色
				color = GetColor(s_color[COLOR_GRAY]);
			}
		}
		int x, y;
		m_monster.GetPos(i, &x, &y);
		SimpleMath::Vector2 pos(static_cast<float>(x + 8), static_cast<float>(y));
		int id = static_cast<int>(m_monster.GetAnimeNo(i));
		RECT srcRect = { id * 56, anime_y * 56, (id + 1) * 56, (anime_y + 1) * 56 };
		m_spriteBatch->Draw(m_monsterTex.Get(), pos, &srcRect, color);
	}
}

void PakPakMonster::DrawPlayer()
{
	// アニメーションの更新
	m_player.UpdateAnime();

	// アニメーションIDを取得
	int id = m_player.GetAnimeId();
	
	// 表示なし
	if (id == static_cast<int>(Player::PlayerAnime::None)) return;

	// 位置を取得
	int x, y;
	m_player.GetPlayerPos(&x, &y);
	SimpleMath::Vector2 pos(static_cast<float>(x + 8), static_cast<float>(y));

	// 切り出し矩形
	RECT srcRect = { id * 56, 0, (id + 1) * 56, 56};
	// 左向きなら左右反転する
	SpriteEffects flip = SpriteEffects_None;
	if (m_player.IsLeft()) flip = SpriteEffects_FlipHorizontally;
	m_spriteBatch->Draw(m_playerTex.Get(), pos, &srcRect, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 1, flip);
}

// プレイヤーが死んだ時の処理
void PakPakMonster::DeadPlayer()
{
	if (m_pauseTimer == 0)
	{
		m_rest--;
		// 残機数がなければゲームオーバー
		if (m_rest == 0)
		{
			// ゲームオーバーへ
			m_pauseReason = PauseReason::Gameover;
			m_pauseTimer = PAUSE_TIME_GAMEOVER;
		}
		else
		{
			// 結果画面へ
			m_scene = SCENE::Result;
			InitializeResult();
		}
	}
}

// ゲームオーバー時の処理
void PakPakMonster::GameOver()
{
	if (m_pauseTimer == 0)
	{
		// タイトルへ
		m_scene = SCENE::Title;

		// ハイスコアの更新
		if (m_highscore[m_level] < m_score) m_highscore[m_level] = m_score;

		// スタート時の残機数の設定
		if (m_level == 0)
		{
			m_rest = LEVEL1_PLAYER_CNT;
		}
		else
		{
			m_rest = LEVEL2_PLAYER_CNT;
		}
		// 結果画面の初期化
		InitializeResult();
	}

	// 画面をチカチカさせる
	if ((m_pauseTimer >> 3) % 2)
	{
		m_bgColor = GetColor(s_color[COLOR_PINK]);
	}
	else
	{
		m_bgColor = GetColor(s_color[COLOR_BLACK]);
	}

	return;
}

// モンスターとの接触判定
bool PakPakMonster::HitCheck(int* type)
{
	int px, py;
	int mx, my;

	// 位置を取得
	m_player.GetPlayerPos(&px, &py);

	for (int i = 0; i < Monster::MONSTER_CNT; i++)
	{
		m_monster.GetPos(i, &mx, &my);
		if (px + 8 < mx + CHARACTER_SIZE - 8 && mx + 8 < px + CHARACTER_SIZE - 8
			&& py + 8 < my + CHARACTER_SIZE - 8 && my + 8 < py + CHARACTER_SIZE - 8)
		{
			*type = i;
			return true;
		}
	}
	return false;
}
