#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI 
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;// 设置清除后的颜色	
		virtual void Clear() override;								// 清除哪些缓冲
		// 通过顶点数组绘制
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}

