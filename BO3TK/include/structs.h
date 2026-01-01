#pragma once
#include <qglobal.h>
// old version of structs

struct gscProfileInfo_t;
struct Object;
struct HashTable;
struct lua_State;
struct lua_Debug;
struct dvar_t;
struct RuntimeProfileData{};
struct MemoryManager{};
struct DebugHook{};
struct DebugInstance{};
struct ApiStack{};
struct CallStack{};
struct CallSite{};
struct cclosure{};
struct UserData{};
struct StructInst{};
struct StringPinner{};
struct Metatable{};
struct StaticStringCache{};
struct UpValue{};
struct Method{};
struct ResumeData_Header{};
struct ResumeData_State{};
struct ResumeData_Table{};
struct ResumeData_Closure{};
struct ResumeData_CClosure{};
struct ResumeData_Userdata{};
union ResumeData_Entry{};
struct GarbageCollector{};

typedef int16_t BoneIndex;
typedef int16_t GridCoord;
typedef int16_t i16;
typedef uint16_t UIModelIndex;
typedef uint16_t GfxSamplerState;
typedef uint16_t ub1;
typedef int32_t i32;
typedef int32_t StateIndex;
typedef uint32_t u32;
typedef uint32_t ub4;
typedef uint32_t AIMovementType;
typedef uint32_t GroupRank;
typedef uint32_t GfxLabel;
typedef uint32_t GfxPackedColor;
typedef uint32_t contents_t;
typedef uint32_t dvarStrHash_t;
typedef uint32_t ScrString_t;
typedef uint32_t ScrVarIndex_t;
typedef int64_t i64;
typedef uint64_t u64;
typedef uint64_t EventParm_t;
typedef uint64_t ClanID;
typedef uint64_t AttachmentMask;
typedef uint64_t bitfield_t;
typedef uint64_t EventParm_t;
typedef uint64_t XUID;
typedef char com_devhost_byte;
typedef char str32_t[32];
typedef char str64_t[64];
typedef char str128_t[128];
typedef char str256_t[256];
typedef char str512_t[512];
typedef float vec_t;
typedef float aimLimits_t;
typedef int flag_t;
typedef int binmap_t;
typedef int bindex_t;
typedef int GameTypeId_t;
typedef int32_t(*debug_map)(const char*, int32_t);
typedef int32_t(*lua_CFunction)(void*);
typedef void(*lua_Hook)(lua_State*, lua_Debug*);
int identity_map(const char* filename, int lua_line);
typedef void (*LogFunc)(lua_State*, const char*, ...);
typedef void (*EmergencyGCFailFunc)(lua_State*, size_t);
typedef CallSite* ErrorHandler;
typedef int GcCost;
typedef Object Register;

enum ClientNum_t : int32_t /*int8_t*/
{
    CLIENT_INDEX_0 = 0,
    CLIENT_INDEX_FIRST = 0,
    CLIENT_INDEX_1 = 1,
    CLIENT_INDEX_2 = 2,
    CLIENT_INDEX_3 = 3,
    CLIENT_INDEX_4 = 4,
    CLIENT_INDEX_5 = 5,
    CLIENT_INDEX_6 = 6,
    CLIENT_INDEX_7 = 7,
    CLIENT_INDEX_8 = 8,
    CLIENT_INDEX_9 = 9,
    CLIENT_INDEX_10 = 10,
    CLIENT_INDEX_11 = 11,
    CLIENT_INDEX_12 = 12,
    CLIENT_INDEX_13 = 13,
    CLIENT_INDEX_14 = 14,
    CLIENT_INDEX_15 = 15,
    CLIENT_INDEX_16 = 16,
    CLIENT_INDEX_17 = 17,
    CLIENT_INDEX_18 = 18,
    CLIENT_INDEX_COUNT = 19,
    CLIENT_INDEX_19 = 20,
    CLIENT_INDEX_20 = 21,
    CLIENT_INDEX_21 = 22,
    CLIENT_INDEX_22 = 23,
    CLIENT_INDEX_23 = 24,
    CLIENT_INDEX_24 = 25,
    CLIENT_INDEX_25 = 26,
    CLIENT_INDEX_26 = 27,
    CLIENT_INDEX_27 = 28,
    CLIENT_INDEX_28 = 29,
    CLIENT_INDEX_29 = 30,
    CLIENT_INDEX_30 = 31,
    CLIENT_INDEX_31 = 32,
    INVALID_CLIENT_INDEX = -1
};

struct vec3_t // 0xC
{
    float x;
    float y;
    float z;
};

struct vec2_t // 0x8
{
    float x;
    float y;
};

struct animationNumber_t // 0x2
{
    uint16_t num;
};

struct PlayerWeaponHandState_t // 0x18
{
    float weaponHandTime;             // 0x0
    float weaponDelay;                // 0x4
    int32_t weaponState;              // enum?, 0x8
    int32_t weaponShotCount;          // 0xC
    animationNumber_t weapAnim;       // 0x10
    uint8_t pad[2];                   // 0x12
    int32_t weaponFrameTimeRemainder; // float?, 0x14
};

enum EViewAngleEaseMode : int32_t
{
    VAEM_LINEAR = 0,
    VAEM_CUBIC,
    VAEM_QUADRATIC,
    VAEM_SINUSOIDAL
};

struct playerViewAngleState_t // 0x24
{
    int32_t flags;               // 0x0
    float time;                  // 0x4
    int32_t startTimeMs;         // 0x8
    int32_t durationMs;          // 0xC
    EViewAngleEaseMode easeMode; // 0x10
    vec2_t startAngles;          // 0x14
    vec2_t goalAngles;           // 0x1C
};

struct AnimScriptedState // 0xC
{
    float time;             // 0x0
    float rate;             // 0x4
    animationNumber_t anim; // 0x8
    uint8_t pad[2];         // 0xA
};

enum PlayerLensState : int32_t
{
    LENS_STATE_DEFAULT = 0,
    LENS_STATE_WALK,
    LENS_STATE_SPRINT,
    LENS_STATE_SWIM,
    LENS_STATE_SWIMSPRINT,
    LENS_STATE_SLIDE,
    LENS_STATE_MAX
};

struct PlayerLens // 0x10
{
    PlayerLensState lensState; // 0x0
    float focalLength;         // 0x4
    float focalDistance;       // 0x8
    float fStop;               // 0xC
};

struct PlayerVehicleState // 0x84
{
    vec3_t origin;                  // 0x0
    vec3_t angles;                  // 0xC
    vec3_t velocity;                // 0x18
    vec3_t angVelocity;             // 0x24
    vec2_t tilt;                    // 0x30
    vec2_t tiltVelocity;            // 0x38
    float targetHeightDelta;        // 0x40
    float lastGroundHeight;         // 0x44
    int32_t entity;                 // struct ptr?, 0x48
    int32_t flags;                  // enum?, 0x4C
    bool fullPhysics;               // 0x50
    uint8_t pad[3];                 // 0x51
    int32_t focusEntNum;            // 0x54
    float focusEntTime;             // 0x58
    vec2_t focusAngleOffset;        // 0x5C
    int32_t playerHeightMapsActive; // ptr?, 0x64
    float boostTimeLeft;            // 0x68
    vec3_t jitterEndTime;           // 0x6C
    vec3_t jitterAccel;             // 0x78
};

struct Weapon // 0x8
{
    struct
    {
        int64_t weaponData; // 0x0
    };
};

enum OffhandSecondaryClass : int32_t
{
    PLAYER_OFFHAND_SECONDARY_SMOKE = 0,
    PLAYER_OFFHAND_SECONDARY_FLASH,
    PLAYER_OFFHAND_SECONDARIES_TOTAL = 2
};

enum OffhandPrimaryClass : int32_t
{
    PLAYER_OFFHAND_PRIMARY_FRAG = 0,
    PLAYER_OFFHAND_PRIMARY_GEAR,
    PLAYER_OFFHAND_PRIMARIES_TOTAL = 2
};

struct renderOptions_t // 0x10
{
    struct
    {
        uint8_t pad[16]; // 0x0 ???
    };
};

union AttachmentCosmeticVariantIndexes // 0x4
{
    uint8_t anon[4]; // 0x0 ??
    uint8_t data[4]; // 0x0 ???
};

enum QueuedActionState : int32_t
{
    QAS_STATE_NONE = 0,
    QAS_STATE_FIRE_WEAPON,
    QAS_STATE_RELOAD_WEAPON,
    QAS_STATE_CHAIN_MELEE,
    QAS_STATE_MAX
};

struct PlayerHeldWeapons // 0x30
{
    Weapon weapon;                                                     // 0x0
    renderOptions_t options;                                           // 0x8
    AttachmentCosmeticVariantIndexes attachmentCosmeticVariantIndexes; // 0x18
    float heatPercent;                                                 // 0x1C
    float fuelTankTime;                                                // 0x20
    int32_t adsZoomSelect;                                             // 0x24
    bool overHeating;                                                  // 0x28
    bool needsRechamber;                                               // 0x29
    bool heldBefore;                                                   // 0x2A
    bool quickReload;                                                  // 0x2B
    bool blockWeaponPickup;                                            // 0x2C
    uint8_t pad_2D[3];                                                 // 0x2D
};

struct AmmoPool // 0x4
{
    int32_t count; // 0x0
};

struct AmmoClip // 0x4
{
    int32_t count; // 0x0
};

enum ViewLockTypes : int32_t
{
    PLAYERVIEWLOCK_NONE = 0,
    PLAYERVIEWLOCK_FULL,
    PLAYERVIEWLOCK_WEAPONJITTER,
    PLAYERVIEWLOCKCOUNT
};

enum locSel_t : int32_t
{
    LOC_SEL_NONE = 0,
    LOC_SEL_ARTILLERY,
    LOC_SEL_AIRSTRIKE,
    LOC_SEL_MORTAR,
    LOC_SEL_NAPALM,
    LOC_SEL_COMLINK
};

struct SprintState // 0x28
{
    int32_t sprintButtonUpRequired; // struct?, 0x0
    int32_t sprintRestore;          // struct?, 0x4
    float sprintRestoreDelayStart;  // 0x8
    float sprintDelay;              // 0xC
    float lastSprintStart;          // 0x10
    float lastSprintEnd;            // 0x14
    float sprintStartMaxLength;     // 0x18
    uint8_t dir;                    // enum?, 0x1C
    uint8_t pad[3];                 // 0x1D
    float sprintDuration;           // 0x20
    float sprintCooldown;           // 0x24
};

struct MantleState // 0x10
{
    float yaw;          // 0x0
    float timer;        // 0x4
    int32_t transIndex; // enum?, 0x8
    int32_t flags;      // enum?, 0xC
};

enum trmAnim_t : int32_t
{
    TRM_ROOT = 0,
    TRM_STEP_F = 1,
    TRM_STEP_B = 2,
    TRM_STEP_L = 3,
    TRM_STEP_R = 4,
    TRM_ON_LOW_FORWARD_32 = 5,
    TRM_ON_LOW_BACK_32 = 6,
    TRM_ON_LOW_LEFT_32 = 7,
    TRM_ON_LOW_RIGHT_32 = 8,
    TRM_ON_MED_FORWARD_60 = 9,
    TRM_ON_MED_LEFT_50 = 0xA,
    TRM_ON_MED_RIGHT_50 = 0xB,
    TRM_OVER_LOW_FORWARD_VAULT = 0xC,
    TRM_FIRST_OVER_ANIM = 0xC,
    TRM_OVER_LOW_FORWARD = 0xD,
    TRM_OVER_LOW_BACK = 0xE,
    TRM_OVER_LOW_LEFT = 0xF,
    TRM_OVER_LOW_RIGHT = 0x10,
    TRM_OVER_HIGH_75 = 0x11,
    TRM_OVER_HIGH_100 = 0x12,
    TRM_FIRST_NONCOMBAT_ANIM = 0x12,
    TRM_OVER_HIGH_140 = 0x13,
    TRM_LAST_OVER_ANIM = 0x13,
    TRM_ON_HIGH_100 = 0x14,
    TRM_ON_HIGH_140 = 0x15,
    TRM_ANIM_COUNT = 0x16
};

struct TRMState // 0x30
{
    float yaw;                 // 0x0
    float edgeYaw;             // 0x4
    float timer;               // 0x8
    int32_t flags;             // 0xC
    trmAnim_t animIndex;       // 0x10
    vec3_t moveVec;            // 0x14
    float animTimeScale;       // 0x20
    vec2_t restoreAngles;      // 0x24
    float doubleJumpEndedTime; // 0x2C
};

struct GrappleState // 0xC
{
    vec3_t startPos; // 0x0
};

