#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

    LayerStack::LayerStack()
    {
        m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)       //遍历层栈
            delete layer;
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);     //向书签位置插入普通层并且更新书签位置
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);     //覆盖层添加到末尾
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsert--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);     //寻找某个层
        if (it != m_Layers.end())       
            m_Layers.erase(it);             //找到后直接移除
    }

}