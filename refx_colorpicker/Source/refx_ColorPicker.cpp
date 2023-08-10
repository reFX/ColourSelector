namespace reFX
{

struct ColourComponentSlider  : public juce::Slider
{
    ColourComponentSlider (const juce::String& name, int max)  : juce::Slider (name)
    {
        setRange (0.0, double (max), 0.0);
    }

    juce::String getTextFromValue (double value) override
    {
        return juce::String ((int) value);
    }

    double getValueFromText (const juce::String& text) override
    {
        return (double) text.getIntValue();
    }
};

//==============================================================================
class ColorPicker::ColourSpaceView  : public Component
{
public:
    ColourSpaceView (ColorPicker& cs, DeepColor& dc, int edgeSize)
        : owner (cs), colour (dc), edge (edgeSize)
    {
        addAndMakeVisible (marker);
        setMouseCursor (juce::MouseCursor::CrosshairCursor);
    }

    void paint (juce::Graphics& g) override
    {
        if (colours.isNull())
        {
            auto width = getWidth() / 2;
            auto height = getHeight() / 2;
            colours = juce::Image (juce::Image::RGB, width, height, false);

            juce::Image::BitmapData pixels (colours, juce::Image::BitmapData::writeOnly);

            for (int y = 0; y < height; ++y)
            {
                auto val = 1.0f - (float) y / (float) height;

                for (int x = 0; x < width; ++x)
                {
                    auto sat = (float) x / (float) width;
                    pixels.setPixelColour (x, y, juce::Colour (colour.getHue(), sat, val, 1.0f));
                }
            }
        }

        g.setOpacity (1.0f);
        g.drawImageTransformed (colours,
                                juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
                                    .getTransformToFit (colours.getBounds().toFloat(),
                                                        getLocalBounds().reduced (edge).toFloat()),
                                false);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        auto sat =        (float) (e.x - edge) / (float) (getWidth()  - edge * 2);
        auto val = 1.0f - (float) (e.y - edge) / (float) (getHeight() - edge * 2);

        owner.setSV (sat, val);
    }

    void updateIfNeeded()
    {
        if (! juce::approximatelyEqual (lastHue, colour.getHue()))
        {
            lastHue = colour.getHue();
            colours = {};
            repaint();
        }

        updateMarker();
    }

    void resized() override
    {
        colours = {};
        updateMarker();
    }

private:
    ColorPicker& owner;
    DeepColor& colour;
    float lastHue = 0;
    const int edge;
    juce::Image colours;

    struct ColourSpaceMarker  : public Component
    {
        ColourSpaceMarker()
        {
            setInterceptsMouseClicks (false, false);
        }

        void paint (juce::Graphics& g) override
        {
            g.setColour (juce::Colour::greyLevel (0.1f));
            g.drawEllipse (1.0f, 1.0f, (float) getWidth() - 2.0f, (float) getHeight() - 2.0f, 1.0f);
            g.setColour (juce::Colour::greyLevel (0.9f));
            g.drawEllipse (2.0f, 2.0f, (float) getWidth() - 4.0f, (float) getHeight() - 4.0f, 1.0f);
        }
    };

    ColourSpaceMarker marker;

    void updateMarker()
    {
        auto markerSize = juce::jmax (14, edge * 2);
        auto area = getLocalBounds().reduced (edge);

        auto hsb = colour.getHSB ();

        marker.setBounds (juce::Rectangle<int> (markerSize, markerSize).withCentre (area.getRelativePoint (hsb.s, 1.0f - hsb.b)));
    }

    JUCE_DECLARE_NON_COPYABLE (ColourSpaceView)
};

//==============================================================================
class ColorPicker::HueSelectorComp  : public Component
{
public:
    HueSelectorComp (ColorPicker& cs, DeepColor& dc, int edgeSize)
        : owner (cs), colour (dc), edge (edgeSize)
    {
        addAndMakeVisible (marker);
    }

    void paint (juce::Graphics& g) override
    {
        juce::ColourGradient cg;
        cg.isRadial = false;
        cg.point1.setXY (0.0f, (float) edge);
        cg.point2.setXY (0.0f, (float) getHeight());

        for (float i = 0.0f; i <= 1.0f; i += 0.02f)
            cg.addColour (i, juce::Colour (i, 1.0f, 1.0f, 1.0f));

        g.setGradientFill (cg);
        g.fillRect (getLocalBounds().reduced (edge));
    }

    void resized() override
    {
        auto markerSize = juce::jmax (14, edge * 2);
        auto area = getLocalBounds().reduced (edge);

        auto hsb = colour.getHSB();

        marker.setBounds (juce::Rectangle<int> (getWidth(), markerSize).withCentre (area.getRelativePoint (0.5f, hsb.h)));
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        owner.setHue ((float) (e.y - edge) / (float) (getHeight() - edge * 2));
    }

    void updateIfNeeded()
    {
        resized();
    }

private:
    ColorPicker& owner;
    DeepColor& colour;
    const int edge;

    struct HueSelectorMarker  : public Component
    {
        HueSelectorMarker()
        {
            setInterceptsMouseClicks (false, false);
        }

        void paint (juce::Graphics& g) override
        {
            auto cw = (float) getWidth();
            auto ch = (float) getHeight();

            juce::Path p;
            p.addTriangle (1.0f, 1.0f,
                           cw * 0.3f, ch * 0.5f,
                           1.0f, ch - 1.0f);

            p.addTriangle (cw - 1.0f, 1.0f,
                           cw * 0.7f, ch * 0.5f,
                           cw - 1.0f, ch - 1.0f);

            g.setColour (juce::Colours::white.withAlpha (0.75f));
            g.fillPath (p);

            g.setColour (juce::Colours::black.withAlpha (0.75f));
            g.strokePath (p, juce::PathStrokeType (1.2f));
        }
    };

    HueSelectorMarker marker;

    JUCE_DECLARE_NON_COPYABLE (HueSelectorComp)
};

//==============================================================================
class ColorPicker::SwatchComponent   : public Component
{
public:
    SwatchComponent (ColorPicker& cs, int itemIndex)
        : owner (cs), index (itemIndex)
    {
    }

    void paint (juce::Graphics& g) override
    {
        auto col = owner.getSwatchColour (index);

        g.fillCheckerBoard (getLocalBounds().toFloat(), 6.0f, 6.0f,
                            juce::Colour (0xffdddddd).overlaidWith (col),
                            juce::Colour (0xffffffff).overlaidWith (col));
    }

    void mouseDown (const juce::MouseEvent&) override
    {
        juce::PopupMenu m;
        m.addItem (1, TRANS("Use this swatch as the current colour"));
        m.addSeparator();
        m.addItem (2, TRANS("Set this swatch to the current colour"));

        m.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (this),
                         juce::ModalCallbackFunction::forComponent (menuStaticCallback, this));
    }

private:
    ColorPicker& owner;
    const int index;

    static void menuStaticCallback (int result, SwatchComponent* comp)
    {
        if (comp != nullptr)
        {
            if (result == 1)  comp->setColourFromSwatch();
            if (result == 2)  comp->setSwatchFromColour();
        }
    }

    void setColourFromSwatch()
    {
        owner.setCurrentColour (owner.getSwatchColour (index));
    }

    void setSwatchFromColour()
    {
        if (owner.getSwatchColour (index) != owner.getCurrentColour())
        {
            owner.setSwatchColour (index, owner.getCurrentColour());
            repaint();
        }
    }

    JUCE_DECLARE_NON_COPYABLE (SwatchComponent)
};