enum SlideType : int32_t
{
    SLIDE_TYPE_BOOST = 0,
    SLIDE_TYPE_REDUCED,
    SLIDE_TYPE_BASE,
    SLIDE_TYPE_COUNT
};

struct SlideState // 0x1C
{
    vec3_t slideAngleOffsets; // 0x0
    int32_t flags;            // enum?, 0xC
    float slideTime;          // 0x10
    int32_t subsequentCount;  // 0x14
    SlideType slideType;      // 0x18
};

struct JukeState // 0xC
{
    float startTime; // 0x0
    float jukeYaw;   // 0x4
    int32_t flags;   // enum?, 0x8
};

struct SlamState // 0xC
{
    float startTime; // 0x0
    float slamYaw;   // 0x4
    int32_t flags;   // enum?, 0x8
};

enum DoubleJumpStateType : int32_t
{
    DOUBLEJUMP_STATE_OFF = 0,
    DOUBLEJUMP_STATE_FIRST_UP_BOOST = 1,
    DOUBLEJUMP_STATE_UP_BOOST = 2,
    DOUBLEJUMP_STATE_IN_AIR = 3,
    NUM_DOUBLEJUMP_STATES = 4
};

enum DoubleJumpBoostUpDirection : int32_t
{
    DOUBLEJUMP_BOOST_UP_NONE = 0,
    DOUBLEJUMP_BOOST_UP_LEFT = 1,
    DOUBLEJUMP_BOOST_UP_RIGHT = 2,
    DOUBLEJUMP_BOOST_UP_FORWARD = 3,
    DOUBLEJUMP_BOOST_UP_BACKWARD = 4,
    NUM_DOUBLEJUMP_BOOST_UP = 5
};

enum gadgetButtonInput_e : int32_t
{
    GADGET_BUTTON_NO_CHANGE = 0,
    GADGET_BUTTON_PRESSED = 1,
    GADGET_BUTTON_HELD = 2,
    GADGET_BUTTON_RELEASED = 3,
    GADGET_BUTTON_COUNT = 4
};

struct playerGadgetState_t // 0x20
{
    int32_t gadgetPlayerStateFlags;        // enum?, 0x0
    float gadgetLastUsedTime;              // 0x4
    float gadgetFlickerTime;               // 0x8
    int32_t gadgetEntNum;                  // 0xC
    float gadgetPressTime;                 // 0x10
    float gadgetActivateTime;              // 0x14
    float gadgetPowerRemaining;            // 0x18
    gadgetButtonInput_e gadgetButtonState; // 0x1C
};

enum cybercomTypes : int32_t
{
    CCOM_TYPE_CONTROL = 0,
    CCOM_TYPE_MARTIAL = 1,
    CCOM_TYPE_CHAOS = 2,
    CCOM_TYPE_COUNT = 3
};

struct CybercomState // 0x10 / 0x4
{
    uint8_t abilityFlags[3];   // 0x0
    uint8_t upgradeFlags[3];   // 0x3
    uint8_t tacticalRigFlags;  // enum?, 0x6
    uint8_t pad_7;             // 0x7
    cybercomTypes activeType;  // 0x8
    bool disableTacticalWheel; // 0xC
    uint8_t pad_D[3];          // 0xD
};

struct WallRunState // 0x2C
{
    int32_t startTimeMs;           // 0x0
    int32_t endTimeMs;             // 0x4
    int16_t flags;                 // 0x8
    uint8_t pad[2];                // 0xA
    vec3_t normal;                 // 0xC
    float startHeight;             // 0x18
    float verticality;             // 0x1C
    float rollFrac;                // 0x20
    int32_t timeLastInputReceived; // 0x24
    int32_t lastShotMs;            // 0x28
};

struct ClimbState // 0x1C
{
    vec3_t normal;  // 0x0
    vec3_t center;  // 0xC
    int8_t flags;   // 0x18
    uint8_t pad[2]; // 0x1A
};

struct LeapState // 0x8
{
    float volumeTimer; // 0x0
    int32_t state;     // enum?, 0x4
};

struct AnimMovement // 0x38
{
    vec3_t startOrigin;        // 0x0
    vec3_t originErrorNormal;  // 0xC
    float timer;               // 0x18
    float originErrorDistance; // 0x1C
    float startYaw;            // 0x20
    float errorYaw;            // 0x24
    float timeScale;           // 0x28
    int32_t targetFlags;       // enum?, 0x2C
    int32_t flags;             // enum?, 0x30
    float lerpTime;            // 0x34
};

// vec4_t::$E8049E02A67BEF20C2B48C1E90A72E45
struct real_vec4_t // real vec4_t?, 0x10 / 0x4
{
    float x; // 0x0
    float y; // 0x4
    float z; // 0x8
    float w; // 0xC
};

// vec4_t::$65A5F67E76558B5B186374890F5F7384
struct color_t // maybe colors struct?, 0x10 / 0x4
{
    float r; // 0x0
    float g; // 0x4
    float b; // 0x8
    float a; // 0x10
};

union vec4_t // 0x10 / 0x4
{
    float v;           // 0x0
    real_vec4_t anon0; // 0x0 ????
    color_t anon1;     // 0x0;
};

enum MeleeDirection : int32_t
{
    MELEE_DIR_NONE = 0,
    MELEE_DIR_LEFT = 1,
    MELEE_DIR_RIGHT = 2,
    MELEE_DIR_FINISH = 3,
    MELEE_DIR_FINISH_CLOSE = 4,
    MELEE_DIR_COUNT = 5
};

struct playerStateCameraInfo // 0x2C
{
    int32_t xcamIndex;           // 0x0
    float xcamStartTime;         // 0x4
    float xcamLerpEndTime;       // 0x8
    int32_t xcamSubCamera;       // struct ptr?, 0xC
    vec3_t xcamBundleOrigin;     // 0x10
    vec4_t xcamBundleAnglesQuat; // 0x1C
};

enum ActionSlotType : int32_t
{
    ACTIONSLOTTYPE_DONOTHING = 0,
    ACTIONSLOTTYPE_SPECIFYWEAPON = 1,
    ACTIONSLOTTYPE_ALTWEAPONTOGGLE = 2,
    ACTIONSLOTTYPECOUNT = 3
};

struct ActionSlotParam // 0x8
{
    struct
    {
        int64_t anon; // ptr?, 0x0
    };
};

struct PlayerRenderInfo // 0x8
{
    uint8_t lightingState;        // enum?, 0x0
    uint8_t pad[3];               // 0x1
    float sunShadowSplitDistance; // 0x4
};

enum objectiveState_t : int32_t
{
    OBJST_EMPTY = 0,
    OBJST_ACTIVE = 1,
    OBJST_INVISIBLE = 2,
    OBJST_DONE = 3,
    OBJST_CURRENT = 4,
    OBJST_FAILED = 5,
    OBJST_NUMSTATES = 6
};

enum objectiveDrawState_t : int32_t
{
    OBJECTIVE_DRAW_STATE_ONSCREEN = 0,
    OBJECTIVE_DRAW_STATE_OFFSCREEN = 1
};

enum UIModelDataType : int32_t
{
    UI_MODEL_DT_INVALID = 0,
    UI_MODEL_DT_BOOL = 1,
    UI_MODEL_DT_UINT64 = 2,
    UI_MODEL_DT_INT = 3,
    UI_MODEL_DT_REAL = 4,
    UI_MODEL_DT_STRING = 5,
    UI_MODEL_DT_FUNCTION = 6,
    UI_MODEL_NUM_DATATYPES = 7
};

struct UIModelData // 0x10 / 0x8
{
    UIModelDataType dataType; // 0x0
    uint8_t pad[4];           // 0x4
    struct
    {
        int64_t anon; // ptr? // 0x8
    };
};

struct objectiveUIModel_t // 0x20 / 0x10
{
    UIModelData modelData; // 0x0
    uint8_t modelName[4];  // 0x10
    uint8_t pad[12];       // 0x14
};

struct objective_t // 0xD0
{
    objectiveState_t objState;      // 0x0
    vec3_t origin;                  // 0x4
    int32_t entnum;                 // 0x10
    int32_t color;                  // enum?, 0x14
    int32_t colorSetColor;          // enum?, 0x18
    uint8_t pad_1A[2];              // 0x1A
    vec2_t objIconSize;             // 0x1C
    int32_t icon;                   // ptr?, 0x24
    int32_t objOwnerNum;            // 0x28
    uint8_t name[2];                // idk?, 0x2C
    uint8_t teamMask;               // mask enum, 0x2E
    uint8_t progress;               // 0x30
    uint8_t pad_31[3];              // 0x31
    int32_t clientUseMask;          // mask enum, 0x34
    int16_t gamemodeFlags;          // flag enum, 0x38
    uint8_t objTeamNum;             // 0x3A
    uint8_t pad_3B;                 // 0x3B
    objectiveDrawState_t drawState; // 0x3C
    float alpha;                    // 0x44
    uint8_t syncFlag;               // flag enum, 0x48
    uint8_t pad_49[7];              // 0x49
    objectiveUIModel_t uiModels[4]; // 0x50
};

union hudelem_color_t // 0x4
{
    struct
    {
        int16_t anon; // ?, 0x0
        int16_t rgba; // 0x0
    };
};

struct hudelem_t // 0x7C / 0x4
{
    float x;                        // 0x0
    float y;                        // 0x4
    float z;                        // 0x8
    float fontScale;                // 0xC
    float fromFontScale;            // 0x10
    float fontScaleStartTime;       // 0x14
    hudelem_color_t color;          // 0x18
    hudelem_color_t fromColor;      // 0x1C
    float fadeStartTime;            // 0x20
    float scaleStartTime;           // 0x24
    float fromX;                    // int32?, 0x28
    float fromY;                    // int32?, 0x2C
    float moveStartTime;            // 0x30
    int32_t time;                   // 0x34
    float duration;                 // 0x38
    int32_t value;                  // 0x3C
    int32_t sort;                   // enum?, 0x40
    hudelem_color_t glowColor;      // 0x44
    float fxBirthTime;              // 0x48
    int32_t flags;                  // enum?, 0x4C
    int16_t targetEntNum;           // 0x50
    int16_t fontScaleTime;          // 0x52
    int16_t fadeTime;               // 0x54
    int16_t label;                  // enum?, 0x56
    int16_t width;                  // 0x58
    int16_t height;                 // 0x5A
    int16_t fromWidth;              // 0x5C
    int16_t fromHeight;             // 0x5E
    int16_t scaleTime;              // 0x60
    int16_t moveTime;               // 0x62
    uint8_t text[2];                // ptr??, 0x64
    int16_t fxLetterTime;           // 0x66
    int16_t fxDecayStartTime;       // 0x68
    int16_t fxRedactDecayStartTime; // 0x6C
    int16_t dxRedactDecayDuration;  // 0x6E
    uint8_t type;                   // enum?, 0x70
    uint8_t font;                   // enum?, 0x71
    uint8_t alignOrg;               // 0x72
    uint8_t alignScreen;            // 0x73
    int16_t materialIndex;          // 0x74
    int16_t offscreenMaterialIdx;   // 0x76
    uint8_t fromAlignOrg;           // 0x78
    uint8_t fromAlignScreen;        // 0x79
    uint8_t soundID;                // enum?, 0x7A
    uint8_t ui3dWindow;             // enum?, 0x7B
};

// playerState_s::$962AF25CB0686328134BBE37A36CE330
struct derived_playerState_s // 0x1E08 / 0x4
{
    hudelem_t current[31];  // 0x0
    hudelem_t archival[31]; // 0xF04
};

struct serverHudMenu_t // 0x150
{
    UIModelData modelData[16]; // 0x0
    int32_t menuNameIndex;     // 0x100
    int32_t modelNames[16];    // ptrs?, 0x104
    uint8_t persistent;        // 0x144
    uint8_t pad[11];           // 0x145
};

// playerState_s::$A47769759799D7CCCA7DA06FAC778FA3
struct derived2_playerState_s // 0xA0 / 0x8
{
    UIModelData modelData[8]; // 0x0
    int32_t modelNames[8];    // ptrs??, 0x80
};

