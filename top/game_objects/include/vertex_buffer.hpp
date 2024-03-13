#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP
class VertexBuffer
{
private:
	unsigned int renderer_id_;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
#endif // !VERTEX_BUFFER_HPP
