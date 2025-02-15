#include "global.h"
#include "malloc.h"
#include "battle_anim.h"
#include "battle_interface.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "math_util.h"
#include "palette.h"
#include "random.h"
#include "scanline_effect.h"
#include "sound.h"
#include "trig.h"
#include "util.h"
#include "constants/rgb.h"
#include "constants/songs.h"

void sub_8103448(struct Sprite *);
void sub_8103498(struct Sprite *);
void sub_810358C(struct Sprite *);
void sub_8103620(struct Sprite *);
void Anim_KinesisZapEnergy(struct Sprite *);
void Anim_SwordsDanceBlade(struct Sprite *);
void AnimSonicBoomProjectile(struct Sprite *);
void sub_8103AA4(struct Sprite *);
void sub_8103FE8(struct Sprite *);
void sub_8104088(struct Sprite *);
void sub_810413C(struct Sprite *);
void sub_81041C4(struct Sprite *);
void Anim_RazorWindTornado(struct Sprite *);
void Anim_ViceGripPincer(struct Sprite *);
void Anim_GuillotinePincer(struct Sprite *);
void Anim_BreathPuff(struct Sprite *);
void Anim_AngerMark(struct Sprite *);
void sub_810501C(struct Sprite *);
void sub_81051C4(struct Sprite *);
void sub_81052A4(struct Sprite *);
void sub_81054E8(struct Sprite *);
void sub_8105538(struct Sprite *);
void sub_8105C48(struct Sprite *);
void sub_8105DE8(struct Sprite *);
void sub_8105E60(struct Sprite *);
void sub_8106140(struct Sprite *);
void sub_81061C4(struct Sprite *);
void sub_810624C(struct Sprite *);
void AnimOrbitFast(struct Sprite *);
void AnimOrbitScatter(struct Sprite *);
void sub_8106944(struct Sprite *);
void sub_81069B8(struct Sprite *);
void sub_81069D0(struct Sprite *);
void sub_8106AD0(struct Sprite *);
void sub_8106B54(struct Sprite *);
void sub_8106C80(struct Sprite *);
void sub_8106CD0(struct Sprite *);
void sub_8106E00(struct Sprite *);
void sub_8106F00(struct Sprite *);
void sub_8106F60(struct Sprite *);
void sub_81070AC(struct Sprite *);
static void sub_81034D8(struct Sprite *);
static void sub_8103658(struct Sprite *);
static void sub_8103680(struct Sprite *);
static void AnimTask_WithdrawStep(u8);
static void Anim_SwordsDanceBladeStep(struct Sprite *);
static void sub_8104018(struct Sprite *);
static void sub_8104154(struct Sprite *);
static void sub_810421C(struct Sprite *);
static void sub_81042A0(struct Sprite *);
static void Anim_ViceGripPincerStep(struct Sprite *);
static void Anim_GuillotinePincerStep1(struct Sprite *);
static void Anim_GuillotinePincerStep2(struct Sprite *);
static void Anim_GuillotinePincerStep3(struct Sprite *);
static void AnimTask_GrowAndGreyscaleStep(u8);
static void AnimTask_MinimizeStep1(u8);
static void CreateMinimizeSprite(struct Task *, u8);
static void ClonedMinizeSprite_Step(struct Sprite *);
static void AnimTask_SplashStep(u8);
static void AnimTask_GrowAndShrinkStep(u8);
static void sub_8104C78(u8);
static void sub_8104D28(u8);
static void sub_8104F54(u8);
static void sub_8105078(struct Sprite *);
static void sub_810557C(struct Sprite *);
static void sub_81055F4(struct Sprite *);
static void sub_810561C(struct Sprite *);
static void sub_8105694(struct Sprite *);
static void sub_81056D4(struct Sprite *);
static void sub_810571C(struct Sprite *);
static void sub_810575C(struct Sprite *);
static void sub_81057B8(u8);
static void sub_8105878(u8);
static void sub_81059E0(u8);
static void sub_8105B08(u8);
static void sub_8105F30(u8);
static void sub_8105F84(u8);
static void sub_810618C(struct Sprite *);
static void sub_810627C(struct Sprite *);
static void sub_81063A8(u8);
static void sub_81065EC(u8);
static void AnimOrbitFastStep(struct Sprite *);
static void AnimOrbitScatterStep(struct Sprite *);
static void sub_8106D5C(struct Sprite *);
static void sub_8106DD4(u8);
static void sub_8106EC8(struct Sprite *);
static void sub_8107018(struct Sprite *);
static void sub_810703C(struct Sprite *);

const struct SpriteTemplate gUnknown_08593264 =
{
    .tileTag = ANIM_TAG_FINGER,
    .paletteTag = ANIM_TAG_FINGER,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8103448,
};

const union AnimCmd gUnknown_0859327C[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593284[] =
{
    gUnknown_0859327C,
};

const struct SpriteTemplate gUnknown_08593288 =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES,
    .paletteTag = ANIM_TAG_MUSIC_NOTES,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8103498,
};

const struct SpriteTemplate gUnknown_085932A0 =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &gDummyOamData,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_810358C,
};

extern const union AffineAnimCmd *const gUnknown_08597060[];
const struct SpriteTemplate gUnknown_085932B8 =
{
    .tileTag = ANIM_TAG_CLAMP,
    .paletteTag = ANIM_TAG_CLAMP,
    .oam = &gOamData_AffineNormal_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08597060,
    .callback = sub_8103620,
};

const union AnimCmd gUnknown_085932D0[] =
{
    ANIMCMD_FRAME(0, 9),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 3),
    ANIMCMD_FRAME(48, 3),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_085932E4[] =
{
    gUnknown_085932D0,
};

