#include "shpch.h"
#include "CameraController.h"

#include "Scotch/Core/Input.h"
#include "Scotch/Core/KeyCodes.h"

namespace Scotch
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep deltaTime)
	{
		if (Input::IsKeyPressed(SH_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		else if (Input::IsKeyPressed(SH_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;

		if (Input::IsKeyPressed(SH_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
		else if (Input::IsKeyPressed(SH_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(SH_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * deltaTime;
			else if (Input::IsKeyPressed(SH_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 2.0f;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SH_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SH_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}