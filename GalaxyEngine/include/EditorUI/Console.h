#pragma once
#include "GalaxyAPI.h"
#include "EditorUI/EditorWindow.h"
#include <deque>

namespace GALAXY 
{
	namespace Debug
	{
		enum class LogType;
		struct LogText
		{
			LogType type;
			std::string text;
		};
	}
	namespace EditorUI
	{
		class Console : public EditorWindow
		{
		public:
			~Console();

			void Draw() override;

			void DisplayText(size_t i);

			void SetResources();

			void AddText(Debug::LogType type, std::string text);

			void Clear();
		private:

		private:
			friend class MainBar;

			const size_t m_maxText = 200;

			std::deque<Debug::LogText> m_texts;
			size_t m_textSelected = -1;

			Weak<Resource::Texture> m_infoTexture;
			Weak<Resource::Texture> m_warningTexture;
			Weak<Resource::Texture> m_errorTexture;

			size_t m_infoNumber = 0;
			size_t m_warningNumber = 0;
			size_t m_errorNumber = 0;

			bool m_infoCheckbox = true;
			bool m_warningCheckbox = true;
			bool m_errorCheckbox = true;

			bool m_resourcesLoaded = false;

			bool m_scrollToBottom = false;
		};
	}
}
#include "EditorUI/Console.inl" 