struct playerState_t // 0xBC00
{
    ClientNum_t clientNum;                                             // 0x0
    int32_t commandTime;                                               // 0x4
    int32_t pm_type;                                                   // enum?, 0x8
    float bobCycle;                                                    // 0xC
    int64_t pm_flags;                                                  // 0x10
    int64_t weapFlags;                                                 // enum, 0x18
    int32_t otherFlags;                                                // enum, 0x20
    float pm_time;                                                     // 0x24
    int32_t loopSoundId;                                               // enum?, 0x28
    float loopSoundFade;                                               // 0x2C
    vec3_t origin;                                                     // ptr?, 0x30
    vec3_t velocity;                                                   // ptr?, 0x3C
    int32_t remoteEyesEnt;                                             // prob ptr, 0x48
    uint8_t remoteEyesTagName[4];                                      // or string ptr, 0x4C
    int32_t remoteControlEnt;                                          // prob ptr, 0x50
    PlayerWeaponHandState_t handState[2];                              // 0x54
    int32_t movementState;                                             // enum?, 0x84
    float movementTime;                                                // 0x88
    float weaponIdleTime;                                              // 0x8C
    float grenadeTimeLeft;                                             // 0x90
    int32_t throwBackGrenadeOwner;                                     // struct ptr?, 0x94
    float throwBackGrenadeTimeLeft;                                    // 0x98
    float weaponRestrictKickTime;                                      // 0x9C
    bool mountAvailable;                                               // 0xA0
    bool bCarryingTurret;                                              // 0xA1
    uint8_t pad_A2[2];                                                 // 0xA2
    vec3_t mountPos;                                                   // 0xA4
    float mountDir;                                                    // 0xB0
    bool bThirdPerson;                                                 // 0xB4
    uint8_t pad_B5[3];                                                 // 0xB5
    float foliageSoundTime;                                            // 0xB8
    float gravity;                                                     // 0xBC
    float leanf;                                                       // 0xC0
    float speed;                                                       // 0xC4
    vec3_t delta_angles;                                               // 0xC8
    playerViewAngleState_t viewAngleState;                             // 0xD4
    int32_t groundEntityNum;                                           // 0xF8
    int32_t moverEntityNum;                                            // 0xFC
    int32_t moverTimestamp;                                            // 0x100
    int32_t groundType;                                                // enum, 0x104
    float jumpTime;                                                    // 0x108
    float jumpOriginZ;                                                 // 0x10C
    int32_t moveType;                                                  // enum, 0x110
    float legsTimer;                                                   // 0x114
    float torsoTimer;                                                  // 0x118
    animationNumber_t legsAnim;                                        // 0x11C
    animationNumber_t torsoAnim;                                       // 0x11E
    float legsAnimDuration;                                            // 0x120
    float torsoAnimDuration;                                           // 0x124
    AnimScriptedState animScripted;                                    // 0x128
    float damageTimer;                                                 // 0x134
    float damageDuration;                                              // 0x138
    float dmgDirection;                                                // 0x13C
    int32_t dmgType;                                                   // enum, 0x140
    int32_t corpseIndex;                                               // 0x144
    float movementDir;                                                 // 0x148
    int32_t eFlags;                                                    // enum, 0x14C
    int32_t eFlags2;                                                   // enum, 0x150
    PlayerLens playerLens;                                             // 0x154
    PlayerVehicleState vehicleState;                                   // 0x164
    int16_t predictableEventSequence;                                  // 0x1E8 idk
    int16_t predictableEventSequenceOld;                               // ptr?, 0x1EA
    int32_t predictableEvents[4];                                      // 0x1EC idk
    uint8_t pad_1FC[4];                                                // 0x1FC
    int64_t predictableEventParms[4];                                  // enum?, 0x200
    int16_t unpredictableEventSequence;                                // 0x220 idk
    int16_t unpredictableEventSequenceOld;                             // ptr?, 0x222
    int32_t unpredictableEvents[4];                                    // 0x224 idk
    uint8_t pad_234[4];                                                // 0x234
    int64_t unpredictableEventParms[4];                                // enum?, 0x238
    Weapon offHandWeapon;                                              // 0x258
    OffhandSecondaryClass offhandSecondary;                            // 0x260
    OffhandPrimaryClass offhandPrimary;                                // 0x264
    renderOptions_t renderOptions;                                     // 0x268
    AttachmentCosmeticVariantIndexes attachmentCosmeticVariantIndexes; // 0x278
    float momentum;                                                    // 0x27C
    Weapon weapon;                                                     // 0x280
    Weapon lastStandPrevWeapon;                                        // 0x288
    Weapon lastWeaponAltModeSwitch;                                    // 0x290
    Weapon stowedWeapon;                                               // 0x298
    uint8_t unusedCompatibilitypad;                                    // 0x2A0
    uint8_t pad_2A1[7];                                                // 0x2A1
    Weapon meleeWeapon;                                                // 0x2A8
    Weapon lastFiredGadget;                                            // 0x2B0
    float fWeaponPosFrac;                                              // 0x2B8
    float fMoveSpeedTransitionLerp;                                    // 0x2BC
    float fIdleSpeedFromFireTransitionLerp;                            // 0x2C0
    float adsDelayTime;                                                // 0x2C4
    float spreadOverride;                                              // 0x2C8
    int32_t spreadOverrideState;                                       // enum, 0x2CC
    int32_t weaponHealth;                                              // 0x2D0
    float weaponSpinLerp;                                              // 0x2D4
    QueuedActionState queuedActionState;                               // 0x2D8
    vec3_t viewangles;                                                 // 0x2DC
    float viewHeightTarget;                                            // 0x2E8
    float viewHeightCurrent;                                           // 0x2EC
    float viewHeightLerpTime;                                          // 0x2F0
    float viewHeightLerpTarget;                                        // 0x2F4
    float viewHeightLerpDown;                                          // 0x2F8
    vec2_t viewAngleClampBase;                                         // 0x2FC
    vec2_t viewAngleClampRange;                                        // 0x304
    int32_t damageEvent;                                               // enum?, 0x30C
    float damageYaw;                                                   // 0x310
    float damagePitch;                                                 // 0x314
    int32_t damageCount;                                               // 0x318
    int32_t stats[4];                                                  // 0x31C
    uint8_t pad_32C[4];                                                // 0x32C
    PlayerHeldWeapons heldWeapons[15];                                 // 0x330
    AmmoPool ammoNotInClip[15];                                        // 0x600
    AmmoClip ammoInClip[15];                                           // 0x63C
    float proneDirection;                                              // 0x678
    float proneDirectionPitch;                                         // 0x67C
    float proneTorsoPitch;                                             // 0x680
    ViewLockTypes viewlocked;                                          // 0x684
    int16_t viewlocked_entNm;                                          // 0x688
    uint8_t pad_68A[2];                                                // 0x68A
    float vehiclePos;                                                  // 0x68C
    int32_t vehicleType;                                               // enum, 0x690
    int32_t vehicleAnimBoneIndex;                                      // 0x694
    int32_t linkFlags;                                                 // enum, 0x698
    vec3_t linkAngles;                                                 // 0x69C
    int32_t cursorHint;                                                // ptr?, 0x6A8
    uint8_t cursorHintString[4];                                       // ptr?, 0x6AC
    int32_t cursorHintEntIndex;                                        // 0x6B0
    uint8_t pad_6B4[4];                                                // 0x6B4
    Weapon cursorHintWeapon;                                           // 0x6B8
    int32_t interactivePromptEntIndex;                                 // 0x6C0
    int32_t iCompassPlayerInfo;                                        // struct ptr?, 0x6C4
    int32_t spyplaneTypeEnabled;                                       // struct?, 0x6C8
    int32_t satelliteTypeEnabled;                                      // struct?, 0x6CC
    float spyplaneTime;                                                // 0x6D0
    float satelliteTime;                                               // 0x6D4
    int32_t locationSelectionInfo;                                     // ptr?, 0x6D8
    locSel_t locationSelectionType;                                    // 0x6DC
    SprintState sprintState;                                           // 0x6E0
    float lastDtpEnd;                                                  // ?, 0x708
    float lastCtxCover;                                                // ?, 0x70C
    float fTorsoPitch;                                                 // 0x710
    float fWaistPitch;                                                 // 0x714
    float holdBreathScale;                                             // 0x718
    float holdBreathTimer;                                             // 0x71C
    float chargeShotTimer;                                             // 0x720
    float chargeShotLevel;                                             // ?, 0x724
    int32_t shotsFiredFromChamber;                                     // 0x728
    float quickScopeScale;                                             // 0x72C
    float quickScopeTimer;                                             // 0x730
    uint32_t privateClientFields[4];                                   // store ptrs?, 0x734
    uint32_t clientFields[4];                                          // store ptrs?, 0x744
    float moveSpeedScaleMultiplier;                                    // 0x754
    MantleState mantleState;                                           // 0x758
    TRMState trmState;                                                 // 0x768
    GrappleState grappleState;                                         // 0x798
    SlideState slideState;                                             // 0x7A4
    JukeState jukeState;                                               // 0x7C0
    SlamState slamState;                                               // 0x7CC
    float impactTime;                                                  // 0x7D8
    float impactIntensity;                                             // 0x7DC
    DoubleJumpStateType doubleJumpState;                               // 0x7E0
    DoubleJumpBoostUpDirection doubleJumpBoostUpDirection;             // 0x7E4
    float doubleJumpPitchOffset;                                       // 0x7E8
    float doubleJumpRollOffset;                                        // 0x7EC
    float playerEnergyRatio;                                           // 0x7F0
    float playerEnergyTime;                                            // 0x7F4
    playerGadgetState_t playerGadgetState[3];                          // 0x7F8
    CybercomState cybercomState;                                       // 0x858
    WallRunState wallRunState;                                         // 0x868
    float playerSwimAnimRate;                                          // 0x894
    ClimbState climbState;                                             // 0x898
    LeapState leapState;                                               // 0x8B4
    AnimMovement animMovement;                                         // 0x8BC
    int32_t vehicleAnimState;                                          // enum?, 0x8F4
    int32_t vehicleEntryPoint;                                         // ptr?, 0x8F8
    float meleeChargeDist;                                             // 0x8FC
    float meleeChargeTime;                                             // 0x900
    int32_t meleeChargeEnt;                                            // ptr?, 0x904
    vec4_t meleeConstraintPlane;                                       // 0x908
    float meleeStartTime;                                              // 0x918
    int32_t meleeServerResult;                                         // enum?, 0x91C
    int32_t meleeState;                                                // enum?, 0x920
    int32_t blockMeleeUsageTimer;                                      // ?, 0x924
    int32_t meleeAttacker;                                             // ptr?, 0x928
    float meleeCounterTimer;                                           // 0x92C
    bool canChainMelee;                                                // 0x930
    uint8_t pad_931;                                                   // 0x931
    int32_t chainMeleeCounter;                                         // 0x934
    MeleeDirection meleeDir;                                           // 0x938
    playerStateCameraInfo mainCam;                                     // 0x93C
    playerStateCameraInfo extraCam[4];                                 // 0x968
    int32_t weapLockFlags[5];                                          // 0xA18
    int32_t weapLockedEntnum[5];                                       // 0xA2C
    int32_t weapLockPrivotEntnum;                                      // 0xA40
    float weapLockTimer;                                               // 0xA44
    float lockOnRequiredMsgSuppressAfterFireTimer;                     // 0xA48
    int32_t perks[3];                                                  // struct ptrs?, 0xA4C
    int32_t perksCache[3];                                             // struct ptrs?, 0xA58
    ActionSlotType actionSlotType;                                     // 0xA64
    uint8_t pad_A74[4];                                                // 0xA74
    ActionSlotParam actionSlotParam[4];                                // 0xA78
    Weapon inventoryWeapon;                                            // 0xA98
    Weapon inventoryHeldWeapon;                                        // 0xAA0
    uint8_t vehicleDefIndex;                                           // 0xAA8
    uint8_t pad_AA9[3];                                                // 0xAA9
    float locBlendTime;                                                // 0xAAC
    PlayerRenderInfo renderInfo;                                       // 0xAB0
    bool attemptedDoubleJump;                                          // 0xAB8
    uint8_t pad_AB9[3];                                                // 0xAB9
    int32_t entityEventSequence;                                       // struct ptr?, 0xABC
    float aimSpreadScale;                                              // 0xAC0
    int32_t shellshockIndex;                                           // 0xAC4
    float shellshockTime;                                              // 0xAC8
    float shellshockDuration;                                          // 0xACC
    float meleeButtStartTime;                                          // 0xAD0
    float meleeButtDirection;                                          // 0xAD4
    uint8_t pad_AD5[3];                                                // 0xAD5
    float dofNearStart;                                                // 0xAD8 depth of field
    float dofNearEnd;                                                  // 0xADC
    float dofFarStart;                                                 // 0xAE0
    float dofFarEnd;                                                   // 0xAE4
    float dofNearBlur;                                                 // 0xAE8
    float dofFarBlur;                                                  // 0xAEC
    float dofViewmodelStart;                                           // 0xAF0
    float dofViewmodelEnd;                                             // 0xAF4
    float waterlevel;                                                  // ?, 0xAF8
    uint8_t walkUnderwater;                                            // 0xAFC
    uint8_t pad_AFD[3];                                                // 0xAFD
    float lastFootInWaterTime;                                         // 0xB00
    int32_t smokeColorIndex;                                           // 0xB04
    int32_t hudElemLastAssignedSoundID;                                // 0xB08
    int32_t adsZoomSelect;                                             // enum?, 0xB0C
    float adsZoomLatchTime;                                            // 0xB10
    uint8_t adsZoomLatchState;                                         // enum?, 0xB14
    uint8_t pad_B15[3];                                                // 0xB15
    int32_t adsPrevZoomSelect;                                         // 0xB18
    float adsPrevZoomSelectTime;                                       // 0xB1C
    int64_t artilleryInboundIconLocation;                              // ptr?, 0xB20
    float visionSetLerpRatio;                                          // 0xB28
    int32_t poisoned;                                                  // flag enum?, 0xB2C
    int32_t binoculars;                                                // ptr?, 0xB30
    uint8_t scriptCursorHintString[4];                                 // ptr?, 0xB34
    uint8_t pad[8];                                                    // 0xB38
    objective_t objective[128];                                        // 0xB40
    float deltaTime;                                                   // 0x7340
    int32_t killCamEntity;                                             // ptr?, 0x7344
    int32_t killCamTargetEntity;                                       // ptr?, 0x7348
    int32_t introShotsFired;                                           // 0x734C
    derived_playerState_s hud;                                         // 0x7350
    uint8_t pad_9158[8];                                               // 0x9158
    serverHudMenu_t hudMenus[32];                                      // 0x9160
    derived2_playerState_s serverModels;                               // 0xBB60
};

