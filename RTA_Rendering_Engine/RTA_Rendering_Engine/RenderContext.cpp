#include "RenderContext.h"

// ===== Constructor / Destructor ===== //
RenderContext::RenderContext() : RenderNode()
{
	// === Set Everything to Null
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pBlendState = nullptr;
	m_pRasterizerState = nullptr;
	m_pDepthStencilView = nullptr;
}

RenderContext::~RenderContext()
{
	// === Release all objects
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pDepthStencilView);
}
// ==================================== //

// ===== Interface ===== //
void RenderContext::Add(RenderNode* _rMaterial, RenderNode* _rShape)
{
	RenderNode* materialNode = m_RMaterialSet.Find(_rMaterial);
	if (materialNode == nullptr) {
		// === Material hasn't been added yet
		materialNode = _rMaterial;
		m_RMaterialSet.Add(materialNode);
	}

	materialNode->Add(nullptr, _rShape);
}
// ===================== //

// ===== Private Interface ===== //
void RenderContext::Apply()
{

}

void RenderContext::Revert()
{

}

void RenderContext::DefaultContext_RenderProcess(RenderNode& _node)
{
	Apply();

	RenderNode* currNode = m_RMaterialSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	Revert();
}
// ============================= //
