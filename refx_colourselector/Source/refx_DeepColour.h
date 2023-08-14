namespace reFX
{

struct RGB
{
    RGB() = default;
    RGB (float r_, float g_, float b_) : r (r_), g (g_), b (b_) {}

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct HSB
{
    HSB() = default;
    HSB (float h_, float s_, float b_) : h (h_), s (s_), b (b_) {}

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
class DeepColour final
{
public:
    //==============================================================================
    /** Creates a transparent black colour. */
    DeepColour() = default;

    /** Creates a copy of another DeepColour object. */
    DeepColour (const DeepColour&) = default;

    /** Creates a copy of a juce::Colour object. */
    DeepColour (const juce::Colour&);

    /** Creates a DeepColour from a 32-bit ARGB value.

        The format of this number is:
            ((alpha << 24) | (red << 16) | (green << 8) | blue).

        All components in the range 0x00 to 0xff.
        An alpha of 0x00 is completely transparent, alpha of 0xff is opaque.

        @see getPixelARGB
    */
    explicit DeepColour (juce::uint32 argb) noexcept;

    explicit DeepColour (HSB hsb, float alpha = 1.0f) noexcept;

    explicit DeepColour (RGB rgb, float alpha = 1.0f) noexcept;

    /** Creates an opaque colour using float red, green and blue values */
    static DeepColour fromRGB (float red, float green, float blue) noexcept;

    /** Creates a colour using 8-bit red, green, blue and alpha values. */
    static DeepColour fromRGBA (float red, float green, float blue, float alpha) noexcept;

    /** Creates a colour using floating point hue, saturation, brightness and alpha values.

        All values must be between 0.0 and 1.0.
        Numbers outside the valid range will be clipped.
    */
    static DeepColour fromHSB (float hue,
                               float saturation,
                               float brightness,
                               float alpha) noexcept;

    /** Destructor. */
    ~DeepColour() = default;

    /** Copies another Colour object. */
    DeepColour& operator= (const DeepColour&) = default;

    /** Compares two colours. */
    bool operator== (const DeepColour& other) const noexcept;
    /** Compares two colours. */
    bool operator!= (const DeepColour& other) const noexcept;

    //==============================================================================
    /** Returns the red component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getRed() const noexcept;

    /** Returns the green component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getGreen() const noexcept;

    /** Returns the blue component of this colour.
        @returns a value between 0.0 and 1.0.
    */
    float getBlue() const noexcept;

    /** Returns the red component of this colour as a floating point value.
        @returns a value between 0.0 and 1.0
    */

    //==============================================================================
    /** Returns the colour's alpha (opacity).

        Alpha of 0.0 is completely transparent, 1.0 is completely opaque.
    */
    float getAlpha() const noexcept                     { return a; }

    DeepColour withAlpha (float newAlpha) const noexcept;

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
    HSB getHSB() const noexcept;

    /** Returns the colour's red, blue and green components all at once.
        The values returned are in the range 0.0 to 1.0
    */
    RGB getRGB() const noexcept;

    /** Returns a juce::Colour */
    juce::Colour getColour () const;

    //==============================================================================

private:
    //==============================================================================
    float a = 0.0f;
    std::variant<HSB, RGB> col;
};

} // namespace reFX
