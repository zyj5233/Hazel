#include "hzpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);  //创建一个对象并生成唯一标识符存储在m_RendererID
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);	//m_RendererID 标识的顶点缓冲对象(VBO)绑定到 OpenGL 的 GL_ARRAY_BUFFER 目标
		//放在构造函数上传数据的目的是：数据上传一次就够了
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);  //将CPU数据上传到GPU，vertices ：CPU 端数据指针，GL_STATIC_DRAW ：提示 GPU 此数据不会频繁修改，优化存储
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}	  

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	//bind用于切换渲染对象
	void OpenGLIndexBuffer::Bind() const		//不改成员变量，即不会改变m_RendererID
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}