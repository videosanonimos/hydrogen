#ifndef AUDIO_ENGINE_INFO_FORM_H
#define AUDIO_ENGINE_INFO_FORM_H

#include <core/Object.h>
#include <core/AudioEngine/AudioEngine.h>

#include "EventListener.h"
#include "ui_AudioEngineInfoForm_UI.h"

#include <QtGui>
#include <QtWidgets>

/**
 * Audio Engine information form
 */
/** \ingroup docGUI docAudioEngine docDebugging*/
class AudioEngineInfoForm :  public QWidget, public Ui_AudioEngineInfoForm_UI, public EventListener,  public H2Core::Object<AudioEngineInfoForm>
{
    H2_OBJECT(AudioEngineInfoForm)
	Q_OBJECT
	private:
		QTimer* m_pTimer;

		// EventListener implementation
	virtual void stateChangedEvent( H2Core::AudioEngine::State state) override;
	virtual void playingPatternsChangedEvent() override;
	virtual void updateSongEvent(int) override;
		//~ EventListener implementation

	public:
		explicit AudioEngineInfoForm(QWidget* parent);
		~AudioEngineInfoForm();

		void showEvent ( QShowEvent *ev ) override;
		void hideEvent ( QHideEvent *ev ) override;

	public slots:
		void updateInfo();

	private:
		void updateAudioEngineState();
};

#endif

