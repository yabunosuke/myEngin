#include "PrimitiveRenderer.h"
#include "PipelineManager.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "Math/Mathf.h"

const int PrimitiveRenderer::kBufferNum;

PrimitiveRenderer &PrimitiveRenderer::GetInstance()
{
	static PrimitiveRenderer instance;
	return instance;
}

void PrimitiveRenderer::CreatePrimitivAll(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	HRESULT result = S_OK;

	// メッシュデータ生成
	CreateLine(dev);
	CreateBox(dev);


	for (int i = 0; i < kBufferNum; ++i)
	{
		
		// 定数バッファの生成
		result = dev->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstantBuffer) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constant_buffer_[i])
		);

	}

}

void PrimitiveRenderer::DrawLine(ComPtr<ID3D12GraphicsCommandList> cmd_list,Line line, XMFLOAT4 color)
{
	DirectX::XMVECTOR start;						// 始点
	DirectX::XMVECTOR end;							// 終点
	DirectX::XMVECTOR start_to_end;					// 始点から終点のベクトル
	float Length;									// 始点から終点までの長さ
	DirectX::XMVECTOR up_cross_start_to_end;		// 始点と終点の外積
	DirectX::XMVECTOR normal_start_to_end;			// 
	DirectX::XMVECTOR normal_stert_cross_end;		//
	DirectX::XMVECTOR _cos;							//
	float rad;										// 角度（ラジアン）
	DirectX::XMMATRIX scale;						//
	DirectX::XMMATRIX rotate;						//
	DirectX::XMMATRIX transform;					//
	DirectX::XMMATRIX world_view_projection;		// 



	// 行列作成
	start = DirectX::XMLoadFloat3(&line.start_positon);
	end = DirectX::XMLoadFloat3(&line.end_positon);
	start_to_end = DirectX::XMVectorSubtract(end, start);
	Length = DirectX::XMVectorGetX(DirectX::XMVector3Length(start_to_end));
	up_cross_start_to_end = DirectX::XMVector3Cross(DirectX::XMVectorSet(0, 1, 0, 0), start_to_end);

	normal_start_to_end = DirectX::XMVector3Normalize(start_to_end);
	normal_stert_cross_end = DirectX::XMVector3Normalize(up_cross_start_to_end);

	_cos = DirectX::XMVector3Dot(normal_start_to_end, DirectX::XMVectorSet(0, 1, 0, 0));
	rad = acosf(DirectX::XMVectorGetX(_cos));

	scale = DirectX::XMMatrixScaling(1.0f, Length, 1.0f);
	rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationNormal(normal_stert_cross_end, rad));
	transform = DirectX::XMMatrixTranslation(line.start_positon.x, line.start_positon.y, line.start_positon.z);

	world_view_projection = (scale * rotate * transform) * Camera::GetCam()->GetViewProjectionMatrix();


	// パイプラインステートとルートシグネチャ設定
	PipelineManager::GetInstance()->SetPipline(cmd_list, "Liner");

	// コンスタントバッファの転送
	cmd_list->SetGraphicsRootConstantBufferView(0, constant_buffer_[buffer_index_]->GetGPUVirtualAddress());
	ConstantBuffer *constant_buffer_map = nullptr;
	HRESULT result = constant_buffer_[buffer_index_]->Map(0, nullptr, (void **)&constant_buffer_map);
	if (SUCCEEDED(result))
	{
		XMStoreFloat4x4(&constant_buffer_map->world,world_view_projection);	//ワールド行列
		constant_buffer_map->color = color;		// カラー
		constant_buffer_[buffer_index_]->Unmap(0, nullptr);
	}

	// 頂点バッファをセット(VBV)
	cmd_list->IASetVertexBuffers(0, 1, &vbView[static_cast<int>(PrimitiveType::LINE)]);
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	//描画コマンド
	cmd_list->DrawInstanced(2,1,0,0);

	++buffer_index_;
}

