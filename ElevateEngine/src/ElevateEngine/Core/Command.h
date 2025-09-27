#pragma once

#include <stack>
#include <memory>
#include <vector>

namespace Elevate
{
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
        virtual bool IsUndoable() const { return true; }
    };

    /// @brief Class to combine multiples commands to execute then or undo them in batches.
    class MacroCommand : public Command
    {
    public:
        MacroCommand(std::vector<std::unique_ptr<Command>>&& commands) : m_commands(std::move(commands)) { }

        inline virtual void Execute() override { 
            for (auto it = m_commands.begin(); it != m_commands.end(); ++it) {
                (*it)->Execute();
            }
        }
        inline virtual void Undo() override {
            for (auto it = m_commands.rbegin(); it != m_commands.rend(); ++it) {
                (*it)->Undo();
            }
        }
    private:
        std::vector<std::unique_ptr<Command>> m_commands;
    };

    class CommandManager
    {
    public:
        inline void PushCommand(std::unique_ptr<Command> command)
        {
            m_executeStack.push(std::move(command));
        }
    protected:
        inline void ExecuteStack()
        {
            while (!m_executeStack.empty())
            {
                EE_CORE_TRACE("ExecuteStack while loop iteration");
                auto command = std::move(m_executeStack.top());
                m_executeStack.pop();
                Execute(std::move(command));
            }
        }

        inline void Execute(std::unique_ptr<Command> command)
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

        inline void Undo() 
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

        inline void Redo()
        {
            if (!m_redoStack.empty())
            {
                auto cmd = std::move(m_redoStack.top());
                m_redoStack.pop();
                PushCommand(std::move(cmd));
            }
        }
    private:
        std::stack<std::unique_ptr<Command>> m_executeStack;
        std::stack<std::unique_ptr<Command>> m_undoStack;
        std::stack<std::unique_ptr<Command>> m_redoStack;
    };
}