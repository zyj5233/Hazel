#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// 创建空顶点着色器对象并赋予句柄
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// 顶点着色器源代码转换成c风格
		// 源码传给OpenGL的顶点着色器对象vertexshader
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// 编译源码
		glCompileShader(vertexShader);

		//检查是否编译
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)		//如果编译失败
		{
			//获取错误信息的长度
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// 创建容器存储错误信息
			std::vector<GLchar> errorInfoLog(maxLength);
			//获取详细的错误信息
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorInfoLog[0]);

			// 删除不再需要的着色器对象
			glDeleteShader(vertexShader);

			//输出错误信息到日志
			HZ_CORE_ERROR("{0}", errorInfoLog.data());
			//触发断言，终止程序并输出错误提示
			HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// 创建片段着色器句柄
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// 获取源码并转为c语言风格
		// 传给OpenGL的片段着色器fragmentShader
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// 编译源码
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			
			std::vector<GLchar> errorInfoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorInfoLog[0]);

			
			glDeleteShader(fragmentShader);
			
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", errorInfoLog.data());
			HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// 创建着色器程序对象并赋予句柄
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// 将顶点着色器和片段着色器附加到程序上
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// 会将各个着色器的代码组合成一个单一的、可在 GPU 上执行的程序
		glLinkProgram(program);

		//是否链接成功
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}
		//删除原始代码
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	//绑定完整着色器
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	//把单个整数作为uniform变量上传到gpu
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());	//获取uniform位置
		glUniform1i(location, value);	//上传数据
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));//GL_FALSE表示不转置/将glm矩阵转化为float指针
	}

}