const union AffineAnimCmd gUnknown_085932E8[] =
{
    AFFINEANIMCMD_FRAME(0x50, 0x50, 0, 0),
    AFFINEANIMCMD_FRAME(0x9, 0x9, 0, 18),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_08593300[] =
{
    gUnknown_085932E8,
};

const struct SpriteTemplate gUnknown_08593304 =
{
    .tileTag = ANIM_TAG_EXPLOSION_6,
    .paletteTag = ANIM_TAG_EXPLOSION_6,
    .oam = &gOamData_AffineNormal_ObjNormal_32x32,
    .anims = gUnknown_085932E4,
    .images = NULL,
    .affineAnims = gUnknown_08593300,
    .callback = sub_80A77C8,
};

const union AnimCmd gUnknown_0859331C[] =
{
    ANIMCMD_FRAME(0, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(24, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(40, 3, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 3, .hFlip = TRUE),
    ANIMCMD_LOOP(1),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593340[] =
{
    gUnknown_0859331C,
};

const struct SpriteTemplate gKinesisZapEnergySpriteTemplate =
{
    .tileTag = ANIM_TAG_ALERT,
    .paletteTag = ANIM_TAG_ALERT,
    .oam = &gOamData_AffineOff_ObjNormal_32x16,
    .anims = gUnknown_08593340,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = Anim_KinesisZapEnergy,
};

const union AffineAnimCmd gUnknown_0859335C[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x14, 0x0, 0, 12),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 32),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_0859337C[] =
{
    gUnknown_0859335C,
};

const struct SpriteTemplate gSwordsDanceBladeSpriteTemplate =
{
    .tileTag = ANIM_TAG_SWORD,
    .paletteTag = ANIM_TAG_SWORD,
    .oam = &gOamData_AffineNormal_ObjBlend_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_0859337C,
    .callback = Anim_SwordsDanceBlade,
};

const struct SpriteTemplate gSonicBoomSpriteTemplate =
{
    .tileTag = ANIM_TAG_AIR_WAVE,
    .paletteTag = ANIM_TAG_AIR_WAVE,
    .oam = &gOamData_AffineDouble_ObjBlend_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = AnimSonicBoomProjectile,
};

const struct SpriteTemplate gUnknown_085933B0 =
{
    .tileTag = ANIM_TAG_AIR_WAVE,
    .paletteTag = ANIM_TAG_AIR_WAVE,
    .oam = &gOamData_AffineOff_ObjBlend_32x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8103AA4,
};

const union AffineAnimCmd gUnknown_085933C8[] =
{
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 0),
    AFFINEANIMCMD_FRAME(0x7, 0x7, 0, -56),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd gUnknown_085933E0[] =
{
    AFFINEANIMCMD_FRAME(0x5, 0x5, 0, 10),
    AFFINEANIMCMD_FRAME(0xFFF6, 0xFFF6, 0, 10),
    AFFINEANIMCMD_FRAME(0xA, 0xA, 0, 10),
    AFFINEANIMCMD_FRAME(0xFFF6, 0xFFF6, 0, 10),
    AFFINEANIMCMD_FRAME(0xA, 0xA, 0, 10),
    AFFINEANIMCMD_FRAME(0xFFF6, 0xFFF6, 0, 10),
    AFFINEANIMCMD_FRAME(0xA, 0xA, 0, 10),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_08593420[] =
{
    gUnknown_085933C8,
};

const union AffineAnimCmd *const gUnknown_08593424[] =
{
    gUnknown_085933E0,
};

const struct SpriteTemplate gSupersonicWaveSpriteTemplate =
{
    .tileTag = ANIM_TAG_GOLD_RING,
    .paletteTag = ANIM_TAG_GOLD_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593420,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gScreechWaveSpriteTemplate =
{
    .tileTag = ANIM_TAG_PURPLE_RING,
    .paletteTag = ANIM_TAG_PURPLE_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593420,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gUnknown_08593458 =
{
    .tileTag = ANIM_TAG_METAL_SOUND_WAVES,
    .paletteTag = ANIM_TAG_METAL_SOUND_WAVES,
    .oam = &gOamData_AffineDouble_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593420,
    .callback = TranslateAnimSpriteToTargetMonLocation,
};

const struct SpriteTemplate gUnknown_08593470 =
{
    .tileTag = ANIM_TAG_BLUE_RING_2,
    .paletteTag = ANIM_TAG_BLUE_RING_2,
    .oam = &gOamData_AffineDouble_ObjNormal_16x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593424,
    .callback = sub_8108C94,
};

const struct SpriteTemplate gUnknown_08593488 =
{
    .tileTag = ANIM_TAG_LARGE_FRESH_EGG,
    .paletteTag = ANIM_TAG_LARGE_FRESH_EGG,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_80A78AC,
};

const struct SpriteTemplate gUnknown_085934A0 =
{
    .tileTag = ANIM_TAG_VOID_LINES,
    .paletteTag = ANIM_TAG_VOID_LINES,
    .oam = &gOamData_AffineOff_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8103FE8,
};

const union AnimCmd gUnknown_085934B8[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_085934C0[] =
{
    gUnknown_085934B8,
};

const union AffineAnimCmd gUnknown_085934C4[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 10, 1),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd *const gUnknown_085934D4[] =
{
    gUnknown_085934C4,
};

const struct SpriteTemplate gUnknown_085934D8 =
{
    .tileTag = ANIM_TAG_COIN,
    .paletteTag = ANIM_TAG_COIN,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gUnknown_085934C0,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8104088,
};

const struct SpriteTemplate gUnknown_085934F0 =
{
    .tileTag = ANIM_TAG_COIN,
    .paletteTag = ANIM_TAG_COIN,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gUnknown_085934C0,
    .images = NULL,
    .affineAnims = gUnknown_085934D4,
    .callback = sub_810413C,
};

const union AffineAnimCmd gUnknown_08593508[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 20, 1),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd *const gUnknown_08593518[] =
{
    gUnknown_08593508,
};

const struct SpriteTemplate gUnknown_0859351C =
{
    .tileTag = ANIM_TAG_SEED,
    .paletteTag = ANIM_TAG_SEED,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593518,
    .callback = sub_81041C4,
};

const union AffineAnimCmd gUnknown_08593534[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x4, 0x0, 0, 40),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_0859354C[] =
{
    gUnknown_08593534,
};

const struct SpriteTemplate gRazorWindTornadoSpriteTemplate =
{
    .tileTag = ANIM_TAG_GUST,
    .paletteTag = ANIM_TAG_GUST,
    .oam = &gOamData_AffineNormal_ObjNormal_32x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_0859354C,
    .callback = Anim_RazorWindTornado,
};

const union AnimCmd gUnknown_08593568[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_FRAME(32, 20),
    ANIMCMD_END,
};

const union AnimCmd gUnknown_08593578[] =
{
    ANIMCMD_FRAME(0, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 3, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 20, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593588[] =
{
    gUnknown_08593568,
    gUnknown_08593578,
};

const struct SpriteTemplate gViceGripSpriteTemplate =
{
    .tileTag = ANIM_TAG_CUT,
    .paletteTag = ANIM_TAG_CUT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gUnknown_08593588,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = Anim_ViceGripPincer,
};

const union AnimCmd gUnknown_085935A8[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(16, 2),
    ANIMCMD_FRAME(32, 1),
    ANIMCMD_END,
};

const union AnimCmd gUnknown_085935B8[] =
{
    ANIMCMD_FRAME(0, 2, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 2, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 1, .vFlip = TRUE, .hFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_085935C8[] =
{
    gUnknown_085935A8,
    gUnknown_085935B8,
};

const struct SpriteTemplate gGuillotineSpriteTemplate =
{
    .tileTag = ANIM_TAG_CUT,
    .paletteTag = ANIM_TAG_CUT,
    .oam = &gOamData_AffineOff_ObjBlend_32x32,
    .anims = gUnknown_085935C8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = Anim_GuillotinePincer,
};

const union AffineAnimCmd gSplashEffectAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-6, 4, 0, 8),
    AFFINEANIMCMD_FRAME(10, -10, 0, 8),
    AFFINEANIMCMD_FRAME(-4, 6, 0, 8),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd gGrowAndShrinkAffineAnimCmds[] =
{
    AFFINEANIMCMD_FRAME(-4, -5, 0, 12),
    AFFINEANIMCMD_FRAME(0, 0, 0, 24),
    AFFINEANIMCMD_FRAME(4, 5, 0, 12),
    AFFINEANIMCMD_END,
};

const union AnimCmd gUnknown_08593628[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 40, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(12, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd gUnknown_0859363C[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 40),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 4),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593650[] =
{
    gUnknown_08593628,
    gUnknown_0859363C,
};

const struct SpriteTemplate gBreathPuffSpriteTemplate =
{
    .tileTag = ANIM_TAG_BREATH,
    .paletteTag = ANIM_TAG_BREATH,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gUnknown_08593650,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = Anim_BreathPuff,
};

const union AffineAnimCmd gUnknown_08593670[] =
{
    AFFINEANIMCMD_FRAME(0xB, 0xB, 0, 8),
    AFFINEANIMCMD_FRAME(0xFFF5, 0xFFF5, 0, 8),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_08593688[] =
{
    gUnknown_08593670,
};

const struct SpriteTemplate gAngerMarkSpriteTemplate =
{
    .tileTag = ANIM_TAG_ANGER,
    .paletteTag = ANIM_TAG_ANGER,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593688,
    .callback = Anim_AngerMark,
};

const union AffineAnimCmd gUnknown_085936A4[] =
{
    AFFINEANIMCMD_FRAME(-10, 9, 0, 7),
    AFFINEANIMCMD_FRAME(20, -20, 0, 7),
    AFFINEANIMCMD_FRAME(-20, 20, 0, 7),
    AFFINEANIMCMD_FRAME(10, -9, 0, 7),
    AFFINEANIMCMD_LOOP(2),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gUnknown_085936D4 =
{
    .tileTag = ANIM_TAG_PENCIL,
    .paletteTag = ANIM_TAG_PENCIL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_810501C,
};

const struct SpriteTemplate gUnknown_085936EC =
{
    .tileTag = ANIM_TAG_SNORE_Z,
    .paletteTag = ANIM_TAG_SNORE_Z,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_80A7938,
};

const union AnimCmd gUnknown_08593704[] =
{
    ANIMCMD_FRAME(0, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(32, 5),
    ANIMCMD_FRAME(48, 5),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593718[] =
{
    gUnknown_08593704,
};
//*
const struct SpriteTemplate gBattleAnimSpriteTemplate_859371C =
{
    .tileTag = ANIM_TAG_EXPLOSION,
    .paletteTag = ANIM_TAG_EXPLOSION,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593718,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_80A77C8,
};

const union AffineAnimCmd gUnknown_08593734[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 2),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 4),
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 2),
    AFFINEANIMCMD_JUMP(0),
};

const union AffineAnimCmd gUnknown_08593754[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd gUnknown_08593764[] =
{
    AFFINEANIMCMD_FRAME(0xFFF8, 0x4, 0, 8),
    AFFINEANIMCMD_LOOP(0),
    AFFINEANIMCMD_FRAME(0x10, 0xFFF8, 0, 8),
    AFFINEANIMCMD_FRAME(0xFFF0, 0x8, 0, 8),
    AFFINEANIMCMD_LOOP(1),
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 15),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_085937A4[] =
{
    gUnknown_08593734,
    gUnknown_08593754,
    gUnknown_08593764,
};
//*
const struct SpriteTemplate gUnknown_085937B0 =
{
    .tileTag = ANIM_TAG_BREAKING_EGG,
    .paletteTag = ANIM_TAG_BREAKING_EGG,
    .oam = &gOamData_AffineDouble_ObjBlend_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_085937A4,
    .callback = sub_8105538,
};

const union AffineAnimCmd gUnknown_085937C8[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 30),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd gUnknown_085937E0[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0x20, 0x20, 0, 15),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd gUnknown_085937F8[] =
{
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0xB, 0xB, 0, 45),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd *const gUnknown_08593810[] =
{
    gUnknown_085937C8,
    gUnknown_085937E0,
};

const union AffineAnimCmd *const gUnknown_08593818[] =
{
    gUnknown_085937F8,
};

const struct SpriteTemplate gUnknown_0859381C =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjNormal_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593810,
    .callback = sub_80A77C8,
};
//*
const union AffineAnimCmd gUnknown_08593834[] =
{
    AFFINEANIMCMD_FRAME(0x200, 0x200, 0, 0),
    AFFINEANIMCMD_FRAME(0xFFF0, 0xFFF0, 0, 30),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd *const gUnknown_0859384C[] =
{
    gUnknown_08593834,
};

const struct SpriteTemplate gUnknown_08593850 =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_0859384C,
    .callback = sub_80A77C8,
};

const struct SpriteTemplate gUnknown_08593868 =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593810,
    .callback = sub_81051C4,
};

const struct SpriteTemplate gUnknown_08593880 =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593818,
    .callback = sub_81052A4,
};

const struct SpriteTemplate gBattleAnimSpriteTemplate_8593898 =
{
    .tileTag = ANIM_TAG_THIN_RING,
    .paletteTag = ANIM_TAG_THIN_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x64,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593810,
    .callback = sub_81054E8,
};

const union AffineAnimCmd gUnknown_085938B0[] =
{
    AFFINEANIMCMD_FRAME(96, -13, 0, 8),
    AFFINEANIMCMD_END,
};

const union AnimCmd gUnknown_085938C0[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_FRAME(4, 3),
    ANIMCMD_FRAME(8, 3),
    ANIMCMD_FRAME(4, 3),
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_085938D8[] =
{
    gUnknown_085938C0,
};

const struct SpriteTemplate gUnknown_085938DC =
{
    .tileTag = ANIM_TAG_SPEED_DUST,
    .paletteTag = ANIM_TAG_SPEED_DUST,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gUnknown_085938D8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8105C48,
};

const s8 gUnknown_085938F4[][2] =
{
    {30, 28},
    {-20, 24},
    {16, 26},
    {-10, 28},
};

const union AnimCmd gUnknown_085938FC[] =
{
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 15),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 15, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 6, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(32, 15),
    ANIMCMD_FRAME(16, 6),
    ANIMCMD_FRAME(0, 6),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593934[] =
{
    gUnknown_085938FC,
};

const struct SpriteTemplate gUnknown_08593938 =
{
    .tileTag = ANIM_TAG_BELL,
    .paletteTag = ANIM_TAG_BELL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593934,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_80A77C8,
};

const u16 gUnknown_08593950[] =
{
    ANIM_TAG_MUSIC_NOTES_2,
    ANIM_SPRITES_START - 1,
    ANIM_SPRITES_START - 2,
};

const struct SpriteTemplate gUnknown_08593958 =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8105DE8,
};

const struct SpriteTemplate gMagentaHeartSpriteTemplate =
{
    .tileTag = ANIM_TAG_MAGENTA_HEART,
    .paletteTag = ANIM_TAG_MAGENTA_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8105E60,
};

const union AffineAnimCmd gUnknown_08593988[] =
{
    AFFINEANIMCMD_FRAME(0x000A, 0xFFF3, 0x00, 0x0A),
    AFFINEANIMCMD_FRAME(0xFFF6, 0x000D, 0x00, 0x0A),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gUnknown_085939A0 =
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106140,
};

const struct SpriteTemplate gBattleAnimSpriteTemplate_85939B8 =
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_81061C4,
};

const struct SpriteTemplate gUnknown_085939D0 =
{
    .tileTag = ANIM_TAG_RED_HEART,
    .paletteTag = ANIM_TAG_RED_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_810624C,
};

const union AffineAnimCmd gUnknown_085939E8[] =
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd *const gUnknown_08593A00[] =
{
    gUnknown_085939E8,
};

const struct SpriteTemplate gHiddenPowerOrbSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_ORB,
    .paletteTag = ANIM_TAG_RED_ORB,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593A00,
    .callback = AnimOrbitFast,
};

const struct SpriteTemplate gHiddenPowerOrbScatterSpriteTemplate =
{
    .tileTag = ANIM_TAG_RED_ORB,
    .paletteTag = ANIM_TAG_RED_ORB,
    .oam = &gOamData_AffineDouble_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593A00,
    .callback = AnimOrbitScatter,
};

const union AffineAnimCmd gUnknown_08593A34[] =
{
    AFFINEANIMCMD_FRAME(0x80, 0x80, 0, 0),
    AFFINEANIMCMD_FRAME(0x8, 0x8, 0, 1),
    AFFINEANIMCMD_JUMP(1),
};

const union AffineAnimCmd *const gUnknown_08593A4C[] =
{
    gUnknown_08593A34,
};

const struct SpriteTemplate gUnknown_08593A50 =
{
    .tileTag = ANIM_TAG_RED_ORB_2,
    .paletteTag = ANIM_TAG_RED_ORB_2,
    .oam = &gOamData_AffineDouble_ObjNormal_8x8,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593A4C,
    .callback = sub_8106944,
};

const union AnimCmd gUnknown_08593A68[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593A80[] =
{
    gUnknown_08593A68,
};

const struct SpriteTemplate gUnknown_08593A84 =
{
    .tileTag = ANIM_TAG_EYE_SPARKLE,
    .paletteTag = ANIM_TAG_EYE_SPARKLE,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gUnknown_08593A80,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_81069B8,
};

const union AnimCmd gUnknown_08593A9C[] =
{
    ANIMCMD_FRAME(0, 24),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593AA4[] =
{
    gUnknown_08593A9C,
};

const struct SpriteTemplate gUnknown_08593AA8 =
{
    .tileTag = ANIM_TAG_ANGEL,
    .paletteTag = ANIM_TAG_ANGEL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593AA4,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_81069D0,
};

const struct SpriteTemplate gUnknown_08593AC0 =
{
    .tileTag = ANIM_TAG_PINK_HEART,
    .paletteTag = ANIM_TAG_PINK_HEART,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106AD0,
};

const union AnimCmd gUnknown_08593AD8[] =
{
    ANIMCMD_FRAME(0, 3),
    ANIMCMD_JUMP(0),
};

const union AnimCmd gUnknown_08593AE0[] =
{
    ANIMCMD_FRAME(16, 3),
    ANIMCMD_JUMP(0),
};

const union AnimCmd *const gUnknown_08593AE8[] =
{
    gUnknown_08593AD8,
    gUnknown_08593AE0,
};

const struct SpriteTemplate gUnknown_08593AF0 =
{
    .tileTag = ANIM_TAG_DEVIL,
    .paletteTag = ANIM_TAG_DEVIL,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593AE8,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106B54,
};

const union AnimCmd gUnknown_08593B08[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(16, 4),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_FRAME(48, 4),
    ANIMCMD_END,
};

const union AnimCmd gUnknown_08593B1C[] =
{
    ANIMCMD_FRAME(0, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 4, .hFlip = TRUE),
    ANIMCMD_FRAME(48, 4, .hFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593B30[] =
{
    gUnknown_08593B08,
    gUnknown_08593B1C,
};

const struct SpriteTemplate gUnknown_08593B38 =
{
    .tileTag = ANIM_TAG_SWIPE,
    .paletteTag = ANIM_TAG_SWIPE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593B30,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106C80,
};

const union AnimCmd gUnknown_08593B50[] =
{
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_END,
};

const union AnimCmd gUnknown_08593B64[] =
{
    ANIMCMD_FRAME(16, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(32, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(16, 8, .hFlip = TRUE),
    ANIMCMD_FRAME(0, 8, .hFlip = TRUE),
    ANIMCMD_END,
};

const union AnimCmd *const gUnknown_08593B78[] =
{
    gUnknown_08593B50,
    gUnknown_08593B64,
};

const struct SpriteTemplate gUnknown_08593B80 =
{
    .tileTag = ANIM_TAG_MOVEMENT_WAVES,
    .paletteTag = ANIM_TAG_MOVEMENT_WAVES,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gUnknown_08593B78,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106CD0,
};

const union AffineAnimCmd gUnknown_08593B98[] =
{
    AFFINEANIMCMD_FRAME(-12, 8, 0, 4),
    AFFINEANIMCMD_FRAME(20, -20, 0, 4),
    AFFINEANIMCMD_FRAME(-8, 12, 0, 4),
    AFFINEANIMCMD_END,
};

const struct SpriteTemplate gBattleAnimSpriteTemplate_8593BB8 =
{
    .tileTag = ANIM_TAG_JAGGED_MUSIC_NOTE,
    .paletteTag = ANIM_TAG_JAGGED_MUSIC_NOTE,
    .oam = &gOamData_AffineOff_ObjNormal_32x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = sub_8106E00,
};

const union AffineAnimCmd gUnknown_08593BD0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 0, 5),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd gUnknown_08593BE0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, -8, 16),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd gUnknown_08593BF0[] =
{
    AFFINEANIMCMD_FRAME(0x0, 0x0, 8, 16),
    AFFINEANIMCMD_END_ALT(1),
};

const union AffineAnimCmd *const gUnknown_08593C00[] =
{
    gUnknown_08593BD0,
    gUnknown_08593BE0,
    gUnknown_08593BF0,
};

extern const union AnimCmd *const gUnknown_08592FCC[];
const struct SpriteTemplate gUnknown_08593C0C =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gUnknown_08592FCC,
    .images = NULL,
    .affineAnims = gUnknown_08593C00,
    .callback = sub_8106F60,
};

const struct SpriteTemplate gUnknown_08593C24 =
{
    .tileTag = ANIM_TAG_MUSIC_NOTES_2,
    .paletteTag = ANIM_TAG_MUSIC_NOTES_2,
    .oam = &gOamData_AffineNormal_ObjNormal_16x16,
    .anims = gUnknown_08592FCC,
    .images = NULL,
    .affineAnims = gUnknown_08593C00,
    .callback = sub_8106F00,
};

const union AffineAnimCmd gUnknown_08593C3C[] =
{
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd gUnknown_08593C4C[] =
{
    AFFINEANIMCMD_FRAME(0x200, 0x100, 0, 0),
    AFFINEANIMCMD_END,
};

const union AffineAnimCmd *const gUnknown_08593C5C[] =
{
    gUnknown_08593C3C,
    gUnknown_08593C4C,
};

const struct SpriteTemplate gUnknown_08593C64 =
{
    .tileTag = ANIM_TAG_GUARD_RING,
    .paletteTag = ANIM_TAG_GUARD_RING,
    .oam = &gOamData_AffineDouble_ObjBlend_64x32,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gUnknown_08593C5C,
    .callback = sub_81070AC,
};

void sub_8103448(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    SetAnimSpriteInitialXOffset(sprite, gBattleAnimArgs[0]);
    sprite->pos1.y += gBattleAnimArgs[1];
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->data[2] = gBattleAnimArgs[4];
    sprite->data[3] = gBattleAnimArgs[5];
    sprite->data[4] = gBattleAnimArgs[3];
    StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
    sprite->callback = TranslateSpriteInEllipseOverDuration;
    sprite->callback(sprite);
}

void sub_8103498(struct Sprite *sprite)
{
    u8 battler;
    if (gBattleAnimArgs[0] == 0)
        battler = gBattleAnimAttacker;
    else
        battler = gBattleAnimTarget;

    sub_810310C(battler, sprite);
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->callback = sub_81034D8;
}

static void sub_81034D8(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        sprite->pos2.y -= 3;
        if (++sprite->data[1] == 6)
            sprite->data[0]++;
        break;
    case 1:
        sprite->pos2.y += 3;
        if (--sprite->data[1] == 0)
            sprite->data[0]++;
        break;
    case 2:
        if (++sprite->data[1] == 64)
            DestroyAnimSprite(sprite);
        break;
    }
}

static void sub_810353C(struct Sprite *sprite)
{
    s16 temp;
    gSprites[sprite->data[2]].pos2.x += sprite->data[1];
    temp = sprite->data[1];
    sprite->data[1] = -temp;
    if (sprite->data[0] == 0)
    {
        gSprites[sprite->data[2]].pos2.x = 0;
        DestroySpriteAndMatrix(sprite);
    }

    sprite->data[0]--;
}

void sub_810358C(struct Sprite *sprite)
{
    u8 spriteId;
    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
    spriteId = gBattlerSpriteIds[gBattleAnimTarget];
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->pos1.x -= gBattleAnimArgs[0];
    else
        sprite->pos1.x += gBattleAnimArgs[0];

    sprite->pos1.y += gBattleAnimArgs[1];
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[2] = spriteId;
    sprite->callback = sub_810353C;
    sprite->invisible = 1;
}

void sub_8103620(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[3];
    sprite->data[5] = gBattleAnimArgs[4];
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, sub_8103658);
}

static void sub_8103658(struct Sprite *sprite)
{
    sprite->data[0] = sprite->data[1];
    sprite->data[2] = sprite->pos1.x;
    sprite->data[4] = sprite->pos1.y + 15;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, sub_8103680);
}

static void sub_8103680(struct Sprite *sprite)
{
    if (sprite->data[5] == 0)
        DestroyAnimSprite(sprite);
    else
        sprite->data[5]--;
}

// Rotates the attacking mon sprite downwards and then back upwards to its original position.
// No args.
void AnimTask_Withdraw(u8 taskId)
{
    PrepareBattlerSpriteForRotScale(gBattlerSpriteIds[gBattleAnimAttacker], ST_OAM_OBJ_NORMAL);
    gTasks[taskId].func = AnimTask_WithdrawStep;
}

static void AnimTask_WithdrawStep(u8 taskId)
{
    u8 spriteId = gBattlerSpriteIds[gBattleAnimAttacker];
    s16 rotation;
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        rotation = -gTasks[taskId].data[0];
    else
        rotation = gTasks[taskId].data[0];

    SetSpriteRotScale(spriteId, 0x100, 0x100, rotation);
    if (gTasks[taskId].data[1] == 0)
    {
        gTasks[taskId].data[0] += 0xB0;
        // this y position update gets overwritten by SetBattlerSpriteYOffsetFromRotation()
        gSprites[spriteId].pos2.y++;
    }
    else if (gTasks[taskId].data[1] == 1)
    {
        if (++gTasks[taskId].data[3] == 30)
            gTasks[taskId].data[1] = 2;

        return;
    }
    else
    {
        gTasks[taskId].data[0] -= 0xB0;
        // this y position update gets overwritten by SetBattlerSpriteYOffsetFromRotation()
        gSprites[spriteId].pos2.y--;
    }

    SetBattlerSpriteYOffsetFromRotation(spriteId);
    if (gTasks[taskId].data[0] == 0xF20 || gTasks[taskId].data[0] == 0)
    {
        if (gTasks[taskId].data[1] == 2)
        {
            ResetSpriteRotScale(spriteId);
            DestroyAnimVisualTask(taskId);
        }
        else
        {
            gTasks[taskId].data[1]++;
        }
    }
}

// Animates a "zap of energy" used in KINESIS.
// arg 0: x pixel offset
// arg 1: y pixel offset
// arg 2: vertical flip
void Anim_KinesisZapEnergy(struct Sprite *sprite)
{
    SetSpriteCoordsToAnimAttackerCoords(sprite);
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        sprite->pos1.x -= gBattleAnimArgs[0];
    else
        sprite->pos1.x += gBattleAnimArgs[0];

    sprite->pos1.y += gBattleAnimArgs[1];
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        sprite->hFlip = 1;
        if (gBattleAnimArgs[2])
            sprite->vFlip = 1;
    }
    else
    {
        if (gBattleAnimArgs[2])
            sprite->vFlip = 1;
    }

    sprite->callback = RunStoredCallbackWhenAnimEnds;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Animates a sword that rises into the air after a brief pause.
// arg 0: x pixel offset
// arg 1: y pixel offset
void Anim_SwordsDanceBlade(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
    StoreSpriteCallbackInData6(sprite, Anim_SwordsDanceBladeStep);
}

static void Anim_SwordsDanceBladeStep(struct Sprite *sprite)
{
    sprite->data[0] = 6;
    sprite->data[2] = sprite->pos1.x;
    sprite->data[4] = sprite->pos1.y - 32;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

// Moves a projectile towards the target mon. The sprite is rotated to be pointing
// in the same direction it's moving.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: target x pixel offset
// arg 3: target y pixel offset
// arg 4: duration
void AnimSonicBoomProjectile(struct Sprite *sprite)
{
    s16 targetXPos;
    s16 targetYPos;
    u16 rotation;

    if (IsContest())
    {
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
    }
    else if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
    {
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];
        gBattleAnimArgs[1] = -gBattleAnimArgs[1];
        gBattleAnimArgs[3] = -gBattleAnimArgs[3];
    }

    InitSpritePosToAnimAttacker(sprite, TRUE);
    targetXPos = GetBattlerSpriteCoord(gBattleAnimTarget, 2) + gBattleAnimArgs[2];
    targetYPos = GetBattlerSpriteCoord(gBattleAnimTarget, 3) + gBattleAnimArgs[3];
    rotation = ArcTan2Neg(targetXPos - sprite->pos1.x, targetYPos - sprite->pos1.y);
    rotation += 0xF000;
    if (IsContest())
        rotation -= 0x6000;

    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, rotation);
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = targetXPos;
    sprite->data[4] = targetYPos;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

static void sub_81039CC(struct Sprite *sprite)
{
    if (sprite->data[0]-- <= 0)
    {
        gTasks[sprite->data[7]].data[1]--;
        DestroySprite(sprite);
    }
}

static void sub_8103A00(struct Sprite *sprite)
{
    struct Task* task = &gTasks[sprite->data[7]];
    if (sprite->data[0] > task->data[5])
    {
        sprite->data[5] += sprite->data[3];
        sprite->data[6] += sprite->data[4];
    }
    else
    {
        sprite->data[5] -= sprite->data[3];
        sprite->data[6] -= sprite->data[4];
    }

    sprite->data[1] += sprite->data[5];
    sprite->data[2] += sprite->data[6];
    if (1 & task->data[7])
        sprite->pos2.x = ((u16)sprite->data[1] >> 8) * -1;
    else
        sprite->pos2.x = (u16)sprite->data[1] >> 8;

    if (1 & task->data[8])
        sprite->pos2.y = ((u16)sprite->data[2] / 256u) * -1;
    else
        sprite->pos2.y = (u16)sprite->data[2] / 256u;

    if (sprite->data[0]-- <= 0)
    {
        sprite->data[0] = 30;
        sprite->callback = sub_81039CC;
    }
}

void sub_8103AA4(struct Sprite *sprite)
{
    s16 a;
    s16 b;
    s16 c;

    struct Task* task = &gTasks[sprite->data[7]];
    sprite->data[1] += (-2 & task->data[7]);
    sprite->data[2] += (-2 & task->data[8]);
    if (1 & task->data[7])
        sprite->pos2.x = ((u16)sprite->data[1] >> 8) * -1;
    else
        sprite->pos2.x = (u16)sprite->data[1] >> 8;

    if (1 & task->data[8])
        sprite->pos2.y = ((u16)sprite->data[2] / 256u) * -1;
    else
        sprite->pos2.y = (u16)sprite->data[2] / 256u;

    if (sprite->data[0]-- <= 0)
    {
        sprite->data[0] = 8;
        task->data[5] = 4;
        a = sub_8151624(0x1000);
        sprite->pos1.x += sprite->pos2.x;
        sprite->pos1.y += sprite->pos2.y;
        sprite->pos2.y = 0;
        sprite->pos2.x = 0;
        if (task->data[11] >= sprite->pos1.x)
            b = (task->data[11] - sprite->pos1.x) << 8;
        else
            b = (sprite->pos1.x - task->data[11]) << 8;

        if (task->data[12] >= sprite->pos1.y)
            c = (task->data[12] - sprite->pos1.y) << 8;
        else
            c = (sprite->pos1.y - task->data[12]) << 8;

        sprite->data[2] = 0;
        sprite->data[1] = 0;
        sprite->data[6] = 0;
        sprite->data[5] = 0;
        sprite->data[3] = sub_8151534(sub_8151534(b, a), sub_8151624(0x1C0));
        sprite->data[4] = sub_8151534(sub_8151534(c, a), sub_8151624(0x1C0));
        sprite->callback = sub_8103A00;
    }
}

static void sub_8103BE4(u8 taskId)
{
    if (gTasks[taskId].data[1] == 0)
        DestroyAnimVisualTask(taskId);
}

static void sub_8103C0C(u8 taskId)
{
    if (gTasks[taskId].data[0]-- <= 0)
    {
        u8 spriteId;
        struct Sprite *sprite;
        spriteId = CreateSprite(&gUnknown_085933B0, gTasks[taskId].data[9], gTasks[taskId].data[10], gTasks[taskId].data[2] - gTasks[taskId].data[1]);
        sprite = &gSprites[spriteId];
        switch (gTasks[taskId].data[4])
        {
        case 1:
            sprite->oam.matrixNum |= (ST_OAM_HFLIP | ST_OAM_VFLIP);
            break;
        case 2:
            sprite->oam.matrixNum = ST_OAM_HFLIP;
            break;
        }

        sprite->data[0] = gTasks[taskId].data[5] - gTasks[taskId].data[6];
        sprite->data[7] = taskId;
        gTasks[taskId].data[gTasks[taskId].data[1] + 13] = spriteId;
        gTasks[taskId].data[0] = gTasks[taskId].data[3];
        gTasks[taskId].data[1]++;
        PlaySE12WithPanning(SE_W059B, BattleAnimAdjustPanning(-63));
        if (gTasks[taskId].data[1] > 2)
            gTasks[taskId].func = sub_8103BE4;
    }
}

void sub_8103CF0(u8 taskId)
{
    s16 attackerY = 0;
    s16 attackerX = 0;
    s16 targetX = 0;
    s16 targetY = 0;
    s16 xDiff, yDiff;

    if (IsContest())
    {
        gTasks[taskId].data[4] = 2;
        gBattleAnimArgs[0] = -gBattleAnimArgs[0];
        if (gBattleAnimArgs[2] & 1)
            gBattleAnimArgs[2] &= ~1;
        else
            gBattleAnimArgs[2] |= 1;
    }
    else
    {
        if ((gBattlerPositions[gBattleAnimTarget] & BIT_SIDE) == B_SIDE_PLAYER)
        {
            gTasks[taskId].data[4] = 1;
            gBattleAnimArgs[0] = -gBattleAnimArgs[0];
            gBattleAnimArgs[1] = -gBattleAnimArgs[1];
            if (gBattleAnimArgs[2] & 1)
                gBattleAnimArgs[2] &= ~1;
            else
                gBattleAnimArgs[2] |= 1;
        }
    }

    attackerX = gTasks[taskId].data[9] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X);
    attackerY = gTasks[taskId].data[10] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y);
    if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
        && IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimTarget)))
    {
        SetAverageBattlerPositions(gBattleAnimTarget, 0, &targetX, &targetY);
    }
    else
    {
        targetX = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X);
        targetY = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y);
    }

    targetX = gTasks[taskId].data[11] = targetX + gBattleAnimArgs[0];
    targetY = gTasks[taskId].data[12] = targetY + gBattleAnimArgs[1];
    if (targetX >= attackerX)
        xDiff = targetX - attackerX;
    else
        xDiff = attackerX - targetX;

    gTasks[taskId].data[5] = sub_8151534(xDiff, sub_8151624(gBattleAnimArgs[2] & ~1));
    gTasks[taskId].data[6] = sub_8151534(gTasks[taskId].data[5], 0x80);
    gTasks[taskId].data[7] = gBattleAnimArgs[2];
    if (targetY >= attackerY)
    {
        yDiff = targetY - attackerY;
        gTasks[taskId].data[8] = sub_8151534(yDiff, sub_8151624(gTasks[taskId].data[5])) & ~1;
    }
    else
    {
        yDiff = attackerY - targetY;
        gTasks[taskId].data[8] = sub_8151534(yDiff, sub_8151624(gTasks[taskId].data[5])) | 1;
    }

    gTasks[taskId].data[3] = gBattleAnimArgs[3];
    if (gBattleAnimArgs[4] & 0x80)
    {
        gBattleAnimArgs[4] ^= 0x80;
        if (gBattleAnimArgs[4] >= 64)
        {
            u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) + (gBattleAnimArgs[4] - 64);
            gTasks[taskId].data[2] = var;
        }
        else
        {
            u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) - gBattleAnimArgs[4];
            gTasks[taskId].data[2] = var;
        }
    }
    else
    {
        if (gBattleAnimArgs[4] >= 64)
        {
            u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) + (gBattleAnimArgs[4] - 64);
            gTasks[taskId].data[2] = var;
        }
        else
        {
            u16 var = GetBattlerSpriteSubpriority(gBattleAnimTarget) - gBattleAnimArgs[4];
            gTasks[taskId].data[2] = var;
        }
    }

    if (gTasks[taskId].data[2] < 3)
        gTasks[taskId].data[2] = 3;

    gTasks[taskId].func = sub_8103C0C;
}

