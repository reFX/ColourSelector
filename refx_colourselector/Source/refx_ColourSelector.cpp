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
class ColourSelector::Parameter2D : public Component
{
public:
    Parameter2D (ColourSelector& cs, DeepColour& dc, int edgeSize)
        : owner (cs), colour (dc), edge (edgeSize)
    {
        addAndMakeVisible (marker);
        setMouseCursor (juce::MouseCursor::CrosshairCursor);
    }

    void setParameters (Params x_, Params y_)
    {
        xParam = x_;
        yParam = y_;

        updateIfNeeded();
    }

    void paint (juce::Graphics& g) override
    {
        if (colours.isNull())
            updateImage();

        g.setOpacity (1.0f);
        g.drawImageTransformed (colours,
                                juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
                                    .getTransformToFit (colours.getBounds().toFloat(),
                                                        getLocalBounds().reduced (edge).toFloat()),
                                false);
    }

    void updateImage()
    {
        auto width = getWidth() / 2;
        auto height = getHeight() / 2;
        colours = juce::Image (juce::Image::RGB, width, height, false);

        juce::Image::BitmapData pixels (colours, juce::Image::BitmapData::writeOnly);

        for (int y = 0; y < height; ++y)
        {
            auto yVal = 1.0f - (float) y / (float) height;

            for (int x = 0; x < width; ++x)
            {
                auto xVal = (float) x / (float) width;

                auto c = owner.colour;

                auto set = [&] (Params param, float val)
                {
                    if (param == Params::hue)
                    {
                        auto hsb = c.getHSB();
                        hsb.h = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (hsb);
                    }
                    else if (param == Params::sat)
                    {
                        auto hsb = c.getHSB();
                        hsb.s = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (hsb);
                    }
                    else if (param == Params::bri)
                    {
                        auto hsb = c.getHSB();
                        hsb.b = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (hsb);
                    }
                    else if (param == Params::red)
                    {
                        auto rgb = c.getRGB();
                        rgb.r = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (rgb);
                    }
                    else if (param == Params::blue)
                    {
                        auto rgb = c.getRGB();
                        rgb.b = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (rgb);
                    }
                    else if (param == Params::green)
                    {
                        auto rgb = c.getRGB();
                        rgb.g = juce::jlimit (0.0f, 1.0f, val);
                        c = DeepColour (rgb);
                    }
                    else
                    {
                        jassertfalse;
                    }
                };

                set (xParam, xVal);
                set (yParam, yVal);

                pixels.setPixelColour (x, y, c.getColour());
            }
        }
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        auto xVal =        (float) (e.x - edge) / (float) (getWidth()  - edge * 2);
        auto yVal = 1.0f - (float) (e.y - edge) / (float) (getHeight() - edge * 2);

        auto set = [&] (Params param, float val)
        {
            if (param == Params::hue)
            {
                auto hsb = colour.getHSB();
                hsb.h = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
            }
            else if (param == Params::sat)
            {
                auto hsb = colour.getHSB();
                hsb.s = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
            }
            else if (param == Params::bri)
            {
                auto hsb = colour.getHSB();
                hsb.b = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
            }
            else if (param == Params::red)
            {
                auto rgb = colour.getRGB();
                rgb.r = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
            }
            else if (param == Params::blue)
            {
                auto rgb = colour.getRGB();
                rgb.b = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
            }
            else if (param == Params::green)
            {
                auto rgb = colour.getRGB();
                rgb.g = juce::jlimit (0.0f, 1.0f, val);
                owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
            }
            else
            {
                jassertfalse;
            }
        };

        set (xParam, xVal);
        set (yParam, yVal);
    }

    void updateIfNeeded()
    {
        colours = {};
        repaint();
        updateMarker();
    }

    void resized() override
    {
        colours = {};
        updateMarker();
    }

private:
    ColourSelector& owner;
    DeepColour& colour;
    const int edge;
    juce::Image colours;
    Params xParam = Params::hue;
    Params yParam = Params::sat;

    struct Parameter2DMarker  : public Component
    {
        Parameter2DMarker()
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

    Parameter2DMarker marker;

