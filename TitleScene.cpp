#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;

void TitleScene::Initialize()
{
}

void TitleScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void TitleScene::Render()
{
	auto* debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"TitleScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));
}

void TitleScene::Finalize()
{
}

void TitleScene::CreateDeviceDependentResources()
{
}

void TitleScene::CreateWindowSizeDependentResources()
{
}

void TitleScene::OnDeviceLost()
{
}
