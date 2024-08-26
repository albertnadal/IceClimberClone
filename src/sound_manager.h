#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <map>
#include <defines.h>
#include <raylib/raylib.h>

using namespace std;

class SoundManager
{
  typedef map<SoundIdentificator, Sound> SoundsMap;
  SoundsMap soundsMap;
public:
  SoundManager();
  ~SoundManager();
  void LoadSounds();
  void UnloadSounds();
  void PlaySoundById(SoundIdentificator) const;
};

#endif