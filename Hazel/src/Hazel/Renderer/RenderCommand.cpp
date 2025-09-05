#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {

	// 默认先指向OpenGLRendererAPI子类
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI; 
}
