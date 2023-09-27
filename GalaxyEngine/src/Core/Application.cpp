
#include "pch.h"

#include "Core/Application.h"
#include "Core/ThreadManager.h"
#include "Core/SceneHolder.h"
#include "Wrapper/GUI.h"
#include "Wrapper/Window.h"
#include "Wrapper/Renderer.h"
#include "Wrapper/ImageLoader.h"
#include "Resource/ResourceManager.h"
#include "Resource/Texture.h"
#include "Resource/Shader.h"
#include "EditorUI/EditorUIManager.h"
#include "Component/ComponentHolder.h"
#include "Scripting/ScriptEngine.h"

#include "Resource/IResource.h"

#include "Utils/FileInfo.h"

#pragma region static
Core::Application Core::Application::m_instance;
#pragma endregion

Core::Application::~Application() {}

void Core::Application::Initalize(const std::filesystem::path& projectPath)
{
	// Initalize Window Lib
	if (!Wrapper::Window::Initialize())
		PrintError("Failed to initalize window API");

	// Create Window
	m_window = std::make_unique<Wrapper::Window>();
	Wrapper::WindowConfig windowConfig;
	windowConfig.width = 1600;
	windowConfig.height = 900;
	windowConfig.name = "Galaxy Engine";
	m_window->Create(windowConfig);
	m_window->SetVSync(false);

	// Initalize GUI Lib
	Wrapper::GUI::Initalize(m_window, "#version 330");

	// Initalize Render API
	Wrapper::Renderer::CreateInstance(Wrapper::RenderAPI::OPENGL);
	m_renderer = Wrapper::Renderer::GetInstance();

	// Initalize Thread Manager
	m_threadManager = Core::ThreadManager::GetInstance();
	m_threadManager->Initalize();

	// Initalize Resource Manager
	m_resourceManager = Resource::ResourceManager::GetInstance();
	m_resourceManager->m_projectPath = projectPath.parent_path();
	m_resourceManager->m_assetPath = projectPath.parent_path() / ASSET_FOLDER_NAME;
	std::string filename = projectPath.filename().string();
	m_resourceManager->m_projectName = filename = filename.substr(0, filename.find_first_of('.'));
	m_resourceManager->ImportAllFilesInFolder(m_resourceManager->m_assetPath);
	m_resourceManager->ImportAllFilesInFolder(ENGINE_RESOURCE_FOLDER_NAME);
	
	// Initialize Scene
	m_sceneHolder = Core::SceneHolder::GetInstance();

	// Initalize EditorUI
	m_editorUI = EditorUI::EditorUIManager::GetInstance();
	m_editorUI->Initalize();

	m_scriptEngine = Scripting::ScriptEngine::GetInstance();
	m_scriptEngine->LoadDLL(projectPath.parent_path() / "Generate", filename);

	// Initalize Components
	Component::ComponentHolder::Initialize();
}

void Core::Application::UpdateResources()
{
	m_scriptEngine->UpdateFileWatcherDLL();

	if (!m_resourceToSend.empty())
	{
		auto resourcePath = m_resourceToSend.front();
		std::weak_ptr<Resource::IResource> resource = m_resourceManager->GetResource<Resource::IResource>(resourcePath);
		if (resource.expired())
		{
			PrintError("Failed to send resource %s", resourcePath.c_str());
			m_resourceToSend.pop_front();
		}
		else
		{
			if (!resource.lock()->HasBeenSent())
				resource.lock()->Send();
			m_resourceToSend.pop_front();
			if (!resource.lock()->HasBeenSent())
				m_resourceToSend.push_back(resourcePath);
		}
	}
}

void Core::Application::Update()
{
	while (!m_window->ShouldClose())
	{
		Wrapper::Window::PollEvent();
		Wrapper::GUI::NewFrame();

		if (ImGui::IsKeyPressed(ImGuiKey_F11))
		{
			m_window->ToggleFullscreen();
		}

		UpdateResources();

		//BEGINDRAW
		m_sceneHolder->Update();
		//ENDDRAW

		// Rendering
		Wrapper::GUI::EndFrame(m_window);

		m_window->SwapBuffers();
	}
}

void Core::Application::Destroy()
{
	// Cleanup:
	// Resource Manager
	m_resourceManager->Release();

	// Thread Manager
	m_threadManager->Destroy();

	// GUI
	Wrapper::GUI::UnInitalize();

	// Window
	m_window->Destroy();
	Wrapper::Window::UnInitialize();

	PrintLog("Application clean-up completed.");
}

void GALAXY::Core::Application::AddResourceToSend(const std::filesystem::path& fullPath)
{
	if (!std::count(m_resourceToSend.begin(), m_resourceToSend.end(), fullPath))
		m_resourceToSend.push_back(fullPath);
}
