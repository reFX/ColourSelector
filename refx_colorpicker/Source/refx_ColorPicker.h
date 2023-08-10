namespace reFX
{

//==============================================================================
/**
    A component that lets the user choose a colour.

    This shows RGB sliders and a colourspace that the user can pick colours from.

    This class is also a ChangeBroadcaster, so listeners can register to be told
    when the colour changes.

    @tags{GUI}
*/
class ColorPicker : public juce::Component,
                    public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    /** Options for the type of selector to show. These are passed into the constructor. */
    enum ColorPickerOptions
    {
        showAlphaChannel    = 1 << 0,   /**< if set, the colour's alpha channel can be changed as well as its RGB. */

        showColourAtTop     = 1 << 1,   /**< if set, a swatch of the colour is shown at the top of the component. */
        editableColour      = 1 << 2,   /**< if set, the colour shows at the top of the component is editable. */
        showSliders         = 1 << 3,   /**< if set, RGB sliders are shown at the bottom of the component. */
        showColourspace     = 1 << 4    /**< if set, a big HSV selector is shown. */
    };

    //==============================================================================
    /** Creates a ColorPicker object.

        The flags are a combination of values from the ColorPickerOptions enum, specifying
        which of the selector's features should be visible.

        The edgeGap value specifies the amount of space to leave around the edge.

        gapAroundColourSpaceComponent indicates how much of a gap to put around the
        colourspace and hue selector components.
    */
    ColorPicker (int flags = (showAlphaChannel | showColourAtTop | showSliders | showColourspace),
                 int edgeGap = 4,
                 int gapAroundColourSpaceComponent = 7);

    /** Destructor. */
    ~ColorPicker() override;

    //==============================================================================
    /** Returns the colour that the user has currently selected.

        The ColorPicker class is also a ChangeBroadcaster, so listeners can
        register to be told when the colour changes.

        @see setCurrentColour
    */
    juce::Colour getCurrentColour() const;

    /** Changes the colour that is currently being shown.

        @param newColour           the new colour to show
        @param notificationType    whether to send a notification of the change to listeners.
                                   A notification will only be sent if the colour has changed.
    */
    void setCurrentColour (juce::Colour newColour, juce::NotificationType notificationType = juce::sendNotification);

    /** Changes the colour that is currently being shown.

        @param newColour           the new colour to show
        @param notificationType    whether to send a notification of the change to listeners.
                                   A notification will only be sent if the colour has changed.
    */
    void setCurrentColour (DeepColor newColour, juce::NotificationType notificationType = juce::sendNotification);

    //==============================================================================
    /** Tells the selector how many preset colour swatches you want to have on the component.

        To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
        setSwatchColour(), to return the number of colours you want, and to set and retrieve
        their values.
    */
    virtual int getNumSwatches() const;

    /** Called by the selector to find out the colour of one of the swatches.

        Your subclass should return the colour of the swatch with the given index.

        To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
        setSwatchColour(), to return the number of colours you want, and to set and retrieve
        their values.
    */
    virtual juce::Colour getSwatchColour (int index) const;

    /** Called by the selector when the user puts a new colour into one of the swatches.

        Your subclass should change the colour of the swatch with the given index.

        To enable swatches, you'll need to override getNumSwatches(), getSwatchColour(), and
        setSwatchColour(), to return the number of colours you want, and to set and retrieve
        their values.
    */
    virtual void setSwatchColour (int index, const juce::Colour& newColour);


    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the keyboard.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        backgroundColourId              = 0x1007000,    /**< the colour used to fill the component's background. */
        labelTextColourId               = 0x1007001     /**< the colour used for the labels next to the sliders. */
    };

    //==============================================================================

private:
    //==============================================================================
    class SwatchComponent;
    class Parameter2D;
    class Parameter1D;
    class ColourPreviewComp;

    enum class Params
    {
        hue,
        sat,
        bri,

        red,
        green,
        blue,
    };

    DeepColor colour;
    std::unique_ptr<juce::ToggleButton> toggles[6];
    std::unique_ptr<juce::Slider> sliders[7];
    std::unique_ptr<Parameter2D> parameter2D;
    std::unique_ptr<Parameter1D> parameter1D;
    std::unique_ptr<ColourPreviewComp> previewComponent;
    juce::OwnedArray<SwatchComponent> swatchComponents;
    const int flags;
    int edgeGap;

    void updateParameters();
    void setHue (float newH);
    void setSV (float newS, float newV);
    void update (juce::NotificationType);
    void changeColour (juce::Slider*);
    void paint (juce::Graphics&) override;
    void resized() override;

    void set (const DeepColor&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorPicker)
};

} // namespace juce