    void updateMarker()
    {
        auto markerSize = juce::jmax (14, edge * 2);
        auto area = getLocalBounds().reduced (edge);

        auto get = [&] (Params param)
        {
            if (param == Params::hue)
                return colour.getHSB().h;
            else if (param == Params::sat)
                return colour.getHSB().s;
            else if (param == Params::bri)
                return colour.getHSB().b;
            else if (param == Params::red)
                return colour.getRGB().r;
            else if (param == Params::blue)
                return colour.getRGB().b;
            else if (param == Params::green)
                return colour.getRGB().g;
            else
                jassertfalse;
            return 0.0f;
        };

        marker.setBounds (juce::Rectangle<int> (markerSize, markerSize).withCentre (area.getRelativePoint (get (xParam), 1.0f - get (yParam))));
    }

    JUCE_DECLARE_NON_COPYABLE (Parameter2D)
};

//==============================================================================
class ColourSelector::Parameter1D  : public Component
{
public:
    Parameter1D (ColourSelector& cs, DeepColour& dc, int edgeSize)
        : owner (cs), colour (dc), edge (edgeSize)
    {
        addAndMakeVisible (marker);
    }

    void setParameter (Params p)
    {
        param = p;

        updateIfNeeded();
    }

    void paint (juce::Graphics& g) override
    {
        juce::ColourGradient cg;
        cg.isRadial = false;
        cg.point1.setXY (0.0f, (float) edge);
        cg.point2.setXY (0.0f, (float) getHeight());

        auto c = owner.colour;
        for (float i = 0.0f; i <= 1.0f; i += 0.02f)
        {
            if (param == Params::hue)
            {
                HSB hsb;
                hsb.h = 1.0f - i;
                hsb.s = 1.0f;
                hsb.b = 1.0f;
                cg.addColour (i, DeepColour (hsb).getColour());
            }
            else if (param == Params::sat)
            {
                auto hsb = c.getHSB();
                hsb.s = 1.0f - i;
                cg.addColour (i, DeepColour (hsb).getColour());
            }
            else if (param == Params::bri)
            {
                auto hsb = c.getHSB();
                hsb.b = 1.0f - i;
                cg.addColour (i, DeepColour (hsb).getColour());
            }
            else if (param == Params::red)
            {
                auto rgb = c.getRGB();
                rgb.r = 1.0f - i;
                cg.addColour (i, DeepColour (rgb).getColour());
            }
            else if (param == Params::blue)
            {
                auto rgb = c.getRGB();
                rgb.b = 1.0f - i;
                cg.addColour (i, DeepColour (rgb).getColour());
            }
            else if (param == Params::green)
            {
                auto rgb = c.getRGB();
                rgb.g = 1.0f - i;
                cg.addColour (i, DeepColour (rgb).getColour());
            }
        }

        g.setGradientFill (cg);
        g.fillRect (getLocalBounds().reduced (edge));
    }

    void resized() override
    {
        auto markerSize = juce::jmax (14, edge * 2);
        auto area = getLocalBounds().reduced (edge);

        auto get = [&] ()
        {
            if (param == Params::hue)
                return colour.getHSB().h;
            else if (param == Params::sat)
                return colour.getHSB().s;
            else if (param == Params::bri)
                return colour.getHSB().b;
            else if (param == Params::red)
                return colour.getRGB().r;
            else if (param == Params::blue)
                return colour.getRGB().b;
            else if (param == Params::green)
                return colour.getRGB().g;
            else
                jassertfalse;
            return 0.0f;
        };

        marker.setBounds (juce::Rectangle<int> (getWidth(), markerSize).withCentre (area.getRelativePoint (0.5f, 1.0f - get())));
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        auto val = 1.0f - (float) (e.y - edge) / (float) (getHeight() - edge * 2);

        if (param == Params::hue)
        {
            auto hsb = colour.getHSB();
            hsb.h = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
        }
        else if (param == Params::sat)
        {
            auto hsb = colour.getHSB();
            hsb.s = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
        }
        else if (param == Params::bri)
        {
            auto hsb = colour.getHSB();
            hsb.b = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromHSB (hsb.h, hsb.s, hsb.b, 1.0f));
        }
        else if (param == Params::red)
        {
            auto rgb = colour.getRGB();
            rgb.r = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
        }
        else if (param == Params::blue)
        {
            auto rgb = colour.getRGB();
            rgb.b = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
        }
        else if (param == Params::green)
        {
            auto rgb = colour.getRGB();
            rgb.g = juce::jlimit (0.0f, 1.0f, val);
            owner.set (DeepColour::fromRGBA (rgb.r, rgb.g, rgb.b, 1.0f));
        }
        else
        {
            jassertfalse;
        }
    }