void PrimitiveRenderer::DrawBox(ComPtr<ID3D12GraphicsCommandList> cmd_list, Box box, XMFLOAT4 color)
{

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(box.scale.x, box.scale.y, box.scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(
		box.rotate.x * Mathf::deg_to_rad,
		box.rotate.y * Mathf::deg_to_rad,
		box.rotate.z * Mathf::deg_to_rad
	);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(box.translate.x, box.translate.y, box.translate.z);

	DirectX::XMMATRIX world = S * R * T;	// ワールドビュー行列
	DirectX::XMMATRIX world_view_projection = world * Camera::GetCam()->GetViewProjectionMatrix();	// ワールドビュー行列


		// パイプラインステートとルートシグネチャ設定
	PipelineManager::GetInstance()->SetPipline(cmd_list, "Liner");

	// コンスタントバッファの転送
	cmd_list->SetGraphicsRootConstantBufferView(0, constant_buffer_[buffer_index_]->GetGPUVirtualAddress());
	ConstantBuffer *constant_buffer_map = nullptr;
	HRESULT result = constant_buffer_[buffer_index_]->Map(0, nullptr, (void **)&constant_buffer_map);
	if (SUCCEEDED(result))
	{
		XMStoreFloat4x4(&constant_buffer_map->world, world_view_projection);	//ワールド行列
		constant_buffer_map->color = color;		// カラー
		constant_buffer_[buffer_index_]->Unmap(0, nullptr);
	}

	// 頂点バッファをセット(VBV)
	cmd_list->IASetVertexBuffers(0, 1, &vbView[static_cast<int>(PrimitiveType::BOX)]);
	// インデックスバッファをセット(IBV)
	//cmd_list->IASetIndexBuffer(&ibView);
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	//描画コマンド
	cmd_list->DrawInstanced(24, 1, 0, 0);

	++buffer_index_;

}

void PrimitiveRenderer::CreateLine(ComPtr<ID3D12Device> dev)
{
	// 線の頂点生成
	// 真上を向いているサイズ1の線
	std::vector<XMFLOAT4> vertices = {
		{0,0,0,1},
		{0,1,0,1}
	};

	HRESULT result = S_OK;

	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT4) * vertices.size());
	// 頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_[static_cast<int>(PrimitiveType::LINE)]));

	// 頂点バッファへのデータ転送
	XMFLOAT4 *vertMap = nullptr;
	result = vertex_buffer_[static_cast<int>(PrimitiveType::LINE)]->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertex_buffer_[static_cast<int>(PrimitiveType::LINE)]->Unmap(0, nullptr);
	}
	// 頂点バッファビュー(VBV)の作成
	vbView[static_cast<int>(PrimitiveType::LINE)].BufferLocation = vertex_buffer_[static_cast<int>(PrimitiveType::LINE)]->GetGPUVirtualAddress();
	vbView[static_cast<int>(PrimitiveType::LINE)].SizeInBytes = sizeVB;
	vbView[static_cast<int>(PrimitiveType::LINE)].StrideInBytes = sizeof(vertices[0]);
}

void PrimitiveRenderer::CreateBox(ComPtr<ID3D12Device> dev)
{
	float tabel[2] = { -0.5f,0.5f };	// 頂点の位置
	const int vertex_num = 24;
	XMFLOAT4 vertices[vertex_num];
	int vertices_index = 0;
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int z = 0; z < 2; ++z)
			{
				for (int edge = 0; edge < 3;++edge)
				{
					if (edge == 0 && x == 0) continue;
					if (edge == 1 && y == 0) continue;
					if (edge == 2 && z == 0) continue;

					// 現在の頂点
					XMFLOAT4 vertex = {
						tabel[x],
						tabel[y],
						tabel[z],
						1
					};

					// 次の頂点
					XMFLOAT4 vertex_next = {
						tabel[edge != 0 ? x : !x],
						tabel[edge != 1 ? y : !y],
						tabel[edge != 2 ? z : !z],
						1 };
					
					vertices[vertices_index]=vertex;
					++vertices_index;
					vertices[vertices_index]=vertex_next;
					++vertices_index;
				}
			}
		}
	}

	HRESULT result = S_OK;

	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT4) * _countof(vertices));
	// 頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_[static_cast<int>(PrimitiveType::BOX)]));

	// 頂点バッファへのデータ転送
	XMFLOAT4 *vertMap = nullptr;
	result = vertex_buffer_[static_cast<int>(PrimitiveType::BOX)]->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices, vertices+ vertex_num, vertMap);
		vertex_buffer_[static_cast<int>(PrimitiveType::BOX)]->Unmap(0, nullptr);
	}
	// 頂点バッファビュー(VBV)の作成
	vbView[static_cast<int>(PrimitiveType::BOX)].BufferLocation = vertex_buffer_[static_cast<int>(PrimitiveType::BOX)]->GetGPUVirtualAddress();
	vbView[static_cast<int>(PrimitiveType::BOX)].SizeInBytes = sizeVB;
	vbView[static_cast<int>(PrimitiveType::BOX)].StrideInBytes = sizeof(vertices[0]);
}

void PrimitiveRenderer::CreateSphere(ComPtr<ID3D12Device> dev)
{
}