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
private: // �G�C���A�X
    // Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
    Mesh();


private:
    static std::map<std::string, std::weak_ptr<Mesh>> mesh_datas_;

    // ���_�z��
    std::vector<Vector3> vertices_;
    // �C���f�b�N�X
    std::vector<uint32_t>	indices;

    // ���_�o�b�t�@
    ComPtr<ID3D12Resource> vertexBuffer;
    // ���_�o�b�t�@�r���[
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    // �C���f�b�N�X�o�b�t�@
    ComPtr<ID3D12Resource> indexBuffer;
    // �C���f�b�N�X�o�b�t�@�r���[
    D3D12_INDEX_BUFFER_VIEW ibView = {};

    // ���b�V���萔�o�b�t�@
    ComPtr<ID3D12Resource> mesh_constant_buffer_;
};

