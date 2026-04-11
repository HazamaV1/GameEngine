#ifndef TEXTURE_H
#define TEXTURE_H
//
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
	int getColorRed() { return m_colorRed; }
	int getColorGreen() { return m_colorGreen; }
	int getColorBlue() { return m_colorBlue; }
	void setColorRed(int colorR) { m_colorRed = colorR; }
	void setColorGreen(int colorG) { m_colorGreen = colorG; }
	void setColorBlue(int colorB) { m_colorBlue = colorB; }

	void AssignNonDefaultValues() override;
	void Serialize(std::ostream& _stream) override;
	void Deserialize(std::istream& _stream) override;
	void ToString() override;
	void Load(string _guid);
	void Reset();

	static ObjectPool<Texture>* Pool;

private:
	ImageInfo m_imageInfo;
	Asset* m_texture;
	SDL_BlendMode m_blendMode;
	unsigned char m_blendAlpha;
	int m_colorRed;
	int m_colorGreen;
	int m_colorBlue;
};

#endif // !TEXTURE_H
