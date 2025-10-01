#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Hazel {

    class Shader
    {
    public:
        virtual ~Shader() = default;       //声明在子类实现

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;


		virtual void SetInt(const std::string& name, int value) = 0;	//虚函数才能让父类指针调用子类方法
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);		//智能指针工厂函数
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);	//添加一个命名着色器到管理器
		void Add(const Ref<Shader>& shader);	//添加着色器
		Ref<Shader> Load(const std::string& filepath);	//从文件加载着色器并添加到管理器
		Ref<Shader> Load(const std::string& name, const std::string& filepath);	//从文件添加着色器并指定自定义名称

		Ref<Shader> Get(const std::string& name);	//获取指定名称的着色器

		bool Exists(const std::string& name) const;		//检查着色器是否已经存在
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;	//使用指定名称为键，着色器智能指针为值
    };

}