struct GfxEntityPreFrame // 0x40 / 0x4
{
    int32_t activePartBits[12]; // 0x0
    int32_t modelCount;         // 0x30
    int32_t numBones;           // 0x34
    int32_t numCosmeticBones;   // 0x38
    int32_t boneArrayOffset;    // 0x3C
};

struct CEntVehicleDefGround // 0x44 / 0x4
{
    int32_t wheelHeight[6];           // float?, 0x0
    float nitrousWheelRotations[6];   // 0x18
    int16_t nitrousWheelYaw[6];       // 0x30
    int16_t tag_extra_tank_wheels[4]; // 0x3C
};

struct CEntVehicleRotor // 0x10 / 0x4
{
    float rotation;    // 0x0
    float speed;       // 0x4
    int16_t tag;       // 0x8
    int16_t armTag;    // 0xA
    float armRotation; // 0xC
};

struct CEntVehicleDefAir // 0x40 / 0x4
{
    CEntVehicleRotor rotor[4]; // 0x0
};

// CEntVehicleDef::$B32A63B71929DD7B8D601BF0B18E3776
union union_CEntVehicleDef // 0x44 / 0x4
{
    CEntVehicleDefGround ground;
    CEntVehicleDefAir air;
};

struct CEntVehicleDef // 0xA8
{
    float barrelRecoil;           // 0x0
    int16_t pitch;                // 0x4
    int16_t roll;                 // 0x6
    int16_t barrelYaw;            // 0x8
    int16_t barrelPitch;          // 0xA
    int16_t steerYaw;             // 0xC
    int16_t steerPitch;           // 0xE
    int16_t gunnerPitch[4];       // 0x10
    int16_t gunnerYaw[4];         // 0x18
    int16_t minigunRotation;      // 0x20
    int16_t antennaPitch[4];      // 0x22
    int16_t antennaRoll[4];       // 0x2A
    int16_t wheelBoneIndex[6];    // 0x32
    int16_t tag_body;             // 0x3E
    int16_t tag_turret;           // 0x40
    int16_t tag_barrel;           // 0x42
    int16_t tag_barrel_recoil;    // 0x44
    int16_t tag_gunner_turret[4]; // 0x46
    int16_t tag_gunner_barrel[4]; // 0x4E
    int16_t tag_player;           // 0x56
    int16_t tag_minigun_spin;     // 0x58
    int16_t tag_antennas[4];      // 0x5A
    uint8_t pad_62[2];            // 0x62
    union_CEntVehicleDef anon;    // 0x64
};

struct CEntPlayerInfo // 0x30
{
    int64_t control;              // ptr?, 0x0
    int16_t tag[6];               // 0x8
    int32_t waterHeight;          // 0x14
    int32_t nextWaterHeightCheck; // 0x18
    float nextRippleTime;         // 0x1C
    vec3_t viewAngles;            // 0x20
    uint8_t pad_2C[4];            // 0x2C
};

struct CEntFx // 0x8 / 0x4
{
    float triggerTime; // 0x0
    int32_t effect;    // enum?, 0x4
};

union union_CEntActorInfo // 0x4
{
    float height;
    float yaw;
};

struct CEntActorInfo // 0x28 / 0x4
{
    int32_t proneType;            // enum?, 0x0
    float pitch;                  // 0x4
    float roll;                   // 0x8
    int16_t headBone;             // 0xC
    uint8_t pad_E[2];             // 0xE
    union_CEntActorInfo anon;     // 0x10
    int32_t waterHeight;          // 0x14
    int32_t nextWaterHeightCheck; // 0x18
    float nextRippleTime;         // 0x1C
    float lookAtBlend;            // 0x20
    uint8_t lookAtFlags;          // 0x24
    uint8_t pad_25[3];            // 0x25
};

struct CEntGeneral // 0x20 / 0x4
{
    int32_t contents; // data?, 0x0
    uint8_t linked;   // bool?, 0x4
    uint8_t pad_5[3]; // 0x5
    vec3_t mins;      // 0x8
    vec3_t maxs;      // 0x14
};

// cpose_t::$354C91ED3657114219504D927665E1A2
union union_cpose_t // 0xA8
{
    CEntPlayerInfo player;
    CEntVehicleDef vehicle;
    CEntFx fx;
    CEntActorInfo actor;
    CEntGeneral general;
};

struct GpuShaderConstantSet // 0x180 / 0x4
{
    vec4_t scriptVector0;   // 0x0
    vec4_t scriptVector1;   // 0x10
    vec4_t scriptVector2;   // 0x20
    vec4_t scriptVector3;   // 0x30
    vec4_t scriptVector4;   // 0x40
    vec4_t scriptVector5;   // 0x50
    vec4_t scriptVector6;   // 0x60
    vec4_t scriptVector7;   // 0x70
    vec4_t weaponParam0;    // 0x80
    vec4_t weaponParam1;    // 0x90
    vec4_t weaponParam2;    // 0xA0
    vec4_t weaponParam3;    // 0xB0
    vec4_t weaponParam4;    // 0xC0
    vec4_t weaponParam5;    // 0xD0
    vec4_t weaponParam6;    // 0xE0
    vec4_t weaponParam7;    // 0xF0
    vec4_t characterParam0; // 0x100
    vec4_t characterParam1; // 0x110
    vec4_t characterParam2; // 0x120
    vec4_t characterParam3; // 0x130
    vec4_t characterParam4; // 0x140
    vec4_t characterParam5; // 0x150
    vec4_t characterParam6; // 0x160
    vec4_t characterParam7; // 0x170
};

struct ShaderConstantSet // 0x184 / 0x4
{
    GpuShaderConstantSet values; // 0x0
    int32_t used;                // 0x180
};

struct cpose_t // 0x2F0 / 0x8
{
    uint8_t eType;                   // enum?, 0x0
    uint8_t eTypePrev;               // enum?, 0x1
    uint8_t localClientNum;          // 0x2
    bool isRagdoll;                  // 0x3
    uint8_t pad_4[4];                // 0x4
    int64_t ragdollHandle;           // ptr?, 0x8
    int64_t physObjId;               // 0x10
    int64_t physUserBody;            // ptr?, 0x18
    int64_t killcamRagdollHandle;    // ptr?, 0x20
    int64_t physUserBodyProneFeet;   // ptr?, 0x28
    uint8_t destructiblePose;        // enum?, 0x30
    uint8_t pad_31[7];               // 0x31
    int64_t instancePoseHandle;      // ptr?, 0x38
    vec3_t origin;                   // 0x40
    vec3_t angles;                   // 0x4C
    vec3_t absmin;                   // 0x58
    vec3_t absmax;                   // 0x64
    GfxEntityPreFrame prevFrame;     // 0x70
    union_cpose_t anon;              // 0xB0
    ShaderConstantSet constantSet;   // 0x158
    uint8_t pad_2DC[4];              // 0x2DC
    int64_t skelCacheHandle;         // ptr?, 0x2E0
    int16_t entDuplicateRenderIndex; // 0x2E8
    uint8_t pad_2EA[6];              // 0x2EA
};

struct trajectory_t // 0x24 / 0x4
{
    uint8_t trType;   // enum?, 0x0
    uint8_t pad_1[3]; // 0x1
    float trTime;     // 0x4
    float trDuration; // 0x8
    vec3_t trBase;    // 0xC
    vec3_t trDelta;   // 0x18
};

struct LerpEntityStateTurret // 0x1C / 0x4
{
    vec3_t gunAngles;    // 0x0
    float heatVal;       // 0xC
    float overheating;   // 0x10
    int32_t pivotOffset; // 0x14
    int32_t flags;       // 0x18
};

struct LerpEntityStateLoopFx // 0x8 / 0x4
{
    int32_t cullDist; // 0x0
    int32_t period;   // 0x4
};

// LerpEntityStateActor::$3E29690DCC3315522390432792FCB2C2
union union_LerpEntityStateActor // 0x4
{
    int32_t actorNum;
    int32_t corpseNum;
};

// LerpEntityStateActor::$5A5BFBCB46F35D1CCBBDB7D8F9621770
union union2_LerpEntityStateActor // 0x2
{
    int16_t fBodyPitch;
};

struct ModelAttachmentIndex // 0x4 / 0x2
{
    int16_t model; // 0x0
    uint8_t tag;   // 0x2
    uint8_t pad_3; // 0x3
};

struct LerpEntityStateActor // 0x34 / 0x4
{
    union_LerpEntityStateActor anon;       // 0x0
    int32_t clonedFromActorNum;            // 0x4
    int32_t species;                       // 0x8
    int32_t enemy;                         // 0xC
    bool freeCameraLockOnAllowed;          // 0x10
    uint8_t missingLegs;                   // 0x11
    uint8_t aiType;                        // 0x12
    uint8_t vehicleSeat;                   // 0x13
    uint8_t ikPriority;                    // 0x14
    uint8_t pad_15;                        // 0x15
    union2_LerpEntityStateActor proneInfo; // 0x16
    ModelAttachmentIndex attachments[6];   // 0x18
    int16_t tmodeHealth;                   // 0x30
    uint8_t tmodeBehavior;                 // 0x32
    uint8_t tmodeVisibility;               // 0x33
};

struct LerpEntityStatePrimaryLight // 0x14 / 0x4
{
    uint8_t colorAndExp[4]; // 0x0
    float intensity;        // 0x4
    float radius;           // 0x8
    float cosHalfFovOuter;  // 0xC
    int32_t mixerIndex;     // 0x10
};

struct LerpEntityStateLensflare // 0x44 / 0x4
{
    float intensity;  // 0x0
    uint8_t name[64]; // 0x4
};

struct entityGadgetState_t // 0x10 / 0x8
{
    Weapon gadgetWeapon;            // 0x0
    float gadgetActivationTime;     // 0x8
    int16_t gadgetEntEnum;          // enum, 0xC
    uint8_t gadgetEntityStateFlags; // 0xE
    uint8_t pad_F;                  // 0xF
};

struct LerpEntityStatePlayer // 0x68 / 0x8
{
    Weapon primaryWeapon;                     // 0x0
    Weapon stowedWeapon;                      // 0x8
    Weapon offhandWeapon;                     // 0x10
    Weapon meleeWeapon;                       // 0x18
    entityGadgetState_t entityGadgetState[3]; // 0x20
    float leanf;                              // 0x50
    int16_t movementDir;                      // 0x54
    uint8_t moveType;                         // enum, 0x56
    uint8_t velYaw;                           // 0x57
    uint8_t velSpeed;                         // 0x58
    uint16_t locBlendTime;                    // 0x59
    uint8_t meleeWeaponCamo;                  // 0x5A
    uint8_t vehicleType;                      // enum, 0x5B
    uint8_t vehicleAnimBoneIndex;             // 0x5C
    uint8_t vehicleSeat;                      // 0x5D
    uint8_t stowedWeaponCamo;                 // 0x5E
    uint8_t weaponHeat;                       // 0x5F
    uint8_t proneLegsAngle;                   // 0x60
    uint8_t pad_61[7];                        // 0x61
};

struct LerpEntityStateVehicleGunnerAngles // 0x6 / 0x2
{
    int16_t pitch;     // 0x0
    int16_t yaw;       // 0x2
    int16_t targetEnt; // 0x4
};

struct LerpEntityStateVehicleRotorArm // 0x4 / 0x2
{
    int16_t pitch; // 0x0
    int16_t yaw;   // 0x2
};

