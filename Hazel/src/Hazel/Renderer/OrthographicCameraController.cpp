#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
namespace Hazel{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();
		//实际移动的是相机m_CameraTranslationSpeed
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();
		else if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();

		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();
		else if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();

		if(m_Rotation)		//带旋转条件，可设置禁用/启用
		{ 
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);

		//基于缩放级别更新相机移动速度
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();
		//键盘控制相机移动是单独的onupdata，鼠标缩放和窗口大小是额外的onevent
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.25f;	//改变缩放大小
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);		//保证最小缩放等级
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}