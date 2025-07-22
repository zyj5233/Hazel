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

        glGenVertexArrays(1, &m_VertexArray);       //创建vao，绘图配方，vertex和vertices都是顶点的意思
        glBindVertexArray(m_VertexArray);           //绑定为当前操作对象

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f, //1.0f, 0.0f, 0.0f,    //左下角（x，y，z），红色
             0.5f, -0.5f, 0.0f, //0.0f, 1.0f, 0.0f,    //右下角，绿色
             0.0f,  0.5f, 0.0f  //0.0f, 0.0f, 1.0f     //顶点，蓝色
        };
        //-reset指针的作用：释放 m_IndexBuffer 之前可能指向的旧索引缓冲对象
        //-接管 IndexBuffer::Create(...) 返回的新索引缓冲对象指针
        // - 确保资源在 m_IndexBuffer 生命周期结束时自动释放
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        //位置属性，0是顶点属性的位置，3表明是三维向量，浮点型和不归一化，在后面是步长，也就是说先取三个数据（三维），然后根据步长，假如步长是6，就跳过三个接着取，nullptr表示没有偏移从第一个取
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);       //启用数组里面的索引0
        //颜色属性，(void*)(3 * sizeof(float))表示偏移三个从第四个取
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);     //启用数组里面的索引1
 
        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
 			#version 330 core
 			
 			layout(location = 0) in vec3 a_Position;    
 			out vec3 v_Position;
 
 			void main()
 			{
                v_Position = a_Position;	
 				gl_Position = vec4(a_Position, 1.0);
 
 			}
 		)";

        std::string fragmentSrc = R"(
 			#version 330 core

 			layout(location = 0) out vec4 color;

 			in vec3 v_Position;

 			void main()
 			{
 				color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
