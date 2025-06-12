#pragma once
#include <qglobal.h>
// old version of structs

struct dvar_t;
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
typedef int64_t i64;
typedef uint64_t u64;
typedef uint64_t EventParm_t;
typedef uint64_t ClanID;
typedef uint64_t AttachmentMask;
typedef uint64_t bitfield_t;
typedef uint64_t EventParm_t;
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

enum ClientNum_t : int32_t
{
    CLIENT_INDEX_0 = 0,
    CLIENT_INDEX_FIRST = 0,
    CLIENT_INDEX_1 = 1,
    CLIENT_INDEX_2,
    CLIENT_INDEX_3,
    CLIENT_INDEX_4,
    CLIENT_INDEX_5,
    CLIENT_INDEX_6,
    CLIENT_INDEX_7,
    CLIENT_INDEX_8,
    CLIENT_INDEX_9,
    CLIENT_INDEX_10,
    CLIENT_INDEX_11,
    CLIENT_INDEX_12,
    CLIENT_INDEX_13,
    CLIENT_INDEX_14,
    CLIENT_INDEX_15,
    CLIENT_INDEX_16,
    CLIENT_INDEX_17,
    CLIENT_INDEX_18,
    CLIENT_INDEX_COUNT,
    CLIENT_INDEX_19,
    CLIENT_INDEX_20,
    CLIENT_INDEX_21,
    CLIENT_INDEX_22,
    CLIENT_INDEX_23,
    CLIENT_INDEX_24,
    CLIENT_INDEX_25,
    CLIENT_INDEX_26,
    CLIENT_INDEX_27,
    CLIENT_INDEX_28,
    CLIENT_INDEX_29,
    CLIENT_INDEX_30,
    CLIENT_INDEX_31,
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

struct dvar_t // 0x88 / 0x8
{
    const char* name;        // 0x0
    const char* description; // 0x8
    u64 hash;                // 0x10
    i32 flags;               // enum?, 0x18
    dvarType_t type;         // 0x1C
    ub1 modified;            // 0x20
    ub1 pad_21[7];           // 0x21
    DvarValue current;       // 0x28
    DvarValue latched;       // 0x40
    DvarValue reset;         // 0x58
    DvarLimits domain;
    dvar_t* hashNext;
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
