namespace reFX
{

void ColourSelectorLF::drawTickBox (juce::Graphics& g, juce::Component& component,
                                    float x, float y, float w, float h,
                                    const bool ticked,
                                    [[maybe_unused]] const bool isEnabled,
                                    [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
                                    [[maybe_unused]] const bool shouldDrawButtonAsDown)
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
        g.setColour (component.findColour (juce::ToggleButton::tickDisabledColourId));
        g.drawRoundedRectangle (tickBounds, 4.0f, 1.0f);

        if (ticked)
        {
            g.setColour (component.findColour (juce::ToggleButton::tickColourId));
            auto tick = getTickShape (0.75f);
            g.fillPath (tick, tick.getTransformToScaleToFit (tickBounds.reduced (4, 5).toFloat(), false));
        }
    }
}

}
