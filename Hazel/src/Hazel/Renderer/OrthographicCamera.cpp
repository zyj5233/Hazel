#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

// 初始化用glm计算正交投影矩阵
namespace Hazel{

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        //投影矩阵初始化与视图矩阵初始化
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    // 投影观察矩阵计算
    void OrthographicCamera::RecalculateViewMatrix()
    {
        // 观察矩阵
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}