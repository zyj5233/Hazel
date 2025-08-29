#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"
#include "Hazel/Log.h"
#include "Input.h"
#include "glad/glad.h"
namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;     //一次只能有一个实例

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

        m_VertexArray.reset(VertexArray::Create());

        float vertices[3 * 7] = {
            //这是客户端数据，也叫数据源
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;     
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));       //让vertexBuffer指针管理vbo
        
        //花括号是代码块限定layout的作用域
        {
            //说明书：告诉顶点由三个位置和四个颜色组成（必须要显示规定才符合规则）
            BufferLayout layout = {
                //这是一个列表
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };

            vertexBuffer->SetLayout(layout);        //把布局信息存储到vertexBuffer对象中
            m_VertexArray->AddVertexBuffer(vertexBuffer);   //vertexBuffer指向OpenGLVertexBuffer对象，所以可以调用m_RendererID
        }

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);
        m_SquareVA.reset(VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        //通过 layout(location) 指定索引0和1
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
        std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

        std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

        m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

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

            m_BlueShader->Bind();
            m_SquareVA->Bind();
            glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            //激活着色器
            m_Shader->Bind();

            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();      //是层级的onupdata，处理onupdata

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
