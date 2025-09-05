#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"
//执行提交命令和非渲染命令
namespace Hazel {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);	
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);// 提交物体的顶点数组
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;


	};


}