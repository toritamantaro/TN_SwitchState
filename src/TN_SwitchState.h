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

#ifndef TN_SWITCHSTATE_H_
#define TN_SWITCHSTATE_H_

#include <Arduino.h>

enum class TnSwitchState
{
    kIdring,        /* Idring(default state) */
    kSinglePress,   /* Single press */
    kSingleHold,    /* Contiuous pressing after SinglePress */
    kSingleRelease, /* Released after SingleHold */
    kDoubleIdring,  /* Idring  before double press */
    kDoublePress,   /* Double press */
    kDoubleHold,    /* Contiuous pressing after DoublePress */
    kDoubleRelease, /* Released after DoubleHold */
    kLongPress,     /* Long pressed */
    kLongHold,      /* Contiuous pressing after LongPress */
    kLongRelease,   /* Released after LongHold */
    kToggleOff,     /* Toggle Switch is OFF (only if 'is_toggle=true') */
    kToggleRising,  /* Toggle Switch is Rising (only if 'is_toggle=true') */
    kToggleOn,      /* Toggle Switch is ON (only if 'is_toggle=true') */
    kToggleFalling, /* Toggle Switch is Falling (only if 'is_toggle=true') */
    kLowPassFiltering /* Waiting to apply Low-pass filter */
};

class TN_State;

/**
 * Class TN_Switch
 * 
 * This class manages switch state transitions.
 * A state transition occurs by giving a signal to this class.
 * 
 * boolean is_toggle : If "is_toggle" is setted "true", this class behaves as toggle switch.
 * 
**/
class TN_Switch
{
    static const uint32_t kLongPlessTime{500};   /* Default long press threshold time in milliseconds */
    static const uint32_t kDoublePressTime{200}; /* Default double press threshold time in milliseconds */
    static const uint32_t kChatterTime{10}; /* Default chattering threshold time in milliseconds */

    void Init();

public:
    TN_Switch();
    TN_Switch(boolean is_toggle);
    ~TN_Switch() = default;
    TN_Switch(const TN_Switch &other) = delete;
    TN_Switch &operator=(const TN_Switch &other) = delete;

    TnSwitchState GiveSignal(boolean is_high); /* Give a signal to proceed the state transition */
    // TnSwitchState State() const;

    // Getter-Setter
    void set_tn_state(TN_State *next) { tn_state_ = next; }
    void set_start_time(uint32_t t) { start_time_ = t; }
    void set_long_press_time(uint32_t t) { long_press_time_ = t; };
    void set_double_press_time(uint32_t t) { double_press_time_ = t; };
    void set_chatter_time(uint32_t t) { chatter_time_ = t; };
    uint32_t start_time() const { return start_time_; }
    uint32_t long_press_time() const { return long_press_time_; }
    uint32_t double_press_time() const { return double_press_time_; }
    uint32_t chatter_time() const { return chatter_time_; }
    boolean is_toggle() const { return is_toggle_; }

    // The function to set the transition destination state
    TN_State *Idring() const;
    TN_State *SinglePress() const;
    TN_State *SingleHold() const;
    TN_State *SingleRelease() const;
    TN_State *DoubleIdring() const;
    TN_State *DoublePress() const;
    TN_State *DoubleHold() const;
    TN_State *DoubleRelease() const;
    TN_State *LongPress() const;
    TN_State *LongHold() const;
    TN_State *LongRelease() const;
    TN_State *ToggleOff() const;
    TN_State *ToggleRising() const;
    TN_State *ToggleOn() const;
    TN_State *ToggleFalling() const;

private:
    boolean is_toggle_{false};
    uint32_t long_press_time_{kLongPlessTime};
    uint32_t double_press_time_{kDoublePressTime};
    uint32_t chatter_time_{kChatterTime};
    uint32_t receipt_time_{0};
    uint32_t start_time_{0};
    TN_State *tn_state_;
};

/* ---------------------------------------------------------------------- */

class TN_State
{
    friend class TN_Switch;

public:
    TN_State();
    virtual ~TN_State();

protected:
    virtual void GiveSignal(boolean is_high, TN_Switch *ctx) = 0;
    virtual TnSwitchState GetStateEnum() = 0;
};

/* ---------------------------------------------------------------------- */

/* Idring(default state) */
class IdringState : public TN_State
{
public:
    IdringState() = default;
    ~IdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kIdring; }
};

/* Single press */
class SinglePressState : public TN_State
{
public:
    SinglePressState() = default;
    ~SinglePressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kSinglePress; }
};

/* Contiuous pressing after SinglePress */
class SingleHoldState : public TN_State
{
public:
    SingleHoldState() = default;
    ~SingleHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kSingleHold; }
};

/* Released after SingleHold */
class SingleReleaseState : public TN_State
{
public:
    SingleReleaseState() = default;
    ~SingleReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kSingleRelease; }
};

/* Idring  before double press */
class DoubleIdringState : public TN_State
{
public:
    DoubleIdringState() = default;
    ~DoubleIdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kDoubleIdring; }
};

/* Double press */
class DoublePressState : public TN_State
{
public:
    DoublePressState() = default;
    ~DoublePressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kDoublePress; }
};

/* Contiuous pressing after DoublePress */
class DoubleHoldState : public TN_State
{
public:
    DoubleHoldState() = default;
    ~DoubleHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kDoubleHold; }
};

/* Released after DoubleHold */
class DoubleReleaseState : public TN_State
{
public:
    DoubleReleaseState() = default;
    ~DoubleReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kDoubleRelease; }
};

/* Long pressed */
class LongPressState : public TN_State
{
public:
    LongPressState() = default;
    ~LongPressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kLongPress; }
};

/* Contiuous pressing after LongPress */
class LongHoldState : public TN_State
{
public:
    LongHoldState() = default;
    ~LongHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kLongHold; }
};

/* Released after LongHold */
class LongReleaseState : public TN_State
{
public:
    LongReleaseState() = default;
    ~LongReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kLongRelease; }
};

/* Toggle Switch is OFF (only if 'is_toggle=true') */
class ToggleOffState : public TN_State
{
public:
    ToggleOffState() = default;
    ~ToggleOffState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kToggleOff; }
};

/* Toggle Switch is Rising (only if 'is_toggle=true') */
class ToggleRisingState : public TN_State
{
public:
    ToggleRisingState() = default;
    ~ToggleRisingState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kToggleRising; }
};

/* Toggle Switch is ON (only if 'is_toggle=true') */
class ToggleOnState : public TN_State
{
public:
    ToggleOnState() = default;
    ~ToggleOnState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kToggleOn; }
};

/* Toggle Switch is Falling (only if 'is_toggle=true') */
class ToggleFallingState : public TN_State
{
public:
    ToggleFallingState() = default;
    ~ToggleFallingState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Switch *ctx) override;
    TnSwitchState GetStateEnum() override { return TnSwitchState::kToggleFalling; }
};

#endif // TN_SWITCHSTATE_H_