    void updateIfNeeded()
    {
        repaint();
        resized();
    }

private:
    ColourSelector& owner;
    DeepColour& colour;
    const int edge;

    struct Parameter1DMarker  : public Component
    {
        Parameter1DMarker()
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

    Parameter1DMarker marker;
    Params param;

    JUCE_DECLARE_NON_COPYABLE (Parameter1D)
};

//==============================================================================
class ColourSelector::SwatchComponent   : public Component
{
public:
    SwatchComponent (ColourSelector& cs, int itemIndex)
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
    ColourSelector& owner;
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
class ColourSelector::ColourPreviewComp  : public Component
{
public:
    ColourPreviewComp (ColourSelector& cs, bool isEditable)
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

    ColourSelector& owner;

    juce::Colour currentColour;
    juce::Font labelFont { 14.0f, juce::Font::bold };
    int labelWidth = 0;
    juce::Label colourLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPreviewComp)
};

//==============================================================================
ColourSelector::ColourSelector (int sectionsToShow, int edge, int gapAroundColourSpaceComponent)
    : colour (juce::Colours::white),
      flags (sectionsToShow),
      edgeGap (edge)
{
    setLookAndFeel (&lf);

    // not much point having a selector with no components in it!
    jassert ((flags & (showColourAtTop | showSliders | showColourspace)) != 0);

    if ((flags & showColourAtTop) != 0)
    {
        previewComponent.reset (new ColourPreviewComp (*this, (flags & editableColour) != 0));
        addAndMakeVisible (previewComponent.get());
    }

    for (auto& t : toggles)
    {
        t.reset (new juce::ToggleButton());
        t->setRadioGroupId (1);
        t->onClick = [this] { updateParameters(); };
        addAndMakeVisible (*t);
    }

    toggles[0]->setToggleState (true, juce::dontSendNotification);

    if ((flags & showSliders) != 0)
    {
        sliders[0].reset (new ColourComponentSlider (TRANS ("H"), 360));
        sliders[1].reset (new ColourComponentSlider (TRANS ("S"), 100));
        sliders[2].reset (new ColourComponentSlider (TRANS ("B"), 100));

        sliders[3].reset (new ColourComponentSlider (TRANS ("R"), 255));
        sliders[4].reset (new ColourComponentSlider (TRANS ("G"), 255));
        sliders[5].reset (new ColourComponentSlider (TRANS ("B"), 255));
        sliders[6].reset (new ColourComponentSlider (TRANS ("A"), 255));

        for ( auto& slider : sliders )
            addAndMakeVisible (slider.get());

        sliders[6]->setVisible ((flags & showAlphaChannel) != 0);

        for (auto& slider : sliders)
            slider->onValueChange = [this, p = slider.get()] { changeColour (p); };
    }

    if ((flags & showColourspace) != 0)
    {
        parameter2D.reset (new Parameter2D (*this, colour, gapAroundColourSpaceComponent));
        parameter1D.reset (new Parameter1D (*this, colour, gapAroundColourSpaceComponent));

        addAndMakeVisible (parameter2D.get());
        addAndMakeVisible (parameter1D.get());
    }

    update (juce::dontSendNotification);
    updateParameters();
}

ColourSelector::~ColourSelector()
{
    setLookAndFeel (nullptr);
    dispatchPendingMessages();
    swatchComponents.clear();
}

//==============================================================================
juce::Colour ColourSelector::getCurrentColour() const
{
    return ((flags & showAlphaChannel) != 0) ? colour.getColour() : colour.getColour().withAlpha ((juce::uint8) 0xff);
}

void ColourSelector::setCurrentColour (juce::Colour c, juce::NotificationType notification)
{
    if (DeepColour (c) != colour)
    {
        colour = ((flags & showAlphaChannel) != 0) ? c : c.withAlpha ((juce::uint8) 0xff);
        update (notification);
    }
}

void ColourSelector::setCurrentColour (DeepColour c, juce::NotificationType notification)
{
    if (c != colour)
    {
        colour = ((flags & showAlphaChannel) != 0) ? c : c.withAlpha (1.0f);
        update (notification);
    }
}

void ColourSelector::set (const DeepColour& newColour)
{
    colour = newColour.withAlpha (colour.getAlpha());
    update (juce::sendNotification);
}

//==============================================================================
void ColourSelector::update (juce::NotificationType notification)
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

