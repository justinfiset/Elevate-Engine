#pragma once

#include <ElevateEngine/Editor/Commands/Command.h>

import Elevate.Core.Scene:GameObject;

namespace Elevate
{
	class AddGameobjectCommand : public Command
	{
	private:
		std::shared_ptr<GameObject> m_object;
		virtual void Execute() override { /* TODO IMPLEMENT */ }
		virtual void Undo() override { /* TODO IMPLEMENT */ }
	};

	class DeleteGameobjectCommand : public Command
	{
	private:
		std::weak_ptr<GameObject> m_object;
	public:
		DeleteGameobjectCommand(std::weak_ptr<GameObject> object) : m_object(object) {}
		virtual void Execute() override { m_object.lock()->Destroy(); }
		virtual void Undo() override { /* TODO IMPLEMENT */ }
	};
}