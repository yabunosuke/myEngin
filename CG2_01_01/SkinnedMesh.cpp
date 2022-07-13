#include "SkinnedMesh.h"
#include <sstream>
#include <functional>
#include <filesystem>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "Texture.h"
#include "Camera.h"

void SkinnedMesh::CreatePipline()
{
}

inline XMFLOAT4X4 SkinnedMesh::ConvertXMFLOAT4X4FromFbx(const FbxMatrix &fbx_matrix)
{
	XMFLOAT4X4 xmfloat4x4;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			xmfloat4x4.m[row][column] = static_cast<float>(fbx_matrix[row][column]);
		}
	}
	return xmfloat4x4;
}

inline XMFLOAT3 SkinnedMesh::ConvertXMFLOAT3FromFbx(const FbxDouble3 &fbx_double3)
{
	XMFLOAT3 xmfloat3;
	xmfloat3.x = static_cast<float>(fbx_double3[0]);
	xmfloat3.y = static_cast<float>(fbx_double3[1]);
	xmfloat3.z = static_cast<float>(fbx_double3[2]);
	return xmfloat3;
}

inline XMFLOAT4 SkinnedMesh::ConvertXMFLOAT4FromFbx(const FbxDouble4 &fbx_double4)
{
	XMFLOAT4 xmfloat4;
	xmfloat4.x = static_cast<float>(fbx_double4[0]);
	xmfloat4.y = static_cast<float>(fbx_double4[1]);
	xmfloat4.z = static_cast<float>(fbx_double4[2]);
	xmfloat4.w = static_cast<float>(fbx_double4[3]);
	return xmfloat4;
}

SkinnedMesh::SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate)
{
	// �}�l�[�W���[�̐���
	FbxManager *fbx_manager = FbxManager::Create();
	// �V�[������
	FbxScene *fbx_scene = FbxScene::Create(fbx_manager, "");

	FbxImporter *fbxImporter = FbxImporter::Create(fbx_manager, "");
	// �t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
	if (!fbxImporter->Initialize(fileName, -1, fbx_manager->GetIOSettings()))
	{
		assert(0);
	}

	//�C���|�[�g
	if (!fbxImporter->Import(fbx_scene)) {
		assert(0);
	}

	//�O�p�`��
	FbxGeometryConverter fbx_converter(fbx_manager);
	if (trianglate) {
		fbx_converter.Triangulate(fbx_scene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
	}

	//�Z���`���[�g���ɕϊ�
	FbxSystemUnit::m.ConvertScene(fbx_scene);

	//�m�[�h�̎擾
	std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
		Scene::Node &node = scene_view_.nodes.emplace_back();
		// type��null��������eUnknown���Z�b�g
		node.atribute = fbxNode->GetNodeAttribute() ?
			fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
		node.name = fbxNode->GetName();
		node.uniqueID = fbxNode->GetUniqueID();
		// �e�����Ȃ����0����
		node.pearentIndex = scene_view_.indexof(fbxNode->GetParent() ?
			fbxNode->GetParent()->GetUniqueID() : 0);
		// �q�ɑ΂��čċN����
		for (int child_index = 0; child_index < fbxNode->GetChildCount(); ++child_index) {
			traverse(fbxNode->GetChild(child_index));
		}
	} };
	
	// �m�[�h�擾
	traverse(fbx_scene->GetRootNode());
	// ���b�V�������擾
	FetchMeshes(fbx_scene, meshes);
	// �}�e���A�������擾
	FetchMaterial(fbx_scene, materials);

	// ���
	fbx_manager->Destroy();
	// �I�u�W�F�N�g����
	CreateComObjects(dev, fileName);
}

