#include "pch.h"
#include "Component/ScriptComponent.h"

#include "Scripting/ScriptEngine.h"
#include "Scripting/ScriptInstance.h"

#include <any>
namespace GALAXY
{

	std::string Component::ScriptComponent::GetComponentName() const
	{
		if (m_component)
			return m_component->GetComponentName();
		return "Script Component";
	}

	std::shared_ptr<Component::BaseComponent> Component::ScriptComponent::Clone()
	{
		auto copy = std::make_shared<ScriptComponent>(*dynamic_cast<ScriptComponent*>(this));
		copy->m_component = m_component->Clone();
		return copy;
	}

	void Component::ScriptComponent::OnCreate()
	{
		m_component->OnCreate();
	}

	void Component::ScriptComponent::OnStart()
	{
		m_component->OnStart();
	}

	void Component::ScriptComponent::OnUpdate()
	{
		m_component->OnUpdate();
	}

	void Component::ScriptComponent::OnDraw()
	{
		m_component->OnDraw();
	}

	void Component::ScriptComponent::OnDestroy()
	{
		m_component->OnDestroy();
	}

	void Component::ScriptComponent::ShowInInspector()
	{
		auto variables = GetAllVariables();
		for (auto& variable : variables)
		{
			switch (variable.second)
			{
			case Scripting::VariableType::Unknown:
			{
				ImGui::Text("Unknown type for %s", variable.first.c_str());
			}
			break;
			case Scripting::VariableType::Bool:
			{
				if (bool* value = GetVariable<bool>(variable.first))
					ImGui::Checkbox(variable.first.c_str(), value);
			}
			break;
			case Scripting::VariableType::Int:
			{
				if (int* value = GetVariable<int>(variable.first))
					ImGui::InputInt(variable.first.c_str(), value);
			}
			break;
			case Scripting::VariableType::Float:
			{
				if (float* value = GetVariable<float>(variable.first))
					ImGui::DragFloat(variable.first.c_str(), value);
			}
			break;
			case Scripting::VariableType::Double:
			{
				if (double* value = GetVariable<double>(variable.first))
					ImGui::InputDouble(variable.first.c_str(), value);
			}
			break;
			case Scripting::VariableType::Vector2:
			{
				if (Math::Vec2f* value = GetVariable<Math::Vec2f>(variable.first))
					ImGui::DragFloat2(variable.first.c_str(), &value->x);
			}
			break;
			case Scripting::VariableType::Vector3:
			{
				if (Math::Vec3f* value = GetVariable<Math::Vec3f>(variable.first))
					ImGui::DragFloat3(variable.first.c_str(), &value->x);
			}
			break;
			case Scripting::VariableType::Vector4:
			{
				if (Math::Vec4f* value = GetVariable<Math::Vec4f>(variable.first))
					ImGui::DragFloat4(variable.first.c_str(), &value->x);
			}
			break;
			default:
				break;
			}
		}
	}

	std::any Component::ScriptComponent::GetVariable(const std::string& variableName)
	{
		Shared<Scripting::ScriptInstance> scriptInstance = Scripting::ScriptEngine::GetInstance()->GetScriptInstance(variableName).lock();
		if (scriptInstance)
		{
			Scripting::VariableType variableType = scriptInstance->m_variables.at(variableName);
			switch (variableType)
			{
			case Scripting::VariableType::Unknown:
				break;
			case Scripting::VariableType::Bool:
				return *GetVariable<bool>(variableName);
			case Scripting::VariableType::Int:
				return *GetVariable<int>(variableName);
			case Scripting::VariableType::Float:
				return *GetVariable<float>(variableName);
			case Scripting::VariableType::Double:
				return *GetVariable<double>(variableName);
			case Scripting::VariableType::Vector2:
				return *GetVariable<Vec2f>(variableName);
			case Scripting::VariableType::Vector3:
				return *GetVariable<Vec3f>(variableName);
			case Scripting::VariableType::Vector4:
				return *GetVariable<Vec4f>(variableName);
				break;
			default:
				break;
			}
		}
		return nullptr;
	}

	void Component::ScriptComponent::SetVariable(const std::string& variableName, std::any value)
	{
		Shared<Scripting::ScriptInstance> scriptInstance = Scripting::ScriptEngine::GetInstance()->GetScriptInstance(variableName).lock();
		if (scriptInstance)
		{
			Scripting::VariableType variableType = scriptInstance->m_variables.at(variableName);
			switch (variableType)
			{
			case Scripting::VariableType::Unknown:
				break;
			case Scripting::VariableType::Bool:
				SetVariable(variableName, std::any_cast<bool>(value));
				break;
			case Scripting::VariableType::Int:
				SetVariable(variableName, std::any_cast<int>(value));
				break;
			case Scripting::VariableType::Float:
				SetVariable(variableName, std::any_cast<float>(value));
				break;
			case Scripting::VariableType::Double:
				SetVariable(variableName, std::any_cast<double>(value));
				break;
			case Scripting::VariableType::Vector2:
				SetVariable(variableName, std::any_cast<Vec2f>(value));
				break;
			case Scripting::VariableType::Vector3:
				SetVariable(variableName, std::any_cast<Vec3f>(value));
				break;
			case Scripting::VariableType::Vector4:
				SetVariable(variableName, std::any_cast<Vec4f>(value));
				break;
			default:
				break;
			}
		}
	}

	std::unordered_map<std::string, Scripting::VariableType> Component::ScriptComponent::GetAllVariables() const
	{
		Scripting::ScriptEngine* scriptEngine = Scripting::ScriptEngine::GetInstance();
		auto scriptInstance = scriptEngine->GetScriptInstance(m_scriptName).lock();
		if (scriptInstance)
			return scriptInstance->GetAllVariables();
		return {};
	}

	void Component::ScriptComponent::BeforeReloadScript()
	{
		auto beforeVariables = GetAllVariables();
		for (auto& variable : beforeVariables)
		{
			m_tempVariables[variable.first] = GetVariable(variable.first);
		}
	}

	void Component::ScriptComponent::AfterReloadScript()
	{
		m_component = Scripting::ScriptEngine::GetInstance()->CreateScript(m_scriptName);

		auto afterVariables = GetAllVariables();
		int i = 0;
		for (auto& variable : afterVariables)
		{
			if (auto anyValue = m_tempVariables.find(variable.first); anyValue != m_tempVariables.end())
			{
				SetVariable(variable.first, anyValue);
			}
		}
	}

	void* Component::ScriptComponent::GetVariableVoid(const std::string& variableName)
	{
		return Scripting::ScriptEngine::GetInstance()->GetVariableOfScript(m_component.get(), m_component.get()->GetComponentName(), variableName);
	}

	void Component::ScriptComponent::SetVariableVoid(const std::string& variableName, void* value)
	{
		Scripting::ScriptEngine::GetInstance()->SetVariableOfScript(m_component.get(), m_component.get()->GetComponentName(), variableName, value);
	}

}
