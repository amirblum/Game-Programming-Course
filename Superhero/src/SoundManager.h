//
//  SoundManager.h
//  CGP-Superhero
//
//  Created by Amir Blum on 5/26/15.
//  Copyright (c) 2015 Amir Blum. All rights reserved.
//

#ifndef __CGP_Superhero__SoundManager__
#define __CGP_Superhero__SoundManager__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <AL/alut.h>

class SoundManager {
public:
    // Singleton implementation
    static SoundManager& Instance() {
        static SoundManager instance;
        
        return instance;
    }
    
    ALuint loadSound(std::string filename, bool loop = false);
    void releaseSound(ALuint sourceID);
    
    void playSound(ALuint sourceID, bool forceRestart = false);
    void pauseSound(ALuint sourceID);
    void pauseAllSounds();
    void resumeSound(ALuint sourceID);
    void resumeAllSounds();
    void stopSound(ALuint sourceID);
    void stopAllSounds();

    
private:
    struct SoundBuffer {
        std::string filename;
        ALuint bufferID;
    };
    
    std::vector<ALuint> _sourcesInUse;
    std::vector<SoundBuffer> _buffers;
    
    // Singleton private instantiation
    SoundManager();
    SoundManager(SoundManager const&);
    void operator=(SoundManager const&);

    SoundBuffer locateSoundBuffer(std::string filename);
    void checkALError(std::string sourceFunc);
};

#endif /* defined(__CGP_Superhero__SoundManager__) */
