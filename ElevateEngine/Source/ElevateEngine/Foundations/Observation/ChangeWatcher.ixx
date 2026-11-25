module;

#include <optional>
#include <utility>

export module Elevate.Foundations.Observation.ChangeWatcher;

export namespace Elevate
{
    template <typename T>
    class ChangeWatcher
    {
    public:
        ChangeWatcher() = default;
        explicit ChangeWatcher(const T &value) : m_value(value) {}

        bool HasChanged(const T &newValue)
        {
            if (m_value && *m_value == newValue)
            {
                return false;
            }

            m_value = newValue;
            return true;
        }

        template <typename Func>
        bool HasChanged(const T &newValue, Func &&onChange)
        {
            if (m_value && *m_value == newValue)
            {
                return false;
            }

            T oldValue = m_value.value_or(newValue);
            m_value = newValue;

            onChange(oldValue, newValue);
            return true;
        }

    private:
        std::optional<T> m_value;
    };
}