void sub_8103FE8(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
    sprite->data[0] = 0x100 + (IndexOfSpritePaletteTag(gUnknown_085934A0.paletteTag) << 4);
    sprite->callback = sub_8104018;
}

static void sub_8104018(struct Sprite *sprite)
{
    u16 id, val;
    int i;

    if (++sprite->data[1] == 2)
    {
        sprite->data[1] = 0;
        id = sprite->data[0];
        val = gPlttBufferFaded[8 + id];
        for (i = 8; i < 16; i++)
            gPlttBufferFaded[i + id] = gPlttBufferFaded[i + id + 1];

        gPlttBufferFaded[id + 15] = val;

        if (++sprite->data[2] == 24)
            DestroyAnimSprite(sprite);
    }
}

void sub_8104088(struct Sprite *sprite)
{
    s16 r6;
    s16 r7;
    u16 var;

    InitSpritePosToAnimAttacker(sprite, TRUE);
    r6 = GetBattlerSpriteCoord(gBattleAnimTarget, 2);
    r7 = GetBattlerSpriteCoord(gBattleAnimTarget, 3) + gBattleAnimArgs[3];
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    r6 += gBattleAnimArgs[2];
    var = ArcTan2Neg(r6 - sprite->pos1.x, r7 - sprite->pos1.y);
    var += 0xC000;
    TrySetSpriteRotScale(sprite, FALSE, 0x100, 0x100, var);
    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = r6;
    sprite->data[4] = r7;
    sprite->callback = sub_80A7000;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void sub_810413C(struct Sprite *sprite)
{
    sprite->data[2] = -16;
    sprite->pos1.y += 8;
    sprite->callback = sub_8104154;
}

static void sub_8104154(struct Sprite *sprite)
{
    sprite->data[0] += 0x80;
    sprite->pos2.x = sprite->data[0] >> 8;
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->pos2.x = -sprite->pos2.x;

    sprite->pos2.y = Sin(sprite->data[1], sprite->data[2]);
    sprite->data[1] += 5;
    if (sprite->data[1] > 126)
    {
        sprite->data[1] = 0;
        sprite->data[2] /= 2;
        if (++sprite->data[3] == 2)
            DestroyAnimSprite(sprite);
    }
}

void sub_81041C4(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = 20;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2);
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->callback = StartAnimLinearTranslation;
    sprite->affineAnimPaused = 1;
    StoreSpriteCallbackInData6(sprite, sub_810421C);
}