// LerpEntityStateVehicle::$84521063438CA8F8CB6B40BD9A0ADD10
union union_LerpEntityStateVehicle // 0x2
{
    int16_t throttle;
    int16_t bodyPitch;
};

struct LerpEntityStateMissile // 0x20 / 0x4
{
    float launchTime;        // 0x0
    int32_t parentClientNum; // 0x4
    float fuseTime;          // 0x8
    int32_t forcedDud;       // float?, 0xC
    float autoDetonateTime;  // 0x10
    vec3_t passThrough;      // 0x14
};

struct LerpEntityStateVehicle // 0x40 / 0x4
{
    float steerYaw;                                     // 0x0
    float bodyRoll;                                     // 0x4
    LerpEntityStateVehicleGunnerAngles gunnerAngles[4]; // 0x8
    LerpEntityStateVehicleRotorArm rotorArms[4];        // 0x20
    LerpEntityStateVehicleGunnerAngles turret;          // 0x30
    union_LerpEntityStateVehicle anon;                  // 0x36
    int16_t clonedFromEntityNum;                        // 0x38
    uint8_t targetRotorSpeed;                           // 0x3A
    uint8_t tmodeVehicleHelath;                         // 0x3B
    uint8_t tmodeVehicleBehavior;                       // 0x3C
    uint8_t tmodeVehicleVisibility;                     // 0x3D
    uint8_t pad_3E[2];                                  // 0x3E
};

struct LerpEntityStateScriptMover // 0x12 / 0x2
{
    uint8_t attachTagIndex[4];   // 0x0
    int16_t attachModelIndex[4]; // 0x4
    int16_t exploderIndex;       // 0xC
    int16_t scale;               // 0xE
    uint8_t flags;               // 0x10
    uint8_t aiType;              // enum, 0x11
};

struct LerpEntityStateBulletHit // 0xC / 0x4
{
    vec3_t start; // 0x0
};

struct LerpEntityStateEarthquake // 0xC / 0x4
{
    float scale;    // 0x0
    float radius;   // 0x4
    float duration; // 0x8
};

struct LerpEntityStateCustomExplode // 0x8 / 0x4
{
    float startTime;     // 0x0
    int32_t effectIndex; // 0x4
};

struct LerpEntityStateExplosion // 0x14 / 0x4
{
    float innerRadius; // 0x0
    float magnitude;   // 0x4
    float innerDamage; // 0x8
    int32_t dummy;     // 0xC
    float outerDamage; // 0x10
};

struct LerpEntityStateExplosionJolt // 0x10 / 0x4
{
    float innerRadius; // 0x0
    vec3_t impulse;    // 0x4
};

struct LerpEntityStateJetThrust // 0x14 / 0x4
{
    vec3_t thrustDir; // 0x0
    int32_t dummy;    // 0xC
    float dotLimit;   // 0x10
};

struct LerpEntityStatePhysicsJitter // 0xC / 0x4
{
    float innerRadius;     // 0x0
    float minDisplacement; // 0x4
    float maxDisplacement; // 0x8
};

struct LerpEntityStateDestructibleHit // 0x18 / 0x4
{
    int32_t modelState[6]; // 0x0
};

struct LerpEntityStateCreateDynEnt // 0xC // 0x4
{
    int32_t model;  // 0x0
    int32_t fxId;   // 0x4
    int32_t mature; // 0x8
};

struct LerpEntityStateStreamerHint // 0x8 / 0x4
{
    float factor;            // 0x0
    int16_t flags;           // enum, 0x4
    int16_t lightStateIndex; // 0x6
};

struct LerpEntityStateZBarrierPiece // 0x2
{
    uint8_t flags;    // enum, 0x0
    uint8_t animTime; // 0x1
};

struct LerpEntityStateZBarrier // 0x10 / 0x4
{
    int32_t barrierTypeIndex; // 0x0
    LerpEntityStateZBarrierPiece pieces[6];
};

struct LerpEntityStateBeam // 0x14 / 0x4
{
    int32_t entnumFrom;  // 0x0
    int32_t entnumTo;    // 0x4
    int32_t tagFrom;     // 0x8
    int32_t tagTo;       // 0xC
    uint8_t beamName[4]; // string index?, 0x10
};

struct LerpEntityStateAnonymous // 0x68 / 0x4
{
    int32_t data[26]; // 0x0
};

union LerpEntityStateTypeUnion // 0x68 / 0x8
{
    LerpEntityStateTurret turret;
    LerpEntityStateLoopFx loopFx;
    LerpEntityStateActor actor;
    LerpEntityStatePrimaryLight primaryLight;
    LerpEntityStateLensflare lensFlareDef;
    LerpEntityStatePlayer player;
    LerpEntityStateVehicle vehicle;
    LerpEntityStateMissile missile;
    LerpEntityStateScriptMover scriptMover;
    LerpEntityStateBulletHit bulletHit;
    LerpEntityStateEarthquake earthquake;
    LerpEntityStateCustomExplode customExplode;
    LerpEntityStateExplosion explosion;
    LerpEntityStateExplosionJolt explosionJolt;
    LerpEntityStateJetThrust jetThrust;
    LerpEntityStatePhysicsJitter physicsJitter;
    LerpEntityStateDestructibleHit destructibleHit;
    LerpEntityStateCreateDynEnt createDynEnt;
    LerpEntityStateStreamerHint streamerHint;
    LerpEntityStateZBarrier zbarrier;
    LerpEntityStateBeam beam;
    LerpEntityStateAnonymous anonymous;
};

struct LerpEntityState // 0xD0 / 0x8
{
    int32_t eFlags;             // enum, 0x0
    int32_t eFlags2;            // 0x4
    trajectory_t pos;           // 0x8
    trajectory_t apos;          // 0x2C
    LerpEntityStateTypeUnion u; // 0x50
    int16_t useCount;           // 0xB8
    uint8_t pad_BA[2];          // 0xBA
    int32_t clientFields[4];    // 0xBC
    uint8_t pad_CC[4];          // 0xCC
};

struct animState_t // 0x20 / 0x4
{
    float deltaRequestTime;           // 0x0
    ub1 deltaSource;                  // 0x4
    ub1 deltaRowLowBits;              // 0x5
    ub1 deltaRowHighBits;             // 0x6
    ub1 aimRow;                       // 0x7
    ub1 shootRow;                     // 0x8
    ub1 transdecActive;               // 0x9
    ub1 currentAnimMappingTableIndex; // 0xA
    ub1 toggleCounter;                // 0xB
    float fAnimationRate;             // 0xC
    int16_t state;                    // 0x10
    ub1 subState;                     // 0x12
    ub1 pad_13;                       // 0x13
    float fLeanAmount;                // 0x14
    float fAimUpDown;                 // 0x18
    float fAimLeftRight;              // 0x1C
};

struct playerAnimState_t // 0xC / 0x4
{
    animationNumber_t legsAnim;  // 0x0
    animationNumber_t torsoAnim; // 0x2
    float fTorsoPitch;           // 0x4
    float fWaistPitch;           // 0x8
};

struct vehicleState_t // 0xC / 0x2
{
    int16_t flags;               // 0x0
    int16_t animId;              // 0x2
    int16_t attachModelIndex[2]; // 0x4
    ub1 attachTagIndex[2];       // 0x8
    ub1 vehicleDefIndex;         // 0xA
    ub1 pad_B;                   // 0xB
};

struct hardlineHint_t // 0x3
{
    ub1 team; // 0x0
    ub1 perk; // 0x1
    ub1 hint; // 0x2
};

struct scriptMoverState_t // 0x4 / 0x2
{
    ub1 fov;        // 0x0
    ub1 pad_1;      // 0x1
    int16_t animId; // 0x2
};

// entityState_s::$17D00931F63CD87A80B484B1B3158FBD
union union_entityState_s // 0xC / 0x4
{
    playerAnimState_t anim;
    vehicleState_t vehicleState;
    hardlineHint_t hardline;
    scriptMoverState_t moverState;
};

// entityState_s::$4FF060808F48C86E5B6F56A386CAC046
union union2_entityState_s // 0x4
{
    i32 hintString;    // index?
    i32 vehicleXModel; // index?
    i32 secondBcAlias; // index?
    i32 soundTag;      // ub1?
};

struct clientLinkInfo_t // 0x4 / 0x2
{
    int16_t parentEnt; // 0x0
    ub1 tagIndex;      // 0x2
    ub1 flags;         // 0x3
};

enum team_t : int32_t
{

};

// entityState_s::$47B0C420A2A679296861D3B45553DCB5
union union3_entityState_t // 0x2
{
    int16_t brushmodel;
    int16_t triggerModel;
    int16_t xmodel;
    int16_t primaryLight;
    int16_t probe;
    int16_t bone;
};

// entityState_s::$2D6CF13FD075F9D30BE4D506B66C998B
union union4_entityState_t // 0x1
{
    ub1 scale;
    ub1 eventParm2;
    ub1 helicopterStage;
    ub1 destructibleid;
    ub1 meleeDir;
};

struct entityState_t // 0x1E0 / 0x8
{
    i32 number;                                                        // 0x0
    ub1 pad_4[4];                                                      // 0x4
    LerpEntityState lerp;                                              // 0x8
    i32 time2;                                                         // 0xD8
    i32 loopSoundId;                                                   // 0xDC
    i32 solid;                                                         // 0xE0
    ub1 pad_E4[4];                                                     // 0xE4
    renderOptions_t renderOptions;                                     // 0xE8
    AttachmentCosmeticVariantIndexes attachmentCosmeticVariantIndexes; // 0xF8
    animState_t animState;                                             // 0xFC
    union_entityState_s un2;                                           // 0x11C
    union2_entityState_s un3;                                          // 0x128
    i32 partBits[12];                                                  // 0x12C
    clientLinkInfo_t clientLinkInfo;                                   // 0x15C
    i32 clientMask;                                                    // 0x160
    ub1 teamMask;                                                      // 0x164
    ub1 events[4];                                                     // 0x165
    ub1 pad_169[7];                                                    // 0x169
    EventParm_t eventParms[4];                                         // 0x170
    EventParm_t eventParm;                                             // 0x190
    i16 eType;                                                         // 0x198
    ub1 pad_19A[2];                                                    // 0x19A
    team_t team;                                                       // 0x19C
    i16 owner;                                                         // 0x1A0
    i16 groundEntityNum;                                               // 0x1A2
    union3_entityState_t index;                                        // 0x1A4
    i16 otherEntityNum;                                                // 0x1A6
    i16 enemyModel;                                                    // 0x1AA
    ub1 pad_1AC[4];                                                    // 0x1AC
    Weapon weapon;                                                     // 0x1B0
    Weapon lastStandPrevWeapon;                                        // 0x1B8
    ub1 targetname;                                                    // 0x1C0
    i16 loopSoundFade;                                                 // 0x1C4
    i16 spawnVarIndex;                                                 // 0x1C6
    i16 animtreeIndex;                                                 // 0x1C8
    ub1 pad_1C9[3];                                                    // 0x1C9
    AnimScriptedState animScripted;
    i16 eventSequence;        // 0x1D8
    ub1 surfType;             // 0x1DA
    ub1 pad_1DB;              // 0x1DB
    i16 clientNum;            // 0x1DC
    ub1 iHeadIcon;            // 0x1DE
    union4_entityState_t un1; // 0x1DF
};

enum dvarType_t : i32
{
    DVAR_TYPE_INVALID = 0,
    DVAR_TYPE_BOOL = 1,
    DVAR_TYPE_FLOAT = 2,
    DVAR_TYPE_FLOAT_2 = 3,
    DVAR_TYPE_FLOAT_3 = 4,
    DVAR_TYPE_FLOAT_4 = 5,
    DVAR_TYPE_INT = 6,
    DVAR_TYPE_ENUM = 7,
    DVAR_TYPE_STRING = 8,
    DVAR_TYPE_COLOR = 9,
    DVAR_TYPE_INT64 = 0xA,
    DVAR_TYPE_UINT64 = 0xB,
    DVAR_TYPE_LINEAR_COLOR_RGB = 0xC,
    DVAR_TYPE_COLOR_XYZ = 0xD,
    DVAR_TYPE_COLOR_LAB = 0xE,
    DVAR_TYPE_SESSIONMODE_BASE_DVAR = 0xF,
    DVAR_TYPE_COUNT = 0x10
};

union DvarLimits // 0x10 / 0x8
{
    // TODO: here
    uint8_t padding[16];
};

