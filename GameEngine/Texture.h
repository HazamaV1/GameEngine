#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"
#include "TGAReader.h"

class Asset;

class Texture : public Resource
{
public:
	Texture();
	virtual ~Texture();

	Asset* GetData() { return m_texture; }
	ImageInfo* GetImageInfo() { return &m_imageInfo; }
	SDL_BlendMode GetBlendMode() { return m_blendMode; }
	void SetBlendMode(SDL_BlendMode _blendMode) { m_blendMode = _blendMode; }
	unsigned char GetBlendAlpha() { return m_blendAlpha; }
	void SetBlendAlpha(unsigned char _blendAlpha) { m_blendAlpha = _blendAlpha; }

	void AssignNonDefaultValues() override;
	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void Load(string _guid);

	static ObjectPool<Texture>* Pool;

private:
	ImageInfo m_imageInfo;
	Asset* m_texture;
	SDL_BlendMode m_blendMode;
	unsigned char m_blendAlpha;
};

#endif // !TEXTURE_H
