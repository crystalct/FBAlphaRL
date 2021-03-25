#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "soundlib/audioplayer.h"

int SND_Init_cpp(u32 spu) {

    int i = SND_Init(spu);
    return i;
}

void SND_Pause_cpp(int paused) {

    SND_Pause(paused);
}


void SND_End_cpp() {
    SND_End();
}
