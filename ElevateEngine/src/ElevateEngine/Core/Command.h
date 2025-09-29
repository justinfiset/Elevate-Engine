#pragma once

#include <stack>
#include <memory>

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
        void ExecuteStack();
        void Execute(std::unique_ptr<Command> command);

        void Undo();
        void Redo();
    private:
        std::stack<std::unique_ptr<Command>> m_executeStack;
        std::stack<std::unique_ptr<Command>> m_undoStack;
        std::stack<std::unique_ptr<Command>> m_redoStack;
    };
}