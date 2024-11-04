#include "GlobalVariables.h"



GlobalVariables* GlobalVariables::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new GlobalVariables;
    }
    return instance_;
}
