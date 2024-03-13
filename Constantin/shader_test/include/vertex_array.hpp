#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "vertex_buffer.hpp"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int renderer_id_;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

#endif // !VERTEX_ARRAY_HPP
