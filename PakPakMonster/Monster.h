#pragma once

class Monster
{
public:

	enum class Type
	{
		Type1,	// �ǂ����������X�^�[
		Type2,	// �C�܂��ꃂ���X�^�[
	};

	enum class Anime
	{
		Normal_01,	// �ʏ�p�^�[���P
		Normal_02,	// �ʏ�p�^�[���Q
		Return_01,	// ���ɖ߂�p�^�[���P
		Return_02,	// ���ɖ߂�p�^�[���Q
		Return_03,	// ���ɖ߂�p�^�[���R�i�ڂ����j
	};

	enum class State
	{
		Normal,	// �ʏ�
		Weak,	// �����Ă�
		Dead,	// �H�ׂ�ꂽ
		Eat,	// �v���C���[��H�ׂ�
		Return,	// ���ɖ߂��Ă���
		Revive,	// ����
	};

private:

	// �����X�^�[�̈ړ����x�̎��
	enum class Speed
	{
		Lv1,	// LV1�i�x���j
		Lv2,	// LV2   ��
		Lv3,	// LV3   ��
		Lv4,	// LV4   ��
		Lv5,	// LV5�i�����j
		Weak,	// �����X�^�[������Ă��鎞
		Return,	// �����X�^�[�����ɖ߂鎞
	};

	// �����X�^�[�̈ړ����x
	static const int SPEED[];

	// �����X�^�[�̍\����
	struct MonsterInfo
	{
		Type type;		// �����X�^�[�^�C�v
		State state;	// ���
		int weakTimer;	// ���^�C�}�[
		Anime animeNo;	// �A�j���[�V����No
		int animeTimer;	// �A�j���[�V�����p�^�C�}�[
		int x, y;		// �ʒu
		int vx, vy;		// ���x
		int dir;		// �i�s����
	};

	// �����X�^�[���
	MonsterInfo m_monster[2];
	
	// �����X�^�[�̃X�^�[�g�ʒu
	static const int START_POS[2][2];

	// �A�j���[�V�����\�����ԁi�H�ׂ��ĖڂɂȂ�j
	static const int ANIME_INTERVAL_DEAD[];

	// �A�j���[�V�����\�����ԁi��������j
	static const int ANIME_INTERVAL_REVIVE[];

private:

	// �����X�^�[�̏�Ԃƃ��x���ɉ��������x���擾����֐�
	int GetSpeed(int type, int stage, int level);

	// �����X�^�[�̑��x�ݒ�֐�
	void SetVelocity(int type, int stage, int level);
	
	// �t�������擾����֐�
	int GetReverseDir(int dir);

	// �����X�^�[�̕����n�_�ɂ��邩�`�F�b�N����֐�
	bool CheckNestPos(int type);

	// �����X�^�[�̈ړ������̎擾
	int GetDir(int no);

	// �ړ��ł�������̐����擾����֐�
	int GetDirCnt(int moveFlag);

	// �^�[�Q�b�g��ǂ�������
	int ChaseTarget(int moveFlag, int targetX, int targetY);

public:

	// �R���X�g���N�^
	Monster();

	// ������
	void Initialize();

	// �X�V
	void Update(int stage, int level, int playerX, int playerY);

	// �����X�^�[���H�ׂ�ꂽ��A��������Ƃ��̊G�̃p�^�[���`�F���W����֐�
	bool ChangeBody(int type, int stage, int level);

	// �����X�^�[���H�ׂ�ꂽ���Ă΂��֐�
	void Dead(int type);

	// �A�j���[�V�����̍X�V�֐�
	void UpdateAnime();

	// �����X�^�[�̏�Ԃ�ݒ肷��֐�
	void SetState(int type, Monster::State state);

	// �����X�^�[�̏�Ԃ��擾����֐�
	Monster::State GetState(int type);

	// �����X�^�[�̎���Ă��鎞�Ԃ̎c�莞�Ԃ��擾����֐�
	int GetWeakTime(int type);

	// �A�j���[�V�����̔ԍ����擾�֐�
	Monster::Anime GetAnimeNo(int type);
	
	// �ʒu���擾����֐�
	void GetPos(int type, int* x, int* y);

	// �����X�^�[���キ�Ȃ�֐�
	void WeakMonster();

};

inline void Monster::GetPos(int type, int* x, int* y)
{
	// 8�h�b�g�P�ʂ̈ړ��ɂ���
	*x = (m_monster[type].x >> 7) << 3;
	*y = (m_monster[type].y >> 7) << 3;
}
