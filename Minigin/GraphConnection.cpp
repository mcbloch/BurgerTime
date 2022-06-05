#include "MiniginPCH.h"

#include "GraphConnection.h"

using namespace dae;

GraphConnection::GraphConnection(const int   from /* = invalid_node_index*/ ,
                                 const int   to /* = invalid_node_index*/,
                                 const float cost /* = 1.f*/)
	: m_From(from)
	  , m_To(to)
	  , m_Cost(cost)
{
}

bool GraphConnection::operator==(const GraphConnection& rhs) const
{
	return rhs.m_From == this->m_From &&
		rhs.m_To == this->m_To &&
		rhs.m_Cost == this->m_Cost;
}

bool GraphConnection::operator!=(const GraphConnection& rhs) const
{
	return !(*this == rhs);
}