static void sub_810421C(struct Sprite *sprite)
{
    int i;
    u16 rand;
    s16* ptr;
    PlaySE12WithPanning(SE_W030, BattleAnimAdjustPanning(63));
    sprite->pos1.x += sprite->pos2.x;
    sprite->pos1.y += sprite->pos2.y;
    sprite->pos2.y = 0;
    sprite->pos2.x = 0;
    ptr = &sprite->data[7];
    for (i = 0; i < 8; i++)
        ptr[i - 7] = 0;

    rand = Random2();
    sprite->data[6] = 0xFFF4 - (rand & 7);
    rand = Random2();
    sprite->data[7] = (rand % 0xA0) + 0xA0;
    sprite->callback = sub_81042A0;
    sprite->affineAnimPaused = 0;
}

static void sub_81042A0(struct Sprite *sprite)
{
    sprite->data[0] += sprite->data[7];
    sprite->pos2.x = sprite->data[0] >> 8;
    if (sprite->data[7] & 1)
        sprite->pos2.x = -sprite->pos2.x;

    sprite->pos2.y = Sin(sprite->data[1], sprite->data[6]);
    sprite->data[1] += 8;
    if (sprite->data[1] > 126)
    {
        sprite->data[1] = 0;
        sprite->data[2] /= 2;
        if (++sprite->data[3] == 1)
            DestroyAnimSprite(sprite);
    }
}

// Moves a tornado in a circlular motion.
// arg 0: initial x pixel offset
// arg 1: initial y pixel offset
// arg 2: wave amplitude
// arg 3: unused
// arg 4: initial wave offset
// arg 5: wave period (higher means faster wave)
// arg 6: duration
void Anim_RazorWindTornado(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
        sprite->pos1.y += 16;

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[1] = gBattleAnimArgs[2];
    sprite->data[2] = gBattleAnimArgs[5];
    sprite->data[3] = gBattleAnimArgs[6];
    sprite->data[4] = gBattleAnimArgs[3];
    sprite->callback = TranslateSpriteInCircleOverDuration;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback(sprite);
}

// Animates a single pincer line that extends towards the center of the target mon.
// arg 0: invert
void Anim_ViceGripPincer(struct Sprite *sprite)
{
    s16 startXOffset = 32;
    s16 startYOffset = -32;
    s16 endXOffset = 16;
    s16 endYOffset = -16;
    if (gBattleAnimArgs[0])
    {
        startXOffset = -32;
        startYOffset = 32;
        endXOffset = -16;
        endYOffset = 16;
        StartSpriteAnim(sprite, 1);
    }

    sprite->pos1.x += startXOffset;
    sprite->pos1.y += startYOffset;
    sprite->data[0] = 6;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + endXOffset;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + endYOffset;
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, Anim_ViceGripPincerStep);
}

static void Anim_ViceGripPincerStep(struct Sprite *sprite)
{
    if (sprite->animEnded)
        DestroyAnimSprite(sprite);
}

// Animates a single pincer line that extends towards the center of the target mon, and then back out.
// arg 0: animation id
void Anim_GuillotinePincer(struct Sprite *sprite)
{
    s16 startXOffset = 32;
    s16 startYOffset = -32;
    s16 endXOffset = 16;
    s16 endYOffset = -16;
    if (gBattleAnimArgs[0])
    {
        startXOffset = -32;
        startYOffset = 32;
        endXOffset = -16;
        endYOffset = 16;
        StartSpriteAnim(sprite, gBattleAnimArgs[0]);
    }

    sprite->pos1.x += startXOffset;
    sprite->pos1.y += startYOffset;
    sprite->data[0] = 6;
    sprite->data[1] = sprite->pos1.x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_X_2) + endXOffset;
    sprite->data[3] = sprite->pos1.y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, BATTLER_COORD_Y_PIC_OFFSET) + endYOffset;
    InitAnimLinearTranslation(sprite);
    sprite->data[5] = gBattleAnimArgs[0];
    sprite->data[6] = sprite->data[0];
    sprite->callback = Anim_GuillotinePincerStep1;
}

static void Anim_GuillotinePincerStep1(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite) && sprite->animEnded)
    {
        SeekSpriteAnim(sprite, 0);
        sprite->animPaused = 1;
        sprite->pos1.x += sprite->pos2.x;
        sprite->pos1.y += sprite->pos2.y;
        sprite->pos2.x = 2;
        sprite->pos2.y = -2;
        sprite->data[0] = sprite->data[6];
        sprite->data[1] ^= 1;
        sprite->data[2] ^= 1;
        sprite->data[4] = 0;
        sprite->data[3] = 0;
        sprite->callback = Anim_GuillotinePincerStep2;
    }
}

static void Anim_GuillotinePincerStep2(struct Sprite *sprite)
{
    if (sprite->data[3])
    {
        sprite->pos2.x = -sprite->pos2.x;
        sprite->pos2.y = -sprite->pos2.y;
    }

    sprite->data[3] ^= 1;
    if (++sprite->data[4] == 51)
    {
        sprite->pos2.y = 0;
        sprite->pos2.x = 0;
        sprite->data[4] = 0;
        sprite->data[3] = 0;
        sprite->animPaused = 0;
        StartSpriteAnim(sprite, sprite->data[5] ^ 1);
        sprite->callback = Anim_GuillotinePincerStep3;
    }
}

static void Anim_GuillotinePincerStep3(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite))
        DestroyAnimSprite(sprite);
}

// Scales up the target mon sprite, and sets the palette to greyscale.
// Used in MOVE_DISABLE.
// No args.
void AnimTask_GrowAndGreyscale(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_BLEND);
    SetSpriteRotScale(spriteId, 0xD0, 0xD0, 0);
    SetGreyscaleOrOriginalPalette(gSprites[spriteId].oam.paletteNum + 16, FALSE);
    gTasks[taskId].data[0] = 80;
    gTasks[taskId].func = AnimTask_GrowAndGreyscaleStep;
}

static void AnimTask_GrowAndGreyscaleStep(u8 taskId)
{
    if (--gTasks[taskId].data[0] == -1)
    {
        u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
        ResetSpriteRotScale(spriteId);
        SetGreyscaleOrOriginalPalette(gSprites[spriteId].oam.paletteNum + 16, TRUE);
        DestroyAnimVisualTask(taskId);
    }
}

// Shrinks and grows the attacking mon several times. Also creates transparent versions of the
// mon's sprite while it is shrinking.
// No args.
void AnimTask_Minimize(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[0] = spriteId;
    PrepareBattlerSpriteForRotScale(spriteId, ST_OAM_OBJ_NORMAL);
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0x100;
    task->data[5] = 0;
    task->data[6] = 0;
    task->data[7] = GetBattlerSpriteSubpriority(gBattleAnimAttacker);
    task->func = AnimTask_MinimizeStep1;
}

