#pragma once

namespace Hazel {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;	//切换状态启用缓存区域
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);	//静态工厂指针，传递顶点数据和顶点总字节数
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;		//获取定点数量和索引数量

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

}