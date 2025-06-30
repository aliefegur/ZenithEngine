#include "zenithpch.h"
#include "Viewport.h"
#include "Zenith/Platform/DirectX/D3D11Graphics.h"

namespace Zenith
{
	Viewport::Viewport()
		:
		m_X(0), m_Y(0), m_W(640), m_H(480)
	{
	}

	Viewport::Viewport(int x, int y, int w, int h)
		:
		m_X(x), m_Y(y), m_W(w), m_H(h)
	{
	}

	void Viewport::Apply(Graphics* gfx) const noexcept
	{
		switch (gfx->GetAPIType())
		{
		case Graphics::API::OpenGL:
			glViewport(m_X, m_Y, m_W, m_H);
			break;
		case Graphics::API::D3D11:
			reinterpret_cast<D3D11Graphics*>(gfx)->ApplyViewport(*this);
			break;
		default:
			// TODO: Throw an exception
			break;
		}
	}

	int Viewport::GetX() const noexcept
	{
		return m_X;
	}
	int Viewport::GetY() const noexcept
	{
		return m_Y;
	}
	int Viewport::GetW() const noexcept
	{
		return m_W;
	}
	int Viewport::GetH() const noexcept
	{
		return m_H;
	}
	float Viewport::GetAspectRatio() const noexcept
	{
		return static_cast<float>(m_W) / static_cast<float>(m_H);
	}

	void Viewport::SetDimensions(int x, int y, int w, int h)
	{
		m_X = x;
		m_Y = y;
		m_W = w;
		m_H = h;
	}
}
