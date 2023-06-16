#pragma once
#include "GalaxyAPI.h"
#include <vector>
#include <memory>

namespace GALAXY::Component
{
	class BaseComponent;
	class ComponentHolder
	{
	public:
		~ComponentHolder() {}

		static void Initialize();

		template<typename T>
		static inline void RegisterComponent();

		static std::vector<std::shared_ptr<BaseComponent>>& GetList() { return m_componentList; }

	private:
		static std::vector<std::shared_ptr<BaseComponent>> m_componentList;
	};


}
#include "Component/ComponentHolder.inl" 