static void AnimTask_MinimizeStep1(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    switch (task->data[1])
    {
    case 0:
        if (task->data[2] == 0 || task->data[2] == 3 || task->data[2] == 6)
            CreateMinimizeSprite(task, taskId);
        task->data[2]++;
        task->data[4] += 0x28;
        SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
        SetBattlerSpriteYOffsetFromYScale(task->data[0]);
        if (task->data[2] == 32)
        {
            task->data[5]++;
            task->data[1]++;
        }
        break;
    case 1:
        if (task->data[6] == 0)
        {
            if (task->data[5] == 3)
            {
                task->data[2] = 0;
                task->data[1] = 3;
            }
            else
            {
                task->data[2] = 0;
                task->data[3] = 0;
                task->data[4] = 0x100;
                SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
                SetBattlerSpriteYOffsetFromYScale(task->data[0]);
                task->data[1] = 2;
            }
        }
        break;
    case 2:
        task->data[1] = 0;
        break;
    case 3:
        if (++task->data[2] > 32)
        {
            task->data[2] = 0;
            task->data[1]++;
        }
        break;
    case 4:
        task->data[2] += 2;
        task->data[4] -= 0x50;
        SetSpriteRotScale(task->data[0], task->data[4], task->data[4], 0);
        SetBattlerSpriteYOffsetFromYScale(task->data[0]);
        if (task->data[2] == 32)
        {
            task->data[2] = 0;
            task->data[1]++;
        }
        break;
    case 5:
        ResetSpriteRotScale(task->data[0]);
        gSprites[task->data[15]].pos2.y = 0;
        DestroyAnimVisualTask(taskId);
        break;
    }
}

static void CreateMinimizeSprite(struct Task* task, u8 taskId)
{
    u16 matrixNum;
    s16 spriteId = CloneBattlerSpriteWithBlend(ANIM_ATTACKER);
    if (spriteId >= 0)
    {
        if ((matrixNum = AllocOamMatrix()) == 0xFF)
        {
            obj_delete_but_dont_free_vram(&gSprites[spriteId]);
        }
        else
        {
            gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
            gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_DOUBLE;
            gSprites[spriteId].affineAnimPaused = TRUE;
            gSprites[spriteId].oam.matrixNum = matrixNum;
            gSprites[spriteId].subpriority = task->data[7] - task->data[3];
            task->data[3]++;
            task->data[6]++;
            gSprites[spriteId].data[0] = 16;
            gSprites[spriteId].data[1] = taskId;
            gSprites[spriteId].data[2] = 6;
            gSprites[spriteId].callback = ClonedMinizeSprite_Step;
            SetSpriteRotScale(spriteId, task->data[4], task->data[4], 0);
            gSprites[spriteId].oam.affineMode = 1;
            CalcCenterToCornerVec(&gSprites[spriteId], gSprites[spriteId].oam.shape, gSprites[spriteId].oam.size, gSprites[spriteId].oam.affineMode);
        }
    }
}

static void ClonedMinizeSprite_Step(struct Sprite *sprite)
{
    if (--sprite->data[0] == 0)
    {
        gTasks[sprite->data[1]].data[sprite->data[2]]--;
        FreeOamMatrix(sprite->oam.matrixNum);
        obj_delete_but_dont_free_vram(sprite);
    }
}

// Task to facilitate expanding and hopping effect seen in Splash.
// arg 0: anim battler
// arg 1: num hops
void AnimTask_Splash(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (gBattleAnimArgs[1] == 0)
    {
        DestroyAnimVisualTask(taskId);
    }
    else
    {
        u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);
        task->data[0] = spriteId;
        task->data[1] = 0;
        task->data[2] = gBattleAnimArgs[1];
        task->data[3] = 0;
        task->data[4] = 0;
        PrepareAffineAnimInTaskData(task, spriteId, gSplashEffectAffineAnimCmds);
        task->func = AnimTask_SplashStep;
    }
}

static void AnimTask_SplashStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    switch (task->data[1])
    {
    case 0:
        RunAffineAnimFromTaskData(task);
        task->data[4] += 3;
        gSprites[task->data[0]].pos2.y += task->data[4];
        if (++task->data[3] > 7)
        {
            task->data[3] = 0;
            task->data[1]++;
        }
        break;
    case 1:
        RunAffineAnimFromTaskData(task);
        gSprites[task->data[0]].pos2.y += task->data[4];
        if (++task->data[3] > 7)
        {
            task->data[3] = 0;
            task->data[1]++;
        }
        break;
    case 2:
        if (task->data[4] != 0)
        {
            gSprites[task->data[0]].pos2.y -= 2;
            task->data[4] -= 2;
        }
        else
            task->data[1]++;
        break;
    case 3:
        if (!RunAffineAnimFromTaskData(task))
        {
            if (--task->data[2] == 0)
            {
                gSprites[task->data[0]].pos2.y = 0;
                DestroyAnimVisualTask(taskId);
            }
            else
            {
                PrepareAffineAnimInTaskData(task, task->data[0], gSplashEffectAffineAnimCmds);
                task->data[1] = 0;
            }
        }
        break;
    }
}

// Grows, pauses, then shrinks the attacking mon.
// Used by MOVE_SWAGGER and MOVE_BULK_UP
// No args.
void AnimTask_GrowAndShrink(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    PrepareAffineAnimInTaskData(task, spriteId, gGrowAndShrinkAffineAnimCmds);
    task->func = AnimTask_GrowAndShrinkStep;
}

static void AnimTask_GrowAndShrinkStep(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (!RunAffineAnimFromTaskData(task))
        DestroyAnimVisualTask(taskId);
}

// Animates a little puff of the mon's breath.
// Used by MOVE_SWAGGER and MOVE_BULK_UP
// No args.
void Anim_BreathPuff(struct Sprite *sprite)
{
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_PLAYER)
    {
        StartSpriteAnim(sprite, 0);
        sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) + 32;
        sprite->data[1] = 64;
    }
    else
    {
        StartSpriteAnim(sprite, 1);
        sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2) - 32;
        sprite->data[1] = -64;
    }

    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    sprite->data[0] = 52;
    sprite->data[2] = 0;
    sprite->data[3] = 0;
    sprite->data[4] = 0;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sprite->callback = TranslateSpriteLinearFixedPoint;
}

// Animates an "angry" mark above a mon's head.
// arg 0: target mon (0 = attacker, 1 = target)
// arg 1: x pixel offset
// arg 2: y pixel offset
void Anim_AngerMark(struct Sprite *sprite)
{
    u8 battler;
    if (!gBattleAnimArgs[0])
        battler = gBattleAnimAttacker;
    else
        battler = gBattleAnimTarget;

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        gBattleAnimArgs[1] *= -1;

    sprite->pos1.x = GetBattlerSpriteCoord(battler, BATTLER_COORD_X_2) + gBattleAnimArgs[1];
    sprite->pos1.y = GetBattlerSpriteCoord(battler, BATTLER_COORD_Y_PIC_OFFSET) + gBattleAnimArgs[2];
    if (sprite->pos1.y < 8)
        sprite->pos1.y = 8;

    StoreSpriteCallbackInData6(sprite, DestroySpriteAndMatrix);
    sprite->callback = RunStoredCallbackWhenAffineAnimEnds;
}

// left/right movements
void sub_8104C38(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[0] = spriteId;
    task->data[1] = 0;
    PrepareAffineAnimInTaskData(task, spriteId, gUnknown_085936A4);
    task->func = sub_8104C78;
}

static void sub_8104C78(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (!RunAffineAnimFromTaskData(task))
        DestroyAnimVisualTask(taskId);
}

// up/down movements
void sub_8104CA4(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    task->data[0] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[1] = 0;
    task->data[2] = 4;
    task->data[3] = 7;
    task->data[4] = 3;
    task->data[5] = gSprites[task->data[0]].pos1.x;
    task->data[6] = gSprites[task->data[0]].pos1.y;
    task->data[7] = 0;
    task->data[8] = 0;
    task->data[9] = 2;
    if (GetBattlerSide(gBattleAnimAttacker) == B_SIDE_OPPONENT)
        task->data[2] *= -1;

    task->func = sub_8104D28;
}

static void sub_8104D28(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (++task->data[7] > 2)
    {
        task->data[7] = 0;
        task->data[8]++;
        if (task->data[8] & 1)
            gSprites[task->data[0]].pos1.y += task->data[9];
        else
            gSprites[task->data[0]].pos1.y -= task->data[9];
    }
    switch (task->data[1])
    {
    case 0:
        gSprites[task->data[0]].pos1.x += task->data[2];
        if (--task->data[3] == 0)
        {
            task->data[3] = 14;
            task->data[1] = 1;
        }
        break;
    case 1:
        gSprites[task->data[0]].pos1.x -= task->data[2];
        if (--task->data[3] == 0)
        {
            task->data[3] = 7;
            task->data[1] = 2;
        }
        break;
    case 2:
        gSprites[task->data[0]].pos1.x += task->data[2];
        if (--task->data[3] == 0)
        {
            if (--task->data[4] != 0)
            {
                task->data[3] = 7;
                task->data[1] = 0;
            }
            else
            {
                if ((task->data[8] & 1) != 0)
                    gSprites[task->data[0]].pos1.y -= task->data[9];

                DestroyAnimVisualTask(taskId);
            }
        }
        break;
    }
}

void sub_8104E74(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    struct ScanlineEffectParams params;

    s16 i;
    task->data[0] = GetBattlerYCoordWithElevation(gBattleAnimTarget) + 32;
    task->data[1] = 4;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0;
    task->data[5] = 0;
    task->data[15] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT);

    if (GetBattlerSpriteBGPriorityRank(gBattleAnimTarget) == 1)
    {
        task->data[6] = gBattle_BG1_X;
        params.dmaDest = (u16 *)REG_ADDR_BG1HOFS;
    }
    else
    {
        task->data[6] = gBattle_BG2_X;
        params.dmaDest = (u16 *)REG_ADDR_BG2HOFS;
    }

    for (i = task->data[0] - 0x40; i <= task->data[0]; i++)
    {
        if (i >= 0)
        {
            gScanlineEffectRegBuffers[0][i] = task->data[6] + 0xF0;
            gScanlineEffectRegBuffers[1][i] = task->data[6] + 0xF0;
        }
    }

    params.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    params.initState = 1;
    params.unused9 = 0;
    ScanlineEffect_SetParams(params);
    task->func = sub_8104F54;
}

static void sub_8104F54(u8 taskId)
{
    struct Task* task = &gTasks[taskId];

    switch (task->data[4])
    {
    case 0:
        if (++task->data[5] > 20)
            task->data[4]++;
        break;
    case 1:
        if (++task->data[1] > 3)
        {
            task->data[1] = 0;
            task->data[2] = task->data[3] & 3;
            task->data[5] = task->data[0] - task->data[3];
            switch (task->data[2])
            {
            case 0:
                break;
            case 1:
                task->data[5] -= 2;
                break;
            case 2:
                task->data[5] += 1;
                break;
            case 3:
                task->data[5] += 1;
                break;
            }

            if (task->data[5] >= 0)
            {
                gScanlineEffectRegBuffers[0][task->data[5]] = task->data[6];
                gScanlineEffectRegBuffers[1][task->data[5]] = task->data[6];
            }

            if (++task->data[3] >= task->data[15])
            {
                gScanlineEffect.state = 3;
                DestroyAnimVisualTask(taskId);
            }
        }
        break;
    }
}

void sub_810501C(struct Sprite *sprite)
{
    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, 0) - 16;
    sprite->pos1.y = GetBattlerYCoordWithElevation(gBattleAnimTarget) + 16;
    sprite->data[0] = 0;
    sprite->data[1] = 0;
    sprite->data[2] = 0;
    sprite->data[3] = 16;
    sprite->data[4] = 0;
    sprite->data[5] = GetBattlerSpriteCoordAttr(gBattleAnimTarget, BATTLER_COORD_ATTR_HEIGHT) + 2;
    sprite->data[6] = BattleAnimAdjustPanning(63);
    sprite->callback = sub_8105078;
}

static void sub_8105078(struct Sprite *sprite)
{
    switch (sprite->data[0])
    {
    case 0:
        if (++sprite->data[2] > 1)
        {
            sprite->data[2] = 0;
            sprite->invisible = !sprite->invisible;
        }
        if (++sprite->data[1] > 16)
        {
            sprite->invisible = 0;
            sprite->data[0]++;
        }
        break;
    case 1:
        if (++sprite->data[1] > 3 && sprite->data[2] < sprite->data[5])
        {
            sprite->data[1] = 0;
            sprite->pos1.y -= 1;
            sprite->data[2]++;
            if (sprite->data[2] % 10 == 0)
                PlaySE12WithPanning(SE_W166, sprite->data[6]);
        }
        sprite->data[4] += sprite->data[3];
        if (sprite->data[4] > 31)
        {
            sprite->data[4] = 0x40 - sprite->data[4];
            sprite->data[3] *= -1;
        }
        else if (sprite->data[4] <= -32)
        {
            sprite->data[4] = -0x40 - sprite->data[4];
            sprite->data[3] *= -1;
        }
        sprite->pos2.x = sprite->data[4];
        if (sprite->data[5] == sprite->data[2])
        {
            sprite->data[1] = 0;
            sprite->data[2] = 0;
            sprite->data[0]++;
        }
        break;
    case 2:
        if (++sprite->data[2] > 1)
        {
            sprite->data[2] = 0;
            sprite->invisible = !sprite->invisible;
        }
        if (++sprite->data[1] > 16)
        {
            sprite->invisible = 0;
            DestroyAnimSprite(sprite);
        }
        break;
    }
}