enum dvarFlags_e
{
    DVAR_ARCHIVE = 1 << 0,
    DVAR_USERINFO = 1 << 1,
    DVAR_SYSTEMINFO = 1 << 2,
    DVAR_CODINFO = 1 << 3,
    DVAR_LATCH = 1 << 4,
    DVAR_ROM = 1 << 5,
    DVAR_SAVED = 1 << 6,
    DVAR_INIT = 1 << 7,
    DVAR_CHEAT = 1 << 8,
    // DVAR_UNKNOWN = 1 << 9,
    DVAR_EXTERNAL = 1 << 10,
    // DVAR_UNKNOWN3x = 1 << 11-13,
    DVAR_SESSIONMODE = 1 << 15
};

enum DvarSetSource : __int32
{
  DVAR_SOURCE_INTERNAL = 0x0,
  DVAR_SOURCE_EXTERNAL = 0x1,
  DVAR_SOURCE_SCRIPT = 0x2,
  DVAR_SOURCE_DEVGUI = 0x3,
};

union DvarValue // 0x18 / 0x8
{
    ub1 enabled;
    i32 integer;
    u32 unsignedInt;
    i64 integer64;
    u64 unsignedInt64;
    void* value;
    vec4_t vector;
    const char* string;
    ub1 color[4];
    const dvar_t* indirect[3]; // ???
};

struct dvar_t // 0xA0 / 0x8
{
    dvarStrHash_t* name;
    const char* description;
    int64_t hash;
    dvarFlags_e flags; // 0x18
    dvarType_t type; // 0x1C
    bool modified; // 0x20
    DvarValue current; // 0x28
    DvarValue latched;
    DvarValue reset;
    DvarValue unk; // ????
    DvarLimits domain;
    dvar_t* hashNext; // 0x98
};

static_assert(sizeof(dvar_t) == 0xA0);

struct score_t // 0x54 / 0x4
{
    int32_t ping;                // 0x0
    int32_t status_icon;         // 0x4
    int32_t place;               // 0x8
    int32_t score;               // 0xC
    int32_t kills;               // 0x10
    int32_t assists;             // 0x14
    int32_t deaths;              // 0x18
    ub1 scoreboardColumns[3][9]; // 0x1C
    int32_t downs;               // 0x40
    int32_t revives;             // 0x44
    int32_t headshots;           // 0x48
    float scoreMultiplier;       // 0x4C
    int32_t currentStreak;       // 0x50
};

struct centity_t // 0x808 / 0x8
{
    cpose_t pose;              // 0x0
    LerpEntityState prevState; // 0x2F0
    entityState_t nextState;   // 0x3C0
    i16 previousEventSequence; // 0x5A0
    ub1 pad_5A2[2];            // 0x5A2
    float spawnTime;           // 0x5A4
    float miscTime;            // 0x5A8
    float lastMuzzleFlash;     // 0x5AC
    i32 numShotsFiredLast;     // 0x5B0
    ub1 pad_5B4[4];            // 0x5B4
    // TODO: here
};

struct gentity_t
{
    // model 0x280
    // targetName 0x290
};

enum consoleLabel_e : uint32_t
{
    CON_LABEL_DEFAULT = 0,
    CON_LABEL_TEMP = 1,
    CON_LABEL_GFX = 2,
    CON_LABEL_TASKMGR2 = 3,
    CON_LABEL_LIVE = 4,
    CON_LABEL_DEMONWARE = 5,
    CON_LABEL_LEADERBOARDS = 6,
    CON_LABEL_LOBBY = 7,
    CON_LABEL_LOBBYHOST = 8,
    CON_LABEL_LOBBYCLIENT = 9,
    CON_LABEL_LOBBYVM = 0xA,
    CON_LABEL_MIGRATION = 0xB,
    CON_LABEL_INGAME_MIGRATION_HOST = 0xC,
    CON_LABEL_INGAME_MIGRATION_CLIENT = 0xD,
    CON_LABEL_SCRIPTER = 0xE,
    CON_LABEL_VM = 0xF,
    CON_LABEL_DVAR = 0x10,
    CON_LABEL_TOOL = 0x11,
    CON_LABEL_ANIM = 0x12,
    CON_LABEL_NETCHAN = 0x13,
    CON_LABEL_BG_CACHE = 0x14,
    CON_LABEL_PM = 0x15,
    CON_LABEL_MAPSWITCH = 0x16,
    CON_LABEL_AI = 0x17,
    CON_LABEL_GADGET = 0x18,
    CON_LABEL_SOUND = 0x19,
    CON_LABEL_SNAPSHOT = 0x1A,
    PLAYGO = 0x1B,
    CON_LABEL_LUI = 0x1C,
    CON_LABEL_LUA = 0x1D,
    CON_LABEL_VOICE = 0x1E,
    CON_LABEL_DEMO = 0x1F,
    CON_LABEL_DB = 0x20,
    CON_LABEL_HTTP = 0x21,
    CON_LABEL_COMPANION = 0x22,
    CON_LABEL_MEM = 0x23,
    CON_LABEL_CINEMATIC = 0x24,
    CON_LABEL_DDL = 0x25,
    CON_LABEL_STORAGE = 0x26,
    CON_LABEL_STEAM = 0x27,
    CON_LABEL_CHECKPOINT = 0x28,
    CON_LABEL_THUNDERHEAD = 0x29,
    CON_LABEL_COMSCORE = 0x2A,
    CON_LABEL_COUNT = 0x2B
};

enum scriptInstance_t : int32_t
{
    SCRIPTINSTANCE_SERVER = 0,
    SCRIPTINSTANCE_CLIENT = 1,
    SCRIPTINSTANCE_MAX = 2
};

struct _jmp_buf;
struct hksInstruction;
struct lua_Debug;
struct lua_State;
struct LUIPoolUserData;
struct OpenedMenuData;
struct HksObject;
struct luaL_Reg;
struct HksCompilerSettings;

union HksValue;

namespace hks
{
	struct RuntimeProfileData;
	struct MemoryManager;
	struct DebugHook;
	struct DebugInstance;
	struct HksGlobal;
	struct ApiStack;
	struct CallStack;
	struct CallSite;
	struct cclosure;
	struct HksClosure;
	struct HashTable;
	struct UserData;
	struct StructInst;
	struct InternString;
	struct StringPinner;
	struct StringTable;
	struct Metatable;
	struct StaticStringCache;
	struct UpValue;
	struct Method;
	struct HksGcWeights;
	struct ResumeData_Header;
	struct ResumeData_State;
	struct ResumeData_Table;
	struct ResumeData_Closure;
	struct ResumeData_CClosure;
	struct ResumeData_Userdata;
	union ResumeData_Entry;
	struct GarbageCollector;
}

typedef int32_t hksBool;
typedef char hksChar;
typedef uint8_t hksByte;
typedef int16_t hksShort16;
typedef uint16_t hksUshort16;
typedef float HksNumber;
typedef int32_t hksInt32;
typedef uint32_t hksUint32;
typedef int64_t hksInt64;
typedef uint64_t hksUint64;
typedef size_t hksSize;
typedef int32_t ErrorCode;

enum HksError : hksInt32
{
	HKS_NO_ERROR = 0x0,
	LUA_ERRSYNTAX = -1,
	LUA_ERRFILE = -2,
	LUA_ERRRUN = -3,
	LUA_ERRMEM = -4,
	LUA_ERRERR = -5,
	HKS_THROWING_ERROR = -6,
	HKS_GC_YIELD = 0x1,
};

enum HksBytecodeEndianness : hksInt32
{
	HKS_BYTECODE_DEFAULT_ENDIAN = 0x0,
	HKS_BYTECODE_BIG_ENDIAN = 0x1,
	HKS_BYTECODE_LITTLE_ENDIAN = 0x2,
};

enum HksBytecodeSharingMode : hksInt64
{
	HKS_BYTECODE_SHARING_OFF = 0,
	HKS_BYTECODE_SHARING_ON = 1,
	HKS_BYTECODE_SHARING_SECURE = 2
};

enum HksObjectType : hksInt32
{
	TANY = -2,
	TNONE = -1,
	TNIL = 0x0,
	TBOOLEAN = 0x1,
	TLIGHTUSERDATA = 0x2,
	TNUMBER = 0x3,
	TSTRING = 0x4,
	TTABLE = 0x5,
	TFUNCTION = 0x6,
	TUSERDATA = 0x7,
	TTHREAD = 0x8,
	TIFUNCTION = 0x9,
	TCFUNCTION = 0xA,
	TUI64 = 0xB,
	TSTRUCT = 0xC,
	NUM_TYPE_OBJECTS = 0xE,
};

typedef hksUint32 hksBytecodeInstruction;
typedef hksUint32 HksNativeValueAsInt;
typedef HksObject HksRegister;
typedef void* (*lua_Alloc)(void*, void*, hksSize, hksSize);
typedef hksInt32(*hks_debug_map)(const char*, hksInt32);
typedef hksInt32(*lua_CFunction)(void*);
typedef void (*lua_Hook)(lua_State*, lua_Debug*);

namespace hks
{
    typedef void (*HksLogFunc)(lua_State*, const char*, ...);
    typedef void (*HksEmergencyGCFailFunc)(lua_State*, size_t);
    typedef CallSite* ErrorHandler;
    typedef int HksGcCost;
}

namespace hks
{
    struct GenericChunkHeader
    {
	    hksSize m_flags;
    };

    struct ChunkHeader : GenericChunkHeader
    {
	    ChunkHeader* m_next;
    };

    struct ChunkList
    {
	    ChunkHeader m_head;
    };
}

struct _jmp_buf
{
	void* _jb[12];
};

struct hksInstruction
{
	hksBytecodeInstruction code;
};

struct lua_Debug
{
	hksInt32 event;
	const char* name;
	const char* namewhat;
	const char* what;
	const char* source;
	hksInt32 currentline;
	hksInt32 nups;
	hksInt32 nparams;
	hksBool ishksfunc;
	hksInt32 linedefined;
	hksInt32 lastlinedefined;
	char short_src[512];
	hksInt32 callstack_level;
	hksBool is_tail_call;
};

struct LUIPoolUserData
{
	hksUshort16 poolIndex;
	hksUshort16 iteration;
};

struct OpenedMenuData
{
	hksInt32 nameHash;
	LUIPoolUserData menuElementPoolData;
	char name[32];
};

union HksValue
{
	hks::cclosure* cClosure;
	hks::HksClosure* closure;
	hks::UserData* userData;
	hks::HashTable* table;
	hks::StructInst* tstruct;
	hks::InternString* str;
	lua_State* thread;
	void* ptr;
	HksNumber number;
	HksNativeValueAsInt native;
	hksInt32 boolean;
};

struct HksObject
{
	hksUint32 t;
	HksValue v;
};

struct luaL_Reg
{
	const char* name;
	lua_CFunction func;
};

int hks_identity_map(const char* filename, int lua_line);

struct HksCompilerSettings
{
	enum BytecodeSharingFormat : hksInt32
	{
		BYTECODE_DEFAULT = 0x0,
		BYTECODE_INPLACE = 0x1,
		BYTECODE_REFERENCED = 0x2,
	};


	enum IntLiteralOptions : hksInt32
	{
		INT_LITERALS_NONE = 0x0,
		INT_LITERALS_LUD = 0x1,
		INT_LITERALS_32BIT = 0x1,
		INT_LITERALS_UI64 = 0x2,
		INT_LITERALS_64BIT = 0x2,
		INT_LITERALS_ALL = 0x3,
	};

	hksBool m_emitStructCode = 0;
	hksInt32 padding;
	const hksChar** m_stripNames = 0;
	BytecodeSharingFormat m_bytecodeSharingFormat = BYTECODE_INPLACE;
	IntLiteralOptions m_enableIntLiterals = INT_LITERALS_NONE;

	hks_debug_map m_debugMap = hks_identity_map;
};

namespace hks
{
    enum GCResumePhase : hksInt32
    {
	GC_STATE_MARKING_UPVALUES = 0x0,
	GC_STATE_MARKING_GLOBAL_TABLE = 0x1,
	GC_STATE_MARKING_REGISTRY = 0x2,
	GC_STATE_MARKING_PROTOTYPES = 0x3,
	GC_STATE_MARKING_SCRIPT_PROFILER = 0x4,
	GC_STATE_MARKING_FINALIZER_STATE = 0x5,
	GC_TABLE_MARKING_ARRAY = 0x6,
	GC_TABLE_MARKING_HASH = 0x7,
    };

    struct ResumeData_Header
    {
	HksObjectType m_type;
    };

    struct ResumeData_State
    {
	ResumeData_Header h;
	int padding;
	lua_State* m_state;
	GCResumePhase m_phase;
	int padding2;
	UpValue* m_pending;
    };

    struct ResumeData_Table
    {
	ResumeData_Header h;
	int padding;
	HashTable* m_table;
	hksUint32 m_arrayIndex;
	hksUint32 m_hashIndex;
	hksInt32 m_weakness;
	int padding2;
    };

