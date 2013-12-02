#include "../JuceLibraryCode/JuceHeader.h"
#include "StandaloneApplicationWindow.h"

#ifdef STANDALONE_VERSION
//==============================================================================
class GrainrStandaloneApplication  
	: public JUCEApplication
{
public:
    //==============================================================================
    GrainrStandaloneApplication() {}

    const String getApplicationName()       { return ProjectInfo::projectName; }
    const String getApplicationVersion()    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()       { return true; }

    //==============================================================================
    void initialise (const String& /*commandLine*/)
    {
		window = new StandaloneApplicationWindow();
		window->setVisible(true);
		window->setSize(800, 600);
    }

    void shutdown()
    {
		window = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& /*commandLine*/)
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

private:
	ScopedPointer<StandaloneApplicationWindow> window;
};

//==============================================================================

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (GrainrStandaloneApplication)
#endif //STANDALONE_VERSION
