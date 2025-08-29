#pragma once

#include <memory>
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;		//vao需要绑定才能用
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;		//添加vbo，因为一个vao可以关联多个vbo
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;		//设置ebo

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;		//获取所有关联的vbo
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;		//获取关联的ebo

		static VertexArray* Create();		//静态工厂方法
	};

}