//==============================================================================
class ColorPicker::ColourPreviewComp  : public Component
{
public:
    ColourPreviewComp (ColorPicker& cs, bool isEditable)
        : owner (cs)
    {
        colourLabel.setFont (labelFont);
        colourLabel.setJustificationType (juce::Justification::centred);

        if (isEditable)
        {
            colourLabel.setEditable (true);

            colourLabel.onEditorShow = [this]
            {
                if (auto* ed = colourLabel.getCurrentTextEditor())
                    ed->setInputRestrictions ((owner.flags & showAlphaChannel) ? 8 : 6, "1234567890ABCDEFabcdef");
            };

            colourLabel.onEditorHide = [this]
            {
                updateColourIfNecessary (colourLabel.getText());
            };
        }

        addAndMakeVisible (colourLabel);
    }

    void updateIfNeeded()
    {
        auto newColour = owner.getCurrentColour();

        if (currentColour != newColour)
        {
            currentColour = newColour;
            auto textColour = (juce::Colours::white.overlaidWith (currentColour).contrasting());

            colourLabel.setColour (juce::Label::textColourId,            textColour);
            colourLabel.setColour (juce::Label::textWhenEditingColourId, textColour);
            colourLabel.setText (currentColour.toDisplayString ((owner.flags & showAlphaChannel) != 0), juce::dontSendNotification);

            labelWidth = labelFont.getStringWidth (colourLabel.getText());

            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        g.fillCheckerBoard (getLocalBounds().toFloat(), 10.0f, 10.0f,
                            juce::Colour (0xffdddddd).overlaidWith (currentColour),
                            juce::Colour (0xffffffff).overlaidWith (currentColour));
    }

    void resized() override
    {
        colourLabel.centreWithSize (labelWidth + 10, (int) labelFont.getHeight() + 10);
    }

private:
    void updateColourIfNecessary (const juce::String& newColourString)
    {
        auto newColour = juce::Colour::fromString (newColourString);

        if (newColour != currentColour)
            owner.setCurrentColour (newColour);
    }

    ColorPicker& owner;

    juce::Colour currentColour;
    juce::Font labelFont { 14.0f, juce::Font::bold };
    int labelWidth = 0;
    juce::Label colourLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPreviewComp)
};

