#pragma once

#include <string>

#define EE_TEXTURE_DIFFUSE  "material.diffuse"
#define EE_TEXTURE_SPECULAR "material.specular"
#define EE_TEXTURE_NORMAL   "material.normal"
#define EE_TEXTURE_HEIGHT   "material.height"

namespace Elevate
{
	class Texture;
	using TexturePtr = std::shared_ptr<Texture>;

	struct TextureLoadResult
	{
		unsigned char* data;
		int width, height, channelsCount;
		std::string path;
		bool loaded = false;
		uint32_t textureID = 0;
	};

	class Texture
	{
	public:
		Texture() = default;
		Texture(std::string path, std::string type) : m_Path(path), m_TextureID(0), m_Type(type) { }
		~Texture() { };
		virtual void Bind(int index = 0) = 0;
		virtual void Unbind(int index = 0) = 0;
		virtual bool IsBound() const = 0;

		// TODO CHANGE TO SMART POINTERS
		static TexturePtr Create(std::string path);
		static TexturePtr Create(char* data, int width, int height, int channelCount, std::string& path);
		static TexturePtr Create(std::string path, unsigned int index, std::string type);
		static TexturePtr CreateAsync(std::string path);

		inline std::string GetPath() { return m_Path; }
		bool MatchesPath(std::string pathToMatch);

		inline const void SetType(std::string type) { m_Type = type; }
		inline std::string GetType() const { return m_Type; }

		inline uint32_t GetID() const { return m_TextureID; }
	protected:
		std::string m_Path;
		uint32_t m_TextureID; // todo maybe move to the opengl texture class if not needed in other APIs
		std::string m_Type;
	};
}