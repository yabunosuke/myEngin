#pragma once

#include <vector>
#include <map>
#include <d3d12.h>
#include <d3dx12.h>

#include "../Object.h"
#include "../../Math/Vector3.h"
class Mesh :
    public Object
{
private: // エイリアス
    // Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
    Mesh();


private:
    static std::map<std::string, std::weak_ptr<Mesh>> mesh_datas_;

    // 頂点配列
    std::vector<Vector3> vertices_;
    // インデックス
    std::vector<uint32_t>	indices;

    // 頂点バッファ
    ComPtr<ID3D12Resource> vertexBuffer;
    // 頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    // インデックスバッファ
    ComPtr<ID3D12Resource> indexBuffer;
    // インデックスバッファビュー
    D3D12_INDEX_BUFFER_VIEW ibView = {};

    // メッシュ定数バッファ
    ComPtr<ID3D12Resource> mesh_constant_buffer_;
};

