#ifndef FILECHUNK_H
#define FILECHUNK_H

#include "Resource.h"
#include "Asset.h"

class FileChunk : public Resource
{
public:
	FileChunk();
	virtual ~FileChunk();

	Asset* GetChunk() { return m_chunk; }
	void SetChunk(Asset* _part) { m_chunk = _part; }

	void Serialize(std::ostream& _stream);
	void Deserialize(std::istream& _stream);
	void ToString();
	void AssignNonDefaultValues();
	void Reset();

	static ObjectPool<FileChunk>* Pool;

private:
	Asset* m_chunk;
};

#endif // !FILECHUNK_H