void SkinnedMesh::FetchMeshes(FbxScene *fbxScene, std::vector<Mesh> &meshes)
{
	// ���ׂẴm�[�h���`�F�b�N
	for (const Scene::Node &node : scene_view_.nodes) {
		// ���b�V���łȂ���΃X�L�b�v
		if (node.atribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}
		// �m�[�h�ƃ��b�V���𔲂��o��
		FbxNode *fbx_node = fbxScene->FindNodeByName(node.name.c_str());
		FbxMesh *fbx_mesh = fbx_node->GetMesh();
		
		// ���b�V���\���̂ɓ��Ă͂߂�
		Mesh &mesh = meshes.emplace_back();
		mesh.unique_id = fbx_mesh->GetNode()->GetUniqueID();
		mesh.name = fbx_mesh->GetNode()->GetName();
		mesh.node_index = scene_view_.indexof(mesh.unique_id);
		
		//mesh.default_grlobal_transform = ConvertXMFLOAT4X4FromFbx(fbx_mesh->GetNode()->EvaluateGlobalTransform());

		// �R���e�i�̎擾
		std::vector<Mesh::Subset> &subsets = mesh.subsets;
		// �}�e���A���J�E���g
		const int material_count = fbx_mesh->GetNode()->GetMaterialCount();
		// ���炩���ߗ̈���m�ہi�Œ�1�j
		subsets.resize(material_count > 0 ? material_count : 1);
		// �S�Ẵ}�e���A��id�Ɩ��O���擾
		for (int material_index = 0; material_index < material_count; ++material_index) {
			const FbxSurfaceMaterial *fbx_material = fbx_mesh->GetNode()->GetMaterial(material_index);
			subsets.at(material_index).material_name = fbx_material->GetName();
			subsets.at(material_index).material_unique_id = fbx_material->GetUniqueID();
		}

		// �|���S�����̎擾
		const int polygon_count = fbx_mesh->GetPolygonCount();

		// �}�e���A�����ݒ肳��Ă���΃|���S�����ƂɃ}�e���A����ݒ肷��
		if (material_count > 0) {
			for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index) {
				const int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index);
				subsets.at(material_index).index_count += 3;
			}
			uint32_t offset = 0;
			for (Mesh::Subset &subset : subsets) {
				subset.start_index = offset;
				offset += subset.index_count;
				subset.index_count = 0;
			}
		}

		// �|���S���̐����璸�_�ƃC���f�b�N�X�����炩���ߊm��
		mesh.vertices.resize(polygon_count * 3LL);
		mesh.indices.resize(polygon_count * 3LL);

		// UV���擾
		FbxStringList uv_names;
		fbx_mesh->GetUVSetNames(uv_names);
		// �R���g���[���|�C���g�̎擾
		const FbxVector4 *control_points = fbx_mesh->GetControlPoints();
		for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index) {
			const int material_index =
				material_count > 0 ?
				fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) :
				0;
			Mesh::Subset &subset = subsets.at(material_index);
			const uint32_t offset = subset.start_index + subset.index_count;

			for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon) {
				// ���_�̃C���f�b�N�X
				const int vertex_index = polygon_index * 3 + position_in_polygon;

				// ���_
				Vertex vertex;
				const int polygonVertex = fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon);
				vertex.position.x = static_cast<float>(control_points[polygonVertex][0]);
				vertex.position.y = static_cast<float>(control_points[polygonVertex][1]);
				vertex.position.z = static_cast<float>(control_points[polygonVertex][2]);

				// �@��������΃R�s�[
				if (fbx_mesh->GetElementNormalCount() > 0) {
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				// UV������΃R�s�[
				if (fbx_mesh->GetElementUVCount() > 0) {
					FbxVector2 uv;
					bool unmapped_uv;
					fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				mesh.vertices.at(vertex_index) = std::move(vertex);
				mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
				subset.index_count++;
			}
		}

	}

}

void SkinnedMesh::FetchMaterial(FbxScene *fbx_scene, std::unordered_map<uint64_t, Material> &materials)
{
	// �m�[�h�̐�
	const size_t node_count = scene_view_.nodes.size();

	for (int node_index = 0; node_index < node_count; ++node_index) {
		const Scene::Node &node = scene_view_.nodes.at(node_index);
		const FbxNode *fbx_node = fbx_scene->FindNodeByName(node.name.c_str());
		
		const int material_count = fbx_node->GetMaterialCount();
		for (int material_index = 0; material_index < material_count; ++material_index) {
			// �}�e���A���擾
			const FbxSurfaceMaterial *fbx_material = fbx_node->GetMaterial(material_index);

			Material material;
			material.name = fbx_material->GetName();
			material.unique_id = fbx_material->GetUniqueID();

			// ���Ƃ�ADS�S���Ƃ��悤�ɂ���
			// �f���t���[�Y
			FbxProperty fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (fbx_property.IsValid()) {

				const FbxDouble3 diffuse = fbx_property.Get<FbxDouble3>();
				material.Kd.x = static_cast<float>(diffuse[0]);
				material.Kd.y = static_cast<float>(diffuse[1]);
				material.Kd.z = static_cast<float>(diffuse[2]);
				material.Kd.w = 1.0f;

				const FbxFileTexture *fbx_texture = fbx_property.GetSrcObject<FbxFileTexture>();
				material.texture_filenames[0] =
					fbx_texture ? fbx_texture->GetRelativeFileName() : "";
			}

			materials.emplace(material.unique_id, std::move(material));
		}
	}
	if (materials.size() == 0) {
		CreateDummyMaterial(materials);
	}
}


