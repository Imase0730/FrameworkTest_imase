// -------------------------------------------------------------------------------------
// File: GameScene.cpp
//
// �Q�[���̃��C���N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "GameScene.h"
#include "Color.h"
#include "TitleScene.h"

using namespace DirectX;
using namespace PakPakMonster;

// �R���X�g���N�^
GameScene::GameScene()
	: m_scene(Scene::Title)
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

// �V�[���̏�����
void GameScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �e�ϐ��̏�����
	m_rest = LEVEL1_PLAYER_CNT;				// �c�@��
	m_score = 0;							// ���_
	m_highscore[0] = m_highscore[1] = 0;	// �n�C�X�R�A
	m_scene = Scene::Title;					// �^�C�g�����
	m_level = 0;							// �Q�[�����[�h�i�ȒP�j

	// �w�i�F�i���j
	m_bgColor = GetColor(s_color[COLOR_BLACK]);
}

// �V�[���̍X�V
void GameScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto key = GetUserResources()->GetKeyboardStateTracker();
	if (key->pressed.A)
	{
		ChangeScene<TitleScene>();
	}

	switch (m_scene)
	{
	case Scene::Title:	// �^�C�g�����
		UpdateTitle();
		break;
	case Scene::Result:	// ���ʉ��
		UpdateResult();
		break;
	case Scene::Game:	// �Q�[����
		UpdateGame();
		break;
	default:
		break;
	}
}

// �V�[���̕`��
void GameScene::Render()
{
	auto* debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"PakPakMonster", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
	debugFont->AddString(L"Z:Start X:Level", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 2));

	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �r���[�|�[�g�̐ݒ�i��ʒ����ɕ`�悷��j
	auto viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	viewport.TopLeftX = (viewport.Width - SCREEN_W) / 2.0f;
	viewport.TopLeftY = (viewport.Height - SCREEN_H) / 2.0f;
	context->RSSetViewports(1, &viewport);

	// �w�i�F�̕`��
	SetBgColor(m_bgColor);

	m_spriteBatch->Begin();

	switch (m_scene)
	{
	case Scene::Title:	// �^�C�g�����
		DrawTitle();
		break;
	case Scene::Result:	// ���ʉ��
		DrawResult();
		break;
	case Scene::Game:	// �Q�[����
		DrawGame();
		break;
	default:
		break;
	}

	m_spriteBatch->End();

	// �r���[�|�[�g��߂�
	viewport = GetUserResources()->GetDeviceResources()->GetScreenViewport();
	context->RSSetViewports(1, &viewport);
}

// �I������
void GameScene::Finalize()
{
	// �e�N�X�`�������
	m_monsterTex.Reset();
	m_foodTex.Reset();
	m_playerTex.Reset();
	m_bg03Tex.Reset();
	m_bg02Tex.Reset();
	m_bg01Tex.Reset();
	m_resultTex.Reset();
	m_numberTex.Reset();

	m_spriteBatch.reset();
	m_primitiveBatch.reset();
	m_basicEffect.reset();
}

// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
void GameScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	// �x�[�V�b�N�G�t�F�N�g
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(false);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// �X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// �e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/number.dds", nullptr, m_numberTex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/result.dds", nullptr, m_resultTex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_01.dds", nullptr, m_bg01Tex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_02.dds", nullptr, m_bg02Tex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/bg_03.dds", nullptr, m_bg03Tex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/player.dds", nullptr, m_playerTex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/food.dds", nullptr, m_foodTex.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/monster.dds", nullptr, m_monsterTex.ReleaseAndGetAddressOf()));
}

// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
void GameScene::CreateWindowSizeDependentResources()
{
	// ��ʃT�C�Y���擾
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();

	// ���ˉe�s��̍쐬
	m_proj = SimpleMath::Matrix::CreateOrthographicOffCenter(
		0.0f, static_cast<float>(rect.right), static_cast<float>(rect.bottom), 0.0f, 0.0f, 1.0f);
}

// �f�o�C�X���X�g�������ɌĂяo�����֐�
void GameScene::OnDeviceLost()
{
	Finalize();
}

// �w�i�F�̕`��
void GameScene::SetBgColor(DirectX::SimpleMath::Color color)
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

	// �G�t�F�N�g�̐ݒ�
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(SimpleMath::Matrix::Identity);
	m_basicEffect->SetProjection(m_proj);
	m_basicEffect->Apply(context);

	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	// �w�i���w��F�ŕ`��
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(v[0], v[1], v[2], v[3]);
	m_primitiveBatch->End();
}

