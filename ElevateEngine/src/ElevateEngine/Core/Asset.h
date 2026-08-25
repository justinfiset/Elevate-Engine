#pragma once

#include <string>

#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/EEObject.h>

namespace Elevate
{
	class Asset : public EEObject
	{
		BEGIN_OBJECT(Asset)
		using Super = Asset;

	public:
		Asset() = default;
		virtual ~Asset() = default;

		bool IsOnDisk() const { return m_isOnDisk; }
		bool IsLoaded() const { return m_isLoaded; }

		bool Load()
		{
			if (m_isLoaded)
			{
				return true;
			}

			if (OnLoad())
			{
				m_isLoaded = true;
				return true;
			}

			return false;
		}

		bool Unbind()
		{
			if (m_isLoaded)
			{
				OnUnbind();
				m_isLoaded = false;
			}
		}

	protected:
		virtual bool OnLoad() { return true; }
		virtual void OnUnbind() { }

		bool m_isOnDisk = false;
		bool m_isLoaded = false;
		std::string m_path;

		END_OBJECT()
	};
}