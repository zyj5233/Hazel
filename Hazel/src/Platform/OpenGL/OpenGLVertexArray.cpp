#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {
	//将引擎自定义的 ShaderDataType 枚举值转换为 OpenGL 底层 API 所需的类型枚举
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Float:    return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
		case Hazel::ShaderDataType::Int:      return GL_INT;
		case Hazel::ShaderDataType::Int2:     return GL_INT;
		case Hazel::ShaderDataType::Int3:     return GL_INT;
		case Hazel::ShaderDataType::Int4:     return GL_INT;
		case Hazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);		//创建一个或者多个vao，这里创建一个并存储到m_RendererID
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);		//删除
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);		//绑定这个vao
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	//智能指针类型，指向vertexbuffer。这个函数接收vbo的数据传给vao，实现vao与vbo的绑定
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)		
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,		//vao，顶点数组对象，也是映射规则，告诉gpu如何解析vbo中的顶点数据
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
