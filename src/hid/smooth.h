#pragma once
#include <stdint.h>
#include <math.h>
#include <algorithm>

namespace daisy
{
// Copyright 2025 Infrasonic Audio LLC, MIT License
// Shoehorned in here for Trinity
// based on https://cytomic.com/files/dsp/DynamicSmoothing.pdf
class DynamicSmoother
{
  public:
    DynamicSmoother()  = default;
    ~DynamicSmoother() = default;

    void Init(float sr, float time_s, float sensitivity = 0.5f)
    {
        sr_ = sr;
        l0_ = l1_ = 0.0f;
        target_   = 0.0f;
        out_      = 0.0f;
        SetTime(time_s, sensitivity);
    }

    void SetTime(float time_s, float sensitivity = 0.5f)
    {
        if(time_s <= 0.0f)
            return;
        float wc = 1.0f / (time_s * sr_);
        float gc = tanf(M_PI * wc);
        g_       = 2.0f * gc / (1.0f + gc);
        s_       = sensitivity * 4.0f;
    }

    void Set(float target) { target_ = target; }

    float Process()
    {
        float l0 = l0_;
        float l1 = l1_;
        float b  = l0 - l1;
        float g  = std::min(g_ + s_ * fabsf(b), 1.0f);
        l0_      = l0 + g * (target_ - l0);
        l1_      = l1 + g * (l0_ - l1);
        out_     = l1_;
        return out_;
    }

    float Value() const { return out_; }

  private:
    float sr_;
    float g_;
    float s_;
    float target_;
    float l0_, l1_;
    float out_;
};

} // namespace daisy
