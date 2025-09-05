#include "hzpch.h"      //需要与premake里面的pchheader "hzpch.h"一致

#include "Application.h"

#include "Hazel/Log.h"

#include "Hazel/Renderer/Renderer.h"
#include "Input.h"


namespace Hazel { 

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application* Application::s_Instance = nullptr;     //一次只能有一个实例

    Application::Application()
        :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {     
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;      //this指向application实例

        //Application创建窗口，控制ui等整体业务逻辑
        m_Window = std::unique_ptr<Window>(Window::Create());
        //保存窗口事件回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());     //创建三角形vao

        float vertices[3 * 7] = {
            //这是客户端数据，也叫数据源
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;     
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));       //让vertexBuffer指针管理vbo
        
        //花括号是代码块限定layout的作用域，目的只是给m_layout传值，结束后自动销毁
        {
            //说明书：告诉顶点由三个位置和四个颜色组成（必须要显示规定才符合规则）
            BufferLayout layout = {
                //这是一个列表
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            //vertexBuffer如果不放进来，layout直接销毁了，这个指针无法传递对象
            vertexBuffer->SetLayout(layout);        //把布局信息存储到m_layout对象中
            m_VertexArray->AddVertexBuffer(vertexBuffer);   //绑定vao和vbo
        }

        uint32_t indices[3] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        //创建正方形vao
        m_SquareVA.reset(VertexArray::Create());

        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        //临时对象在完整表达式结束时销毁
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };       //三个为一组绘制三角形，所以矩形需要六个索引（两个三角形构成一个矩形）
        std::shared_ptr<IndexBuffer> squareIB;

        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        //通过 layout(location) 指定索引0和1
        std::string vertexSrc = R"(
 			#version 330 core
 			
 			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

 			out vec3 v_Position;
            out vec4 v_Color; 
 			void main()
 			{
                v_Position = a_Position;
                v_Color = a_Color;	
 				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
 
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

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
 			#version 330 core
 			
 			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

 			out vec3 v_Position;
 
 			void main()
 			{
                v_Position = a_Position;
 				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
 
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
            RenderCommand::SetClearColor({ 0.1f, 0.5f, 0.1f, 1 });  //设置帧缓冲区的默认清除颜色
            RenderCommand::Clear();     //清除帧缓冲区的指定缓存

            m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
            m_Camera.SetRotation(45.0f);

            Renderer::BeginScene(m_Camera);
            Renderer::Submit(m_BlueShader, m_SquareVA);
            Renderer::Submit(m_Shader, m_VertexArray);

            Renderer::EndScene();
            
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
