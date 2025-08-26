#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"
#include "Hazel/Log.h"
#include "Input.h"
#include "glad/glad.h"
namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;     //一次只能有一个实例

    //GLenum 是 OpenGL 定义的 枚举类型，这个函数把ShaderDataType转换成GLenum
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Hazel::ShaderDataType::Float:    return GL_FLOAT;
        case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
        case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
        case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
        case Hazel::ShaderDataType::Int:      return GL_INT;
        case Hazel::ShaderDataType::Int2:     return GL_INT;
        case Hazel::ShaderDataType::Int3:     return GL_INT;
        case Hazel::ShaderDataType::Int4:     return GL_INT;
        case Hazel::ShaderDataType::Bool:     return GL_BOOL;
        }

        //如果没有switch匹配就触发断言保护
        HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    Application::Application()
    {     
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;      //this指向application实例

        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);       //创建vao，绘图配方，vertex和vertices都是顶点的意思
        glBindVertexArray(m_VertexArray);           //绑定为当前操作对象

        float vertices[3 * 7] = {
            //前面三个是float3，xyz坐标
            //后面四个是float4.rgba值
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        //-reset指针的作用：释放 m_VertexBuffer 之前可能指向的旧顶点缓冲对象
        //-接管 IndexBuffer::Create(...) 返回的新索引缓冲对象指针
        // - 确保资源在 m_VertexBuffer 生命周期结束时自动释放
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        
        //花括号是代码块限定layout的作用域
        {
            //说明书：告诉顶点由三个位置和四个颜色组成（必须要显示规定才符合规则）
            BufferLayout layout = {
                //这是一个列表
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;     //位置索引
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);       //OpenGL 默认会禁用所有顶点属性索引，这行代码显式启用索引为index的顶点属性
           // 建立cpu与gpu之间的映射关系。
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,        //是否归一化数据
                layout.GetStride(),
                (const void*)element.Offset);       //属性在顶点数据中的偏移量
            index++;
        }

        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
 			#version 330 core
 			
 			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
    
 			out vec3 v_Position;
            out vec4 v_Color; 
 			void main()
 			{
                v_Position = a_Position;
                v_Color = a_Color;	
 				gl_Position = vec4(a_Position, 1.0);
 
 			}
 		)";

        std::string fragmentSrc = R"(
 			#version 330 core

 			layout(location = 0) out vec4 color;

 			in vec3 v_Position;
            in vec4 v_Color;
 			void main()
 			{
 				color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
 			}
 		)";

        //创建一个shader对象
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )        //逆向遍历处理事件
        {
            (*--it)->OnEvent(e);        //这个调用的是当前层的onevent
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);       //窗口颜色
            glClear(GL_COLOR_BUFFER_BIT);

            //激活着色器
            m_Shader->Bind();

            //方便多个vao切换
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();      //是层级的onupdata，处理onupdata

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();       //处理onevent
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