    struct ResumeData_Closure
    {
	ResumeData_Header h;
	int padding;
	HksClosure* m_closure;
	hksInt32 m_index;
	int padding2;
    };

    struct ResumeData_CClosure
    {
	ResumeData_Header h;
	int padding;
	cclosure* m_cclosure;
	hksInt32 m_upvalueIndex;
	int padding2;
    };

    struct ResumeData_Userdata
    {
	ResumeData_Header h;
	int padding;
	UserData* m_data;
    };

    union ResumeData_Entry
    {
	ResumeData_State State;
	ResumeData_Table HashTable;
	ResumeData_Closure Closure;
	ResumeData_CClosure CClosure;
	ResumeData_Userdata Userdata;
    };

    struct HksGcWeights
    {
	HksGcCost m_removeString;
	HksGcCost m_finalizeUserdataNoMM;
	HksGcCost m_finalizeUserdataGcMM;
	HksGcCost m_cleanCoroutine;
	HksGcCost m_removeWeak;
	HksGcCost m_markObject;
	HksGcCost m_traverseString;
	HksGcCost m_traverseUserdata;
	HksGcCost m_traverseCoroutine;
	HksGcCost m_traverseWeakTable;
	HksGcCost m_freeChunk;
	HksGcCost m_sweepTraverse;
    };

    struct Method : ChunkHeader
    {
	struct hksInstructionArray
	{
		hksUint32 size;
		const hksInstruction* data;
	};

	struct HksObjectArray
	{
		hksUint32 size;
		HksObject* data;
	};

	struct MethodArray
	{
		hksUint32 size;
		Method** data;
	};

	struct LocalInfo
	{
		InternString* name;
		hksInt32 start_pc;
		hksInt32 end_pc;
	};

	struct hksUint32Array
	{
		hksUint32 size;
		hksUint32* data;
	};

	struct LocalInfoArray
	{
		hksUint32 size;
		LocalInfo* data;
	};

	struct InternStringArray
	{
		hksUint32 size;
		InternString** data;
	};

	typedef hksInstructionArray Instructions;
	typedef HksObjectArray Constants;
	typedef MethodArray Children;
	typedef LocalInfoArray Locals;
	typedef hksUint32Array LineInfo;
	typedef InternStringArray UpValueInfo;

	struct DebugInfo
	{
		hksUint32 line_defined;
		hksUint32 last_line_defined;
		LineInfo lineInfo;
		UpValueInfo upvalInfo;
		InternString* source;
		InternString* name;
		Locals localInfo;
	};

	hksUint32 hash;
	hksUshort16 num_upvals;
	hksUshort16 m_numRegisters;
	hksByte num_params;
	hksByte m_flags;
	Instructions instructions;
	Constants constants;
	Children children;
	DebugInfo* m_debug;
    };

    struct UpValue : ChunkHeader
    {
	HksObject m_storage;
	HksObject* loc;
	UpValue* m_next;
    };

    struct cclosure : ChunkHeader
    {
	lua_CFunction m_function;
	HashTable* m_env;
	hksShort16 m_numUpvalues;
	hksShort16 m_flags;
	InternString* m_name;
	HksObject m_upvalues[1];
    };

    struct UserData : ChunkHeader
    {
	HashTable* m_env;
	Metatable* m_meta;
	char m_data[8];
    };

    struct StaticStringCache
    {
	HksObject m_objects[42];
    };

    struct HashTable : ChunkHeader
    {
	struct Node
	{
		HksRegister m_key;
		HksRegister m_value;
	};

	Metatable* m_meta;
	hksUint32 m_mask;
	Node* m_hashPart;
	HksRegister* m_arrayPart;
	hksUint32 m_arraySize;
	Node* m_freeNode;
    };

    struct Metatable
    {
	__int8 gap0[1];
    };

    struct InternString : GenericChunkHeader
    {
	hksSize m_lengthbits;
	hksUint32 m_hash;
	char m_data[30];
	char padding[6];
    };

    struct StringTable
    {
	InternString** m_data;
	hksUint32 m_count;
	hksUint32 m_mask;
	StringPinner* m_pinnedStrings;
    };

    struct StringPinner
    {
	struct Node
	{
		InternString* m_strings[32];
		Node* m_prev;
	};

	lua_State* const m_state;
	StringPinner* const m_prev;
	InternString** m_nextStringsPlace;
	Node m_firstNode;
	Node* m_currentNode;
    };

    struct CallSite
    {
	_jmp_buf m_jumpBuffer;
	CallSite* m_prev;
    };

    struct CallStack
    {
	struct ActivationRecord
	{
	    HksObject* m_base;
	    const hksInstruction* m_returnAddress;
	    hksShort16 m_tailCallDepth;
	    hksShort16 m_numVarargs;
	    hksInt32 m_numExpectedReturns;
	};

	ActivationRecord* m_records;
	ActivationRecord* m_lastrecord;
	ActivationRecord* m_current;
	const hksInstruction* m_current_lua_pc;
	const hksInstruction* m_hook_return_addr;
	hksInt32 m_hook_level;
	hksInt32 padding;
    };

    struct ApiStack
    {
	HksObject* top;
	HksObject* base;
	HksObject* alloc_top;
	HksObject* bottom;
    };

    struct MemoryManager
    {
	enum ChunkColor : hksInt32
	{
		WHITE = 0x0,
		BLACK = 0x1,
	};

	lua_Alloc m_allocator;
	void* m_allocatorUd;
	ChunkColor m_chunkColor;
	hksSize m_used;
	hksSize m_highwatermark;
	ChunkList m_allocationList;
	ChunkList m_sweepList;
	ChunkHeader* m_lastKeptChunk;
	lua_State* m_state;
    };

    struct GarbageCollector
    {
	struct ResumeStack
	{
	    ResumeData_Entry* m_storage;
	    hksInt32 m_numEntries;
	    hksUint32 m_numAllocated;
	};

	struct GreyStack
	{
	    HksObject* m_storage;
	    hksSize m_numEntries;
	    hksSize m_numAllocated;
	};

	struct RemarkStack
	{
	    HashTable** m_storage;
	    hksSize m_numAllocated;
	    hksSize m_numEntries;
	};

	struct WeakStack_Entry
	{
	    hksInt32 m_weakness;
	    HashTable* m_table;
	};

	struct WeakStack
	{
	    WeakStack_Entry* m_storage;
	    hksInt32 m_numEntries;
	    hksUint32 m_numAllocated;
	};

	HksGcCost m_target;
	HksGcCost m_stepsLeft;
	HksGcCost m_stepLimit;
	HksGcWeights m_costs;
	HksGcCost m_unit;
	_jmp_buf* m_jumpPoint;
	lua_State* m_mainState;
	lua_State* m_finalizerState;
	MemoryManager* m_memory;
	void* m_emergencyGCMemory;
	hksInt32 m_phase;
	ResumeStack m_resumeStack;
	GreyStack m_greyStack;
	RemarkStack m_remarkStack;
	WeakStack m_weakStack;
	hksBool m_finalizing;
	HksObject m_safeTableValue;
	lua_State* m_startOfStateStackList;
	lua_State* m_endOfStateStackList;
	lua_State* m_currentState;
	HksObject m_safeValue;
	void* m_compiler;
	void* m_bytecodeReader;
	void* m_bytecodeWriter;
	hksInt32 m_pauseMultiplier;
	HksGcCost m_stepMultiplier;
	hksSize m_emergencyMemorySize;
	bool m_stopped;
	lua_CFunction m_gcPolicy;
	hksSize m_pauseTriggerMemoryUsage;
	hksInt32 m_stepTriggerCountdown;
	hksUint32 m_stringTableIndex;
	hksUint32 m_stringTableSize;
	UserData* m_lastBlackUD;
	UserData* m_activeUD;
    };

    struct RuntimeProfileData
    {
	struct Stats
	{
	    hksUint64 hksTime;
	    hksUint64 callbackTime;
	    hksUint64 gcTime;
	    hksUint64 cFinalizerTime;
	    hksUint64 compilerTime;
	    hksUint32 hkssTimeSamples;
	    hksUint32 callbackTimeSamples;
	    hksUint32 gcTimeSamples;
	    hksUint32 compilerTimeSamples;
	    hksUint32 num_newuserdata;
	    hksUint32 num_tablerehash;
	    hksUint32 num_pushstring;
	    hksUint32 num_pushcfunction;
	    hksUint32 num_newtables;
	    hksInt32 padding;
	};

	hksInt64 stackDepth;
	hksInt64 callbackDepth;
	hksUint64 lastTimer;
	Stats frameStats;
	hksUint64 gcStartTime;
	hksUint64 finalizerStartTime;
	hksUint64 compilerStartTime;
	hksUint64 compilerStartGCTime;
	hksUint64 compilerStartGCFinalizerTime;
	hksUint64 compilerCallbackStartTime;
	hksInt64 compilerDepth;
	void* outFile;
	lua_State* rootState;
    };

    struct HksGlobal
    {
	MemoryManager m_memory;
	GarbageCollector m_collector;
	StringTable m_stringTable;
	hksInt64 padding3;
	HksBytecodeSharingMode m_bytecodeSharingMode;
	hksInt32 padding;
	HksObject m_registry;
	ChunkList m_userDataList;
	lua_State* m_root;
	StaticStringCache m_staticStringCache;
	DebugInstance* m_debugger;
	void* m_profiler;
	RuntimeProfileData m_runProfilerData;
	HksCompilerSettings m_compilerSettings;
	lua_CFunction m_panicFunction;
	void* m_luaplusObjectList;
	hksInt32 m_heapAssertionFrequency;
	hksInt32 m_heapAssertionCount;
	HksLogFunc m_logFunction;
	HksEmergencyGCFailFunc m_emergencyGCFailFunction;
	HksBytecodeEndianness m_bytecodeDumpEndianness;
	hksInt32 padding2;
    };

    struct DebugHook
    {
	lua_Hook m_callback;
	hksInt32 m_mask;
	hksInt32 m_count;
	hksInt32 m_counter;
	bool m_inuse;
	const hksInstruction* m_prevPC;
    };

    struct DebugInstance
    {
	struct RuntimeProfilerStats
	{
	    hksInt32 hksTime;
	    hksInt32 callbackTime;
	    hksInt32 gcTime;
	    hksInt32 cFinalizerTime;
	    hksInt64 heapSize;
	    hksInt64 num_newuserdata;
	    hksInt64 num_pushstring;
	};

	hksInt32 m_savedObjects;
	hksInt32 m_keepAliveObjects;
	lua_State* m_activeState;
	lua_State* m_mainState;
	void* m_owner;
	hksInt32 m_DebuggerLevel;
	hksInt32 stored_Hook_level;
	bool m_clearHook;
	const hksInstruction* stored_Hook_return_addr;
	hksInt32 m_debugStepLastLine;
	DebugInstance* m_next;
	const hksInstruction* m_activePC;
	hksInt32 runtimeProfileSendBufferWritePosition;
	RuntimeProfilerStats runtimeProfileSendBuffer[30];
    };

    struct HksClosure : ChunkHeader
    {
	struct MethodCache
	{
	    const HksObject* consts;
	    const hksInstruction* inst;
	    hksUshort16 m_numRegisters;
	    hksByte m_flags;
	    hksByte num_params;
	};

	Method* m_method;
	HashTable* m_env;
	hksByte m_mayHaveUpvalues;
	MethodCache m_cache;
	UpValue* m_upvalues[1];
    };
}

struct lua_State // 0xE8
{
    enum Status : hksInt32
    {
	NEW = 0x1,
	RUNNING = 0x2,
	YIELDED = 0x3,
	DEAD_ERROR = 0x4,
    };

    hks::ChunkHeader baseclass;
    hks::HksGlobal* m_global;
    hks::CallStack m_callStack;
    hks::ApiStack m_apistack;
    hks::UpValue* pending;
    HksObject globals;
    HksObject m_cEnv;
    hks::ErrorHandler m_callsites;
    hksInt32 m_numberOfCCalls;
    void* m_context;
    hks::InternString* m_name;
    lua_State* m_next;
    lua_State* m_nextStateStack;
    Status m_status;
    HksError m_error;
    hks::DebugHook m_debugHook;
};

/* 294 */
struct SCR_PROFILER_DATA
{
  gscProfileInfo_t *profileInfo[8];
  uint32_t profileInfoCount;
  uint8_t pad41;
  uint8_t pad42;
  uint8_t pad43;
  uint8_t pad44;
};

/* 295 */
struct function_stack_t
{
  void *pos;
  void *top;
  uint32_t localId;
  uint32_t localVarCount;
  void *startTop;
  SCR_PROFILER_DATA profileData;
  void *savedPos;
};

