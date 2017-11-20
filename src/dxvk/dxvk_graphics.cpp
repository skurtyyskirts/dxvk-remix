#include "dxvk_graphics.h"

namespace dxvk {
  
  template<typename T>
  size_t hashPtr(T* ptr) {
    return reinterpret_cast<size_t>(ptr);
  }
  
  size_t DxvkGraphicsPipelineStateInfo::hash() const {
    DxvkHashState state;
    state.add(hashPtr(this->inputAssembly.ptr()));
    state.add(hashPtr(this->inputLayout.ptr()));
    state.add(hashPtr(this->rasterizerState.ptr()));
    state.add(hashPtr(this->multisampleState.ptr()));
    state.add(hashPtr(this->depthStencilState.ptr()));
    state.add(hashPtr(this->blendState.ptr()));
    state.add(std::hash<VkRenderPass>()(this->renderPass));
    state.add(viewportCount);
    return state;
  }
  
  
  bool DxvkGraphicsPipelineStateInfo::operator == (const DxvkGraphicsPipelineStateInfo& other) const {
    return this->inputAssembly      == other.inputAssembly
        && this->inputLayout        == other.inputLayout
        && this->rasterizerState    == other.rasterizerState
        && this->multisampleState   == other.multisampleState
        && this->depthStencilState  == other.depthStencilState
        && this->blendState         == other.blendState
        && this->renderPass         == other.renderPass
        && this->viewportCount      == other.viewportCount;
  }
  
  
  bool DxvkGraphicsPipelineStateInfo::operator != (const DxvkGraphicsPipelineStateInfo& other) const {
    return !this->operator == (other);
  }
  
  
  DxvkGraphicsPipeline::DxvkGraphicsPipeline(
      const Rc<vk::DeviceFn>& vkd,
      const Rc<DxvkShader>&   vs,
      const Rc<DxvkShader>&   tcs,
      const Rc<DxvkShader>&   tes,
      const Rc<DxvkShader>&   gs,
      const Rc<DxvkShader>&   fs)
  : m_vkd(vkd), m_vs(vs), m_tcs(tcs),
    m_tes(tes), m_gs(gs), m_fs(fs) {
    
  }
  
  
  DxvkGraphicsPipeline::~DxvkGraphicsPipeline() {
    
  }
  
  
  VkPipeline DxvkGraphicsPipeline::getPipelineHandle(
    const DxvkGraphicsPipelineStateInfo& state) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto pair = m_pipelines.find(state);
    if (pair != m_pipelines.end())
      return pair->second;
    
    VkPipeline pipeline = this->compilePipeline(state);
    m_pipelines.insert(std::make_pair(state, pipeline));
    return pipeline;
  }
  
  
  VkPipeline DxvkGraphicsPipeline::compilePipeline(
    const DxvkGraphicsPipelineStateInfo& state) const {
    
  }
  
}