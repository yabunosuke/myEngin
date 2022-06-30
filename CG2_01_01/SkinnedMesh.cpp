#include "SkinnedMesh.h"
#include <sstream>
#include <functional>

SkinnedMesh::SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate)
{
	// マネージャーの生成
	FbxManager *fbxManager = FbxManager::Create();
	// シーン生成
	FbxScene *fbxScene = FbxScene::Create(fbxManager, "");

	FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "");
	// ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}

	//インポート
	if (!fbxImporter->Import(fbxScene)) {
		assert(0);
	}

	//三角形化
	FbxGeometryConverter fbx_converter(fbxManager);
	if (trianglate) {
		fbx_converter.Triangulate(fbxScene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbxScene);
	}

	//センチメートルに変換
	FbxSystemUnit::m.ConvertScene(fbxScene);

	//ノードの取得
	std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
		Scene::Node &node = sceneView.nodes.emplace_back();
		//typeがnullだったらeUnknownをセット
		node.atribute = fbxNode->GetNodeAttribute() ?
			fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
		node.name = fbxNode->GetName();
		node.uniqueID = fbxNode->GetUniqueID();
		//親がいなければ0を代入
		node.pearentIndex = sceneView.indexof(fbxNode->GetParent() ?
			fbxNode->GetParent()->GetUniqueID() : 0);
		//子に対して再起処理
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
	// すべてのノードをチェック
	for (const Scene::Node &node : sceneView.nodes) {
		// メッシュでなければスキップ
		if (node.atribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}
		// ノードとメッシュを抜き出し
		FbxNode *fbxNode = fbxScene->FindNodeByName(node.name.c_str());
		FbxMesh *fbxMesh = fbxNode->GetMesh();
		
		// メッシュ構造体に当てはめる
		Mesh &mesh = meshes.emplace_back();
		mesh.uniqueID = fbxMesh->GetNode()->GetUniqueID();
		mesh.name = fbxMesh->GetNode()->GetName();
		mesh.nodeIndex = sceneView.indexof(mesh.uniqueID);

		// ポリゴンの数から頂点とインデックスをあらかじめ確保
		const int polygonCount = fbxMesh->GetPolygonCount();
		mesh.vertices.resize(polygonCount * 3LL);
		mesh.indices.resize(polygonCount * 3LL);

		// UVを取得
		FbxStringList uvNames;
		fbxMesh->GetUVSetNames(uvNames);
		// コントロールポイントの取得
		const FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex) {
			for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon) {
				// 頂点のインデックス
				const int vertexIndex = polygonIndex * 3 + positionInPolygon;

				// 頂点
				Vertex vertex;
				const int polygonVertex = fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon);
				vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
				vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
				vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);

				// 法線があればコピー
				if (fbxMesh->GetElementNormalCount() > 0) {
					FbxVector4 normal;
					fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				// UVがあればコピー
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