void SkinnedMesh::CreateComObjects(ID3D12Device *dev, const char *fileName)
{
	//���b�V���̐��������J��Ԃ�
	for (Mesh &mesh : meshes) {
		HRESULT result;

		// ���_�f�[�^�S�̂̃T�C�Y
		UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
		// ���_�o�b�t�@�̐���
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.vertexBuffer));
		// ���_�o�b�t�@�ւ̃f�[�^�]��
		Vertex *vertMap = nullptr;
		result = mesh.vertexBuffer->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertexBuffer->Unmap(0, nullptr);
		}
		// ���_�o�b�t�@�r���[(VBV)�̍쐬
		mesh.vbView.BufferLocation = mesh.vertexBuffer->GetGPUVirtualAddress();
		mesh.vbView.SizeInBytes = sizeVB;
		mesh.vbView.StrideInBytes = sizeof(mesh.vertices[0]);


		// ���_�C���f�b�N�X�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh.indices.size());
		// �C���f�b�N�X�o�b�t�@����
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.indexBuffer));
		// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
		unsigned short *indexMap = nullptr;
		result = mesh.indexBuffer->Map(0, nullptr, (void **)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.indices.begin(), mesh.indices.end(), indexMap);
			mesh.indexBuffer->Unmap(0, nullptr);
		}
		// �C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬
		mesh.ibView.BufferLocation = mesh.indexBuffer->GetGPUVirtualAddress();
		mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
		mesh.ibView.SizeInBytes = sizeIB;
	}


	//�p�C�v���C����������
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; //���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    //�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g


	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile
	(
		L"SkinnedMeshVS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob
	);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile
	(
		L"SkinnedMeshPS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob
	);
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W(�e�N�X�`�����l���܂�)
	CD3DX12_DESCRIPTOR_RANGE desc_range_srv[4] = {};
	desc_range_srv[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^
	desc_range_srv[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 ���W�X�^
	desc_range_srv[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // t2 ���W�X�^
	desc_range_srv[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 ���W�X�^


	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// CBV�i�V�[���o�b�t�@�j
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV�i�e�N�X�`���j
	rootparams[2].InitAsDescriptorTable(_countof(desc_range_srv), desc_range_srv, D3D12_SHADER_VISIBILITY_ALL);
	////CBV(�X�L�j���O�p)
	//rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//// CBV�i�}�e���A���p�j
	//rootparams[3].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	//// CBV�i���C�g�p�j
	//rootparams[4].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc[3] = { 
		CD3DX12_STATIC_SAMPLER_DESC(0),
		CD3DX12_STATIC_SAMPLER_DESC(1),
		CD3DX12_STATIC_SAMPLER_DESC(2)
	};

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, _countof(sampler_desc), sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature_.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }



	// ���b�V���萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MeshConstantBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mesh_constant_buffer_)
	);
	// �V�[���萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneConstantBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&scene_constant_buffer_)
	);


	// �V�F�[�_�[���\�[�X�r���[�̐���
	for (std::unordered_map<uint64_t, Material>::iterator itr = materials.begin(); itr != materials.end(); ++itr) {
		// �e�N�X�`�����w�肳��Ă���ΐݒ�
		if (itr->second.texture_filenames[0].size() > 0) {
			// �t�@�C�����ۑ�
			std::filesystem::path path = fileName;
			path.replace_filename(itr->second.texture_filenames[0]);

			// �e�N�X�`���ǂݍ���
			itr->second.shader_resource_views[0] = Texture::LoadTextureFromFile(dev, path.c_str());
			
		}
		// �e�N�X�`�����Ȃ��ꍇ
		else {
			// �_�~�[�e�N�X�`������
			Texture::MakeDummmyTexture(dev/*, itr->second.shader_resource_views[0]*/);
		}
	}
}