// �����̕`��
void GameScene::DrawNumber(int x, int y, int number, int keta, DirectX::SimpleMath::Color color, int dis)
{
	int max = 1;

	// �����������ɓ���Ȃ��ꍇ�\�����C������
	for (int i = 0; i < keta; i++)
	{
		max *= 10;
	}
	if (number >= max) number = max - 1;

	// ���_�������Ƃɕ`�悵�Ă���
	for (int i = 0; i < keta; i++)
	{
		int val = number % 10;
		number /= 10;
		// �e�N�X�`���̐؂�o����`
		RECT srcRect = { 40 * val, 0, 40 * val + 5 * 8, 5 * 8 };
		m_spriteBatch->Draw(
			m_numberTex.Get(),
			SimpleMath::Vector2(static_cast<float>(x - (32 + dis) * i + (keta - 1) * (32 + dis)), static_cast<float>(y)),
			&srcRect,
			color);
	}
}

// �Q�[������~�������̏���
bool GameScene::Pause()
{
	if (m_pauseReason == PauseReason::None) return false;

	// �^�C�}�[���Z
	if (m_pauseTimer > 0) m_pauseTimer--;

	// �|�[�Y���Ă��闝�R�ɂ����
	switch (m_pauseReason)
	{
	case PauseReason::Start:	// �Q�[���X�^�[�g��
		if (m_pauseTimer == 0)
		{
			// �|�[�Y���������ăQ�[���X�^�[�g�I
			m_pauseReason = PauseReason::None;
			// �v���C���[�������o��
			m_player.SetState(Player::PlayerState::Normal);
		}
		break;
	case PauseReason::Eat:		// �����X�^�[��H�ׂ���
		// �����X�^�[���ڂɂȂ�܂ł̓|�[�Y����
		if ( (m_monster.ChangeBody(static_cast<int>(Monster::Type::Type1), m_stage, m_level) == true)
		  && (m_monster.ChangeBody(static_cast<int>(Monster::Type::Type2), m_stage, m_level) == true))
		{
			m_pauseReason = PauseReason::None;
		}
		break;
	case PauseReason::Clear:	// �Q�[���N���A��
		if (m_pauseTimer == 0)
		{
			m_pauseReason = PauseReason::None;
			m_stage++;
			m_map.InitiaizeFood();
			m_scene = Scene::Result;
			InitializeResult();
		}
		break;
	case PauseReason::Dead:		// �v���C���[�����񂾎�
		DeadPlayer();
		break;
	case PauseReason::Gameover:	// �Q�[���I�[�o�[��
		GameOver();
		break;
	}
	return true;
}

// �^�C�g����ʂ̍X�V
void GameScene::UpdateTitle()
{
	auto tracker = GetUserResources()->GetKeyboardStateTracker();

	// Z�L�[�ŃX�^�[�g
	if (tracker->pressed.Z)
	{
		// �Q�[����
		m_scene = Scene::Game;
		// �Q�[���J�n���̏�����
		InitializeGame(true);
	}

	// X�L�[�œ�Փx�ύX 
	if (tracker->pressed.X)
	{
		if (m_level == 0)
		{
			m_level = 1;
			m_rest = LEVEL2_PLAYER_CNT;		// �c�@��
		}
		else
		{
			m_level = 0;
			m_rest = LEVEL1_PLAYER_CNT;		// �c�@��
		}
	}
}