    if (parameter2D != nullptr)
    {
        parameter2D->updateIfNeeded();
        parameter1D->updateIfNeeded();
    }

    if (previewComponent != nullptr)
        previewComponent->updateIfNeeded();

    if (notification != juce::dontSendNotification)
        sendChangeMessage();

    if (notification == juce::sendNotificationSync)
        dispatchPendingMessages();
}

//==============================================================================
void ColourSelector::paint (juce::Graphics& g)
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

void ColourSelector::resized()
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

        parameter2D->setBounds (edgeGap, y,
                                getWidth() - hueWidth - edgeGap - 4,
                                getHeight() - topSpace - sliderSpace - swatchSpace - edgeGap);

        parameter1D->setBounds (parameter2D->getRight() + 4, y,
                                getWidth() - edgeGap - (parameter2D->getRight() + 4),
                                parameter2D->getHeight());

        y = getHeight() - sliderSpace - swatchSpace - edgeGap;
    }

    if ((flags & showSliders) != 0)
    {
        auto sliderHeight = juce::jmax (4, sliderSpace / numSliders);

        for (int i = 0; i < numSliders; ++i)
        {
            auto rc = juce::Rectangle<int> (proportionOfWidth (0.2f), y, proportionOfWidth (0.72f), sliderHeight - 2);

            auto trc = rc.removeFromLeft (rc.getHeight() + 2);
            if (i < std::ssize (toggles))
                toggles[i]->setBounds (trc.translated (-sliderHeight, 0));

            sliders[i]->setBounds (rc);

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

void ColourSelector::changeColour ( juce::Slider* slider )
{
    if (sliders[0] == nullptr)
        return;

    if (sliders[0].get() == slider || sliders[1].get() == slider || sliders[2].get() == slider)
    {
        auto col = DeepColour::fromHSB (float (sliders[0]->getValue() / 360.0),
                                        float (sliders[1]->getValue() / 100.0),
                                        float (sliders[2]->getValue() / 100.0),
                                        float (sliders[6]->getValue() / 255.0));

        setCurrentColour (col);
    }
    else
    {
        auto col = DeepColour::fromRGBA (float (sliders[3]->getValue() / 255.0),
                                         float (sliders[4]->getValue() / 255.0),
                                         float (sliders[5]->getValue() / 255.0),
                                         float (sliders[6]->getValue() / 255.0));

        setCurrentColour (col);
    }
}

void ColourSelector::updateParameters()
{
    if (toggles[0]->getToggleState())
    {
        parameter1D->setParameter (Params::hue);
        parameter2D->setParameters (Params::sat, Params::bri);
    }
    else if (toggles[1]->getToggleState())
    {
        parameter1D->setParameter (Params::sat);
        parameter2D->setParameters (Params::hue, Params::bri);
    }
    else if (toggles[2]->getToggleState())
    {
        parameter1D->setParameter (Params::bri);
        parameter2D->setParameters (Params::hue, Params::sat);
    }
    else if (toggles[3]->getToggleState())
    {
        parameter1D->setParameter (Params::red);
        parameter2D->setParameters (Params::blue, Params::green);
    }
    else if (toggles[4]->getToggleState())
    {
        parameter1D->setParameter (Params::green);
        parameter2D->setParameters (Params::blue, Params::red);
    }
    else if (toggles[5]->getToggleState())
    {
        parameter1D->setParameter (Params::blue);
        parameter2D->setParameters (Params::red, Params::green);
    }
}

//==============================================================================
int ColourSelector::getNumSwatches() const
{
    return 0;
}

juce::Colour ColourSelector::getSwatchColour (int) const
{
    jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
    return juce::Colours::black;
}

void ColourSelector::setSwatchColour (int, const juce::Colour&)
{
    jassertfalse; // if you've overridden getNumSwatches(), you also need to implement this method
}

} // namespace juce
