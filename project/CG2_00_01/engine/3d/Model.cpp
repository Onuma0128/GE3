#include "Model.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "ModelBase.h"
#include "TextureManager.h"
#include "SrvManager.h"
#include "CreateBufferResource.h"


void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
    this->modelBase_ = ModelBase::GetInstance();

    modelData_ = LoadObjFile(directoryPath, filename);

    TextureManager::GetInstance()->LoadTexture(modelData_.material.directoryPath + modelData_.material.filePath);

    modelData_.material.textureIndex =
        TextureManager::GetInstance()->GetSrvIndex(modelData_.material.directoryPath + modelData_.material.filePath);

    MakeVertexData();
}

void Model::Draw()
{
    modelBase_->GetDxEngine()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, modelData_.material.textureIndex);
    // 描画
    modelBase_->GetDxEngine()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::MakeVertexData()
{
    // 実際に頂点リソースを作る
    vertexResource_ = CreateBufferResource(modelBase_->GetDxEngine()->GetDevice(), sizeof(VertexData) * modelData_.vertices.size()).Get();
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

std::wstring Model::s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

Model::ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
    assert(scene->HasMeshes());

    ModelData modelData;

    // meshを解析
    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* mesh = scene->mMeshes[meshIndex];
        assert(mesh->HasNormals());
        //assert(mesh->HasTextureCoords(0));

        // faceを解析
        for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
            aiFace& face = mesh->mFaces[faceIndex];
            assert(face.mNumIndices == 3);

            // vertexを解析
            for (uint32_t element = 0; element < face.mNumIndices; ++element) {
                uint32_t vertexIndex = face.mIndices[element];
                aiVector3D& position = mesh->mVertices[vertexIndex];
                aiVector3D& normal = mesh->mNormals[vertexIndex];

                VertexData vertex;
                vertex.position = { position.x,position.y,position.z,1.0f };
                vertex.normal = { normal.x,normal.y,normal.z };
                if (mesh->HasTextureCoords(0)) {
                    aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
                    vertex.texcoord = { texcoord.x,texcoord.y };
                }
                else {
                    vertex.texcoord = { 0.0f,0.0f };
                }

                vertex.position.x *= -1.0f;
                vertex.normal.x *= -1.0f;
                modelData.vertices.push_back(vertex);
            }
        }
    }

    // materialを解析
    bool textureFound = false;  // テクスチャが見つかったかどうかのフラグ
    for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
        aiMaterial* material = scene->mMaterials[materialIndex];

        // Diffuseテクスチャを確認
        if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
            aiString textureFilePath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
            modelData.material.directoryPath = directoryPath + "/";
            modelData.material.filePath = textureFilePath.C_Str();
            textureFound = true; // テクスチャが見つかった
        }
        // 他にも必要なテクスチャタイプがあればここで追加で確認する

        // いずれのテクスチャも見つからなければデフォルトのwhite1x1.pngを割り当てる
        if (!textureFound) {
            modelData.material.directoryPath = "resources/";
            modelData.material.filePath = "white1x1.png";
        }
    }

    modelData.rootNode = ReadNode(scene->mRootNode);

    return modelData;
}

void Model::SetTexture(const std::string& directoryPath, const std::string& filename)
{
    modelData_.material.directoryPath = directoryPath + "/";
    modelData_.material.filePath = filename;
    TextureManager::GetInstance()->LoadTexture(modelData_.material.directoryPath + modelData_.material.filePath);
    modelData_.material.textureIndex =
        TextureManager::GetInstance()->GetSrvIndex(modelData_.material.directoryPath + modelData_.material.filePath);
}

Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
    MaterialData materialData; // 構築するMaterialData
    std::string line; // ファイルから読んだ1行を格納するもの
    std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
    assert(file.is_open()); // とりあえず開けなかったら止める
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream s(line);
        s >> identifier;
        // identifierに応じた処理
        if (identifier == "map_Kd") {
            std::string textureFilename;
            s >> textureFilename;
            //連結してファイルパスにする
            materialData.directoryPath = directoryPath + "/";
            materialData.filePath = textureFilename;
        }
    }
    std::string materialTexture = materialData.directoryPath + materialData.filePath;
    if (materialTexture.empty()) {
        std::string textureFilename = "white1x1.png";
        materialData.directoryPath = directoryPath + "/";
        materialData.filePath = textureFilename;
    }
    return materialData;
}

Model::Node Model::ReadNode(aiNode* node)
{
    Node result;
    aiMatrix4x4 aiLocalMatrix = node->mTransformation;
    aiLocalMatrix.Transpose();
    for (uint32_t i = 0; i < 4; ++i) {
        for (uint32_t j = 0; j < 4; ++j) {
            result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
        }
    }
    result.name = node->mName.C_Str();
    result.children.resize(node->mNumChildren);
    for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
    }
    return result;
}