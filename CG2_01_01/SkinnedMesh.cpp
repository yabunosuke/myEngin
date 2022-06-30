#include "SkinnedMesh.h"
#include <assert.h>
#include <sstream>
#include <functional>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

SkinnedMesh::SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate)
{
	FbxManager *fbxManager = FbxManager::Create();
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

	FetchMeshes(fbxScene, meshes);

	fbxManager->Destroy();

	CreateComObjects(dev, fileName);
}

void SkinnedMesh::FetchMeshes(FbxScene *fbxScene, std::vector<Mesh> &meshes)
{
	for (const Scene::Node &node : sceneView.nodes) {
		// メッシュでなければコンテニュー
		if (node.atribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}

		FbxNode *fbxNode = fbxScene->FindNodeByName(node.name.c_str());
		FbxMesh *fbxMesh = fbxNode->GetMesh();

		Mesh &mesh = meshes.emplace_back();
		mesh.uniqueID = fbxMesh->GetNode()->GetUniqueID();
		mesh.name = fbxMesh->GetNode()->GetName();
		mesh.nodeIndex = sceneView.indexof(mesh.uniqueID);

		const int polygonCount = fbxMesh->GetPolygonCount();
		mesh.vertices.resize(polygonCount * 3LL);
		mesh.indices.resize(polygonCount * 3LL);

		FbxStringList uvNames;
		fbxMesh->GetUVSetNames(uvNames);
		const FbxVector4 *controlPoints = fbxMesh->GetControlPoints();
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex) {
			for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon) {
				const int vertexIndex = polygonIndex * 3 + positionInPolygon;

				Vertex vertex;
				const int polygonVertex = fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon);
				vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
				vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
				vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);

				if (fbxMesh->GetElementNormalCount() > 0) {
					FbxVector4 normal;
					fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

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

void SkinnedMesh::CreateComObjects(ID3D12Device *dev, const char *fileName)
{
	//メッシュの数だけ
	for (Mesh &mesh : meshes) {
		HRESULT result;

		// 頂点データ全体のサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
		// 頂点バッファ生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.vertexBuffer));
		// 頂点バッファへのデータ転送
		Vertex *vertMap = nullptr;
		result = mesh.vertexBuffer->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertexBuffer->Unmap(0, nullptr);
		}
		// 頂点バッファビュー(VBV)の作成
		mesh.vbView.BufferLocation = mesh.vertexBuffer->GetGPUVirtualAddress();
		mesh.vbView.SizeInBytes = sizeVB;
		mesh.vbView.StrideInBytes = sizeof(mesh.vertices[0]);


		// 頂点インデックス全体のサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh.indices.size());
		// インデックスバッファ生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.indexBuffer));
		// インデックスバッファへのデータ転送
		unsigned short *indexMap = nullptr;
		result = mesh.indexBuffer->Map(0, nullptr, (void **)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.indices.begin(), mesh.indices.end(), indexMap);
			mesh.indexBuffer->Unmap(0, nullptr);
		}
		// インデックスバッファビュー(IBV)の作成
		mesh.ibView.BufferLocation = mesh.indexBuffer->GetGPUVirtualAddress();
		mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
		mesh.ibView.SizeInBytes = sizeIB;

#if 1
		mesh.vertices.clear();
		mesh.indices.clear();
#endif
	}

	HRESULT result;

	//定数バッファの生成
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Constants) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBufferTransform)
	);
}

void SkinnedMesh::Render(ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor)
{
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constantBufferTransform->GetGPUVirtualAddress());

	
	for (const Mesh &mesh : meshes) {
		HRESULT result;


		// 頂点バッファをセット(VBV)
		cmdList->IASetVertexBuffers(0, 1, &mesh.vbView);
		// インデックスバッファをセット(IBV)
		cmdList->IASetIndexBuffer(&mesh.ibView);
		
		// 定数バッファへデータ転送
		Constants *constMap = nullptr;
		result = constantBufferTransform->Map(0, nullptr, (void **)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->world = world;
			constMap->materialColor = materialColor;
			constantBufferTransform->Unmap(0, nullptr);
		}
		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(0, constantBufferTransform->GetGPUVirtualAddress());


		// 描画コマンド
		cmdList->DrawIndexedInstanced((UINT)mesh.indices.size(), 1, 0, 0, 0);

	}
}
