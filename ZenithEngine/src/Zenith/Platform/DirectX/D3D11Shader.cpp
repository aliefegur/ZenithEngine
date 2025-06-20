#include "zenithpch.h"
#include "D3D11Shader.h"
#include "Zenith/Utils/ZenithUtils.h"

namespace Zenith
{
    D3D11Shader::D3D11Shader(Graphics* gfx_, const std::string& vsFile, const std::string& psFile)
        :
        Shader(gfx_, vsFile, psFile)
    {
        D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(gfx_);
        INFO_MANAGER(gfx);

        // Convert from string to wide string
        std::wstring vs = Utils::ToWide(vsFile);
        std::wstring ps = Utils::ToWide(psFile);

        // Read shader files
        GFX_THROW(D3DReadFileToBlob(vs.c_str(), &pVsbc));
        GFX_THROW(D3DReadFileToBlob(ps.c_str(), &pPsbc));

        // Create shader objects
        GFX_THROW(GetDevice(gfx)->CreateVertexShader(pVsbc->GetBufferPointer(), pVsbc->GetBufferSize(), nullptr, &pVertexShader));
        GFX_THROW(GetDevice(gfx)->CreatePixelShader(pPsbc->GetBufferPointer(), pPsbc->GetBufferSize(), nullptr, &pPixelShader));
    }

    D3D11Shader::~D3D11Shader()
    {
        pVsbc->Release();
        pPsbc->Release();
        pVertexShader->Release();
        pPixelShader->Release();
    }

    void D3D11Shader::Bind(Graphics* gfx_) noexcept
    {
        D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(gfx_);
        GetContext(gfx)->VSSetShader(pVertexShader, nullptr, NULL);
        GetContext(gfx)->PSSetShader(pPixelShader, nullptr, NULL);
    }

    ID3DBlob* D3D11Shader::GetVSBytecode() const noexcept
    {
        return pVsbc;
    }

    ID3DBlob* D3D11Shader::GetPSBytecode() const noexcept
    {
        return pPsbc;
    }
}
