namespace reFX
{

struct RGB
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct HSB
{
    float h = 0.0f;
    float s = 0.0f;
    float b = 0.0f;
};

//==============================================================================
/**
    Represents a colour, also including a transparency value.

    The colour is stored internally as float red, green, blue and alpha values.

    @tags{Graphics}
*/
class DeepColor final
{
public:
    //==============================================================================
    /** Creates a transparent black colour. */
    DeepColor() = default;

    /** Creates a copy of another DeepColor object. */
    DeepColor (const DeepColor&) = default;

    /** Creates a copy of a juce::Colour object. */
    DeepColor (const juce::Colour&);

    /** Creates a DeepColor from a 32-bit ARGB value.

        The format of this number is:
            ((alpha << 24) | (red << 16) | (green << 8) | blue).

        All components in the range 0x00 to 0xff.
        An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.

        @see getPixelARGB
    */
    explicit DeepColor (juce::uint32 argb) noexcept;

    /** Creates an opaque colour using float red, green and blue values */
    static DeepColor fromRGB (float red, float green, float blue) noexcept;

    /** Creates a colour using 8-bit red, green, blue and alpha values. */
    static DeepColor fromRGBA (float red, float green, float blue, float alpha) noexcept;

    /** Creates a colour using floating point hue, saturation, brightness and alpha values.

        All values must be between 0.0 and 1.0.
        Numbers outside the valid range will be clipped.
    */
    static DeepColor fromHSB (float hue,
                              float saturation,
                              float brightness,
                              float alpha) noexcept;

    /** Destructor. */
    ~DeepColor() = default;

    /** Copies another Colour object. */
    DeepColor& operator= (const DeepColor&) = default;

    /** Compares two colours. */
    bool operator== (const DeepColor& other) const noexcept;
    /** Compares two colours. */
    bool operator!= (const DeepColor& other) const noexcept;

    //==============================================================================
    /** Returns the red component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getRed() const noexcept                       { return rgb.r; }

    /** Returns the green component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getGreen() const noexcept                     { return rgb.g; }

    /** Returns the blue component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getBlue() const noexcept                      { return rgb.b; }

    /** Returns the red component of this colour as a floating point value.
        @returns a value between 0.0 and 1.0
    */

    //==============================================================================
    /** Returns the colour's alpha (opacity).

        Alpha of 0.0 is completely transparent, 1.0 is completely opaque.
    */
    float getAlpha() const noexcept                     { return a; }

    //==============================================================================
    /** Returns the colour's hue component.
        The value returned is in the range 0.0 to 1.0
    */
    float getHue() const noexcept;

    /** Returns the colour's saturation component.
        The value returned is in the range 0.0 to 1.0
    */
    float getSaturation() const noexcept;

    /** Returns the colour's brightness component.
        The value returned is in the range 0.0 to 1.0
    */
    float getBrightness() const noexcept;

    /** Returns the colour's hue, saturation and brightness components all at once.
        The values returned are in the range 0.0 to 1.0
    */
    void getHSB (float& hue,
                 float& saturation,
                 float& brightness) const noexcept;

    /** Returns a juce::Colour */
    juce::Colour getColour () const;

    //==============================================================================

private:
    //==============================================================================
    float a = 0.0f;
    RGB rgb;
};

} // namespace juce
