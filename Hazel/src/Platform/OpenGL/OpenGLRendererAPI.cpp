#include "hzpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) 
	{
		glClearColor(color.r, color.g, color.b, color.a);		//设置清屏并设置屏幕颜色
	}
	void OpenGLRendererAPI::Clear() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//执行清除，清除颜色缓冲和深度缓冲
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		//触发渲染，一次性只能触发一个物体
		//三角形绘制/获取索引数量/索引类型/从数据缓冲区开头读取
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
