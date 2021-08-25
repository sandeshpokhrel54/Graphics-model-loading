#include "irrKlang/include/irrKlang.h"

using namespace irrklang;

ISoundEngine *SoundEngine  = createIrrKlangDevice();

void Init()
{
    SoundEngine->play2D("./background.mp3",true);    
}