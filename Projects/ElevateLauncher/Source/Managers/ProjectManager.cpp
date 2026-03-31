#include "ProjectManager.h"

namespace EL
{
	std::string ProjectManager::GetLastMessage()
	{
		std::string temp = std::move(m_lastMessage);
		m_lastMessage = "";
		return temp;
	}
}