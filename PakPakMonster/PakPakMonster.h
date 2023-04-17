#pragma once

#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "Common.h"
#include "Map.h"
#include "Player.h"
#include "Monster.h"

// パクパクモンスターのシーンクラス
class PakPakMonster : public Imase::Scene<UserResources>
{
public:

	// コンストラクタ
	PakPakMonster();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 終了処理
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:

	// ベーシックエフェクト（背景色用）
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// 入力レイアウト（背景色用）
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numberTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg01Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg02Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg03Tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_foodTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_monsterTex;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// シーン
	enum class GameScene
	{
		Title,	// タイトル画面
		Result,	// 結果画面
		Game	// ゲーム中
	};

	// 現在のシーン
	GameScene m_scene;
	
	// スコア
	int m_score;
	
	// ハイスコア
	int m_highscore[2];
	
	// 残機数
	int m_rest;

	// ステージ
	int m_stage;

	// レベル（難易度）
	int m_level;

	// タイマー
	int m_timer;

	// ポーズした理由
	enum class PauseReason
	{
		None,		// ポーズしていない
		Start,		// ゲームスタート時
		Clear,		// ゲームクリア時
		Eat,		// モンスターを食べた時
		Dead,		// 死んだ時
		Gameover,	// ゲーム終了時
	};

	// ポーズフラグ
	bool m_pause;

	// ポーズ理由
	PauseReason m_pauseReason;

	// ポーズタイマー
	int m_pauseTimer;

	// 背景色
	DirectX::SimpleMath::Color m_bgColor;

	// マップ
	Map m_map;

	// モンスター
	Monster m_monster;
	
	// プレイヤー
	Player m_player;

	// モンスターを連続で食べた回数
	int m_monsterEatCnt;

private:

	// 背景色の変更
	void SetBgColor(DirectX::SimpleMath::Color color);


	// タイトル画面の更新
	void UpdateTitle();

	// タイトル画面の描画
	void DrawTitle();


	// 結果画面の初期化
	void InitializeResult();

	// 結果画面の更新
	void UpdateResult();

	// 結果画面の描画
	void DrawResult();


	// ゲームの初期化
	void InitializeGame(bool value);

	// ゲームの更新
	void UpdateGame();

	// ゲームの描画
	void DrawGame();


	// マップの描画
	void DrawMap();

	// モンスターの描画
	void DrawMonster();

	// プレイヤーの描画
	void DrawPlayer();

	// 色変換関数
	DirectX::SimpleMath::Color GetColor(const int* rgb);

	// 数字の描画
	void DrawNumber(int x, int y, int number, int keta, DirectX::SimpleMath::Color color, int dis);

	// ゲームが停止した時の処理
	bool Pause();

	// プレイヤーが死んだ時の処理
	void DeadPlayer();

	// ゲームオーバー時の処理
	void GameOver();

	// プレイヤーとモンスターの衝突判定をする関数
	bool HitCheck(int* type);

};

// 色変換関数
inline DirectX::SimpleMath::Color PakPakMonster::GetColor(const int* rgb)
{
	return DirectX::SimpleMath::Color(rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f, 1.0f);
}
