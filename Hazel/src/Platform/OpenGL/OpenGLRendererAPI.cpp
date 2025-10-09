#include "hzpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace Hazel {

	void OpenGLRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);	//启用深度测试（防遮挡）
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)	//设置视口
	{
		glViewport(x, y, width, height);
	}


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

		glBindTexture(GL_TEXTURE_2D, 0);	//0表示解除纹理绑定。glBindTexture(GL_TEXTURE_2D, textureID)表示使用textureID
	}
}
