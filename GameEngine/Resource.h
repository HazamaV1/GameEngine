#ifndef RESOURCE_H
#define RESOURCE_H

#include "Serializable.h"
#include "ObjectPool.h"
#include "AssetController.h"

using namespace std;

class Resource : public Serializable
{
public:
	Resource();
	virtual ~Resource();

	virtual void Serialize(ostream& _stream);
	virtual void Deserialize(istream& _stream);
	virtual void AssignNonDefaultValues() = 0;
	virtual void ToString();

	static ObjectPool<Resource>* Pool;

protected:

	template<class T>
	void SerializePointer(std::ostream& _stream, T* _pointer)
	{
		unsigned char exists = 1;
		if (_pointer != nullptr)
		{
			_stream.write(reinterpret_cast<char*>(&exists), sizeof(unsigned char));
			_pointer->Serialize(_stream);
		}
		else
		{
			exists = 0;
			_stream.write(reinterpret_cast<char*>(&exists), sizeof(unsigned char));
		}
	}	
	template<class T>
	void DeserializePointer(std::istream& _stream, T*& _pointer)
	{
		unsigned char exists = 0;
		_stream.read(reinterpret_cast<char*>(&exists), sizeof(exists));
		if (exists == 1)
		{
			_pointer = T::Pool->GetResource();
			_pointer->Deserialize(_stream);
		}
	}

	void SerializeAsset(std::ostream& _stream, Asset* _asset);
	void DeserializeAsset(std::istream& _stream, Asset*& _asset);

private:
};

#endif // RESOURCE_H
