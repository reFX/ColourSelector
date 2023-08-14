namespace reFX
{

void ColourSelectorLF::drawTickBox (juce::Graphics& g, juce::Component& component,
                                    float x, float y, float w, float h,
                                    const bool ticked,
                                    const bool isEnabled,
                                    const bool shouldDrawButtonAsHighlighted,
                                    const bool shouldDrawButtonAsDown)
{
    juce::Rectangle<float> tickBounds (x, y, w, h);

    auto isRadioButton = [&]
    {
        if (auto b = dynamic_cast<juce::Button*> (&component))
            return b->getRadioGroupId() != 0;
        return false;
    };

    if (isRadioButton())
    {
        g.setColour (component.findColour (juce::ToggleButton::tickDisabledColourId));
        g.drawEllipse (tickBounds.reduced (2.0f), 1.0f);

        if (ticked)
        {
            g.setColour (component.findColour (juce::ToggleButton::tickColourId));
            g.fillEllipse (tickBounds.reduced (5.0f));
        }
    }
    else
    {
        juce::LookAndFeel_V4::drawTickBox (g, component, x, y, w, h, ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    }
}

}