void sub_81051C4(struct Sprite *sprite)
{
    u8 battler = 0;
    u16 sp0 = 0;
    u16 sp1 = 0;
    u8 r4;

    if (gBattleAnimArgs[2] == 0)
        battler = gBattleAnimAttacker;
    else
        battler = gBattleAnimTarget;

    r4 = gBattleAnimArgs[3] ^ 1;
    if (IsDoubleBattle() && IsBattlerSpriteVisible(BATTLE_PARTNER(battler)))
    {
        SetAverageBattlerPositions(battler, r4, &sp0, &sp1);
        if (r4 == 0)
            r4 = GetBattlerSpriteCoord(battler, 0);
        else
            r4 = GetBattlerSpriteCoord(battler, 2);

        if (GetBattlerSide(battler) != B_SIDE_PLAYER)
            gBattleAnimArgs[0] -= (sp0 - r4) - gBattleAnimArgs[0];  // This is weird.
        else
            gBattleAnimArgs[0] = sp0 - r4;
    }

    sprite->callback = sub_80A77C8;
    sprite->callback(sprite);
}

void sub_8105284(struct Sprite *sprite)
{
    if (AnimTranslateLinear(sprite))
    {
        FreeSpriteOamMatrix(sprite);
        DestroyAnimSprite(sprite);
    }
}

void sub_81052A4(struct Sprite *sprite)
{
    u16 r9 = 0;
    u16 r6 = 0;
    s16 sp0 = 0;
    s16 sp1 = 0;
    u8 sp4;
    u8 battler1;
    u8 battler2;
    u8 r10;

    if (gBattleAnimArgs[5] == 0)
    {
        battler1 = gBattleAnimAttacker;
        battler2 = gBattleAnimTarget;
    }
    else
    {
        battler1 = gBattleAnimTarget;
        battler2 = gBattleAnimAttacker;
    }

    if (!gBattleAnimArgs[6])
    {
        r10 = 0;
        sp4 = 1;
    }
    else
    {
        r10 = 2;
        sp4 = 3;
    }

    if (GetBattlerSide(battler1) != B_SIDE_PLAYER)
    {
        r9 = GetBattlerSpriteCoord(battler1, r10) + gBattleAnimArgs[0];
        if (IsBattlerSpriteVisible(BATTLE_PARTNER(battler2)))
            sprite->subpriority = gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler2)]].subpriority - 1;
        else
            sprite->subpriority = gSprites[gBattlerSpriteIds[battler2]].subpriority - 1;
    }
    else
    {
        r9 = GetBattlerSpriteCoord(battler1, r10) - gBattleAnimArgs[0];
        if (!IsContest() && IsBattlerSpriteVisible(BATTLE_PARTNER(battler1)))
        {
            if (gSprites[gBattlerSpriteIds[battler1]].pos1.x < gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler1)]].pos1.x)
                sprite->subpriority = gSprites[gBattlerSpriteIds[BATTLE_PARTNER(battler1)]].subpriority + 1;
            else
                sprite->subpriority = gSprites[gBattlerSpriteIds[battler1]].subpriority - 1;
        }
        else
        {
            sprite->subpriority = gSprites[gBattlerSpriteIds[battler1]].subpriority - 1;
        }

    }

    r6 = GetBattlerSpriteCoord(battler1, sp4) + gBattleAnimArgs[1];
    if (!IsContest() && IsBattlerSpriteVisible(BATTLE_PARTNER(battler2)))
    {
        SetAverageBattlerPositions(battler2, gBattleAnimArgs[6], &sp0, &sp1);
    }
    else
    {
        sp0 = GetBattlerSpriteCoord(battler2, r10);
        sp1 = GetBattlerSpriteCoord(battler2, sp4);
    }

    if (GetBattlerSide(battler2))
        sp0 += gBattleAnimArgs[3];
    else
        sp0 -= gBattleAnimArgs[3];

    sp1 += gBattleAnimArgs[4];
    sprite->pos1.x = sprite->data[1] = r9;
    sprite->pos1.y = sprite->data[3] = r6;
    sprite->data[2] = sp0;
    sprite->data[4] = sp1;
    sprite->data[0] = gBattleAnimArgs[0];
    InitAnimLinearTranslation(sprite);
    sprite->callback = sub_8105284;
    sprite->callback(sprite);
}

void sub_81054E8(struct Sprite *sprite)
{
    u8 index = IndexOfSpritePaletteTag(ANIM_TAG_THIN_RING);
    if (index != 0xFF)
    {
        BlendPalette(((index << 20) + 0x1010000) >> 16, 15, gBattleAnimArgs[5], gBattleAnimArgs[4]);
    }

    StartSpriteAffineAnim(sprite, 1);
    sprite->callback = sub_80A77C8;
    sprite->callback(sprite);
}

void sub_8105538(struct Sprite *sprite)
{
    s16 r1;
    InitSpritePosToAnimAttacker(sprite, FALSE);
    r1 = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -160 : 160;
    sprite->data[0] = 0x380;
    sprite->data[1] = r1;
    sprite->data[7] = gBattleAnimArgs[2];
    sprite->callback = sub_810557C;
}

static void sub_810557C(struct Sprite *sprite)
{
    s16 add;
    sprite->pos2.y -= (sprite->data[0] >> 8);
    sprite->pos2.x = sprite->data[1] >> 8;
    sprite->data[0] -= 32;
    add = GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER ? -160 : 160;
    sprite->data[1] += add;
    if (sprite->pos2.y > 0)
    {
        sprite->pos1.y += sprite->pos2.y;
        sprite->pos1.x += sprite->pos2.x;
        sprite->pos2.y = 0;
        sprite->pos2.x = 0;
        sprite->data[0] = 0;
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = sub_81055F4;
    }
}

static void sub_81055F4(struct Sprite *sprite)
{
    if (sprite->data[0]++ > 19)
    {
        StartSpriteAffineAnim(sprite, 2);
        sprite->callback = sub_810561C;
    }
}

static void sub_810561C(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
        sprite->data[0] = 0;
        if (sprite->data[7] == 0)
        {
            sprite->oam.tileNum += 16;
            sprite->callback = sub_8105694;
        }
        else
        {
            sprite->oam.tileNum += 32;
            sprite->callback = sub_810571C;
        }
    }
}

static void sub_8105694(struct Sprite *sprite)
{
    sprite->pos2.y -= 2;
    if (++sprite->data[0] == 9)
    {
        sprite->data[0] = 16;
        sprite->data[1] = 0;
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND((u16)sprite->data[0], 0));
        sprite->callback = sub_81056D4;
    }
}

static void sub_81056D4(struct Sprite *sprite)
{
    if (sprite->data[1]++ % 3 == 0)
    {
        sprite->data[0]--;
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(sprite->data[0], 16 - sprite->data[0]));
        if (sprite->data[0] == 0)
            sprite->callback = sub_810571C;
    }
}

static void sub_810571C(struct Sprite *sprite)
{
    if ((u16)gBattleAnimArgs[7] == 0xFFFF)
    {
        sprite->invisible = 1;
        if (sprite->data[7] == 0)
            sprite->callback = sub_810575C;
        else
            sprite->callback = DestroyAnimSprite;
    }
}

static void sub_810575C(struct Sprite *sprite)
{
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    DestroyAnimSprite(sprite);
}

void sub_810577C(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->data[0] = spriteId;
    PrepareAffineAnimInTaskData(task, spriteId, gUnknown_085938B0);
    task->func = sub_81057B8;
}

static void sub_81057B8(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (!RunAffineAnimFromTaskData(task))
    {
        gSprites[task->data[0]].pos2.y = 0;
        gSprites[task->data[0]].invisible = 1;
        DestroyAnimVisualTask(taskId);
    }
}

void sub_8105810(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[12] = 3;
    if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_PLAYER)
    {
        task->data[13] = 0xFFFF;
        task->data[14] = 8;
    }
    else
    {
        task->data[13] = 1;
        task->data[14] = -8;
    }

    task->data[15] = GetAnimBattlerSpriteId(ANIM_TARGET);
    task->func = sub_8105878;
}

static void sub_8105878(u8 taskId)
{
    struct Task* task = &gTasks[taskId];

    switch (task->data[0])
    {
    case 0:
        gSprites[task->data[15]].pos2.x += task->data[14];
        task->data[1] = 0;
        task->data[2] = 0;
        task->data[3] = 0;
        task->data[0]++;
        break;
    case 1:
        if (++task->data[1] > 1)
        {
            task->data[1] = 0;
            task->data[2]++;
            if (task->data[2] & 1)
                gSprites[task->data[15]].pos2.x += 6;
            else
                gSprites[task->data[15]].pos2.x -= 6;

            if (++task->data[3] > 4)
            {
                if (task->data[2] & 1)
                    gSprites[task->data[15]].pos2.x -= 6;

                task->data[0]++;
            }
        }
        break;
    case 2:
        if (--task->data[12] != 0)
            task->data[0] = 0;
        else
            task->data[0]++;
        break;
    case 3:
        gSprites[task->data[15]].pos2.x += task->data[13];
        if (gSprites[task->data[15]].pos2.x == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

void sub_810599C(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    task->data[0] = 0;
    task->data[1] = 0;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 1;
    task->data[13] = 14;
    task->data[14] = 2;
    task->data[15] = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    task->func = sub_81059E0;
}

static void sub_81059E0(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    if (task->data[0] == 0 && ++task->data[1] > task->data[4])
    {
        task->data[1] = 0;
        if (++task->data[2] & 1)
            gSprites[task->data[15]].invisible = 0;
        else
            gSprites[task->data[15]].invisible = 1;

        if (++task->data[3] >= task->data[13])
        {
            if (++task->data[4] < task->data[14])
            {
                task->data[1] = 0;
                task->data[2] = 0;
                task->data[3] = 0;
            }
            else
            {
                gSprites[task->data[15]].invisible = 0;
                DestroyAnimVisualTask(taskId);
            }
        }
    }
}

void sub_8105AAC(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    task->data[0] = 0;
    task->data[1] = 4;
    task->data[2] = 0;
    task->data[3] = 0;
    task->data[4] = 0;
    task->data[5] = 0;
    task->data[6] = 0;
    task->data[7] = 0;
    task->data[8] = 0;
    task->data[13] = 0;
    task->data[14] = GetBattlerSpriteCoord(gBattleAnimAttacker, ANIM_ATTACKER);
    task->data[15] = GetBattlerSpriteCoord(gBattleAnimAttacker, ANIM_TARGET);
    task->func = sub_8105B08;
}

static void sub_8105B08(u8 taskId)
{
    struct Task* task = &gTasks[taskId];
    switch (task->data[8])
    {
    case 0:
        if (++task->data[4] > 1)
        {
            task->data[4] = 0;
            task->data[5] = (task->data[5] + 1) & 1;
            if (++task->data[6] > 20)
            {
                if (task->data[7] == 0)
                {
                    task->data[6] = 0;
                    task->data[8] = 1;
                }
                else
                    task->data[8] = 2;
            }
        }
        break;
    case 1:
        task->data[5] = 0;
        if (++task->data[4] > 20)
        {
            task->data[7] = 1;
            task->data[8] = 0;
        }
        break;
    case 2:
        task->data[5] = 1;
        break;
    }

    switch (task->data[0])
    {
    case 0:
        if (++task->data[1] > 4)
        {
            u8 spriteId;
            task->data[1] = 0;
            spriteId = CreateSprite(&gUnknown_085938DC, task->data[14], task->data[15], 0);
            if (spriteId != MAX_SPRITES)
            {
                gSprites[spriteId].data[0] = taskId;
                gSprites[spriteId].data[1] = 13;
                gSprites[spriteId].pos2.x = gUnknown_085938F4[task->data[2]][0];
                gSprites[spriteId].pos2.y = gUnknown_085938F4[task->data[2]][1];
                task->data[13]++;
                if (++task->data[2] > 3)
                {
                    task->data[2] = 0;
                    if (++task->data[3] > 5)
                        task->data[0]++;
                }
            }
        }
        break;
    case 1:
        if (task->data[13] == 0)
            DestroyAnimVisualTask(taskId);
        break;
    }
}

void sub_8105C48(struct Sprite *sprite)
{
    sprite->invisible = gTasks[sprite->data[0]].data[5];
    if (sprite->animEnded)
    {
        gTasks[sprite->data[0]].data[sprite->data[1]]--;
        DestroySprite(sprite);
    }
}

void sub_8105CB4(u8 taskId)
{
    int i;
    u8 paletteNums[3];

    paletteNums[0] = IndexOfSpritePaletteTag(ANIM_TAG_MUSIC_NOTES_2);
    for (i = 1; i < 3; i++)
        paletteNums[i] = AllocSpritePalette(ANIM_SPRITES_START - i);

    gMonSpritesGfxPtr->field_17C = AllocZeroed(0x2000);
    LZDecompressWram(gBattleAnimSpritePal_MusicNotes2, gMonSpritesGfxPtr->field_17C);
    for (i = 0; i < 3; i++)
        LoadPalette(&gMonSpritesGfxPtr->field_17C[i * 32], (u16)((paletteNums[i] << 4) + 0x100), 32);

    FREE_AND_SET_NULL(gMonSpritesGfxPtr->field_17C);
    DestroyAnimVisualTask(taskId);
}

void sub_8105D60(u8 taskId)
{
    int i;
    for (i = 0; i < 3; i++)
        FreeSpritePaletteByTag(gUnknown_08593950[i]);

    DestroyAnimVisualTask(taskId);
}

static void sub_8105D88(struct Sprite *sprite, u8 a, u8 b)
{
    u8 tile;
    tile = (b & 1);
    tile = ((-tile | tile) >> 31) & 32;
    sprite->oam.tileNum += tile + (a << 2);
    sprite->oam.paletteNum = IndexOfSpritePaletteTag(gUnknown_08593950[b >> 1]);
}

void sub_8105DE8(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, FALSE);
    if (GetBattlerSide(gBattleAnimAttacker) != B_SIDE_PLAYER)
        gBattleAnimArgs[2] = -gBattleAnimArgs[2];

    sprite->data[0] = gBattleAnimArgs[4];
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimAttacker, 0) + gBattleAnimArgs[2];
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + gBattleAnimArgs[3];
    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
    sub_8105D88(sprite, gBattleAnimArgs[5], gBattleAnimArgs[6]);
}

void sub_8105E60(struct Sprite *sprite)
{
    if (++sprite->data[0] == 1)
        InitSpritePosToAnimAttacker(sprite, FALSE);

    sprite->pos2.x = Sin(sprite->data[1], 8);
    sprite->pos2.y = sprite->data[2] >> 8;
    sprite->data[1] = (sprite->data[1] + 7) & 0xFF;
    sprite->data[2] -= 0x80;
    if (sprite->data[0] == 60)
        DestroyAnimSprite(sprite);
}

void sub_8105EB0(u8 taskId)
{
    u16 win0h = IsContest() ? 0x98 : 0xF0;
    u16 win0v = 0;

    gBattle_WIN0H = win0h;
    gBattle_WIN0V = 0xA0;
    SetGpuReg(REG_OFFSET_WIN0H, gBattle_WIN0H);
    SetGpuReg(REG_OFFSET_WIN0V, gBattle_WIN0V);
    SetGpuReg(REG_OFFSET_WININ, 0x3F1F);
    SetGpuReg(REG_OFFSET_WINOUT, 0x3F3F);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDY, 0x10);
    gTasks[taskId].data[0] = win0v;
    gTasks[taskId].data[1] = win0h;
    gTasks[taskId].func = sub_8105F30;
}

