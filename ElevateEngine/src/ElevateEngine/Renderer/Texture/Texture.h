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
	};

	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& path) : m_path(path) { }
		Texture(const std::string& path, const std::string& type) : m_path(path), m_type(type) { }
		~Texture() = default;

		// TODO: IMPL
		//virtual bool IsTextureLoaded() = 0;
		virtual void Bind(int index = 0) = 0;
		virtual void Unbind(int index = 0) = 0;
		virtual bool IsBound() const = 0;

		// TODO CHANGE TO SMART POINTERS
		static TexturePtr Create(const std::string& path);
		static TexturePtr CreateAsync(const std::string& path);

		static TexturePtr Create(unsigned char* data, int width, int height, int channelCount, const std::string& path);
		static TexturePtr Create(const std::string& path, unsigned int index, const std::string type);

		virtual void SetData(unsigned char* data, int width, int height, int channelCount) = 0;

		inline std::string GetPath() { return m_path; }
		bool MatchesPath(std::string pathToMatch);

		inline const void SetType(std::string type) { m_type = type; }
		inline std::string GetType() const { return m_type; }

		inline uint32_t GetID() const { return m_textureID; }
	protected:
		std::string m_path;
		uint32_t m_textureID;
		std::string m_type;
	};
}