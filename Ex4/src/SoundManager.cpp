//
//  SoundManager.cpp
//  CGP-Ex4
//
//  Created by Amir Blum on 5/26/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#include "SoundManager.h"

SoundManager::SoundManager() :
_sourcesInUse(), _buffers()
{
}

ALuint SoundManager::loadSound(std::string filename, bool loop)
{
    // Clear Error Code
    alGetError();
    
    // Check and see if the pSoundFile is already loaded into a buffer
    SoundBuffer buffer = locateSoundBuffer(filename);
    
    // Now inform OpenAL of the sound assignment and attach the audio buffer
    // to the audio source
    ALuint sourceID;
    alGenSources(1, &sourceID);
    alSourcei(sourceID, AL_BUFFER, buffer.bufferID);
    alSourcei(sourceID, AL_LOOPING, loop);
    
    checkALError("loadSound");
    
    return sourceID;
}

SoundManager::SoundBuffer SoundManager::locateSoundBuffer(std::string filename)
{
    for (SoundBuffer buffer : _buffers) {
        if (buffer.filename == filename) {
            return buffer;
        }
    }
    
    // Not found, so create one
    ALuint bufferID = alutCreateBufferFromFile(filename.c_str());
    SoundBuffer buffer{filename, bufferID};
    _buffers.push_back(buffer);
    return buffer;
}

void SoundManager::releaseSound(ALuint sourceID)
{
    alGetError();
    
    alSourceStop(sourceID);
    
    alDeleteSources(1, &sourceID);
    
    checkALError("releaseSound");
}

void SoundManager::playSound(ALuint sourceID, bool forceRestart)
{
    alGetError();
    
    // Are we currently playing the audio source?
    ALint sourceAudioState;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceAudioState);
    
    if (sourceAudioState == AL_PLAYING) {
        if (forceRestart) {
            stopSound(sourceID);
        }
        else {
            return; // Not forced, so we don't do anything
        }
    }
    
    alSourcePlay(sourceID);
    
    checkALError("playSound");
}

void SoundManager::pauseSound(ALuint sourceID)
{
    alGetError();
    
    alSourcePause(sourceID);
    
    checkALError("pauseSound");
}

void SoundManager::pauseAllSounds()
{
    alGetError();
    
    alSourcePausev(_sourcesInUse.size(), &_sourcesInUse[0]);
    
    checkALError("pauseAllSounds");
}

void SoundManager::resumeSound(ALuint sourceID)
{
    alGetError();
    
    // If the sound was paused the sound will resume, else it will play from
    // the beginning !
    alSourcePlay(sourceID);
    
    checkALError("resumeSound");
}

void SoundManager::resumeAllSounds()
{
    alGetError();
    
    int sourceAudioState;
    for (ALuint sourceID : _sourcesInUse)
    {
        // Are we currently playing the audio source?
        alGetSourcei(sourceID, AL_SOURCE_STATE, &sourceAudioState);
        
        if (sourceAudioState == AL_PAUSED) {
            resumeSound(sourceID);
        }
    }
    
    checkALError("resumeAllSounds");
}

void SoundManager::stopSound(ALuint sourceID)
{
    alGetError();
    
    alSourceStop(sourceID);
    
    checkALError("stopSound");
}

void SoundManager::stopAllSounds()
{
    alGetError();
    
    for (ALuint sourceID : _sourcesInUse) {
        stopSound(sourceID);
    }
    
    checkALError("stopAllSounds");
}

void SoundManager::checkALError(std::string sourceFunc)
{
    ALenum errCode = alGetError();
    if (errCode != AL_NO_ERROR) {
        std::cout << "OpenAL Error: " << alutGetErrorString(errCode) << " in: " << sourceFunc << std::endl;
    }
}