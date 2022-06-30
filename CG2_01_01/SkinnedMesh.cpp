#include "SkinnedMesh.h"
#include <sstream>
#include <functional>

SkinnedMesh::SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate)
{
	// �}�l�[�W���[�̐���
	FbxManager *fbxManager = FbxManager::Create();
	// �V�[������
	FbxScene *fbxScene = FbxScene::Create(fbxManager, "");

	FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "");
	// �t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
	if (!fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}

	//�C���|�[�g
	if (!fbxImporter->Import(fbxScene)) {
		assert(0);
	}

	//�O�p�`��
	FbxGeometryConverter fbx_converter(fbxManager);
	if (trianglate) {
		fbx_converter.Triangulate(fbxScene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbxScene);
	}

	//�Z���`���[�g���ɕϊ�
	FbxSystemUnit::m.ConvertScene(fbxScene);

	//�m�[�h�̎擾
	std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
		Scene::Node &node = sceneView.nodes.emplace_back();
		//type��null��������eUnknown���Z�b�g
		node.atribute = fbxNode->GetNodeAttribute() ?
			fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
		node.name = fbxNode->GetName();
		node.uniqueID = fbxNode->GetUniqueID();
		//�e�����Ȃ����0����
		node.pearentIndex = sceneView.indexof(fbxNode->GetParent() ?
			fbxNode->GetParent()->GetUniqueID() : 0);
		//�q�ɑ΂��čċN����
		for (int childIndex = 0; childIndex < fbxNode->GetChildCount(); ++childIndex) {
			traverse(fbxNode->GetChild(childIndex));
		}
	} };

	traverse(fbxScene->GetRootNode());

#if 0
	for (const Scene::Node &node : sceneView.nodes) {
		FbxNode *fbxNode = fbxScene->FindNodeByName(node.name.c_str());
		std::string nodeName = fbxNode->GetName();
		uint64_t uid = fbxNode->GetUniqueID();
		uint64_t parentUid =
			fbxNode->GetParent() ? fbxNode->GetParent()->GetUniqueID() : 0;
		int32_t type =
			fbxNode->GetNodeAttribute() ? fbxNode->GetNodeAttribute()->GetAttributeType() : 0;

		std::stringstream debugString;
		debugString << nodeName << ":" << uid << ":" << parentUid << ":" << type << "\n";
		OutputDebugStringA(debugString.str().c_str());
	}
#endif

	fbxManager->Destroy();

}

void SkinnedMesh::FetchMeshes(FbxScene *fbxScene, std::vector<Mesh> &meshes)
{
	// ���ׂẴm�[�h���`�F�b�N
	for (const Scene::Node &node : sceneView.nodes) {
		// ���b�V���łȂ���΃X�L�b�v
		if (node.atribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}
		// �m�[�h�ƃ��b�V���𔲂��o��
		FbxNode *fbxNode = fbxScene->FindNodeByName(node.name.c_str());
		FbxMesh *fbxMesh = fbxNode->GetMesh();
		
		// ���b�V���\���̂ɓ��Ă͂߂�
		Mesh &mesh = meshes.emplace_back();
		mesh.uniqueID = fbxMesh->GetNode()->GetUniqueID();
		mesh.name = fbxMesh->GetNode()->GetName();
		mesh.nodeIndex = sceneView.indexof(mesh.uniqueID);

		// �|���S���̐����璸�_�ƃC���f�b�N�X�����炩���ߊm��
		const int polygonCount = fbxMesh->GetPolygonCount();
		mesh.vertices.resize(polygonCount * 3LL);
		mesh.indices.resize(polygonCount * 3LL);

		// UV���擾
		FbxStringList uvNames;
		fbxMesh->GetUVSetNames(uvNames);
		// �R���g���[���|�C���g�̎擾
		const FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex) {
			for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon) {
				// ���_�̃C���f�b�N�X
				const int vertexIndex = polygonIndex * 3 + positionInPolygon;

				// ���_
				Vertex vertex;
				const int polygonVertex = fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon);
				vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
				vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
				vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);

				// �@��������΃R�s�[
				if (fbxMesh->GetElementNormalCount() > 0) {
					FbxVector4 normal;
					fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				// UV������΃R�s�[
				if (fbxMesh->GetElementUVCount() > 0) {
					FbxVector2 uv;
					bool unmappedUv;
					fbxMesh->GetPolygonVertexUV(polygonIndex, positionInPolygon, uvNames[0], uv, unmappedUv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				mesh.vertices.at(vertexIndex) = std::move(vertex);
				mesh.indices.at(vertexIndex) = vertexIndex;
			}
		}

	}

}