static void sub_8105F30(u8 taskId)
{
    gTasks[taskId].data[0] += 13;
    gTasks[taskId].data[1] -= 13;
    if (gTasks[taskId].data[0] >= gTasks[taskId].data[1])
    {
        gBattle_WIN0H = 0;
        gTasks[taskId].func = sub_8105F84;
    }
    else
    {
        gBattle_WIN0H = gTasks[taskId].data[1] | (gTasks[taskId].data[0] << 8);
    }
}

static void sub_8105F84(u8 taskId)
{
    if (++gTasks[taskId].data[10] == 5)
    {
        gTasks[taskId].data[11] = 0x88;
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_LIGHTEN);
        BlendPalettes(sub_80A75AC(1, 0, 0, 0, 0, 0, 0), 16, RGB(31, 31, 31));
    }
    else if (gTasks[taskId].data[10] > 4)
    {
        gBattle_WIN0H = 0;
        gBattle_WIN0V = 0;
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG_ALL | WININ_WIN1_OBJ | WININ_WIN1_CLR);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG_ALL | WINOUT_WINOBJ_OBJ | WINOUT_WINOBJ_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        DestroyAnimVisualTask(taskId);
    }
}

void sub_8106020(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_TARGET);
    if (++gTasks[taskId].data[0] == 1)
    {
        PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(1), gUnknown_08593988);
        gSprites[spriteId].pos2.x = 4;
    }
    else
    {
        gSprites[spriteId].pos2.x = -gSprites[spriteId].pos2.x;
        if (!RunAffineAnimFromTaskData(&gTasks[taskId]))
        {
            gSprites[spriteId].pos2.x = 0;
            gSprites[spriteId].pos2.y = 0;
            DestroyAnimVisualTask(taskId);
        }
    }
}

void sub_81060B0(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(ANIM_ATTACKER);
    if (++gTasks[taskId].data[0] == 1)
    {
        PrepareAffineAnimInTaskData(&gTasks[taskId], GetAnimBattlerSpriteId(0), gUnknown_08593988);
        gSprites[spriteId].pos2.x = 4;
    }
    else
    {
        gSprites[spriteId].pos2.x = -gSprites[spriteId].pos2.x;
        if (!RunAffineAnimFromTaskData(&gTasks[taskId]))
        {
            gSprites[spriteId].pos2.x = 0;
            gSprites[spriteId].pos2.y = 0;
            DestroyAnimVisualTask(taskId);
        }
    }
}

void sub_8106140(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->data[0] = 95;
    sprite->data[1] = sprite->pos1.x;
    sprite->data[2] = GetBattlerSpriteCoord(gBattleAnimTarget, 2);
    sprite->data[3] = sprite->pos1.y;
    sprite->data[4] = GetBattlerSpriteCoord(gBattleAnimTarget, 3);
    InitAnimLinearTranslation(sprite);
    sprite->callback = sub_810618C;
}

static void sub_810618C(struct Sprite *sprite)
{
    if (!AnimTranslateLinear(sprite))
    {
        sprite->pos2.y += Sin(sprite->data[5], 14);
        sprite->data[5] = (sprite->data[5] + 4) & 0xFF;
    }
    else
    {
        DestroyAnimSprite(sprite);
    }
}

void sub_81061C4(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[1] = gBattleAnimArgs[0];
        sprite->data[2] = gBattleAnimArgs[1];
        sprite->data[0]++;
    }
    else
    {
        sprite->data[4] += sprite->data[1];
        sprite->pos2.x = sprite->data[4] >> 8;
        sprite->pos2.y = Sin(sprite->data[3], sprite->data[2]);
        sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
        if (sprite->data[3] > 100)
            sprite->invisible = sprite->data[3] % 2;

        if (sprite->data[3] > 120)
            DestroyAnimSprite(sprite);
    }
}

void sub_810624C(struct Sprite *sprite)
{
    sprite->pos1.x = gBattleAnimArgs[0];
    sprite->pos1.y = 160;
    sprite->data[0] = gBattleAnimArgs[2];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->callback = WaitAnimForDuration;
    StoreSpriteCallbackInData6(sprite, sub_810627C);
}

static void sub_810627C(struct Sprite *sprite)
{
    s16 y;
    sprite->data[2] += sprite->data[1];
    sprite->pos2.y = -((u16)sprite->data[2] >> 8);
    sprite->pos2.x = Sin(sprite->data[3], 4);
    sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
    y = sprite->pos1.y + sprite->pos2.y;
    if (y <= 72)
    {
        sprite->invisible = sprite->data[3] % 2;
        if (y <= 64)
            DestroyAnimSprite(sprite);
    }
}

void AnimTask_HeartsBackground(u8 taskId)
{
    struct BattleAnimBgData animBg;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 3);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    if (!IsContest())
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);

    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    sub_80A6B30(&animBg);
    AnimLoadCompressedBgGfx(animBg.bgId, &gUnknown_08C232E0, animBg.tilesOffset);
    sub_80A6D60(&animBg, &gUnknown_08C23D78, 0);
    LoadCompressedPalette(&gUnknown_08C23D50, animBg.paletteId * 16, 32);
    gTasks[taskId].func = sub_81063A8;
}

static void sub_81063A8(u8 taskId)
{
    struct BattleAnimBgData animBg;

    switch (gTasks[taskId].data[12])
    {
    case 0:
        if (++gTasks[taskId].data[10] == 4)
        {
            gTasks[taskId].data[10] = 0;
            gTasks[taskId].data[11]++;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
            if (gTasks[taskId].data[11] == 16)
            {
                gTasks[taskId].data[12]++;
                gTasks[taskId].data[11] = 0;
            }
        }
        break;
    case 1:
        if (++gTasks[taskId].data[11] == 141)
        {
            gTasks[taskId].data[11] = 16;
            gTasks[taskId].data[12]++;
        }
        break;
    case 2:
        if (++gTasks[taskId].data[10] == 4)
        {
            gTasks[taskId].data[10] = 0;
            gTasks[taskId].data[11]--;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
            if (gTasks[taskId].data[11] == 0)
            {
                gTasks[taskId].data[12]++;
                gTasks[taskId].data[11] = 0;
            }
        }
        break;
    case 3:
        sub_80A6B30(&animBg);
        sub_80A6C68(animBg.bgId);
        gTasks[taskId].data[12]++;
        break;
    case 4:
        if (!IsContest())
            SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);

        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

void sub_81064F8(u8 taskId)
{
    struct BattleAnimBgData animBg;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
    SetAnimBgAttribute(1, BG_ANIM_SCREEN_SIZE, 0);
    if (!IsContest())
        SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 1);

    gBattle_BG1_X = 0;
    gBattle_BG1_Y = 0;
    SetGpuReg(REG_OFFSET_BG1HOFS, gBattle_BG1_X);
    SetGpuReg(REG_OFFSET_BG1VOFS, gBattle_BG1_Y);
    sub_80A6B30(&animBg);
    if (IsContest())
        sub_80A6D60(&animBg, &gBattleAnimBgTilemap_ScaryFaceContest, 0);
    else if (GetBattlerSide(gBattleAnimTarget) == B_SIDE_OPPONENT)
        sub_80A6D60(&animBg, &gBattleAnimBgTilemap_ScaryFacePlayer, 0);
    else
        sub_80A6D60(&animBg, &gBattleAnimBgTilemap_ScaryFaceOpponent, 0);

    AnimLoadCompressedBgGfx(animBg.bgId, gUnknown_08C249F8, animBg.tilesOffset);
    LoadCompressedPalette(gUnknown_08C249D0, animBg.paletteId * 16, 32);
    gTasks[taskId].func = sub_81065EC;
}

static void sub_81065EC(u8 taskId)
{
    struct BattleAnimBgData animBg;

    switch (gTasks[taskId].data[12])
    {
    case 0:
        if (++gTasks[taskId].data[10] == 2)
        {
            gTasks[taskId].data[10] = 0;
            gTasks[taskId].data[11]++;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
            if (gTasks[taskId].data[11] == 14)
            {
                gTasks[taskId].data[12]++;
                gTasks[taskId].data[11] = 0;
            }
        }
        break;
    case 1:
        if (++gTasks[taskId].data[11] == 21)
        {
            gTasks[taskId].data[11] = 14;
            gTasks[taskId].data[12]++;
        }
        break;
    case 2:
        if (++gTasks[taskId].data[10] == 2)
        {
            gTasks[taskId].data[10] = 0;
            gTasks[taskId].data[11]--;
            SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[11], 16 - gTasks[taskId].data[11]));
            if (gTasks[taskId].data[11] == 0)
            {
                gTasks[taskId].data[12]++;
                gTasks[taskId].data[11] = 0;
            }
        }
        break;
    case 3:
        sub_80A6B30(&animBg);
        sub_80A6C68(1);
        sub_80A6C68(2);
        gTasks[taskId].data[12]++;
        // fall through
    case 4:
        if (!IsContest())
            SetAnimBgAttribute(1, BG_ANIM_CHAR_BASE_BLOCK, 0);

        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetAnimBgAttribute(1, BG_ANIM_PRIORITY, 1);
        DestroyAnimVisualTask(taskId);
        break;
    }
}

// Orbits a sphere in an ellipse around the mon.
// Used by MOVE_HIDDEN_POWER
// arg 0: duration
// arg 1: initial wave offset
void AnimOrbitFast(struct Sprite *sprite)
{
    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
    sprite->affineAnimPaused = 1;
    sprite->data[0] = gBattleAnimArgs[0];
    sprite->data[1] = gBattleAnimArgs[1];
    sprite->data[7] = GetBattlerSpriteSubpriority(gBattleAnimAttacker);
    sprite->callback = AnimOrbitFastStep;
    sprite->callback(sprite);
}

static void AnimOrbitFastStep(struct Sprite *sprite)
{
    if (sprite->data[1] >= 64 && sprite->data[1] <= 191)
        sprite->subpriority = sprite->data[7] + 1;
    else
        sprite->subpriority = sprite->data[7] - 1;

    sprite->pos2.x = Sin(sprite->data[1], sprite->data[2] >> 8);
    sprite->pos2.y = Cos(sprite->data[1], sprite->data[3] >> 8);
    sprite->data[1] = (sprite->data[1] + 9) & 0xFF;
    switch (sprite->data[5])
    {
    case 1:
        sprite->data[2] -= 0x400;
        sprite->data[3] -= 0x100;
        if (++sprite->data[4] == sprite->data[0])
        {
            sprite->data[5] = 2;
            return;
        }
        break;
    case 0:
        sprite->data[2] += 0x400;
        sprite->data[3] += 0x100;
        if (++sprite->data[4] == sprite->data[0])
        {
            sprite->data[4] = 0;
            sprite->data[5] = 1;
        }
        break;
    }

    if ((u16)gBattleAnimArgs[7] == 0xFFFF)
        DestroyAnimSprite(sprite);
}

