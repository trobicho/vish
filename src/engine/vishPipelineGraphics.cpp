#include "vish.hpp"
#include "vishHelper.hpp"

//--------------Basic Fixed Function---------------

static VkPipelineVertexInputStateCreateInfo     vertexInputStateInfo_noInput() {
  VkPipelineVertexInputStateCreateInfo	vertexInput = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .vertexBindingDescriptionCount = 0,
    .pVertexBindingDescriptions = nullptr, // Optional
    .vertexAttributeDescriptionCount = 0,
    .pVertexAttributeDescriptions = nullptr, // Optional
  };
  return (vertexInput);
}

static VkPipelineInputAssemblyStateCreateInfo   inputAssemblyStateInfo() {
  VkPipelineInputAssemblyStateCreateInfo  inputAssembly = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = VK_FALSE,
  };
  return (inputAssembly);
}

static VkPipelineRasterizationStateCreateInfo   rasterizationStateInfo() {
  VkPipelineRasterizationStateCreateInfo rasterizer = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_BACK_BIT,
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0.0f, // Optional
    .depthBiasClamp = 0.0f, // Optional
    .depthBiasSlopeFactor = 0.0f, // Optional
    .lineWidth = 1.0f,
  };
  return (rasterizer);
}

static VkPipelineViewportStateCreateInfo        viewportStateInfo() {
  VkPipelineViewportStateCreateInfo   viewportInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .viewportCount = 1,
    .scissorCount = 1,
  };
  return (viewportInfo);
}

static VkPipelineMultisampleStateCreateInfo     multisampleStateInfo() {
  VkPipelineMultisampleStateCreateInfo multisampling = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 1.0f, // Optional
    .pSampleMask = nullptr, // Optional
    .alphaToCoverageEnable = VK_FALSE, // Optional
    .alphaToOneEnable = VK_FALSE, // Optional
  };
  return (multisampling);
}

static VkPipelineColorBlendStateCreateInfo  colorBlendStateInfo_noBlending(
    VkPipelineColorBlendAttachmentState &colorBlendAttachment) {

  colorBlendAttachment = (VkPipelineColorBlendAttachmentState) {
    .blendEnable = VK_FALSE,
    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, //Optional
    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
    .colorBlendOp = VK_BLEND_OP_ADD, // Optional
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, //Optional
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, //Optional
    .alphaBlendOp = VK_BLEND_OP_ADD, // Optional
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
      | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };

  VkPipelineColorBlendStateCreateInfo colorBlending = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY, // Optional
    .attachmentCount = 1,
    .pAttachments = &colorBlendAttachment,
  };
  colorBlending.blendConstants[0] = 0.0f; // Optional
  colorBlending.blendConstants[1] = 0.0f; // Optional
  colorBlending.blendConstants[2] = 0.0f; // Optional
  colorBlending.blendConstants[3] = 0.0f; // Optional

  return (colorBlending);
}

//-----------------------------------------

void  Vish::createGraphicsPipeline(PipelineWrapper &pipelineWrap) {

  VkShaderModule	vertShaderModule =
    VishHelper::createShaderModuleFromFile(m_device, "./shaders/vert.spv");
  VkShaderModule	fragShaderModule =
    VishHelper::createShaderModuleFromFile(m_device, "./shaders/frag.spv");

  VkPipelineVertexInputStateCreateInfo    vertexInputInfo = vertexInputStateInfo_noInput();
  VkPipelineInputAssemblyStateCreateInfo  inputAssemblyInfo = inputAssemblyStateInfo();
  VkPipelineRasterizationStateCreateInfo  rasterizationInfo = rasterizationStateInfo();
  VkPipelineMultisampleStateCreateInfo    multisampleInfo = multisampleStateInfo();
  VkPipelineColorBlendAttachmentState     colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo     colorBlendInfo = colorBlendStateInfo_noBlending(colorBlendAttachment); // To avoid static attachment
  VkPipelineViewportStateCreateInfo       viewportInfo = viewportStateInfo();

  VkDynamicState dynamicStates[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
  };

  VkPipelineDynamicStateCreateInfo  dynamicStateInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = 2,
    .pDynamicStates = dynamicStates,
  };

  VkPipelineShaderStageCreateInfo   vertShaderStageInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = vertShaderModule,
    .pName = "main", //TODO: entrypoint
    .pSpecializationInfo = nullptr,
  };
  VkPipelineShaderStageCreateInfo   fragShaderStageInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
    .module = fragShaderModule,
    .pName = "main", //TODO: entrypoint
    .pSpecializationInfo = nullptr,
  };
  VkPipelineShaderStageCreateInfo   shaderStages[] = {
    vertShaderStageInfo,
    fragShaderStageInfo,
  };

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = 0, // Optional
    .pSetLayouts = nullptr, // Optional
    .pushConstantRangeCount = 0, // Optional
    .pPushConstantRanges = nullptr, // Optional
  };

  if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo
        , nullptr, &pipelineWrap.layout) != VK_SUCCESS)
    throw VishHelper::FatalError("failed to create pipeline layout!");

  VkGraphicsPipelineCreateInfo  pipelineInfo = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .stageCount = 2,
    .pStages = shaderStages,
    .pVertexInputState = &vertexInputInfo,
    .pInputAssemblyState = &inputAssemblyInfo,
    .pViewportState = &viewportInfo,
    .pRasterizationState = &rasterizationInfo,
    .pMultisampleState = &multisampleInfo,
    .pDepthStencilState = nullptr, // Optional
    .pColorBlendState = &colorBlendInfo,
    .pDynamicState = &dynamicStateInfo,
    .layout = pipelineWrap.layout,
    .renderPass = m_renderPass,
  };

  if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1,
        &pipelineInfo, nullptr, &pipelineWrap.pipeline) != VK_SUCCESS)
    throw VishHelper::FatalError("failed to create graphics pipeline!");

  vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
  vkDestroyShaderModule(m_device, fragShaderModule, nullptr);
}

void  Vish::destroyPipeline(PipelineWrapper &pipelineWrap) {
  vkDestroyPipeline(m_device, pipelineWrap.pipeline, nullptr);
  vkDestroyPipelineLayout(m_device, pipelineWrap.layout, nullptr);
}