// �^�C�g����ʂ̕`��
void GameScene::DrawTitle()
{
	// �w�i�̕`��
	m_spriteBatch->Draw(m_resultTex.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// �Q�[�����[�h�̕\��
	DrawNumber(18 * 16 + 8, 5 * 16 + 8, m_level + 1, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// �c�@��
	DrawNumber(16 * 16 + 8, 20 * 16 + 8, m_rest, 1,	GetColor(s_color[COLOR_CYAN]), 8);

	// �n�C�X�R�A
	DrawNumber(8 * 16, 15 * 16 + 4, m_highscore[m_level], 5, GetColor(s_color[COLOR_GRAY]), 8);
}

void GameScene::InitializeResult()
{
	// �w�i�F�i���j
	m_bgColor = GetColor(s_color[COLOR_BLACK]);

	// 3�b
	m_timer = 60 * 3;
}

// ���ʉ�ʂ̍X�V
void GameScene::UpdateResult()
{
	// �^�C�}�[���O�ɂȂ�����Q�[����ʂ�
	if (m_timer > 0)
	{
		m_timer--;
	}
	else
	{
		// ���̃X�e�[�W��
		m_scene = Scene::Game;
		// �Q�[���̏�����
		InitializeGame(false);
	}
}

// ���ʉ�ʂ̕`��
void GameScene::DrawResult()
{
	// �w�i�̕`��
	m_spriteBatch->Draw(m_resultTex.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// �Q�[�����[�h�̕\��
	DrawNumber(18 * 16 + 8, 5 * 16 + 8, m_level + 1, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// �c�@��
	DrawNumber(16 * 16 + 8, 20 * 16 + 8, m_rest, 1, GetColor(s_color[COLOR_CYAN]), 8);

	// ���_
	DrawNumber(8 * 16, 15 * 16 + 4, m_score, 5, GetColor(s_color[COLOR_GRAY]), 8);
}

// �Q�[���̏�����
void GameScene::InitializeGame(bool value)
{
	// �w�i�F�i���j
	m_bgColor = GetColor(s_color[COLOR_BLACK]);

	// �|�[�Y
	m_pause = true;

	// �|�[�Y���R
	m_pauseReason = PauseReason::Start;

	// �|�[�Y����
	m_pauseTimer = START_WAIT;

	// �}�b�v�̏�����
	m_map.InitializeMap();

	// �{�[�i�X�����̏�����
	m_map.InitializeBonus();

	// �v���C���[�̏�����
	m_player.Initialize();

	// �����X�^�[�̏�����
	m_monster.Initialize();

	// �Q�[���J�n���̏�����
	if (value)
	{
		m_stage = 0;			// �X�e�[�W������
		m_score = 0;			// �X�R�A������
		m_map.InitiaizeFood();	// ������������
	}
}

// �Q�[���̍X�V
void GameScene::UpdateGame()
{
	// ��ʂ��~�܂������̏���
	if (Pause()) return;

	// �v���C���[�̈ʒu���擾
	int x, y;
	m_player.GetPos(&x, &y);

	// �}�b�v�̍X�V
	m_map.Update();

	// �����X�^�[�̍X�V
	m_monster.Update(m_stage, m_level, x, y);

	// �v���C���[�̍X�V
	bool eatPowerup = false;
	m_player.Update(m_map, &m_score, &eatPowerup);
	// �p���[������H�ׂ��̂Ń����X�^�[����点��
	if (eatPowerup) m_monster.WeakMonster();
	// �p���[�A�b�v���łȂ���ΘA���ŐH�ׂ�����������
	if (!m_player.IsPowerup()) m_monsterEatCnt = 0;

	// �����X�^�[�ƐڐG�����H
	int type;
	if (IsHit(&type) == true)
	{
		// �ʏ�̓v���C���[�͎��S
		if (m_monster.GetState(type) == Monster::State::Normal)
		{
			// �v���C���[�͐H�ׂ�ꂽ
			m_player.Dead();
			m_monster.SetState(type, Monster::State::Eat);
			m_pauseReason = PauseReason::Dead;
			m_pauseTimer = PAUSE_TIME_DEAD;
		}
		else
		{
			// �����X�^�[������Ă���ꍇ�͐H�ׂ�
			if (m_monster.GetState(type) == Monster::State::Weak)
			{
				// �����X�^�[�͐H�ׂ�ꂽ
				m_monster.Dead(type);
				m_monsterEatCnt++;
				m_score += SCORE_MONSTER * m_monsterEatCnt;
				m_pauseReason = PauseReason::Eat;
			}
		}
	}
	// �X�e�[�W�N���A�H
	if (m_map.GetFoodCnt() == 0)
	{
		// ��ʂ��~�߂āA���ʉ�ʂ�
		m_pauseReason = PauseReason::Clear;
		m_pauseTimer = PAUSE_TIME_CLEAR;
		// �v���C���[�̓A�j���[�V�������~������
		m_player.SetState(Player::PlayerState::Stop);
	}
}

// �Q�[���̕`��
void GameScene::DrawGame()
{
	// �}�b�v�̕`��
	DrawMap();

	// �����X�^�[�̕`��
	DrawMonster();

	// �v���C���[�̕`��
	DrawPlayer();

	//auto* debugFont = GetUserResources()->GetDebugFont();
	//std::wostringstream oss;
	//// �f�o�b�O�\��
	//static const char* mode[] =
	//{
	//	"PAUSE_NONE",
	//	"PAUSE_START",
	//	"PAUSE_CLEAR",
	//	"PAUSE_EAT",
	//	"PAUSE_DEAD",
	//	"PAUSE_OVER",
	//};
	//oss << mode[static_cast<int>(m_pauseReason)];
	//debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 2));
	//oss.clear();
	//oss << "SCORE:" << m_score;
	//debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() * 3));
}

// �}�b�v�̕`��
void GameScene::DrawMap()
{
	// �X�e�[�W�̐F
	int stage_color[] = { COLOR_GREEN, COLOR_CYAN, COLOR_BLACK };

	// �����̐F
	int food_color[] =
	{
		COLOR_BLACK, COLOR_GREEN, COLOR_PINK, COLOR_ORANGE, COLOR_GREEN, COLOR_PINK, COLOR_ORANGE
	};

	int id;

	// ��Փx�ɂ���ăX�e�[�W�̐F�̕ς�����ς���
	if (m_level == 0)
	{
		id = m_stage % 2;
	}
	else
	{
		id = m_stage % 3;
	}

	int color = stage_color[id];

	// �w�i�̕`��
	m_spriteBatch->Draw(m_bg01Tex.Get(), SimpleMath::Vector2(0.0f, 0.0f));
	m_spriteBatch->Draw(m_bg02Tex.Get(), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Color(GetColor(s_color[color])));

	// �f�o�b�O�p�i�ʘH�̊G�j
	//m_spriteBatch->Draw(m_bg03Tex.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// �����̕`��
	for (int i = 0; i < FOOD_MAX; i++)
	{
		const Map::FoodInfo* food = m_map.GetFoodInfo(i);

		// �\���ʒu��ݒ�
		float x = static_cast<float>(food->x * CHIP_SIZE + 8);
		float y = static_cast<float>(food->y * CHIP_SIZE);

		// �����̐F��ݒ�
		color = food_color[static_cast<int>(food->state)];

		// �����̋�`
		RECT srcRect = { 0, 0, CHARACTER_SIZE, CHARACTER_SIZE };

		switch (food->state)
		{
			// �ʏ�̂���
		case Map::FoodState::Food_3:
		case Map::FoodState::Food_2:
		case Map::FoodState::Food_1:
			m_spriteBatch->Draw(m_foodTex.Get(), SimpleMath::Vector2(x, y), &srcRect, SimpleMath::Color(GetColor(s_color[color])));
			break;
			// �p���[����
		case Map::FoodState::Power_3:
		case Map::FoodState::Power_2:
		case Map::FoodState::Power_1:
			srcRect.left += CHARACTER_SIZE;
			srcRect.right += CHARACTER_SIZE;
			m_spriteBatch->Draw(m_foodTex.Get(), SimpleMath::Vector2(x, y), &srcRect, SimpleMath::Color(GetColor(s_color[color])));
			break;
		}
	}

	const Map::BonusInfo* bonus = m_map.GetBonusInfo();

	// �{�[�i�X�����̕\��
	if (bonus->state == Map::BonusState::Display)
	{
		// �����̋�`
		RECT srcRect = { CHARACTER_SIZE * 2, 0, CHARACTER_SIZE * 3, CHARACTER_SIZE };
		// �F�̓`�J�`�J������
		static int blinkColor = 0;
		blinkColor = (blinkColor + 1) % (COLOR_MAX - 1);
		m_spriteBatch->Draw(m_foodTex.Get(),
			SimpleMath::Vector2(static_cast<float>(bonus->x + 8), static_cast<float>(bonus->y)),
			&srcRect, SimpleMath::Color(GetColor(s_color[blinkColor + 1])));
	}

	// �{�[�i�X�̓��_�̕\��
	if (bonus->state == Map::BonusState::Score)
	{
		DrawNumber(9 * 16 + 8, 12 * 16 + 8, bonus->score, 3, GetColor(s_color[COLOR_BLACK]), 16);
	}
}

// �����X�^�[�̕`��
void GameScene::DrawMonster()
{
	m_monster.UpdateAnime();

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		SimpleMath::Color color;
		int anime_y;
		if (static_cast<Monster::Type>(i) == Monster::Type::Type1)
		{
			// �ǂ����������X�^�[�̐F
			anime_y = 0;
			color = GetColor(s_color[COLOR_PINK]);
		}
		else
		{
			// �C�܂��ꃂ���X�^�[�̐F
			anime_y = 1;
			color = GetColor(s_color[COLOR_PURPLE]);
		}
		// �����X�^�[������Ă���
		Monster::State state = m_monster.GetState(i);
		if ( state == Monster::State::Weak
		  || state == Monster::State::Dead
		  || state == Monster::State::Return
		  || state == Monster::State::Revive )
		{
			// ���F
			color = GetColor(s_color[COLOR_CYAN]);
			// ����Ԃ��I���Q�b�O����_�ł�����
			int weakTime = m_monster.GetWeakTime(i);
			if (weakTime < 2 * 60 && (weakTime >> 2) % 2)
			{
				// �D�F
				color = GetColor(s_color[COLOR_GRAY]);
			}
		}
		int x, y;
		m_monster.GetPos(i, &x, &y);
		SimpleMath::Vector2 pos(static_cast<float>(x + 8), static_cast<float>(y));
		int id = static_cast<int>(m_monster.GetAnimeNo(i));
		RECT srcRect = { id * CHARACTER_SIZE, anime_y * CHARACTER_SIZE, (id + 1) * CHARACTER_SIZE, (anime_y + 1) * CHARACTER_SIZE };
		m_spriteBatch->Draw(m_monsterTex.Get(), pos, &srcRect, color);
	}
}

// �v���C���[�̕`��
void GameScene::DrawPlayer()
{
	// �A�j���[�V�����̍X�V
	m_player.UpdateAnime();

	// �A�j���[�V����ID���擾
	int id = m_player.GetAnimeId();
	
	// �\���Ȃ�
	if (id == static_cast<int>(Player::PlayerAnime::None)) return;

	// �ʒu���擾
	int x, y;
	m_player.GetPos(&x, &y);
	SimpleMath::Vector2 pos(static_cast<float>(x + 8), static_cast<float>(y));

	// �؂�o����`
	RECT srcRect = { id * CHARACTER_SIZE, 0, (id + 1) * CHARACTER_SIZE, CHARACTER_SIZE };
	// �������Ȃ獶�E���]����
	SpriteEffects flip = SpriteEffects_None;
	if (m_player.IsLeft()) flip = SpriteEffects_FlipHorizontally;
	m_spriteBatch->Draw(m_playerTex.Get(), pos, &srcRect, Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), 1, flip);
}

// �v���C���[�����񂾎��̏���
void GameScene::DeadPlayer()
{
	if (m_pauseTimer == 0)
	{
		m_rest--;
		// �c�@�����Ȃ���΃Q�[���I�[�o�[
		if (m_rest == 0)
		{
			// �Q�[���I�[�o�[��
			m_pauseReason = PauseReason::Gameover;
			m_pauseTimer = PAUSE_TIME_GAMEOVER;
		}
		else
		{
			// ���ʉ�ʂ�
			m_scene = Scene::Result;
			InitializeResult();
		}
	}
}

// �Q�[���I�[�o�[���̏���
void GameScene::GameOver()
{
	if (m_pauseTimer == 0)
	{
		// �^�C�g����
		m_scene = Scene::Title;

		// �n�C�X�R�A�̍X�V
		if (m_highscore[m_level] < m_score) m_highscore[m_level] = m_score;

		// �X�^�[�g���̎c�@���̐ݒ�
		if (m_level == 0)
		{
			m_rest = LEVEL1_PLAYER_CNT;
		}
		else
		{
			m_rest = LEVEL2_PLAYER_CNT;
		}
		// ���ʉ�ʂ̏�����
		InitializeResult();
	}

	// ��ʂ��`�J�`�J������
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

// �v���C���[�ƃ����X�^�[�̏Փ˔��������֐�
bool GameScene::IsHit(int* type)
{
	int px, py;
	int mx, my;

	// �v���C���[�̈ʒu���擾
	m_player.GetPos(&px, &py);

	for (int i = 0; i < MONSTER_CNT; i++)
	{
		// �����X�^�[�̈ʒu���擾
		m_monster.GetPos(i, &mx, &my);
		if ( px + COLLISION_SPACE < mx + CHARACTER_SIZE - COLLISION_SPACE
		  && mx + COLLISION_SPACE < px + CHARACTER_SIZE - COLLISION_SPACE
		  && py + COLLISION_SPACE < my + CHARACTER_SIZE - COLLISION_SPACE
		  && my + COLLISION_SPACE < py + CHARACTER_SIZE - COLLISION_SPACE )
		{
			*type = i;
			return true;
		}
	}

	return false;
}