// Moves orbs away from the mon, based on where they are in their orbit.
// Used in MOVE_HIDDEN_POWER.
// arg 0: initial wave offset
void AnimOrbitScatter(struct Sprite *sprite)
{
    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
    sprite->data[0] = Sin(gBattleAnimArgs[0], 10);
    sprite->data[1] = Cos(gBattleAnimArgs[0], 7);
    sprite->callback = AnimOrbitScatterStep;
}

static void AnimOrbitScatterStep(struct Sprite *sprite)
{
    sprite->pos2.x += sprite->data[0];
    sprite->pos2.y += sprite->data[1];
    if (sprite->pos1.x + sprite->pos2.x + 16 > 272u || sprite->pos1.y + sprite->pos2.y > 160 || sprite->pos1.y + sprite->pos2.y < -16)
        DestroyAnimSprite(sprite);
}

static void sub_8106914(struct Sprite *sprite)
{
    sprite->pos2.x += sprite->data[0];
    sprite->pos2.y += sprite->data[1];
    if (sprite->data[3]++ >= sprite->data[2])
        DestroyAnimSprite(sprite);
}

void sub_8106944(struct Sprite *sprite)
{
    sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
    sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
    sprite->data[0] = Sin(gBattleAnimArgs[0], 10);
    sprite->data[1] = Cos(gBattleAnimArgs[0], 7);
    sprite->data[2] = gBattleAnimArgs[1];
    sprite->callback = sub_8106914;
}

static void sub_810699C(struct Sprite *sprite)
{
    if (sprite->animEnded)
        DestroyAnimSprite(sprite);
}

void sub_81069B8(struct Sprite *sprite)
{
    InitSpritePosToAnimAttacker(sprite, TRUE);
    sprite->callback = sub_810699C;
}

void sub_81069D0(struct Sprite *sprite)
{
    s16 var0;
    if (!sprite->data[0])
    {
        sprite->pos1.x += gBattleAnimArgs[0];
        sprite->pos1.y += gBattleAnimArgs[1];
    }

    sprite->data[0]++;
    var0 = (sprite->data[0] * 10) & 0xFF;
    sprite->pos2.x = Sin(var0, 80) >> 8;
    if (sprite->data[0] < 80)
        sprite->pos2.y = (sprite->data[0] / 2) + (Cos(var0, 80) >> 8);

    if (sprite->data[0] > 90)
    {
        sprite->data[2]++;
        sprite->pos2.x -= sprite->data[2] / 2;
    }

    if (sprite->data[0] > 100)
        DestroyAnimSprite(sprite);
}

static void sub_8106A64(struct Sprite *sprite)
{
    sprite->data[5]++;
    sprite->pos2.x = Sin(sprite->data[3], 5);
    sprite->pos2.y = sprite->data[5] / 2;
    sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
    if (sprite->data[5] > 20)
        sprite->invisible = sprite->data[5] % 2;

    if (sprite->data[5] > 30)
        DestroyAnimSprite(sprite);
}

void sub_8106AD0(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->data[1] = gBattleAnimArgs[0];
        sprite->data[2] = gBattleAnimArgs[1];
        sprite->data[0]++;
    }
    else
    {
        sprite->data[4] += sprite->data[1];
        sprite->pos2.x = sprite->data[4] >> 8;
        sprite->pos2.y = Sin(sprite->data[3], sprite->data[2]);
        sprite->data[3] = (sprite->data[3] + 3) & 0xFF;
        if (sprite->data[3] > 70)
        {
            sprite->callback = sub_8106A64;
            sprite->pos1.x += sprite->pos2.x;
            sprite->pos1.y += sprite->pos2.y;
            sprite->pos2.x = 0;
            sprite->pos2.y = 0;
            sprite->data[3] = Random2() % 180;
        }
    }
}

void sub_8106B54(struct Sprite *sprite)
{
    if (sprite->data[3] == 0)
    {
        sprite->pos1.x += gBattleAnimArgs[0];
        sprite->pos1.y += gBattleAnimArgs[1];
        StartSpriteAnim(sprite, 0);
        sprite->subpriority = GetBattlerSpriteSubpriority(gBattleAnimTarget) - 1;
        sprite->data[2] = 1;
    }
    sprite->data[0] += sprite->data[2];
    sprite->data[1] = (sprite->data[0] * 4) % 256;
    if (sprite->data[1] < 0)
        sprite->data[1] = 0;
    sprite->pos2.x = Cos(sprite->data[1], 30 - sprite->data[0] / 4);
    sprite->pos2.y = Sin(sprite->data[1], 10 - sprite->data[0] / 8);
    if (sprite->data[1] > 128 && sprite->data[2] > 0)
        sprite->data[2] = -1;
    if (sprite->data[1] == 0 && sprite->data[2] < 0)
        sprite->data[2] = 1;
    sprite->data[3]++;
    if (sprite->data[3] < 10 || sprite->data[3] > 80)
        sprite->invisible = sprite->data[0] % 2;
    else
        sprite->invisible = FALSE;
    if (sprite->data[3] > 90)
        DestroyAnimSprite(sprite);
}

void sub_8106C80(struct Sprite *sprite)
{
    if (sprite->data[0] == 0)
    {
        sprite->pos1.x += gBattleAnimArgs[0];
        sprite->pos1.y += gBattleAnimArgs[1];
        StartSpriteAnim(sprite, gBattleAnimArgs[2]);
        sprite->data[0]++;
    }
    else if (sprite->animEnded)
    {
        DestroyAnimSprite(sprite);
    }
}

void sub_8106CD0(struct Sprite *sprite)
{
    if (!gBattleAnimArgs[2])
    {
        DestroyAnimSprite(sprite);
    }
    else
    {
        if (!gBattleAnimArgs[0])
        {
            sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 2);
            sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 3);
        }
        else
        {
            sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimTarget, 2);
            sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimTarget, 3);
        }

        if (!gBattleAnimArgs[1])
            sprite->pos1.x += 32;
        else
            sprite->pos1.x -= 32;

        sprite->data[0] = gBattleAnimArgs[2];
        sprite->data[1] = gBattleAnimArgs[1];
        StartSpriteAnim(sprite, sprite->data[1]);
        sprite->callback = sub_8106D5C;
    }
}

static void sub_8106D5C(struct Sprite *sprite)
{
    if (sprite->animEnded)
    {
        if (--sprite->data[0])
            StartSpriteAnim(sprite, sprite->data[1]);
        else
            DestroyAnimSprite(sprite);
    }
}

void sub_8106D90(u8 taskId)
{
    u8 spriteId = GetAnimBattlerSpriteId(gBattleAnimArgs[0]);

    PrepareAffineAnimInTaskData(&gTasks[taskId], spriteId, gUnknown_08593B98);
    gTasks[taskId].func = sub_8106DD4;
}

static void sub_8106DD4(u8 taskId)
{
    if (!RunAffineAnimFromTaskData(&gTasks[taskId]))
        DestroyAnimVisualTask(taskId);
}

void sub_8106E00(struct Sprite *sprite)
{
    int var1;
    u8 battler = !gBattleAnimArgs[0] ? gBattleAnimAttacker : gBattleAnimTarget;

    if (GetBattlerSide(battler) == B_SIDE_OPPONENT)
        gBattleAnimArgs[1] *= -1;

    sprite->pos1.x = GetBattlerSpriteCoord(battler, 2)  + gBattleAnimArgs[1];
    sprite->pos1.y = GetBattlerSpriteCoord(battler, 3) + gBattleAnimArgs[2];
    sprite->data[0] = 0;
    sprite->data[1] = (u16)sprite->pos1.x << 3;
    sprite->data[2] = (u16)sprite->pos1.y << 3;

    var1 = gBattleAnimArgs[1] << 3;
    if (var1 < 0)
        var1 += 7;
    sprite->data[3] = var1 >> 3;

    var1 = gBattleAnimArgs[2] << 3;
    if (var1 < 0)
        var1 += 7;
    sprite->data[4] = var1 >> 3;

    sprite->oam.tileNum += gBattleAnimArgs[3] * 16;
    sprite->callback = sub_8106EC8;
}

static void sub_8106EC8(struct Sprite *sprite)
{
    sprite->data[1] += sprite->data[3];
    sprite->data[2] += sprite->data[4];
    sprite->pos1.x = sprite->data[1] >> 3;
    sprite->pos1.y = sprite->data[2] >> 3;
    if (++sprite->data[0] > 16)
        DestroyAnimSprite(sprite);
}

void sub_8106F00(struct Sprite *sprite)
{
    if (!sprite->data[0])
    {
        sprite->data[1] = 120 - gBattleAnimArgs[0];
        sprite->invisible = 1;
    }

    if (++sprite->data[0] == sprite->data[1])
        SetGreyscaleOrOriginalPalette(sprite->oam.paletteNum + 16, 0);

    if (sprite->data[0] == sprite->data[1] + 80)
        DestroyAnimSprite(sprite);
}

void sub_8106F60(struct Sprite *sprite)
{
    int index;
    int var2;

    if (!sprite->data[0])
    {
        sprite->pos1.x = 120;
        sprite->pos1.y = (gBattleAnimArgs[0] + (((u16)gBattleAnimArgs[0]) >> 31)) / 2 - 15;

        StartSpriteAnim(sprite, gBattleAnimArgs[1]);

        sprite->data[5] = 120;
        sprite->data[3] = gBattleAnimArgs[2];
    }

    sprite->data[0]++;

    sprite->data[1] = (sprite->data[0] + ((u16)sprite->data[0] >> 31)) / 2;
    index = ((sprite->data[0] * 3) + (u16)sprite->data[3]);
    var2 = 0xFF;
    sprite->data[6] = (sprite->data[6] + 10) & 0xFF;

    index &= var2;
    sprite->pos2.x = Cos(index, 100);

    sprite->pos2.y = sprite->data[1] + Sin(index, 10) + Cos(sprite->data[6], 4);

    if (sprite->data[0] > sprite->data[5])
    {
        sprite->callback = sub_8107018;

        sprite->data[0] = 0;
        SetSpritePrimaryCoordsFromSecondaryCoords(sprite);
        sprite->data[2] = 5;
        sprite->data[4] = 0;
        sprite->data[3] = 0;

        StartSpriteAffineAnim(sprite, 1);
    }
}

static void sub_8107018(struct Sprite *sprite)
{
    if (++sprite->data[0] > 10)
    {
        sprite->data[0] = 0;
        sprite->callback = sub_810703C;
    }
}

static void sub_810703C(struct Sprite *sprite)
{
    sprite->data[3] += sprite->data[2];
    sprite->pos2.y = sprite->data[3];

    sprite->data[2]++;

    if (sprite->data[3] > 48 && sprite->data[2] > 0)
    {
        sprite->data[2] = sprite->data[4] - 5;
        sprite->data[4]++;
    }

    if (sprite->data[4] > 3)
    {
        int var1 = sprite->data[2];
        sprite->invisible = var1 - (((s32)(var1 + ((u32)var1 >> 31)) >> 1) << 1);
        DestroyAnimSprite(sprite);
    }

    if (sprite->data[4] == 4)
    {
        DestroyAnimSprite(sprite);
    }
}

void sub_81070AC(struct Sprite *sprite)
{
    if ((gBattleTypeFlags & BATTLE_TYPE_DOUBLE) && IsBattlerSpriteVisible(BATTLE_PARTNER(gBattleAnimAttacker)))
    {
        SetAverageBattlerPositions(gBattleAnimAttacker, 0, &sprite->pos1.x, &sprite->pos1.y);
        sprite->pos1.y += 40;

        StartSpriteAffineAnim(sprite, 1);
    }
    else
    {
        sprite->pos1.x = GetBattlerSpriteCoord(gBattleAnimAttacker, 0);
        sprite->pos1.y = GetBattlerSpriteCoord(gBattleAnimAttacker, 1) + 40;
    }

    sprite->data[0] = 13;
    sprite->data[2] = sprite->pos1.x;
    sprite->data[4] = sprite->pos1.y - 72;

    sprite->callback = StartAnimLinearTranslation;
    StoreSpriteCallbackInData6(sprite, DestroyAnimSprite);
}

void AnimTask_IsFuryCutterHitRight(u8 taskId)
{
    gBattleAnimArgs[7] = gAnimDisableStructPtr->furyCutterCounter & 1;
    DestroyAnimVisualTask(taskId);
}

void AnimTask_GetFuryCutterHitCount(u8 taskId)
{
    gBattleAnimArgs[7] = gAnimDisableStructPtr->furyCutterCounter;
    DestroyAnimVisualTask(taskId);
}
