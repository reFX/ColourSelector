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
    auto h = hsb.h * 360.0f;
    auto c = hsb.b * hsb.s;
    auto x = c * (1.0f - std::abs (std::fmod (h / 60, 2.0f) - 1.0f));
    auto m = hsb.b - c;

    auto r = 0.0f;
    auto g = 0.0f;
    auto b = 0.0f;

    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    return { r + m, g + m, b + m };
}

//==============================================================================
bool DeepColour::operator== (const DeepColour& other) const noexcept
{
    return juce::approximatelyEqual (a, other.a) &&
           juce::approximatelyEqual (getRed(), other.getRed()) &&
           juce::approximatelyEqual (getBlue(), other.getBlue()) &&
           juce::approximatelyEqual (getGreen(), other.getGreen());
}

bool DeepColour::operator!= (const DeepColour& other) const noexcept
{
    return ! (*this == other);
}

//==============================================================================
DeepColour::DeepColour (juce::uint32 c) noexcept
{
    a = (((c >> 24) & 0xff) / 255.0f);
    col = RGB ((((c >> 16) & 0xff) / 255.0f), (((c >> 8)  & 0xff) / 255.0f), (((c >> 0)  & 0xff) / 255.0f));
}

DeepColour::DeepColour (HSB hsb, float alpha) noexcept
{
    a = alpha;
    col = hsb;
}

DeepColour::DeepColour (RGB rgb, float alpha) noexcept
{
    a = alpha;
    col = rgb;
}

DeepColour::DeepColour (const juce::Colour& c)
{
    col = RGB ({c.getFloatRed(), c.getFloatGreen(), c.getFloatBlue()});
    a = c.getFloatAlpha();
}

DeepColour DeepColour::fromRGB (float red, float green, float blue) noexcept
{
    DeepColour c;
    c.a = 1.0f;
    c.col = RGB (red, green, blue);
    return c;
}

DeepColour DeepColour::fromRGBA (float red, float green, float blue, float alpha) noexcept
{
    DeepColour c;
    c.a = alpha;
    c.col = RGB (red, green, blue);
    return c;
}

DeepColour DeepColour::fromHSB (float hue, float saturation, float brightness, float alpha) noexcept
{
    DeepColour c;
    c.a = alpha;
    c.col = HSB (hue, saturation, brightness);
    return c;
}

//==============================================================================
HSB DeepColour::getHSB() const noexcept
{
    if (auto hsb = std::get_if<HSB>(&col))
        return *hsb;
    else if (auto rgb = std::get_if<RGB>(&col))
        return rgbToHsb (*rgb);

    jassertfalse;
    return {};
}

RGB DeepColour::getRGB() const noexcept
{
    if (auto hsb = std::get_if<HSB>(&col))
        return hsbToRgb (*hsb);
    else if (auto rgb = std::get_if<RGB>(&col))
        return *rgb;

    jassertfalse;
    return {};
}

float DeepColour::getRed() const noexcept           { return getRGB().r; }
float DeepColour::getGreen() const noexcept         { return getRGB().g; }
float DeepColour::getBlue() const noexcept          { return getRGB().b; }

float DeepColour::getHue() const noexcept           { return getHSB().h; }
float DeepColour::getSaturation() const noexcept    { return getHSB().s; }
float DeepColour::getBrightness() const noexcept    { return getHSB().b; }

juce::Colour DeepColour::getColour () const
{
    return juce::Colour::fromFloatRGBA (getRed(), getGreen(), getBlue(), a);
}

DeepColour DeepColour::withAlpha (float newAlpha) const noexcept
{
    auto c = *this;
    c.a = newAlpha;
    return c;
}

}
