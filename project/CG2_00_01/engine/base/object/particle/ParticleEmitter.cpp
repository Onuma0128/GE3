#include "ParticleEmitter.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

ParticleEmitter::ParticleEmitter(const std::string name)
{
    GlobalInitialize(name);

    emitter_.name = name;
    emitter_.transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
    emitter_.frequency = 0.1f;
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

void ParticleEmitter::GlobalInitialize(const std::string name)
{
    std::string globalName = name + "Emitter";

    global_->AddValue<Vector3>(globalName, "position", Vector3{});
    global_->AddValue<bool>(globalName, "lockPosition", false);
    global_->AddValue<Vector3>(globalName, "min", Vector3{ -1.0f,-1.0f,-1.0f });
    global_->AddValue<Vector3>(globalName, "max", Vector3{ 1.0f,1.0f,1.0f });
    global_->AddValue<Vector3>(globalName, "acceleration", Vector3{ 0.0f,10.0f,0.0f });
    global_->AddValue<Vector3>(globalName, "color", Vector3{ 1.0f,1.0f,1.0f });
    global_->AddValue<float>(globalName, "frequency", 0.5f);
    global_->AddValue<int>(globalName, "count", 3);
    global_->AddValue<bool>(globalName, "move", false);
    global_->AddValue<bool>(globalName, "field", false);
    global_->AddValue<bool>(globalName, "create", true);

}

void ParticleEmitter::Update()
{
    /*==================== パーティクルの範囲更新 ====================*/

    std::string globalName = emitter_.name + "Emitter";

    // 新たな数値を代入
    if (!global_->GetValue<bool>(globalName, "lockPosition")) {
        emitter_.transform.translate = global_->GetValue<Vector3>(globalName, "position");
    }
    Vector3 min = global_->GetValue<Vector3>(globalName, "min");
    Vector3 max = global_->GetValue<Vector3>(globalName, "max");
    // min,maxが最大値を超えていないかclamp
    emitter_.size.min = {
        std::clamp(min.x,-256.0f,0.0f),std::clamp(min.y,-256.0f,0.0f),std::clamp(min.z,-256.0f,0.0f),
    };
    emitter_.size.max = {
        std::clamp(max.x,0.0f,256.0f),std::clamp(max.y,0.0f,256.0f),std::clamp(max.z,0.0f,256.0f),
    };

    emitter_.frequency = global_->GetValue<float>(globalName, "frequency");
    emitter_.count = global_->GetValue<int>(globalName, "count");

    accelerationField_.area = {
        .min = emitter_.size.min + emitter_.transform.translate - Vector3{0.5f,0.5f,0.5f},
        .max = emitter_.size.max + emitter_.transform.translate + Vector3{0.5f,0.5f,0.5f}
    };
    accelerationField_.acceleration = global_->GetValue<Vector3>(globalName, "acceleration");

    moveStart_ = global_->GetValue<bool>(globalName, "move");
    isFieldStart_ = global_->GetValue<bool>(globalName, "field");

#ifdef _DEBUG
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

    /*==================== パーティクルの範囲描画 ====================*/
    PrimitiveDrawer::GetInstance()->DrawBase();
    for (auto& line : lines_) {
        line->Draw();
    }
#endif // _DEBUG
}

void ParticleEmitter::CreateParticles(ParticleManager::ParticleGroup& group)
{
    std::string globalName = emitter_.name + "Emitter";
    if (moveStart_ && global_->GetValue<bool>(globalName, "create")) {
        emitter_.frequencyTime += kDeltaTime;
        if (emitter_.frequency <= emitter_.frequencyTime) {
            std::mt19937 randomEngine_(seedGenerator_());
            group.particles.splice(group.particles.end(), Emit(emitter_, randomEngine_));
            emitter_.frequencyTime -= emitter_.frequency;
        }
    }
}

void ParticleEmitter::UpdateParticle(std::list<ParticleManager::Particle>::iterator& particle)
{
    if (moveStart_) {
        if (IsCollision(accelerationField_.area, particle->transform.translate) && isFieldStart_) {
            particle->velocity += accelerationField_.acceleration * kDeltaTime;
        }
        particle->transform.translate += particle->velocity * kDeltaTime;
        particle->currentTime += kDeltaTime;

        std::string globalName = emitter_.name + "Emitter";
        particle->color.x = global_->GetValue<Vector3>(globalName, "color").x;
        particle->color.y = global_->GetValue<Vector3>(globalName, "color").y;
        particle->color.z = global_->GetValue<Vector3>(globalName, "color").z;
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
    //std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
    ParticleManager::Particle particle{};
    particle.transform.scale = { 1.0f,1.0f,1.0f };
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    Vector3 randomTranslate = { distPosX(randomEngine),distPosY(randomEngine) ,distPosZ(randomEngine) };
    particle.transform.translate = emitter.transform.translate + randomTranslate;
    particle.velocity = { distVelocity(randomEngine),distVelocity(randomEngine) ,distVelocity(randomEngine) };
    particle.color = { 1.0f,1.0f,1.0f,1.0f };
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
