#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer:: m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)		//设置一整个场景的渲染开始
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()		//渲染结束
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();					// 顶点数组绑定
		RenderCommand::DrawIndexed(vertexArray);// 调用drawcall
	}
}

