#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Hazel {

	Renderer::SceneData* Renderer:: s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)		//设置一整个场景的渲染开始
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	//获取VP矩阵（因为VP矩阵是场景级资源）
	}

	void Renderer::EndScene()		//渲染结束
	{
		//目前为空
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		//强制转换为子类指针
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();					// 顶点数组绑定
		RenderCommand::DrawIndexed(vertexArray);// 调用drawcall
	}
}

