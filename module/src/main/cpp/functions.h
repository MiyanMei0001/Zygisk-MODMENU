#ifndef ZYCHEATS_SGUYS_FUNCTIONS_H
#define ZYCHEATS_SGUYS_FUNCTIONS_H

// here you can define variables for the patches
bool cGod;


bool (*old_godmod)(void *instance);
bool isGodMod(void *instance)
{
    if (instance != NULL) {
        if(cGod) {
            return true;
        }
    }
    return old_godmod(instance);
}

void Hooks() {
    IL2Cpp::Il2CppAttach();

    Tools::Hook((void *) (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("PlayerStatus") , OBFUSCATE("get_IsInvincibility"), 0), (void *) isGodMod, (void **) &old_godmod);
}

#endif //ZYCHEATS_SGUYS_FUNCTIONS_H
