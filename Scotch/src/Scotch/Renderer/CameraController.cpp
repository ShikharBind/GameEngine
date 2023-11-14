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
		SH_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		else if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;

		if (Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
		else if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * deltaTime;
			else if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * deltaTime;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 2.0f;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		SH_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SH_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SH_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = (float)width / (float)height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		SH_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		SH_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}