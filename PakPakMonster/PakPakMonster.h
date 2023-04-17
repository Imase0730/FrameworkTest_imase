#pragma once

#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "Common.h"
#include "Map.h"
#include "Player.h"
#include "Monster.h"

// �p�N�p�N�����X�^�[�̃V�[���N���X
class PakPakMonster : public Imase::Scene<UserResources>
{
public:

	// �R���X�g���N�^
	PakPakMonster();

	// ������
	void Initialize() override;

	// �X�V
	void Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �I������
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;

private:

	// �x�[�V�b�N�G�t�F�N�g�i�w�i�F�p�j
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// ���̓��C�A�E�g�i�w�i�F�p�j
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numberTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg01Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg02Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg03Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_foodTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_monsterTex;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	// �V�[��
	enum class GameScene
	{
		Title,	// �^�C�g�����
		Result,	// ���ʉ��
		Game	// �Q�[����
	};

	// ���݂̃V�[��
	GameScene m_scene;
	
	// �X�R�A
	int m_score;
	
	// �n�C�X�R�A
	int m_highscore[2];
	
	// �c�@��
	int m_rest;

	// �X�e�[�W
	int m_stage;

	// ���x���i��Փx�j
	int m_level;

	// �^�C�}�[
	int m_timer;

	// �|�[�Y�������R
	enum class PauseReason
	{
		None,		// �|�[�Y���Ă��Ȃ�
		Start,		// �Q�[���X�^�[�g��
		Clear,		// �Q�[���N���A��
		Eat,		// �����X�^�[��H�ׂ���
		Dead,		// ���񂾎�
		Gameover,	// �Q�[���I����
	};

	// �|�[�Y�t���O
	bool m_pause;

	// �|�[�Y���R
	PauseReason m_pauseReason;

	// �|�[�Y�^�C�}�[
	int m_pauseTimer;

	// �w�i�F
	DirectX::SimpleMath::Color m_bgColor;

	// �}�b�v
	Map m_map;

	// �����X�^�[
	Monster m_monster;
	
	// �v���C���[
	Player m_player;

	// �����X�^�[��A���ŐH�ׂ���
	int m_monsterEatCnt;

private:

	// �w�i�F�̕ύX
	void SetBgColor(DirectX::SimpleMath::Color color);


	// �^�C�g����ʂ̍X�V
	void UpdateTitle();

	// �^�C�g����ʂ̕`��
	void DrawTitle();


	// ���ʉ�ʂ̏�����
	void InitializeResult();

	// ���ʉ�ʂ̍X�V
	void UpdateResult();

	// ���ʉ�ʂ̕`��
	void DrawResult();


	// �Q�[���̏�����
	void InitializeGame(bool value);

	// �Q�[���̍X�V
	void UpdateGame();

	// �Q�[���̕`��
	void DrawGame();


	// �}�b�v�̕`��
	void DrawMap();

	// �����X�^�[�̕`��
	void DrawMonster();

	// �v���C���[�̕`��
	void DrawPlayer();

	// �F�ϊ��֐�
	DirectX::SimpleMath::Color GetColor(const int* rgb);

	// �����̕`��
	void DrawNumber(int x, int y, int number, int keta, DirectX::SimpleMath::Color color, int dis);

	// �Q�[������~�������̏���
	bool Pause();

	// �v���C���[�����񂾎��̏���
	void DeadPlayer();

	// �Q�[���I�[�o�[���̏���
	void GameOver();

	// �v���C���[�ƃ����X�^�[�̏Փ˔��������֐�
	bool HitCheck(int* type);

};

// �F�ϊ��֐�
inline DirectX::SimpleMath::Color PakPakMonster::GetColor(const int* rgb)
{
	return DirectX::SimpleMath::Color(rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f, 1.0f);
}
