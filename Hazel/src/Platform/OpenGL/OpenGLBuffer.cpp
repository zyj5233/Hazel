#include "hzpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {


	//作用就是上传vbo，绑定id
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererID);  //创建vbo并获取id
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);	//绑定vbo，	GL_ARRAY_BUFFER表示顶点属性数据（位置、颜色等）
		//放在构造函数上传数据的目的是：数据上传一次就够了
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);  //上传数据到vbo，vertices ：CPU 端数据指针（地址），GL_STATIC_DRAW ：提示 GPU 此数据不会频繁修改，优化存储
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()	//删除vbo
	{
		HZ_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}	  

	void OpenGLVertexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)		
		: m_Count(count)		//索引数量
	{
		HZ_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererID);		//虽然代码相同，但其实这里是创建ibo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//GL_ELEMENT_ARRAY_BUFFER存储顶点索引数据
		//下面不仅指明了数据的总字节和指针，还说明了数据类型uint32_t
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
	}

	//bind用于切换渲染对象
	void OpenGLIndexBuffer::Bind() const		//不改成员变量，即不会改变m_RendererID
	{
		HZ_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}