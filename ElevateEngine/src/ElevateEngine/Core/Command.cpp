#include "eepch.h"
#include "Command.h"

#include <ElevateEngine/Core/Log.h>

void Elevate::CommandManager::ExecuteStack()
{
    while (!m_executeStack.empty())
    {
        EE_CORE_TRACE("ExecuteStack while loop iteration");
        auto command = std::move(m_executeStack.top());
        m_executeStack.pop();
        Execute(std::move(command));
    }
}

void Elevate::CommandManager::Execute(std::unique_ptr<Command> command)
{
    EE_CORE_TRACE("Execute");
    command->Execute();

    if (command->IsUndoable())
    {
        m_undoStack.push(std::move(command));

        // Clear the redo stack with a swap trick (more safe with unique ptrs)
        std::stack<std::unique_ptr<Command>> empty;
        m_redoStack.swap(empty);
    }
}

void Elevate::CommandManager::Undo()
{
    if (!m_undoStack.empty())
    {
        while (!m_undoStack.empty() && !m_undoStack.top()->IsUndoable())
        {
            m_undoStack.pop();
        }

        if (!m_undoStack.empty())
        {
            auto cmd = std::move(m_undoStack.top());
            m_undoStack.pop();
            cmd->Undo();
            m_redoStack.push(std::move(cmd));
        }
    }
}

void Elevate::CommandManager::Redo()
{
    if (!m_redoStack.empty())
    {
        auto cmd = std::move(m_redoStack.top());
        m_redoStack.pop();
        PushCommand(std::move(cmd));
    }
}
