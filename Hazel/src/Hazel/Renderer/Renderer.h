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

		//提交物体数据
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));// 提交物体的顶点数组
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;	//VP矩阵
		};

		static SceneData* s_SceneData;		//场景数据就是vp矩阵


	};


}