#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Hazel {

	class RendererAPI {

	public:
		enum class API {
			None = 0, OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;	//设置视口
		virtual void SetClearColor(const glm::vec4& color) = 0;	// 设置清除后的颜色
		virtual void Clear() = 0;								// 清除哪些缓冲

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		inline static API GetAPI() { return s_API; }		//对于小型简单的类型，不占内存，值返回更高效

	private:
		static API s_API;
	};
}
