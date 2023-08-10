namespace reFX
{

HSB rgbToHsb (const RGB& rgb)
{
    auto maxVal = std::max ({rgb.r, rgb.g, rgb.b});
    auto minVal = std::min ({rgb.r, rgb.g, rgb.b});
    auto delta = maxVal - minVal;

    auto h = 0.0f;
    auto s = 0.0f;
    auto b = maxVal;

    if (delta == 0)
    {
        h = 0.0f;
        s = 0.0f;
    }
    else
    {
        s = delta / maxVal;

        if (maxVal == rgb.r)
            h = (rgb.g - rgb.b) / delta;
        else if (maxVal == rgb.g)
            h = 2 + (rgb.b - rgb.r) / delta;
        else
            h = 4 + (rgb.r - rgb.g) / delta;

        h *= 60;
        if (h < 0)
            h += 360;
    }

    return { h / 360.0f, s, b };
}

RGB hsbToRgb (const HSB& hsb)
{
    auto c = hsb.b * hsb.s;
    auto x = c * (1 - std::abs (std::fmod (hsb.h / 60, 2) - 1));
    auto m = hsb.b - c;

    auto r = 0.0f;
    auto g = 0.0f;
    auto bVal = 0.0f;

    if (hsb.h < 60)
    {
        r = c;
        g = x;
        bVal = 0;
    }
    else if (hsb.h < 120)
    {
        r = x;
        g = c;
        bVal = 0;
    }
    else if (hsb.h < 180)
    {
        r = 0;
        g = c;
        bVal = x;
    }
    else if (hsb.h < 240)
    {
        r = 0;
        g = x;
        bVal = c;
    }
    else if (hsb.h < 300)
    {
        r = x;
        g = 0;
        bVal = c;
    }
    else
    {
        r = c;
        g = 0;
        bVal = x;
    }

    return { r + m, g + m, bVal + m };
}

//==============================================================================
bool DeepColor::operator== (const DeepColor& other) const noexcept
{
    return juce::approximatelyEqual (a, other.a) &&
           juce::approximatelyEqual (rgb.r, other.rgb.r) &&
           juce::approximatelyEqual (rgb.g, other.rgb.g) &&
           juce::approximatelyEqual (rgb.b, other.rgb.b);
}

bool DeepColor::operator!= (const DeepColor& other) const noexcept
{
    return ! (*this == other);
}

//==============================================================================
DeepColor::DeepColor (juce::uint32 col) noexcept
    : a (((col >> 24) & 0xff) / 255.0f),
      rgb ({(((col >> 16) & 0xff) / 255.0f), (((col >> 8)  & 0xff) / 255.0f), (((col >> 0)  & 0xff) / 255.0f)})
{
}

DeepColor::DeepColor (const juce::Colour& c)
{
    rgb.r = c.getFloatRed();
    rgb.g = c.getFloatGreen();
    rgb.g = c.getFloatBlue();
    a = c.getFloatAlpha();
}

DeepColor DeepColor::fromRGB (float red, float green, float blue) noexcept
{
    DeepColor c;
    c.a = 1.0f;
    c.rgb.r = red;
    c.rgb.g = green;
    c.rgb.b = blue;
    return c;
}

DeepColor DeepColor::fromRGBA (float red, float green, float blue, float alpha) noexcept
{
    DeepColor c;
    c.a = alpha;
    c.rgb.r = red;
    c.rgb.g = green;
    c.rgb.b = blue;
    return c;
}

DeepColor DeepColor::fromHSB (float hue, float saturation, float brightness, float alpha) noexcept
{
    DeepColor c;
    c.a = alpha;
    c.rgb = hsbToRgb ({hue, saturation, brightness});
    return c;
}

//==============================================================================
void DeepColor::getHSB (float& h, float& s, float& b) const noexcept
{
    auto hsb = rgbToHsb (rgb);
    h = hsb.h;
    s = hsb.s;
    b = hsb.b;
}

float DeepColor::getHue() const noexcept           { return rgbToHsb (rgb).h; }
float DeepColor::getSaturation() const noexcept    { return rgbToHsb (rgb).s; }
float DeepColor::getBrightness() const noexcept    { return rgbToHsb (rgb).b; }

juce::Colour DeepColor::getColour () const
{
    return juce::Colour::fromFloatRGBA (rgb.r, rgb.g, rgb.b, a);
}

}
