﻿#ifndef SABA_MODEL_MMD_PMXMODEL_H_
#define SABA_MODEL_MMD_PMXMODEL_H_

#include "MMDMaterial.h"
#include "MMDModel.h"
#include "MMDIkSolver.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <algorithm>

namespace saba
{
	class PMXNode : public MMDNode
	{
	public:
		PMXNode();

		void SetDeformDepth(int32_t depth) { m_deformDepth = depth; }
		int32_t GetDeformdepth() const { return m_deformDepth; }

		void SetAppendNode(PMXNode* node) { m_appendNode = node; }
		PMXNode* GetAppendNode() const { return m_appendNode; }

		void EnableAppendRotate(bool enable) { m_isAppendRotate = enable; }
		void EnableAppendTranslate(bool enable) { m_isAppendTranslate = enable; }
		void EnableAppendLocal(bool enable) { m_isAppendLocal = enable; }
		void SetAppendWeight(float weight) { m_appendWeight = weight; }
		float GetAppendWeight() const { return m_appendWeight; }

		const glm::vec3& GetAppendTranslate() const { return m_appendTranslate; }
		const glm::quat& GetAppendRotate() const { return m_appendRotate; }

		void SetIKSolver(MMDIkSolver* ik) { m_ikSolver = ik; }
		MMDIkSolver* GetIKSolver() const { return m_ikSolver; }

		void UpdateAppendTransform();


	protected:
		void OnBeginUpdateTransform() override;
		void OnEndUpdateTransfrom() override;
		void OnUpdateLocalTransform() override;

	private:
		int32_t		m_deformDepth;

		PMXNode*	m_appendNode;
		bool		m_isAppendRotate;
		bool		m_isAppendTranslate;
		bool		m_isAppendLocal;
		float		m_appendWeight;

		glm::vec3	m_appendTranslate;
		glm::quat	m_appendRotate;

		MMDIkSolver*	m_ikSolver;

	};

	class PMXModel : public MMDModel
	{
	public:
		MMDNodeManager* GetNodeManager() override { return &m_nodeMan; }
		MMDIKManager* GetIKManager() override { return &m_ikSolverMan; }
		MMDBlendShapeManager* GetBlendShapeManager() override { return &m_blendShapeMan; }
		MMDPhysicsManager* GetPhysicsManager() override { return &m_physicsMan; }

		size_t GetVertexCount() const override { return m_positions.size(); }
		const glm::vec3* GetPositions() const override { return &m_positions[0]; }
		const glm::vec3* GetNormals() const override { return &m_normals[0]; }
		const glm::vec2* GetUVs() const override { return &m_uvs[0]; }
		const glm::vec3* GetUpdatePositions() const override { return &m_updatePositions[0]; }
		const glm::vec3* GetUpdateNormals() const override { return &m_updateNormals[0]; }
		const glm::vec2* GetUpdateUVs() const override { return &m_uvs[0]; }

		size_t GetIndexElementSize() const override { return m_indexElementSize; }
		size_t GetIndexCount() const override { return m_indexCount; }
		const void* GetIndices() const override { return &m_indices[0]; }

		size_t GetMaterialCount() const override { return m_materials.size(); }
		const MMDMaterial* GetMaterials() const override { return &m_materials[0]; }

		size_t GetSubMeshCount() const override { return m_subMeshes.size(); }
		const MMDSubMesh* GetSubMeshes() const override { return &m_subMeshes[0]; }

		MMDPhysics* GetMMDPhysics() override { return m_physicsMan.GetMMDPhysics(); }

		void InitializeAnimation() override;
		// アニメーションの前後で呼ぶ (VMDアニメーションの前後)
		void BeginAnimation() override;
		void EndAnimation() override;
		// ノードを更新する
		void UpdateAnimation(float elapsed) override;
		// 頂点データーを更新する
		void Update(float elapsed) override;

		bool Load(const std::string& filepath, const std::string& mmdDataDir);
		void Destroy();

		const glm::vec3& GetBBoxMin() const { return m_bboxMin; }
		const glm::vec3& GetBBoxMax() const { return m_bboxMax; }

	private:
		enum class SkinningType
		{
			Weight1,
			Weight2,
			Weight4,
		};
		struct VertexBoneInfo
		{
			SkinningType	m_skinningType;
			glm::ivec4		m_boneIndex;
			glm::vec4		m_boneWeight;
		};

	private:
		std::vector<glm::vec3>	m_positions;
		std::vector<glm::vec3>	m_normals;
		std::vector<glm::vec2>	m_uvs;
		std::vector<VertexBoneInfo>	m_vertexBoneInfos;
		std::vector<glm::vec3>	m_updatePositions;
		std::vector<glm::vec3>	m_updateNormals;

		std::vector<char>	m_indices;
		size_t				m_indexCount;
		size_t				m_indexElementSize;

		MMDBlendShape				m_baseShape;

		glm::vec3		m_bboxMin;
		glm::vec3		m_bboxMax;

		std::vector<MMDMaterial>	m_materials;
		std::vector<MMDSubMesh>		m_subMeshes;
		std::vector<PMXNode*>		m_sortedNodes;

		MMDNodeManagerT<PMXNode>	m_nodeMan;
		MMDIKManagerT<MMDIkSolver>	m_ikSolverMan;
		MMDBlendShapeManagerT<MMDBlendShape>	m_blendShapeMan;
		MMDPhysicsManager			m_physicsMan;
	};
}

#endif // !SABA_MODEL_MMD_PMXMODEL_H_