void SkinnedMesh::Render(ID3D12Device *dev, ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &material_color)
{
	HRESULT result;
	
	// �V�[���o�b�t�@�X�V
	SceneConstantBuffer *scene_constant_buffer_map = nullptr;
	result = scene_constant_buffer_->Map(0, nullptr, (void **)&scene_constant_buffer_map);
	if (SUCCEEDED(result))
	{
		//���ŌÂ��J�����N���X���g�p����
		DirectX::XMStoreFloat4x4(&scene_constant_buffer_map->view_projection, Camera::GetCam()->GetViewProjectionMatrix());
		scene_constant_buffer_map->light_direction = { 0,0.5,0.5, 0 };
		scene_constant_buffer_map->camera_position = { Camera::GetCam()->eye.x,Camera::GetCam()->eye.y,Camera::GetCam()->eye.z ,1};
		scene_constant_buffer_->Unmap(0, nullptr);
	}

	//���b�V���񐔕�
	for (const Mesh &mesh : meshes) {

		//�e�o�b�t�@�̃Z�b�g
		// �p�C�v���C���X�e�[�g�̐ݒ�
		cmdList->SetPipelineState(pipelinestate_.Get());
		// ���[�g�V�O�l�`���̐ݒ�
		cmdList->SetGraphicsRootSignature(rootsignature_.Get());
		// �v���~�e�B�u�`���ݒ�
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// ���b�V���萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(0, mesh_constant_buffer_->GetGPUVirtualAddress());
		// �V�[���萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(1, scene_constant_buffer_->GetGPUVirtualAddress());


		// ���_�o�b�t�@���Z�b�g(VBV)
		cmdList->IASetVertexBuffers(0, 1, &mesh.vbView);
		// �C���f�b�N�X�o�b�t�@���Z�b�g(IBV)
		cmdList->IASetIndexBuffer(&mesh.ibView);

		//// �f�X�N���v�^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap *ppHeaps[] = { Texture::descriptor_heap_.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		
		//// ���b�V���萔�o�b�t�@�X�V
		//MeshConstantBuffer *mesh_constant_buffer_map = nullptr;
		//result = mesh_constant_buffer_->Map(0, nullptr, (void **)&mesh_constant_buffer_map);
		//if (SUCCEEDED(result))
		//{
		//	mesh_constant_buffer_map->world = world;
		//	//XMStoreFloat4x4(&mesh_constant_buffer_map->world, XMLoadFloat4x4(&mesh.default_grlobal_transform) /** XMLoadFloat4x4(&world)*/);

		//	// �F���
		//	//mesh_constant_buffer_map->materialColor = material_color;
		//	XMStoreFloat4(&mesh_constant_buffer_map->material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&materials.cbegin()->second.Kd));

		//	mesh_constant_buffer_->Unmap(0, nullptr);
		//}

		//// �V�F�[�_���\�[�X�r���[���Z�b�g
		//cmdList->SetGraphicsRootDescriptorTable(
		//	2, materials.cbegin()->second.shader_resource_views[0].Get()->GetGPUDescriptorHandleForHeapStart());

		////�`��R�}���h
		//cmdList->DrawIndexedInstanced((UINT)mesh.indices.size(), 1, 0, 0, 0);



		// �T�u�Z�b�g�P�ʂŕ`��
		for (const Mesh::Subset &subset : mesh.subsets) {
			const Material &material = materials.at(subset.material_unique_id);
			
			// ���b�V���萔�o�b�t�@�X�V
			MeshConstantBuffer *mesh_constant_buffer_map = nullptr;
			result = mesh_constant_buffer_->Map(0, nullptr, (void **)&mesh_constant_buffer_map);
			if (SUCCEEDED(result))
			{
				mesh_constant_buffer_map->world = world;
				//XMStoreFloat4x4(&mesh_constant_buffer_map->world, XMLoadFloat4x4(&mesh.default_grlobal_transform) /** XMLoadFloat4x4(&world)*/);
				// �F���
				XMStoreFloat4(&mesh_constant_buffer_map->material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));
				//XMStoreFloat4(&mesh_constant_buffer_map->material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));

				mesh_constant_buffer_->Unmap(0, nullptr);
			}

			// �V�F�[�_���\�[�X�r���[���Z�b�g
			cmdList->SetGraphicsRootDescriptorTable(
				2, material.shader_resource_views[0]);

			//�`��R�}���h
			cmdList->DrawIndexedInstanced(subset.index_count,1, subset.start_index, 0, 0);
		}
	}
}

void SkinnedMesh::CreateDummyMaterial(std::unordered_map<uint64_t, Material> &materials)
{
	Material material;
	material.name = "dummy";
	material.unique_id = 0;

	materials.emplace(material.unique_id, std::move(material));


}