//==============================================================================
ColorPicker::ColorPicker (int sectionsToShow, int edge, int gapAroundColourSpaceComponent)
    : colour (juce::Colours::white),
      flags (sectionsToShow),
      edgeGap (edge)
{
    // not much point having a selector with no components in it!
    jassert ((flags & (showColourAtTop | showSliders | showColourspace)) != 0);

    if ((flags & showColourAtTop) != 0)
    {
        previewComponent.reset (new ColourPreviewComp (*this, (flags & editableColour) != 0));
        addAndMakeVisible (previewComponent.get());
    }

    if ((flags & showSliders) != 0)
    {
        sliders[0].reset (new ColourComponentSlider (TRANS ("hue"), 360));
        sliders[1].reset (new ColourComponentSlider (TRANS ("saturation"), 100));
        sliders[2].reset (new ColourComponentSlider (TRANS ("brightness"), 100));

        sliders[3].reset (new ColourComponentSlider (TRANS ("red"), 255));
        sliders[4].reset (new ColourComponentSlider (TRANS ("green"), 255));
        sliders[5].reset (new ColourComponentSlider (TRANS ("blue"), 255));
        sliders[6].reset (new ColourComponentSlider (TRANS ("alpha"), 255));

        for ( auto& slider : sliders )
            addAndMakeVisible (slider.get());

        sliders[6]->setVisible ((flags & showAlphaChannel) != 0);

        for (auto& slider : sliders)
            slider->onValueChange = [this, p = slider.get()] { changeColour (p); };
    }

    if ((flags & showColourspace) != 0)
    {
        colourSpace.reset (new ColourSpaceView (*this, colour, gapAroundColourSpaceComponent));
        hueSelector.reset (new HueSelectorComp (*this, colour, gapAroundColourSpaceComponent));

        addAndMakeVisible (colourSpace.get());
        addAndMakeVisible (hueSelector.get());
    }

    update (juce::dontSendNotification);
}

