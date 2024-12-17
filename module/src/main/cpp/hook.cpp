#include <cstring>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyScanner.h"
#include "KittyMemory/KittyUtils.h"
#include "Includes/Dobby/dobby.h"
#include "Include/Unity.h"
#include "Vector3.h"
#include "../ByNameModding/Tools.h"
#include "../ByNameModding/Il2Cpp.h"
#include "Misc.h"
#include "hook.h"
#include "Include/Roboto-Regular.h"
#include <iostream>
#include <chrono>
#include "Include/Quaternion.h"
#include "Rect.h"
#include <fstream>
#include <limits>
#define GamePackageName "com.dois.greedgame" // define the game package name here please


int glHeight, glWidth;

int isGame(JNIEnv *env, jstring appDataDir)
{
    if (!appDataDir)
        return 0;
    const char *app_data_dir = env->GetStringUTFChars(appDataDir, nullptr);
    int user = 0;
    static char package_name[256];
    if (sscanf(app_data_dir, "/data/%*[^/]/%d/%s", &user, package_name) != 2) {
        if (sscanf(app_data_dir, "/data/%*[^/]/%s", package_name) != 1) {
            package_name[0] = '\0';
            LOGW(OBFUSCATE("can't parse %s"), app_data_dir);
            return 0;
        }
    }
    if (strcmp(package_name, GamePackageName) == 0) {
        LOGI(OBFUSCATE("detect game: %s"), package_name);
        game_data_dir = new char[strlen(app_data_dir) + 1];
        strcpy(game_data_dir, app_data_dir);
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 1;
    } else {
        env->ReleaseStringUTFChars(appDataDir, app_data_dir);
        return 0;
    }
}

bool setupimg;

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac)
{
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

#include "functions.h"
#include "menu.h"
struct Ambator3 {
float x,y,z;
};

int32_t (*oldSetJump)(void *instance);
void (*oldSetStealth)(void *instance);
bool (*oldSetThroughPlayer)(void *instance);
bool (*oldSetThroughMonster)(void *instance);
bool (*oldSetChest)(void *instance);
float (*oldSetSkillSpd)(void *instance);
bool (*oldSetSearch)(void *instance, uint64_t id);
bool (*oldSetEquip)(void *instance, int32_t inventoryId, uint64_t uid, int32_t charId, int32_t classId);
void (*oldSetPosition)(void *instance, Ambator3 position);

int32_t SetJump(void *instance) {
    if (instance && unliJump) return 9999;
    return oldSetJump(instance);
}

void SetStealth(void *instance) {
    if (instance && seeRogue) return;
    if (oldSetStealth) oldSetStealth(instance);
}

bool SetThroughPlayer(void *instance) {
    if (instance && throughAll) return true;
    return oldSetThroughPlayer(instance);
}

bool SetThroughMonster(void *instance) {
    if (instance && throughAll) return true;
    return oldSetThroughMonster(instance);
}

bool SetChest(void *instance) {
    if (instance && colorChest) return true;
    return oldSetChest(instance);
}

float SetSkillSpd(void *instance) {
    if (instance && skillSpd > 1.0f) return oldSetSkillSpd(instance);
    return oldSetSkillSpd(instance);
}

bool SetSearch(void *instance, uint64_t id) {
    if (instance && lootSearch) return true;
    return oldSetSearch(instance, id);
}

bool SetEquip(void *instance, int32_t inventoryId, uint64_t uid, int32_t charId, int32_t classId) {
    if (instance && equipArmor) return true;
    return oldSetEquip(instance, inventoryId, uid, charId, classId);
}

void SetPosition(void *instance, Ambator3 position) {
    if (instance && teleportAll) {
        Ambator3 newPosition = { position.x, 100.0f, position.z };
        return oldSetPosition(instance, newPosition);
    }
    return oldSetPosition(instance, position);
}

void *hack_thread(void *arg) {
    do {
        sleep(1);
        g_il2cppBaseMap = KittyMemory::getLibraryBaseMap("libil2cpp.so");
    } while (!g_il2cppBaseMap.isValid());

    IL2Cpp::Il2CppAttach();

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.Logic.dll"), OBFUSCATE("KradGame.Logic"), OBFUSCATE("CharacterLogic"), OBFUSCATE("get_AirJumpCount"), 0),
                (void *)SetJump, (void **)&oldSetJump);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.Logic.dll"), OBFUSCATE("KradGame.Logic"), OBFUSCATE("CharacterLogic"), OBFUSCATE("get_IgnoreCollision"), 0),
                (void *)SetThroughPlayer, (void **)&oldSetThroughPlayer);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.Logic.dll"), OBFUSCATE("KradGame.Logic"), OBFUSCATE("MonsterLogic"), OBFUSCATE("get_IgnoreCollision"), 0),
                (void *)SetThroughMonster, (void **)&oldSetThroughMonster);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.Logic.dll"), OBFUSCATE("KradGame.Logic"), OBFUSCATE("ChestBoxLogic"), OBFUSCATE("get_IsOpen"), 0),
                (void *)SetChest, (void **)&oldSetChest);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.Logic.dll"), OBFUSCATE("KradGame.Logic"), OBFUSCATE("SkillLogic"), OBFUSCATE("GetSkillPlaySpeed"), 3),
                (void *)SetSkillSpd, (void **)&oldSetSkillSpd);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.dll"), OBFUSCATE("KradGame"), OBFUSCATE("PEntity"), OBFUSCATE("ProcessEntityStealth"), 0),
                (void *)SetStealth, (void **)&oldSetStealth);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.dll"), OBFUSCATE("KradGame"), OBFUSCATE("InventoryService"), OBFUSCATE("CanEquip"), 4),
                (void *)SetEquip, (void **)&oldSetEquip);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("KradGame.dll"), OBFUSCATE("KradGame"), OBFUSCATE("InventoryService"), OBFUSCATE("CheckItemSearched"), 1),
                (void *)SetSearch, (void **)&oldSetSearch);

    Tools::Hook((void *)(uintptr_t)IL2Cpp::Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position"), 1),
                (void *)SetPosition, (void **)&oldSetPosition);
                
    auto eglhandle = dlopen("libunity.so", RTLD_LAZY);
    auto eglSwapBuffers = dlsym(eglhandle, "eglSwapBuffers");
    DobbyHook((void*)eglSwapBuffers,(void*)hook_eglSwapBuffers,
              (void**)&old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        DobbyHook(sym_input,(void*)myInput,(void**)&origInput);
    }
    LOGI("Draw Done!");
    return nullptr;
}
