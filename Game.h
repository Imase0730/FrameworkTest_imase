//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "ImaseLib/DebugFont.h"
#include "ImaseLib/SceneManager.h"
#include "UserResources.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

public:

    void SetFullscreenMode(bool value)
    {
        m_deviceResources->SetFullscreenMode(value);
    }

    void QuitFullscreen()
    {
        m_deviceResources->GetSwapChain()->SetFullscreenState(FALSE, nullptr);
    }

private:

    // キーボードステートトラッカー
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
    
    // マウスステートトラッカー
    DirectX::Mouse::ButtonStateTracker m_mouseTracker;

    // デバッグ文字列出力
    std::unique_ptr<Imase::DebugFont> m_debugFont;

    // 共通ステート
    std::unique_ptr<DirectX::CommonStates> m_states;
 
    // シーンに渡すユーザーリソースへのポインタ
    std::unique_ptr<UserResources> m_userResources;

    // シーンマネージャー
    std::unique_ptr<Imase::SceneManager<UserResources>> m_sceneManager;

};
