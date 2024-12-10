#include "AudioManager.h"

AudioManager* AudioManager::instance_ = nullptr;

AudioManager* AudioManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new AudioManager;
    }
    return instance_;
}
