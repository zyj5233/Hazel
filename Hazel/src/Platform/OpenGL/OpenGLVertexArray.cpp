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
		HZ_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);		//创建一个或者多个vao，这里创建一个并存储到m_RendererID
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);		//删除
	}

	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);		//绑定这个vao
	}

	void OpenGLVertexArray::Unbind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	//智能指针类型，指向vertexbuffer。这个函数接收vbo的数据传给vao，实现vao与vbo的绑定
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();		//获取layout
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);		//启用index通道
			//下面的index对应location（n）
			glVertexAttribPointer(index,		//vao，顶点数组对象，也是映射规则，告诉gpu如何解析vbo中的顶点数据
				element.GetComponentCount(),		//向量维数
				ShaderDataTypeToOpenGLBaseType(element.Type),	//传为OpenGL类型
				element.Normalized ? GL_TRUE : GL_FALSE,		//是否归一化
				layout.GetStride(),		//获取步长
				(const void*)element.Offset);		//获取偏移量
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	//实现vao与ibo的绑定
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
