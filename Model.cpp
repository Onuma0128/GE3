#include "Model.h"
#include "ModelBase.h"
#include "TextureManager.h"

void Model::Initialize(const std::string& filename)
{
    this->modelBase_ = ModelBase::GetInstance();

    modelData_ = LoadObjFile("resources", filename);

    TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);

    modelData_.material.textureIndex =
        TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);

    MakeVertexData();

    MakeMaterialData();
}

void Model::Draw()
{
    modelBase_->GetDxEngine()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
    modelBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    modelBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureIndex));
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

void Model::MakeMaterialData()
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    materialResource_ = CreateBufferResource(modelBase_->GetDxEngine()->GetDevice(), sizeof(Material)).Get();
    // 書き込むためのアドレスを取得
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    // 今回は白を書き込んでいく
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = true;
    materialData_->uvTransform = MakeIdentity4x4();
    materialData_->shininess = 8.0f;
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
    std::wstring filePath = s2ws(directoryPath + "/" + filename);
    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    assert(hFile != INVALID_HANDLE_VALUE);

    DWORD fileSize = GetFileSize(hFile, nullptr);
    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, fileSize, nullptr);
    assert(hMapping != nullptr);

    char* data = static_cast<char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize));
    assert(data != nullptr);

    CloseHandle(hMapping);
    CloseHandle(hFile);

    ModelData modelData;
    std::vector<Vector4> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;

    char* ptr = data;
    char* end = data + fileSize;
    while (ptr < end) {
        char* lineEnd = std::find(ptr, end, '\n');
        std::string line(ptr, lineEnd);
        ptr = lineEnd + 1;

        std::istringstream s(line);
        std::string identifier;
        s >> identifier;

        if (identifier == "v") {
            Vector4 position{};
            s >> position.x >> position.y >> position.z;
            position.x *= -1.0f;
            position.w = 1.0f;
            positions.push_back(position);
        }
        else if (identifier == "vt") {
            Vector2 texcoord{};
            s >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if (identifier == "vn") {
            Vector3 normal{};
            s >> normal.x >> normal.y >> normal.z;
            normal.x *= -1.0f;
            normals.push_back(normal);
        }
        else if (identifier == "f") {
            VertexData triangle[3]{};
            for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
                std::string vertexDefinition;
                s >> vertexDefinition;
                std::istringstream v(vertexDefinition);
                uint32_t elementIndices[3] = { 0,0,0 };
                std::string index;
                int32_t element = 0;
                while (std::getline(v, index, '/'))
                {
                    if (!index.empty())
                    {
                        elementIndices[element] = std::stoi(index);
                    }
                    element++;
                }
                Vector4 position = positions[elementIndices[0] - 1];
                Vector2 texcoord = { 0,0 };
                if (elementIndices[1] > 0) {
                    texcoord = texcoords[elementIndices[1] - 1];
                }
                Vector3 normal = normals[elementIndices[2] - 1];
                triangle[faceVertex] = { position, texcoord, normal };
            }
            modelData.vertices.push_back(triangle[2]);
            modelData.vertices.push_back(triangle[1]);
            modelData.vertices.push_back(triangle[0]);
        }
        else if (identifier == "mtllib") {
            std::string materialFilename;
            s >> materialFilename;
            modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
        }
    }
    UnmapViewOfFile(data);
    return modelData;
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
            materialData.textureFilePath = directoryPath + "/" + textureFilename;
        }
    }
    if (materialData.textureFilePath.empty()) {
        std::string textureFilename = "white1x1.png";
        materialData.textureFilePath = directoryPath + "/" + textureFilename;
    }
    return materialData;
}