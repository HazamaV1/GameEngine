#include "Unit.h"

ObjectPool<Unit>* Unit::Pool = nullptr;

Unit::Unit()
{
	m_soundEffect = nullptr;
	m_chunk = nullptr;
}

Unit::~Unit()
{
}

void Unit::AssignNonDefaultValues()
{
	m_soundEffect = SoundEffect::Pool->GetResource();
	m_soundEffect->AssignNonDefaultValues();
	Resource::AssignNonDefaultValues();
}

void Unit::Serialize(std::ostream& _stream)
{
	SerializePointer(_stream, m_chunk);
	Resource::Serialize(_stream);
}

void Unit::Deserialize(std::istream& _stream)
{
	DeserializePointer(_stream, m_chunk);
	Resource::Deserialize(_stream);
}

void Unit::ToString()
{
	cout << "UNIT" << endl;
	m_chunk->ToString();
	Resource::ToString();
}