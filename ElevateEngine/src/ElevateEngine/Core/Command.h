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
                m_redoStack.top()->Execute();
                m_redoStack.pop();
            }
        }
    private:
        std::stack<std::unique_ptr<Command>> m_undoStack;
        std::stack<std::unique_ptr<Command>> m_redoStack;
    };
}