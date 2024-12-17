#ifndef ZYCHEATS_SGUYS_FUNCTIONS_H
#define ZYCHEATS_SGUYS_FUNCTIONS_H

#include <cstdint>

bool unliJump = false;
bool equipArmor = false;
bool seeRogue = false;
bool throughAll = false;
bool colorChest = false;
bool lootSearch = false;
float skillSpd = 1.0f;
bool teleportAll = false;

struct Vector3 {
    float x, y, z;
};

#define ENABLE_HOOKS true

// Function pointers
int32_t (*oldSetJump)(void *instance) = nullptr;
void (*oldSetStealth)(void *instance) = nullptr;
bool (*oldSetThroughPlayer)(void *instance) = nullptr;
bool (*oldSetThroughMonster)(void *instance) = nullptr;
bool (*oldSetChest)(void *instance) = nullptr;
float (*oldSetSkillSpd)(void *instance) = nullptr;
bool (*oldSetSearch)(void *instance, uint64_t id) = nullptr;
bool (*oldSetEquip)(void *instance, int32_t inventoryId, uint64_t uid, int32_t charId, int32_t classId) = nullptr;
Vector3 (*oldSetPosition)(void *instance, Vector3 position) = nullptr;

int32_t SetJump(void *instance) {
    if (instance && unliJump) return 9999;
    return oldSetJump ? oldSetJump(instance) : 0;
}

void SetStealth(void *instance) {
    if (instance && seeRogue) return;
    if (oldSetStealth) oldSetStealth(instance);
}

bool SetThroughPlayer(void *instance) {
    if (instance && throughAll) return true;
    return oldSetThroughPlayer ? oldSetThroughPlayer(instance) : false;
}

bool SetThroughMonster(void *instance) {
    if (instance && throughAll) return true;
    return oldSetThroughMonster ? oldSetThroughMonster(instance) : false;
}

bool SetChest(void *instance) {
    if (instance && colorChest) return true;
    return oldSetChest ? oldSetChest(instance) : false;
}

float SetSkillSpd(void *instance) {
    if (instance && skillSpd > 1.0f) return oldSetSkillSpd ? oldSetSkillSpd(instance) * skillSpd : 1.0f;
    return oldSetSkillSpd ? oldSetSkillSpd(instance) : 0.0f;
}

bool SetSearch(void *instance, uint64_t id) {
    if (instance && lootSearch) return true;
    return oldSetSearch ? oldSetSearch(instance, id) : false;
}

bool SetEquip(void *instance, int32_t inventoryId, uint64_t uid, int32_t charId, int32_t classId) {
    if (instance && equipArmor) return true;
    return oldSetEquip ? oldSetEquip(instance, inventoryId, uid, charId, classId) : false;
}

Vector3 SetPosition(void *instance, Vector3 position) {
    if (instance && teleportAll) {
        Vector3 newPosition = { position.x, 100.0f, position.z };
        return oldSetPosition ? oldSetPosition(instance, newPosition) : newPosition;
    }
    return oldSetPosition ? oldSetPosition(instance, position) : position;
}

void Hooks() {
#if ENABLE_HOOKS
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
#endif
}

#endif // ZYCHEATS_SGUYS_FUNCTIONS_H