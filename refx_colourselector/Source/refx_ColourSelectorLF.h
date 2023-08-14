#pragma once

namespace reFX
{

class ColourSelectorLF : public juce::LookAndFeel_V4
{
public:
    void drawTickBox (juce::Graphics& g, juce::Component& component,
                      float x, float y, float w, float h,
                      const bool ticked,
                      [[maybe_unused]] const bool isEnabled,
                      [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
                      [[maybe_unused]] const bool shouldDrawButtonAsDown) override;
};

}
