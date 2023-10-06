#pragma once
#include "GalaxyAPI.h"
#include "IComponent.h"
#include "Scripting/ScriptEngine.h"

#include <unordered_map>
#include <any>

namespace GALAXY
{
	namespace Scripting {}
	namespace Component
	{
		class GALAXY_API ScriptComponent : public IComponent<ScriptComponent>
		{
		public:
			ScriptComponent() {}
			ScriptComponent& operator=(const ScriptComponent& other) = default;
			ScriptComponent(const ScriptComponent&) = default;
			ScriptComponent(ScriptComponent&&) noexcept = default;
			virtual ~ScriptComponent() {}

			virtual const char* GetComponentName() const override
			{
				return "Script Component";
			}

			virtual std::vector<const char*> GetComponentNames() const override;

			virtual void ShowInInspector() override;

			template<typename T>
			T* GetVariable(const std::string& variableName)
			{
				auto value = reinterpret_cast<T*>(GetVariableVoid(variableName));
				return value;
			}

			std::any GetVariable(const std::string& variableName);
			void SetVariable(const std::string& variableName, std::any value);

			template<typename T>
			void SetVariable(const std::string& variableName, T value)
			{
				T* newValue = &value;
				SetVariableVoid(variableName, newValue);
			}

			std::unordered_map<std::string, Scripting::VariableData> GetAllVariables() const;

		private:
			template<typename T> inline void DisplayAndManageVariable(const std::pair<std::string, Scripting::VariableData>& variable);

			template<typename T> void DisplayVariableT(const std::pair<std::string, Scripting::VariableData>& variable, T* value);

			void DisplayVariableField(const std::pair<std::string, Scripting::VariableData>& variable);

			void* GetVariableVoid(const std::string& variableName);

			void SetVariableVoid(const std::string& variableName, void* value);
		};

		struct ComponentInfo
		{
			uint64_t gameObjectID = -1;
			uint32_t componentID = -1;
			std::string componentName = "";
			std::optional<size_t> indexOnList;
			std::string variableName = "";
		};

		class ReloadScript
		{
		public:
			ReloadScript(Shared<ScriptComponent> component) : m_component(component) {}

			void BeforeReloadScript();
			void AfterReloadScript();
			void SyncComponentsValues();
			Shared<GALAXY::Component::ScriptComponent> GetComponent() const { return m_component; }

			std::string GetScriptName() const { return m_scriptName; }
		private:
			std::any ConvertComponentToInfo(const std::any& value);

			std::any ConvertGameObjectToID(const std::any& value);

			bool ConvertInfoToComponent(std::any& value);

			std::any ConvertIDToGameObject(const std::any& value);
		private:
			Shared<ScriptComponent> m_component;

			std::string m_scriptName;

			std::unordered_map<std::string, ComponentInfo> m_missingComponentRefs;

			std::unordered_map<std::string, std::pair<std::any, Scripting::VariableData>> m_tempVariables;
			
		};
	}
}
#include "Component/ScriptComponent.inl" 