/* 229 */
typedef unsigned __int64 uint64_t;

/* 296 */
struct __declspec(align(8)) function_frame_t
{
  function_stack_t _fs;
  uint8_t nested;
  uint64_t creationPos;
};

struct ScrVarStackBuffer_t
{
  byte *pos;
  byte *creationPos;
  int waitTime;
  unsigned __int16 size;
  unsigned __int16 bufLen;
  ScrVarIndex_t localId;
  byte buf[1];
};

/* 255 */
union __declspec(align(8)) ScrVarValueUnion_t
{
  int intValue;
  uint uintValue;
  uint64_t uint64Value;
  uintptr_t uintptrValue;
  float floatValue;
  ScrString_t stringValue;
  const float *vectorValue;
  byte *codePosValue;
  unsigned int pointerValue;
  ScrVarStackBuffer_t *stackValue;
};

/* 254 */
struct __declspec(align(8)) ScrVarValue_t
{
  ScrVarValueUnion_t u;
  int type;
};

/* 297 */
typedef uint32_t ScrVarCanonicalName_t;

/* 298 */
typedef void (__fastcall *NotifyListener)(unsigned int, ScrVarCanonicalName_t);

/* 258 */
struct __declspec(align(8)) scrVmPub_t
{
  unsigned int *localVars;
  ScrVarValue_t *maxstack;
  int function_count;
  function_frame_t *function_frame;
  ScrVarValue_t *top;
  bool debugCode;
  bool abort_on_error;
  bool terminal_error;
  bool block_execution;
  int callNesting;
  unsigned int inparamcount;
  unsigned int outparamcount;
  unsigned int breakpointOutparamcount;
  bool showError;
  function_frame_t function_frame_start[64];
  ScrVarValue_t stack[2048];
  NotifyListener notifyListeners[1];
};

/* 293 */
struct gscProfileInfo_t
{
  uint64_t inclusive_time;
  uint64_t exclusive_time;
  uint64_t hit_count;
};

// notes:
// ugcinfo struct on steam is 0x4C8
//                on wstor is 0x4BC (-0xC)

struct ugcinfo_entry_steam // 0x4C8
{
    char pad[0x4C8];
    // name: 0x0 (0x64)
    // internalName: 0x64 (
    // description: 0xA4
    // ugcName: 0x84
    // ugcVersion: ??
};

struct ugcinfo_steam
{
    uint32_t num_entries;
    uint32_t unk4;
    ugcinfo_entry_steam entries[128];
};

struct ugcinfo_entry_wstor // 0x4BC
{
    char name[0x64];
    char internalName[0x20]; // 64
    char ugcName[0x20]; // 84
    char description[0x100]; // A4
    char ugcPathBasic[0x104]; // 1A4
    char ugcRoot[0x104]; // 2A8
    char ugcPath[0x104]; // 3AC
    uint32_t unk_4B0;
    uint32_t hash_id; // 4B4
    uint32_t unk_4B8;
};

enum AssetPool
{
    xasset_physpreset,
    xasset_physconstraints,
    xasset_destructibledef,
    xasset_xanim,
    xasset_xmodel,
    xasset_xmodelmesh,
    xasset_material,
    xasset_computeshaderset,
    xasset_techset,
    xasset_image,
    xasset_sound,
    xasset_sound_patch,
    xasset_col_map,
    xasset_com_map,
    xasset_game_map,
    xasset_map_ents,
    xasset_gfx_map,
    xasset_lightdef,
    xasset_lensflaredef,
    xasset_ui_map,
    xasset_font,
    xasset_fonticon,
    xasset_localize,
    xasset_weapon,
    xasset_weapondef,
    xasset_weaponvariant,
    xasset_weaponfull,
    xasset_cgmediatable,
    xasset_playersoundstable,
    xasset_playerfxtable,
    xasset_sharedweaponsounds,
    xasset_attachment,
    xasset_attachmentunique,
    xasset_weaponcamo,
    xasset_customizationtable,
    xasset_customizationtable_feimages,
    xasset_customizationtablecolor,
    xasset_snddriverglobals,
    xasset_fx,
    xasset_tagfx,
    xasset_klf,
    xasset_impactsfxtable,
    xasset_impactsoundstable,
    xasset_player_character,
    xasset_aitype,
    xasset_character,
    xasset_xmodelalias,
    xasset_rawfile,
    xasset_stringtable,
    xasset_structuredtable,
    xasset_leaderboarddef,
    xasset_ddl,
    xasset_glasses,
    xasset_texturelist,
    xasset_scriptparsetree,
    xasset_keyvaluepairs,
    xasset_vehicle,
    xasset_addon_map_ents,
    xasset_tracer,
    xasset_slug,
    xasset_surfacefxtable,
    xasset_surfacesounddef,
    xasset_footsteptable,
    xasset_entityfximpacts,
    xasset_entitysoundimpacts,
    xasset_zbarrier,
    xasset_vehiclefxdef,
    xasset_vehiclesounddef,
    xasset_typeinfo,
    xasset_scriptbundle,
    xasset_scriptbundlelist,
    xasset_rumble,
    xasset_bulletpenetration,
    xasset_locdmgtable,
    xasset_aimtable,
    xasset_animselectortable,
    xasset_animmappingtable,
    xasset_animstatemachine,
    xasset_behaviortree,
    xasset_behaviorstatemachine,
    xasset_ttf,
    xasset_sanim,
    xasset_lightdescription,
    xasset_shellshock,
    xasset_xcam,
    xasset_bgcache,
    xasset_texturecombo,
    xasset_flametable,
    xasset_bitfield,
    xasset_attachmentcosmeticvariant,
    xasset_maptable,
    xasset_maptableloadingimages,
    xasset_medal,
    xasset_medaltable,
    xasset_objective,
    xasset_objectivelist,
    xasset_umbra_tome,
    xasset_navmesh,
    xasset_navvolume,
    xasset_binaryhtml,
    xasset_laser,
    xasset_beam,
    xasset_streamerhint,
    xasset__string,
    xasset_assetlist,
    xasset_report,
    xasset_depend
};

enum netsrc_t
{
    NS_NULL = -1,
    NS_CLIENT1 = 0,
    NS_CLIENT2 = 1,
    NS_CLIENT3 = 2,
    NS_CLIENT4 = 3,
    NS_SERVER = 4,
    NS_MAXCLIENTS = 4,
    NS_PACKET = 5
};

enum netadrtype_t
{
    NA_BOT = 0x0,
    NA_BAD = 0x1,
    NA_LOOPBACK = 0x2,
    NA_RAWIP = 0x3,
    NA_IP = 0x4,
};

enum LobbyType
{
    LOBBY_TYPE_INVALID = 0xFFFFFFFF,
    LOBBY_TYPE_PRIVATE = 0x0,
    LOBBY_TYPE_GAME = 0x1,
    LOBBY_TYPE_TRANSITION = 0x2,
    LOBBY_TYPE_COUNT = 0x3,
    LOBBY_TYPE_FIRST = 0x0,
    LOBBY_TYPE_LAST = 0x2,
    LOBBY_TYPE_AUTO = 0x3,
};

enum LobbyModule
{
    LOBBY_MODULE_INVALID = 0xFFFFFFFF,
    LOBBY_MODULE_HOST = 0x0,
    LOBBY_MODULE_CLIENT = 0x1,
    LOBBY_MODULE_PEER_TO_PEER = 0x3,
    LOBBY_MODULE_COUNT = 0x4,
};

enum LobbyMode
{
    LOBBY_MODE_INVALID = 0xFFFFFFFF,
    LOBBY_MODE_PUBLIC = 0x0,
    LOBBY_MODE_CUSTOM = 0x1,
    LOBBY_MODE_THEATER = 0x2,
    LOBBY_MODE_ARENA = 0x3,
    LOBBY_MODE_FREERUN = 0x4,
    LOBBY_MODE_COUNT = 0x5,
};

enum SessionActive
{
    SESSION_INACTIVE = 0x0,
    SESSION_KEEP_ALIVE = 0x1,
    SESSION_ACTIVE = 0x2,
};

struct ugcinfo_wstor
{
    uint32_t num_entries;
    ugcinfo_entry_wstor entries[128];
};

struct netadr_t
{
    uint8_t ipv4[4];
    uint16_t port;
    uint16_t pad;
    netadrtype_t type;
    netsrc_t localNetID;
};

struct msg_t
{
    uint8_t overflowed; // 0x0
    uint8_t readOnly; // 0x1
    uint16_t pad1; // 0x2
    uint32_t pad2; // 0x4
    char* data; // 0x8
    char* splitData; // 0x10
    uint32_t maxsize; // 0x18
    uint32_t cursize; // 0x1C
    uint32_t splitSize; // 0x20
    uint32_t readcount; // 0x24
    uint32_t bit; // 0x28
    uint32_t lastEntityRef; // 0x2C
    uint32_t flush; // 0x30
    netsrc_t targetLocalNetID; // 0x34
};

enum errorCode : int32_t
{
    ERROR_NONE = 0x0,
    ERROR_FATAL = 0x1,
    ERROR_DROP = 0x2,
    ERROR_FROM_STARTUP = 0x4,
    ERROR_SERVERDISCONNECT = 0x8,
    ERROR_DISCONNECT = 0x10,
    ERROR_SCRIPT = 0x20,
    ERROR_SCRIPT_DROP = 0x40,
    ERROR_LOCALIZATION = 0x80,
    ERROR_UI = 0x100,
    ERROR_LUA = 0x200,
    ERROR_SOFTRESTART = 0x400,
    ERROR_SOFTRESTART_KEEPDW = 0x800
};

enum MsgType
{
    MESSAGE_TYPE_NONE = -1,
    MESSAGE_TYPE_INFO_REQUEST = 0,
    MESSAGE_TYPE_INFO_RESPONSE = 1,
    MESSAGE_TYPE_LOBBY_STATE_PRIVATE = 2,
    MESSAGE_TYPE_LOBBY_STATE_GAME = 3,
    MESSAGE_TYPE_LOBBY_STATE_GAMEPUBLIC = 4,
    MESSAGE_TYPE_LOBBY_STATE_GAMECUSTOM = 5,
    MESSAGE_TYPE_LOBBY_STATE_GAMETHEATER = 6,
    MESSAGE_TYPE_LOBBY_HOST_HEARTBEAT = 7,
    MESSAGE_TYPE_LOBBY_HOST_DISCONNECT = 8,
    MESSAGE_TYPE_LOBBY_HOST_DISCONNECT_CLIENT = 9,
    MESSAGE_TYPE_LOBBY_HOST_LEAVE_WITH_PARTY = 0xA,
    MESSAGE_TYPE_LOBBY_CLIENT_HEARTBEAT = 0xB,
    MESSAGE_TYPE_LOBBY_CLIENT_DISCONNECT = 0xC,
    MESSAGE_TYPE_LOBBY_CLIENT_RELIABLE_DATA = 0xD,
    MESSAGE_TYPE_LOBBY_CLIENT_CONTENT = 0xE,
    MESSAGE_TYPE_LOBBY_MODIFIED_STATS = 0xF,
    MESSAGE_TYPE_JOIN_LOBBY = 0x10,
    MESSAGE_TYPE_JOIN_RESPONSE = 0x11,
    MESSAGE_TYPE_JOIN_AGREEMENT_REQUEST = 0x12,
    MESSAGE_TYPE_JOIN_AGREEMENT_RESPONSE = 0x13,
    MESSAGE_TYPE_JOIN_COMPLETE = 0x14,
    MESSAGE_TYPE_JOIN_MEMBER_INFO = 0x15,
    MESSAGE_TYPE_SERVERLIST_INFO = 0x16,
    MESSAGE_TYPE_PEER_TO_PEER_CONNECTIVITY_TEST = 0x17,
    MESSAGE_TYPE_PEER_TO_PEER_INFO = 0x18,
    MESSAGE_TYPE_LOBBY_MIGRATE_TEST = 0x19,
    MESSAGE_TYPE_MIGRATE_ANNOUNCE_HOST = 0x1A,
    MESSAGE_TYPE_MIGRATE_START = 0x1B,
    MESSAGE_TYPE_INGAME_MIGRATE_TO = 0x1C,
    MESSAGE_TYPE_MIGRATE_NEW_HOST = 0x1D,
    MESSAGE_TYPE_VOICE_PACKET = 0x1E,
    MESSAGE_TYPE_VOICE_RELAY_PACKET = 0x1F,
    MESSAGE_TYPE_DEMO_STATE = 0x20,
    MESSAGE_TYPE_COUNT = MESSAGE_TYPE_DEMO_STATE + 1
};

struct LobbyMsg
{
    msg_t msg;
    MsgType msgType; // 0x38
    char encodeFlags;
    uint32_t packageType;
};