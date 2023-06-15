#include "pch.h"
#include "EditorUI/Inspector.h"
#include "Core/GameObject.h"
#include "Wrapper/Reflection.h"

void EditorUI::Inspector::Draw()
{
	if (!p_open)
		return;
	if (ImGui::Begin("Inspector"))
	{
		if (m_selectedGameObject.size() == 1)
		{
			if (!m_selectedGameObject[0].expired())
				ShowGameObject(m_selectedGameObject[0].lock().get());
		}
	}
	ImGui::End();
}

void EditorUI::Inspector::ShowGameObject(Core::GameObject* object)
{
	if (!object->GetParent().lock())
		return;
	ImGui::PushID((int)object->m_id);

	//TODO: Add tag & layer
	static std::string name;
	name = object->m_name;
	ImGui::Checkbox("##", &object->m_active);
	ImGui::SameLine();
	ImGui::InputText("##InputName", &name);

	ImGui::Separator();

	// Transform
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		object->m_transform->ShowInInspector();
	}

	// Other Components
	for (size_t i = 0; i < object->m_components.size(); i++) {
		ImGui::PushID(i);

		bool enable = object->m_components[i]->IsEnable();
		if (ImGui::Checkbox("##", &enable))
		{
			object->m_components[i]->SetEnable(enable);
		}
		ImGui::SameLine();

		bool destroy = true;
		bool open = ImGui::CollapsingHeader(object->m_components[i]->GetComponentName().c_str(), &destroy, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
		if (open) {
			ImGui::BeginDisabled(!enable);
			ImGui::TreePush(object->m_components[i]->GetComponentName().c_str());
			object->m_components[i]->ShowInInspector();
			ImGui::TreePop();
			ImGui::EndDisabled();
		}
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::PopID();
		if (!destroy) {
			object->m_components[i]->RemoveFromGameObject();
			i--;
		}
	}
	ImGui::NewLine();
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 100);
	if (ImGui::Button("New Component", Math::Vec2f(200, 0)))
	{
		ImGui::OpenPopup("ComponentPopup");
	}
	if (std::shared_ptr<Component::BaseComponent> component = Wrapper::GUI::ComponentPopup())
	{
		object->AddComponent(component);
	}

	ImGui::PopID();
}


void EditorUI::Inspector::AddSelected(std::weak_ptr<Core::GameObject> gameObject)
{
	auto it = std::remove_if(m_selectedGameObject.begin(), m_selectedGameObject.end(),
		[&](const std::weak_ptr<Core::GameObject>& c) {	return c.lock() == gameObject.lock(); });
	if (it != m_selectedGameObject.end()) {
		m_selectedGameObject.erase(it);
		gameObject.lock()->m_selected = false;
	}
	else {
		m_selectedGameObject.push_back(gameObject);
		gameObject.lock()->m_selected = true;
	}
}

void EditorUI::Inspector::SetSelected(std::weak_ptr<Core::GameObject> gameObject)
{
	ClearSelected();
	AddSelected(gameObject);
}

void EditorUI::Inspector::ClearSelected()
{
	for (size_t i = 0; i < m_selectedGameObject.size(); i++)
	{
		if (auto object = m_selectedGameObject[i].lock())
			object->m_selected = false;
	}
	m_selectedGameObject.clear();
}

std::vector<std::weak_ptr<Core::GameObject>> EditorUI::Inspector::GetSelected()
{
	for (size_t i = 0; i < m_selectedGameObject.size(); i++)
	{
		if (m_selectedGameObject[i].expired())
		{
			m_selectedGameObject.erase(m_selectedGameObject.begin() + i);
			--i;
		}
	}
	return m_selectedGameObject;
}
