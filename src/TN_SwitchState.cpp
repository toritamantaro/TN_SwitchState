/*
MIT License

Copyright (c) 2019 ntaro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "TN_SwitchState.h"

TN_Switch::TN_Switch() : TN_Switch::TN_Switch(false) {}

TN_Switch::TN_Switch(boolean is_toggle) : is_toggle_{is_toggle}
{
    Init();
}

void TN_Switch::Init()
{
    if (is_toggle_)
    {
        tn_state_ = ToggleOff();
    }
    else
    {
        tn_state_ = Idring();
    }

}

/**
 * TnSwitchState TN_Switch::GiveSignal(boolean is_high)
 * 
 * Give a signal to proceed the state transition
 * boolesn is_high : high/low signal (high is true)
 * 
 * return : enumerated type of switch state(TnSwitchState)
**/
TnSwitchState TN_Switch::GiveSignal(boolean is_high)
{
    /* Low-pass filter to prevent chattering */
    if(millis() - receipt_time_ > chatter_time_){
        tn_state_->GiveSignal(is_high, this);
        receipt_time_ = millis();
        return tn_state_->GetStateEnum();
    }
    return TnSwitchState::kLowPassFiltering;
}

// TnSwitchState TN_Switch::State() const
// {

//     return tn_state_->GetStateEnum();
// }

/* ---------------------------------------------------------------------- */

TN_State *TN_Switch::Idring() const
{
    static IdringState state;
    return &state;
}

TN_State *TN_Switch::SinglePress() const
{
    static SinglePressState state;
    return &state;
}

TN_State *TN_Switch::SingleHold() const
{
    static SingleHoldState state;
    return &state;
}

TN_State *TN_Switch::SingleRelease() const
{
    static SingleReleaseState state;
    return &state;
}

TN_State *TN_Switch::DoubleIdring() const
{
    static DoubleIdringState state;
    return &state;
}

TN_State *TN_Switch::DoublePress() const
{
    static DoublePressState state;
    return &state;
}

TN_State *TN_Switch::DoubleHold() const
{
    static DoubleHoldState state;
    return &state;
}

TN_State *TN_Switch::DoubleRelease() const
{
    static DoubleReleaseState state;
    return &state;
}

TN_State *TN_Switch::LongPress() const
{
    static LongPressState state;
    return &state;
}

TN_State *TN_Switch::LongHold() const
{
    static LongHoldState state;
    return &state;
}

TN_State *TN_Switch::LongRelease() const
{
    static LongReleaseState state;
    return &state;
}

/* ---------------------------------------------------------------------- */

TN_State *TN_Switch::ToggleOff() const
{
    static ToggleOffState state;
    return &state;
}

TN_State *TN_Switch::ToggleRising() const
{
    static ToggleRisingState state;
    return &state;
}

TN_State *TN_Switch::ToggleOn() const
{
    static ToggleOnState state;
    return &state;
}

TN_State *TN_Switch::ToggleFalling() const
{
    static ToggleFallingState state;
    return &state;
}

/* ---------------------------------------------------------------------- */

TN_State::TN_State() {}
TN_State::~TN_State() {}

/* ---------------------------------------------------------------------- */

/* Idring(default state) */
void IdringState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->SinglePress()); /* state transition */
    }
}

/* Single press */
void SinglePressState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* start timing measurement */
        ctx->set_tn_state(ctx->SingleHold()); /* state transition */
    }
    else
    {
        ctx->set_tn_state(ctx->SingleRelease()); /* state transition */
    }
}

/* Contiuous pressing after SinglePress */
void SingleHoldState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        if (millis() - start > long_press)
        {
            ctx->set_tn_state(ctx->LongPress()); /* state transition */
        }
    }
    else
    {
        ctx->set_tn_state(ctx->SingleRelease()); /* state transition */
    }
}

/* Released after SingleHold */
void SingleReleaseState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* state transition */
    }
    else
    {
        ctx->set_start_time(millis());          /* reset timing measurement */
        ctx->set_tn_state(ctx->DoubleIdring()); /* state transition */
    }
}

/* Idring before double press */
void DoubleIdringState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t double_click = ctx->double_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* state transition */
    }
    else
    {
        if (millis() - start > double_click)
        {
            ctx->set_tn_state(ctx->Idring()); /* state transition */
        }
    }
}

/* Double press */
void DoublePressState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_start_time(millis());        /* reset timing measurement */
        ctx->set_tn_state(ctx->DoubleHold()); /* state transition */
    }
    else
    {
        ctx->set_tn_state(ctx->DoubleRelease()); /* state transition */
    }
}

/* Contiuous pressing after DoublePress */
void DoubleHoldState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        if (millis() - start > long_press)
        {
            ctx->set_tn_state(ctx->LongPress()); /* state transition */
        }
    }
    else
    {
        ctx->set_tn_state(ctx->DoubleRelease()); /* state transition */
    }
}

/* Released after DoubleHold */
void DoubleReleaseState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    uint32_t start = ctx->start_time();
    uint32_t long_press = ctx->long_press_time();

    if (is_high)
    {
        ctx->set_tn_state(ctx->DoublePress()); /* state transition */
    }
    else
    {
        ctx->set_tn_state(ctx->Idring()); /* state transition */
    }
}

/* Long pressed */
void LongPressState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->LongHold()); /* state transition */
    }
    else
    {
        ctx->set_tn_state(ctx->LongRelease()); /* state transition */
    }
}

/* Contiuous pressing after LongPress */
void LongHoldState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        // nothing
    }
    else
    {
        ctx->set_tn_state(ctx->LongRelease()); /* state transition */
    }
}

/* Released after LongHold */
void LongReleaseState::GiveSignal(boolean is_high, TN_Switch *ctx)
{

    if (is_high)
    {
        ctx->set_tn_state(ctx->SinglePress()); /* state transition */
    }
    else
    {
        ctx->set_tn_state(ctx->Idring()); /* state transition */
    }
}

/* ---------------------------------------------------------------------- */

void ToggleOffState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->ToggleRising()); /* state transition */
    }
}

void ToggleRisingState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (!is_high)
    {
        ctx->set_tn_state(ctx->ToggleOn()); /* state transition */
    }
}

void ToggleOnState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (is_high)
    {
        ctx->set_tn_state(ctx->ToggleFalling()); /* state transition */
    }
}

void ToggleFallingState::GiveSignal(boolean is_high, TN_Switch *ctx)
{
    if (!is_high)
    {
        ctx->set_tn_state(ctx->ToggleOff()); /* state transition */
    }
}
