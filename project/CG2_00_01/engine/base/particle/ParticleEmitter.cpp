#include "ParticleEmitter.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

ParticleEmitter::ParticleEmitter(const std::string name)
{
    emitter_.name = name;
    emitter_.transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    accelerationField_.acceleration = { 0.0f,10.0f,0.0f };
    accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField_.area.max = { 1.0f,1.0f,1.0f };
    // emitter_.frequencyごとに出すパーティクルの個数
    emitter_.count = 3;
    emitter_.size = {
        .min = {-1.0f,-1.0f,-1.0f},
        .max = {1.0f,1.0f,1.0f}
    };

    moveStart_ = true;
    isFieldStart_ = false;

    // Emitterの範囲を線で描画
    AABB aabb = emitter_.size;
    linePosition_ = CreateLineBox(emitter_.size);

    for (int i = 0; i * 2 < (int)linePosition_.size(); ++i) {
        int j = i * 2 + 1;
        std::unique_ptr<Line3d> line = std::make_unique<Line3d>();
        line->Initialize(linePosition_[i * 2], linePosition_[j]);
        lines_.push_back(std::move(line));
    }
}

void ParticleEmitter::Update()
{
#ifdef _DEBUG
    /*==================== パーティクルの範囲更新 ====================*/
    int i = 0;
    linePosition_ = CreateLineBox(emitter_.size);
    for (auto& line : lines_) {
        Vector3 translate = emitter_.transform.translate;
        line->SetPosition(linePosition_[i] + translate, linePosition_[i + 1] + translate);
        line->Update();
        i += 2;
    }
#endif // _DEBUG
}

void ParticleEmitter::Draw()
{
#ifdef _DEBUG
    std::string minSize = emitter_.name + "SizeMin";
    std::string maxSize = emitter_.name + "SizeMax";
    std::string isMove = emitter_.name + "Move";
    std::string isFieldStart = emitter_.name + "Field";
    AABB emitterSize = emitter_.size;
    ImGui::Begin("Emitter");
    ImGui::DragFloat3(minSize.c_str(), &emitterSize.min.x, 0.01f);
    ImGui::DragFloat3(maxSize.c_str(), &emitterSize.max.x, 0.01f);
    ImGui::Checkbox(isMove.c_str(), &moveStart_);
    ImGui::SameLine();
    ImGui::Checkbox(isFieldStart.c_str(), &isFieldStart_);
    ImGui::Text("\n");
    ImGui::End();
    emitter_.size = emitterSize;
    accelerationField_.area = {
        .min = emitterSize.min + emitter_.transform.translate,
        .max = emitterSize.max + emitter_.transform.translate,
    };

    /*==================== パーティクルの範囲描画 ====================*/
    PrimitiveDrawer::GetInstance()->DrawBase();
    for (auto& line : lines_) {
        line->Draw();
    }
#endif // _DEBUG
}

void ParticleEmitter::CreateParticles(ParticleManager::ParticleGroup& group)
{
    if (moveStart_) {
        emitter_.frequencyTime += kDeltaTime;
        if (emitter_.frequency <= emitter_.frequencyTime) {
            std::mt19937 randomEngine_(seedGenerator_());
            group.particles.splice(group.particles.end(), Emit(emitter_, randomEngine_));
            emitter_.frequencyTime -= emitter_.frequency;
        }
    }
}

void ParticleEmitter::UpdateParticle(std::list<ParticleManager::Particle>::iterator& paritcle)
{
    if (moveStart_) {
        if (IsCollision(accelerationField_.area, paritcle->transform.translate) && isFieldStart_) {
            paritcle->velocity += accelerationField_.acceleration * kDeltaTime;
        }
        paritcle->transform.translate += paritcle->velocity * kDeltaTime;
        paritcle->currentTime += kDeltaTime;
    }
}

std::list<ParticleManager::Particle> ParticleEmitter::Emit(const Emitter& emitter, std::mt19937& randomEngine)
{
    std::list<ParticleManager::Particle> particles;
    for (uint32_t count = 0; count < emitter.count; ++count) {
        particles.push_back(MakeNewParticle(randomEngine, emitter));
    }
    return particles;
}

ParticleManager::Particle ParticleEmitter::MakeNewParticle(std::mt19937& randomEngine, const Emitter& emitter)
{
    std::uniform_real_distribution<float> distPosX(emitter.size.min.x, emitter.size.max.x);
    std::uniform_real_distribution<float> distPosY(emitter.size.min.y, emitter.size.max.y);
    std::uniform_real_distribution<float> distPosZ(emitter.size.min.z, emitter.size.max.z);

    std::uniform_real_distribution<float> distVelocity(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
    ParticleManager::Particle particle{};
    particle.transform.scale = { 1.0f,1.0f,1.0f };
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    Vector3 randomTranslate = { distPosX(randomEngine),distPosY(randomEngine) ,distPosZ(randomEngine) };
    particle.transform.translate = emitter.transform.translate + randomTranslate;
    particle.velocity = { distVelocity(randomEngine),distVelocity(randomEngine) ,distVelocity(randomEngine) };
    particle.color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
    particle.lifeTime = distTime(randomEngine);
    particle.currentTime = 0.0f;
    return particle;
}

std::vector<Vector3> ParticleEmitter::CreateLineBox(AABB aabb)
{
    std::vector<Vector3> linePosition = {
        aabb.min,
        { aabb.max.x, aabb.min.y, aabb.min.z },

        aabb.min,
        { aabb.min.x, aabb.max.y, aabb.min.z },

        aabb.min,
        { aabb.min.x, aabb.min.y, aabb.max.z },

        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z },

        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.min.x, aabb.max.y, aabb.max.z },

        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.min.y, aabb.max.z },

        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.min.y, aabb.max.z },

        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.min.x, aabb.max.y, aabb.max.z },

        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z },

        { aabb.min.x, aabb.max.y, aabb.max.z },
        aabb.max,

        { aabb.max.x, aabb.min.y, aabb.max.z },
        aabb.max,

        { aabb.max.x, aabb.max.y, aabb.min.z },
        aabb.max,
    };

    return linePosition;
}


bool ParticleEmitter::IsCollision(const AABB& aabb, const Vector3& point)
{
    if (aabb.min.x < point.x && aabb.max.x > point.x &&
        aabb.min.y < point.y && aabb.max.y > point.y &&
        aabb.min.z < point.z && aabb.max.z > point.z) {
        return true;
    }
    return false;
}