ColorPicker::~ColorPicker()
{
    dispatchPendingMessages();
    swatchComponents.clear();
}

//==============================================================================
juce::Colour ColorPicker::getCurrentColour() const
{
    return ((flags & showAlphaChannel) != 0) ? colour.getColour() : colour.getColour().withAlpha ((juce::uint8) 0xff);
}

void ColorPicker::setCurrentColour (juce::Colour c, juce::NotificationType notification)
{
    if (c != colour)
    {
        colour = ((flags & showAlphaChannel) != 0) ? c : c.withAlpha ((juce::uint8) 0xff);
        update (notification);
    }
}

void ColorPicker::setCurrentColour (DeepColor c, juce::NotificationType notification)
{
    if (c != colour)
    {
        colour = ((flags & showAlphaChannel) != 0) ? c : c.withAlpha (1.0f);
        update (notification);
    }
}

void ColorPicker::setHue (float newH)
{
    auto hsb = colour.getHSB();

    hsb.h = juce::jlimit (0.0f, 1.0f, newH);

    colour = DeepColor::fromHSB (hsb.h, hsb.s, hsb.b, colour.getAlpha());
    update (juce::sendNotification);
}

void ColorPicker::setSV (float newS, float newB)
{
    auto hsb = colour.getHSB();

    hsb.s = juce::jlimit (0.0f, 1.0f, newS);
    hsb.b = juce::jlimit (0.0f, 1.0f, newB);

    colour = DeepColor::fromHSB (hsb.h, hsb.s, hsb.b, colour.getAlpha());
    update (juce::sendNotification);
}

//==============================================================================
void ColorPicker::update (juce::NotificationType notification)
{
    if (sliders[0] != nullptr)
    {
        sliders[0]->setValue (colour.getHue() * 360,        juce::dontSendNotification);
        sliders[1]->setValue (colour.getSaturation() * 100, juce::dontSendNotification);
        sliders[2]->setValue (colour.getBrightness() * 100, juce::dontSendNotification);

        sliders[3]->setValue (colour.getRed() * 255,   juce::dontSendNotification);
        sliders[4]->setValue (colour.getGreen() * 255, juce::dontSendNotification);
        sliders[5]->setValue (colour.getBlue() * 255,  juce::dontSendNotification);
        sliders[6]->setValue (colour.getAlpha() * 255, juce::dontSendNotification);
    }

    DBG(juce::String::formatted ("HSB: %2.2f %2.2f %2.2f", colour.getHue(), colour.getSaturation(), colour.getBrightness()));
    DBG(juce::String::formatted ("RGB: %2.2f %2.2f %2.2f", colour.getRed(), colour.getBlue(), colour.getGreen()));

    if (colourSpace != nullptr)
    {
        colourSpace->updateIfNeeded();
        hueSelector->updateIfNeeded();
    }

    if (previewComponent != nullptr)
        previewComponent->updateIfNeeded();

    if (notification != juce::dontSendNotification)
        sendChangeMessage();

    if (notification == juce::sendNotificationSync)
        dispatchPendingMessages();
}

//==============================================================================
void ColorPicker::paint (juce::Graphics& g)
{
    g.fillAll (findColour (backgroundColourId));

    if ((flags & showSliders) != 0)
    {
        g.setColour (findColour (labelTextColourId));
        g.setFont (11.0f);

        for (auto& slider : sliders)
        {
            if (slider->isVisible())
                g.drawText (slider->getName() + ":",
                            0, slider->getY(),
                            slider->getX() - 8, slider->getHeight(),
                            juce::Justification::centredRight, false);
        }
    }
}

