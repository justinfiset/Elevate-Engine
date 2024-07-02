#pragma once

#include <string>

namespace Elevate
{
	class Texture
	{
	public:
		Texture(std::string path, std::string type) : m_Path(path), m_TextureID(0), m_Type(type) { }
		~Texture() { };
		virtual void Bind(int index = 0) const = 0;
		virtual void Unbind(int index = 0) const = 0;

		static Texture* Create(std::string path);
		static Texture* Create(std::string path, unsigned int index, std::string type);

		inline std::string GetPath() { return m_Path; }

		inline const void SetType(std::string type) { m_Type = type; }
		inline std::string GetType() const { return m_Type; }

		inline uint32_t GetID() const { return m_TextureID; }
	protected:
		std::string m_Path;
		uint32_t m_TextureID; // todo maybe move to the opengl texture class if not needed in other APIs
		std::string m_Type;
	};
}