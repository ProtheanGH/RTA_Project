#pragma once
#include "RenderNode.h"
#include "RenderSet.h"
#include "RasterizerStateManager.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderContext : public RenderNode
{
private: 
	// === Members
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11BlendState*			m_pBlendState;
	RasterizerStates			m_RasterizerStateType;
	RenderSet					m_RMaterialSet;

	// === Private Interface === //
	void Apply();
	void Revert();
	void DefaultContext_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderContext();
	virtual ~RenderContext();
	// ================================ //

	// === Interface === //
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	// ================= //

	// === Mutators === //
	inline void SetSetting(ID3D11VertexShader* _vertexShader = nullptr, ID3D11PixelShader* _pixelShader = nullptr, ID3D11BlendState* _blendState = nullptr, ID3D11RasterizerState* _rasterizerState = nullptr, ID3D11DepthStencilState* _depthStencilState = nullptr) {
		m_pVertexShader = _vertexShader;
		m_pPixelShader = _pixelShader;
		m_pBlendState = _blendState;
		m_pRasterizerState = _rasterizerState;
		m_pDepthStencilState = _depthStencilState;
	}
	inline void SetVertexShader(ID3D11VertexShader* _vertexShader) {
		m_pVertexShader = _vertexShader;
	}
	inline void SetPixelShader(ID3D11PixelShader* _pixelShader) {
		m_pPixelShader = _pixelShader;
	}
	inline void SetBlendState(ID3D11BlendState* _blendState) {
		m_pBlendState = _blendState;
	}
	inline void SetRasterizerState(ID3D11RasterizerState* _rasterizerState) {
		m_pRasterizerState = _rasterizerState;
	}
	inline void SetDepthStencilView(ID3D11DepthStencilState* _depthStencilState) {
		m_pDepthStencilState = _depthStencilState;
	}
	// ================ //
};