void ColorPicker::resized()
{
    const int swatchesPerRow = 8;
    const int swatchHeight = 22;

    const int numSliders = ((flags & showAlphaChannel) != 0) ? 7 : 6;
    const int numSwatches = getNumSwatches();

    const int swatchSpace = numSwatches > 0 ? edgeGap + swatchHeight * ((numSwatches + 7) / swatchesPerRow) : 0;
    const int sliderSpace = ((flags & showSliders) != 0)  ? juce::jmin (22 * numSliders + edgeGap, proportionOfHeight (0.3f)) : 0;
    const int topSpace = ((flags & showColourAtTop) != 0) ? juce::jmin (30 + edgeGap * 2, proportionOfHeight (0.2f)) : edgeGap;

    if (previewComponent != nullptr)
        previewComponent->setBounds (edgeGap, edgeGap, getWidth() - edgeGap * 2, topSpace - edgeGap * 2);

    int y = topSpace;

    if ((flags & showColourspace) != 0)
    {
        const int hueWidth = juce::jmin (50, proportionOfWidth (0.15f));

        colourSpace->setBounds (edgeGap, y,
                                getWidth() - hueWidth - edgeGap - 4,
                                getHeight() - topSpace - sliderSpace - swatchSpace - edgeGap);

        hueSelector->setBounds (colourSpace->getRight() + 4, y,
                                getWidth() - edgeGap - (colourSpace->getRight() + 4),
                                colourSpace->getHeight());

        y = getHeight() - sliderSpace - swatchSpace - edgeGap;
    }

    if ((flags & showSliders) != 0)
    {
        auto sliderHeight = juce::jmax (4, sliderSpace / numSliders);

        for (int i = 0; i < numSliders; ++i)
        {
            sliders[i]->setBounds (proportionOfWidth (0.2f), y,
                                   proportionOfWidth (0.72f), sliderHeight - 2);

            y += sliderHeight;
        }
    }

    if (numSwatches > 0)
    {
        const int startX = 8;
        const int xGap = 4;
        const int yGap = 4;
        const int swatchWidth = (getWidth() - startX * 2) / swatchesPerRow;
        y += edgeGap;

        if (swatchComponents.size() != numSwatches)
        {
            swatchComponents.clear();

            for (int i = 0; i < numSwatches; ++i)
            {
                auto* sc = new SwatchComponent (*this, i);
                swatchComponents.add (sc);
                addAndMakeVisible (sc);
            }
        }

        int x = startX;

        for (int i = 0; i < swatchComponents.size(); ++i)
        {
            auto* sc = swatchComponents.getUnchecked(i);

            sc->setBounds (x + xGap / 2,
                           y + yGap / 2,
                           swatchWidth - xGap,
                           swatchHeight - yGap);

            if (((i + 1) % swatchesPerRow) == 0)
            {
                x = startX;
                y += swatchHeight;
            }
            else
            {
                x += swatchWidth;
            }
        }
    }
}

void ColorPicker::changeColour ( juce::Slider* slider )
{
    if (sliders[0] == nullptr)
        return;

    if (sliders[0].get() == slider || sliders[1].get() == slider || sliders[2].get() == slider)
    {
        auto col = DeepColor::fromHSB (float (sliders[0]->getValue() / 360.0),
                                       float (sliders[1]->getValue() / 100.0),
                                       float (sliders[2]->getValue() / 100.0),
                                       float (sliders[6]->getValue() / 255.0));

        setCurrentColour (col);
    }
    else
    {
        auto col = DeepColor::fromRGBA (float (sliders[3]->getValue() / 255.0),
                                        float (sliders[4]->getValue() / 255.0),
                                        float (sliders[5]->getValue() / 255.0),
                                        float (sliders[6]->getValue() / 255.0));

        setCurrentColour (col);
    }
}

//==============================================================================
int ColorPicker::getNumSwatches() const
{
    return 0;
}

juce::Colour ColorPicker::getSwatchColour (int) const
{
    jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
    return juce::Colours::black;
}

void ColorPicker::setSwatchColour (int, const juce::Colour&)
{
    jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
}

} // namespace juce
