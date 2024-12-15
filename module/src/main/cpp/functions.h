#ifndef ZYCHEATS_SGUYS_FUNCTIONS_H
#define ZYCHEATS_SGUYS_FUNCTIONS_H

// here you can define variables for the patches
bool UnliJump;


bool (*oldCanJump)(void *instance);
bool CanJump(void *instance)
{
    if (instance != NULL) {
        if(UnliJump) {
            return true;
        }
    }
    return oldCanJump(instance);
}

void Hooks() {
    IL2Cpp::Il2CppAttach();

    Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("SYBO.RunnerCore.Character"), OBFUSCATE("CharacterMotor") , OBFUSCATE("get_CanJump"), 0), (void *) CanJump, (void **) &oldCanJump);
}

#endif //ZYCHEATS_SGUYS_FUNCTIONS_H
