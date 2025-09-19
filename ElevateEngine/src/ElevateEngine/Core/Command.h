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
        inline void Execute(std::unique_ptr<Command> command)
        {
            command->Execute();

            if (command->IsUndoable())
            {
                m_undoStack.push(std::move(command));

                // Clear the redo stack
                while (!m_redoStack.empty()) m_redoStack.pop();
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
                    m_undoStack.top()->Undo();
                    m_redoStack.push(std::move(m_undoStack.top()));
                    m_undoStack.pop();
                }
            }
        }

        inline void Redo()
        {
            if (!m_redoStack.empty())
            {
                auto cmd = std::move(m_redoStack.top());
                m_redoStack.pop();
                cmd->Execute();
                if (cmd->IsUndoable()) {
                    m_undoStack.push(std::move(cmd));
                }
            }
        }
    private:
        std::stack<std::unique_ptr<Command>> m_undoStack;
        std::stack<std::unique_ptr<Command>> m_redoStack;
    };
}