#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer:: s_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)		//设置一整个场景的渲染开始
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	//获取VP矩阵
	}

	void Renderer::EndScene()		//渲染结束
	{
		//目前为空
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();					// 顶点数组绑定
		RenderCommand::DrawIndexed(vertexArray);// 调用drawcall
	}
}

