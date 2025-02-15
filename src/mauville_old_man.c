#include "global.h"
#include "main.h"
#include "constants/songs.h"
#include "constants/easy_chat.h"
#include "constants/event_objects.h"
#include "constants/vars.h"
#include "mauville_old_man.h"
#include "event_data.h"
#include "string_util.h"
#include "text.h"
#include "easy_chat.h"
#include "script.h"
#include "random.h"
#include "event_scripts.h"
#include "task.h"
#include "menu.h"
#include "m4a.h"
#include "bard_music.h"
#include "sound.h"
#include "strings.h"
#include "overworld.h"
#include "field_message_box.h"
#include "script_menu.h"
#include "trader.h"

#define CHAR_SONG_WORD_SEPARATOR 0x37

extern struct MusicPlayerInfo gMPlayInfo_SE2;

static void InitGiddyTaleList(void);
static void StartBardSong(bool8 useTemporaryLyrics);
static void Task_BardSong(u8 taskId);
static void StorytellerSetup(void);
static void Storyteller_ResetFlag(void);

static u8 sSelectedStory;

struct BardSong gBardSong;

static EWRAM_DATA u16 sUnknownBardRelated = 0;
static EWRAM_DATA struct MauvilleManStoryteller * sStorytellerPtr = NULL;
static EWRAM_DATA u8 sStorytellerWindowId = 0;

static const u16 sDefaultBardSongLyrics[6] = {
    EC_WORD_SHAKE,
    EC_WORD_IT,
    EC_WORD_DO,
    EC_WORD_THE,
    EC_WORD_DIET,
    EC_WORD_DANCE
};

static const u8 * const sGiddyAdjectives[] = {
    gText_SoPretty,
    gText_SoDarling,
    gText_SoRelaxed,
    gText_SoSunny,
    gText_SoDesirable,
    gText_SoExciting,
    gText_SoAmusing,
    gText_SoMagical
};

static const u8 * const sGiddyQuestions[] = {
    gMauvilleManText_ISoWantToGoOnAVacation,
    gMauvilleManText_IBoughtCrayonsWith120Colors,
    gMauvilleManText_WouldntItBeNiceIfWeCouldFloat,
    gMauvilleManText_WhenYouWriteOnASandyBeach,
    gMauvilleManText_WhatsTheBottomOfTheSeaLike,
    gMauvilleManText_WhenYouSeeTheSettingSunDoesIt,
    gMauvilleManText_LyingBackInTheGreenGrass,
    gMauvilleManText_SecretBasesAreSoWonderful
};

static void SetupBard(void)
{
    u16 i;
    struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;

    bard->id = MAUVILLE_MAN_BARD;
    bard->hasChangedSong = FALSE;
    bard->language = gGameLanguage;
    for (i = 0; i < 6; i++)
        bard->songLyrics[i] = sDefaultBardSongLyrics[i];
}

static void SetupHipster(void)
{
    struct MauvilleManHipster *hipster = &gSaveBlock1Ptr->oldMan.hipster;

    hipster->id = MAUVILLE_MAN_HIPSTER;
    hipster->alreadySpoken = FALSE;
    hipster->language = gGameLanguage;
}

static void SetupStoryteller(void)
{
    StorytellerSetup();
}

static void SetupGiddy(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;

    giddy->id = MAUVILLE_MAN_GIDDY;
    giddy->taleCounter = 0;
    giddy->language = gGameLanguage;
}

static void SetupTrader(void)
{
    TraderSetup();
}

void SetMauvilleOldMan(void)
{
    u16 trainerId = (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];


    // Determine man based on the last digit of the player's trainer ID.
    switch ((trainerId % 10) / 2)
    {
        case MAUVILLE_MAN_BARD:
            SetupBard();
            break;
        case MAUVILLE_MAN_HIPSTER:
            SetupHipster();
            break;
        case MAUVILLE_MAN_TRADER:
            SetupTrader();
            break;
        case MAUVILLE_MAN_STORYTELLER:
            SetupStoryteller();
            break;
        case MAUVILLE_MAN_GIDDY:
            SetupGiddy();
            break;
    }
    ScrSpecial_SetMauvilleOldManEventObjGfx();
}

u8 GetCurrentMauvilleOldMan(void)
{
    struct MauvilleManCommon *common = &gSaveBlock1Ptr->oldMan.common;

    return common->id;
}

void ScrSpecial_GetCurrentMauvilleMan(void)
{
    gSpecialVar_Result = GetCurrentMauvilleOldMan();
}

void ScrSpecial_HasBardSongBeenChanged(void)
{
    u16 *scriptResult = &gSpecialVar_Result; // why??
    struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;

    *scriptResult = bard->hasChangedSong;
}

void ScrSpecial_SaveBardSongLyrics(void)
{
    u16 i;
    struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;

    StringCopy(bard->playerName, gSaveBlock2Ptr->playerName);

    for (i = 0; i < 4; i++)
        bard->playerTrainerId[i] = gSaveBlock2Ptr->playerTrainerId[i];

    for (i = 0; i < 6; i++)
        bard->songLyrics[i] = bard->temporaryLyrics[i];

    bard->hasChangedSong = TRUE;
}

// Copies lyrics into gStringVar4
static void PrepareSongText(void)
{
    struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;
    u16 * lyrics = gSpecialVar_0x8004 == 0 ? bard->songLyrics : bard->temporaryLyrics;
    u8 * wordEnd = gStringVar4;
    u8 * str = wordEnd;
    u16 lineNum;

    // Put three words on each line
    for (lineNum = 0; lineNum < 2; lineNum++)
    {
        wordEnd = CopyEasyChatWord(wordEnd, *(lyrics++));
        while (wordEnd != str)
        {
            if (*str == CHAR_SPACE)
                *str = CHAR_SONG_WORD_SEPARATOR;
            str++;
        }

        str++;
        *(wordEnd++) = CHAR_SPACE;

        wordEnd = CopyEasyChatWord(wordEnd, *(lyrics++));
        while (wordEnd != str)
        {
            if (*str == CHAR_SPACE)
                *str = CHAR_SONG_WORD_SEPARATOR;
            str++;
        }

        str++;
        *(wordEnd++) = CHAR_NEWLINE;

        wordEnd = CopyEasyChatWord(wordEnd, *(lyrics++));
        while (wordEnd != str)
        {
            if (*str == CHAR_SPACE)
                *str = CHAR_SONG_WORD_SEPARATOR;
            str++;
        }

        if (lineNum == 0)
        {
            *(wordEnd++) = EXT_CTRL_CODE_BEGIN;
            *(wordEnd++) = 15;
        }
    }
}

void ScrSpecial_PlayBardSong(void)
{
    StartBardSong(gSpecialVar_0x8004);
    ScriptContext1_Stop();
}

void ScrSpecial_GetHipsterSpokenFlag(void)
{
    u16 *scriptResult = &gSpecialVar_Result; // again??
    struct MauvilleManHipster *hipster = &gSaveBlock1Ptr->oldMan.hipster;

    *scriptResult = hipster->alreadySpoken;
}

void ScrSpecial_SetHipsterSpokenFlag(void)
{
    struct MauvilleManHipster *hipster = &gSaveBlock1Ptr->oldMan.hipster;

    hipster->alreadySpoken = TRUE;
}

void ScrSpecial_HipsterTeachWord(void)
{
    u16 var = sub_811F01C();

    if (var == 0xFFFF)
    {
        gSpecialVar_Result = FALSE;
    }
    else
    {
        CopyEasyChatWord(gStringVar1, var);
        gSpecialVar_Result = TRUE;
    }
}

void ScrSpecial_GiddyShouldTellAnotherTale(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;

    if (giddy->taleCounter == 10)
    {
        gSpecialVar_Result = FALSE;
        giddy->taleCounter = 0;
    }
    else
    {
        gSpecialVar_Result = TRUE;
    }
}

void ScrSpecial_GenerateGiddyLine(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;

    if (giddy->taleCounter == 0)
        InitGiddyTaleList();

    if (giddy->randomWords[giddy->taleCounter] != 0xFFFF) // is not the last element of the array?
    {
        u8 *stringPtr;
        u32 adjective = Random();

        adjective %= 8;
        stringPtr = CopyEasyChatWord(gStringVar4, giddy->randomWords[giddy->taleCounter]);
        stringPtr = StringCopy(stringPtr, gOtherText_Is);
        stringPtr = StringCopy(stringPtr, sGiddyAdjectives[adjective]);
        StringCopy(stringPtr, gOtherText_DontYouAgree);
    }
    else
    {
        StringCopy(gStringVar4, sGiddyQuestions[giddy->questionList[giddy->questionNum++]]);
    }

    if (!(Random() % 10))
        giddy->taleCounter = 10;
    else
        giddy->taleCounter++;

    gSpecialVar_Result = TRUE;
}

static void InitGiddyTaleList(void)
{
    struct MauvilleManGiddy *giddy = &gSaveBlock1Ptr->oldMan.giddy;
    u16 arr[][2] = {
        {EC_GROUP_POKEMON,   0},
        {EC_GROUP_LIFESTYLE, 0},
        {EC_GROUP_HOBBIES,   0},
        {EC_GROUP_MOVE_1,    0},
        {EC_GROUP_MOVE_2,    0},
        {EC_GROUP_POKEMON_2, 0}
    };
    u16 i;
    u16 r10;
    u16 r7;
    u16 r1;

    for (i = 0; i < 8; i++)
        giddy->questionList[i] = i;

    for (i = 0; i < 8; i++)
    {
        r1 = Random() % (i + 1);
        r7 = giddy->questionList[i];
        giddy->questionList[i] = giddy->questionList[r1];
        giddy->questionList[r1] = r7;
    }

    r10 = 0;
    for (i = 0; i < 6; i++)
    {
        arr[i][1] = EasyChat_GetNumWordsInGroup(arr[i][0]);
        r10 += arr[i][1];
    }

    giddy->questionNum = 0;
    r7 = 0;
    for (i = 0; i < 10; i++)
    {
        r1 = Random() % 10;
        if (r1 < 3 && r7 < 8)
        {
            giddy->randomWords[i] = 0xFFFF;
            r7++;
        }
        else
        {
            s16 r2 = Random() % r10;
            for (r1 = 0; i < 6; r1++)
                if ((r2 -= arr[r1][1]) <= 0)
                    break;
            if (r1 == 6)
                r1 = 0;
            giddy->randomWords[i] = sub_811EE90(arr[r1][0]);
        }
    }
}
static void ResetBardFlag(void)
{
    struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;

    bard->hasChangedSong = FALSE;
}

static void ResetHipsterFlag(void)
{
    struct MauvilleManHipster *hipster = &gSaveBlock1Ptr->oldMan.hipster;

    hipster->alreadySpoken = FALSE;
}

static void ResetTraderFlag(void)
{
    Trader_ResetFlag();
}

static void ResetStorytellerFlag(void)
{
    Storyteller_ResetFlag();
}

void ResetMauvilleOldManFlag(void)
{
    switch (GetCurrentMauvilleOldMan())
    {
        case MAUVILLE_MAN_BARD:
            ResetBardFlag();
            break;
        case MAUVILLE_MAN_HIPSTER:
            ResetHipsterFlag();
            break;
        case MAUVILLE_MAN_STORYTELLER:
            ResetStorytellerFlag();
            break;
        case MAUVILLE_MAN_TRADER:
            ResetTraderFlag();
            break;
        case MAUVILLE_MAN_GIDDY:
            break;
    }
    ScrSpecial_SetMauvilleOldManEventObjGfx();
}


#define tState data[0]
#define tCharIndex data[3]
#define tCurrWord data[4]
#define tUseTemporaryLyrics data[5]

#define MACRO1(a) (((a) & 3) + (((a) / 8) & 1))
#define MACRO2(a) (((a) % 4) + (((a) / 8) & 1))

static void StartBardSong(bool8 useTemporaryLyrics)
{
    u8 taskId = CreateTask(Task_BardSong, 80);

    gTasks[taskId].tUseTemporaryLyrics = useTemporaryLyrics;
}

static void sub_81206F0(void)
{
    gUnknown_03002F84 = FALSE;
}

static void BardSong_TextSubPrinter(struct TextPrinterTemplate * printer, u16 a1)
{
    gUnknown_03002F84 = TRUE;
}

static void sub_8120708(const u8 * src)
{
    DrawDialogueFrame(0, 0);
    AddTextPrinterParameterized(0, 1, src, 0, 1, 1, BardSong_TextSubPrinter);
    gUnknown_03002F84 = TRUE;
    CopyWindowToVram(0, 3);
}

static void BardSing(struct Task *task, struct BardSong *song)
{
    switch (task->tState)
    {
        case 0:  // Initialize song
        {
            struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;
            u16 *lyrics;
            s32 i;

            // Copy lyrics
            if (gSpecialVar_0x8004 == 0)
                lyrics = bard->songLyrics;
            else
                lyrics = bard->temporaryLyrics;
            for (i = 0; i < 6; i++)
                song->lyrics[i] = lyrics[i];
            song->currWord = 0;
        }
            break;
        case 1:  // Wait for BGM to end
            break;
        case 2:  // Initialize word
        {
            u16 word = song->lyrics[song->currWord];
            song->sound = GetWordSounds(word);
            GetWordPhonemes(song, MACRO1(word));
            song->currWord++;
            if (song->sound->var00 != 0xFF)
                song->state = 0;
            else
            {
                song->state = 3;
                song->phonemeTimer = 2;
            }
            break;
        }
        case 3:
        case 4:
        {
            const struct BardSound *sound = &song->sound[song->currPhoneme];

            switch (song->state)
            {
                case 0:
                    song->phonemeTimer = song->phonemes[song->currPhoneme].length;
                    if (sound->var00 <= 50)
                    {
                        u8 num = sound->var00 / 3;
                        m4aSongNumStart(PH_TRAP_HELD + 3 * num);
                    }
                    song->state = 2;
                    song->phonemeTimer--;
                    break;
                case 2:
                    song->state = 1;
                    if (sound->var00 <= 50)
                    {
                        song->volume = 0x100 + sound->volume * 16;
                        m4aMPlayVolumeControl(&gMPlayInfo_SE2, 0xFFFF, song->volume);
                        song->pitch = 0x200 + song->phonemes[song->currPhoneme].pitch;
                        m4aMPlayPitchControl(&gMPlayInfo_SE2, 0xFFFF, song->pitch);
                    }
                    break;
                case 1:
                    if (song->voiceInflection > 10)
                        song->volume -= 2;
                    if (song->voiceInflection & 1)
                        song->pitch += 64;
                    else
                        song->pitch -= 64;
                    m4aMPlayVolumeControl(&gMPlayInfo_SE2, 0xFFFF, song->volume);
                    m4aMPlayPitchControl(&gMPlayInfo_SE2, 0xFFFF, song->pitch);
                    song->voiceInflection++;
                    song->phonemeTimer--;
                    if (song->phonemeTimer == 0)
                    {
                        song->currPhoneme++;
                        if (song->currPhoneme != 6 && song->sound[song->currPhoneme].var00 != 0xFF)
                            song->state = 0;
                        else
                        {
                            song->state = 3;
                            song->phonemeTimer = 2;
                        }
                    }
                    break;
                case 3:
                    song->phonemeTimer--;
                    if (song->phonemeTimer == 0)
                    {
                        m4aMPlayStop(&gMPlayInfo_SE2);
                        song->state = 4;
                    }
                    break;
            }
        }
            break;
        case 5:
            break;
    }
}

static void Task_BardSong(u8 taskId)
{
    struct Task *task = &gTasks[taskId];  // r5

    BardSing(task, &gBardSong);
    switch (task->tState)
    {
        case 0:  // Initialize song
            PrepareSongText();
            sub_8120708(gStringVar4);
            task->data[1] = 0;
            task->data[2] = 0;
            task->tCharIndex = 0;
            task->tCurrWord = 0;
            FadeOutBGMTemporarily(4);
            task->tState = 1;
            break;
        case 1:  // Wait for BGM to end
            if (IsBGMPausedOrStopped())
                task->tState = 2;
            break;
        case 2:  // Initialize word
        {
            struct MauvilleManBard *bard = &gSaveBlock1Ptr->oldMan.bard;
            u8 *str = gStringVar4 + task->tCharIndex;
            u16 wordLen = 0;

            while (*str != CHAR_SPACE
                   && *str != CHAR_NEWLINE
                   && *str != EXT_CTRL_CODE_BEGIN
                   && *str != EOS)
            {
                str++;
                wordLen++;
            }
            if (!task->tUseTemporaryLyrics)
                sUnknownBardRelated = MACRO2(bard->songLyrics[task->tCurrWord]);
            else
                sUnknownBardRelated = MACRO2(bard->temporaryLyrics[task->tCurrWord]);

            gBardSong.length /= wordLen;
            if (gBardSong.length <= 0)
                gBardSong.length = 1;
            task->tCurrWord++;

            if (task->data[2] == 0)
            {
                task->tState = 3;
                task->data[1] = 0;
            }
            else
            {
                task->tState = 5;
                task->data[1] = 0;
            }
        }
            break;
        case 5:
            if (task->data[2] == 0)
                task->tState = 3;
            else
                task->data[2]--;
            break;
        case 3:
            if (gStringVar4[task->tCharIndex] == EOS)
            {
                FadeInBGM(6);
                m4aMPlayFadeOutTemporarily(&gMPlayInfo_SE2, 2);
                EnableBothScriptContexts();
                DestroyTask(taskId);
            }
            else if (gStringVar4[task->tCharIndex] == CHAR_SPACE)
            {

                sub_81206F0();
                task->tCharIndex++;
                task->tState = 2;
                task->data[2] = 0;
            }
            else if (gStringVar4[task->tCharIndex] == CHAR_NEWLINE)
            {
                task->tCharIndex++;
                task->tState = 2;
                task->data[2] = 0;
            }
            else if (gStringVar4[task->tCharIndex] == EXT_CTRL_CODE_BEGIN)
            {
                task->tCharIndex += 2;  // skip over control codes
                task->tState = 2;
                task->data[2] = 8;
            }
            else if (gStringVar4[task->tCharIndex] == CHAR_SONG_WORD_SEPARATOR)
            {
                gStringVar4[task->tCharIndex] = CHAR_SPACE;  // restore it back to a space
                sub_81206F0();
                task->tCharIndex++;
                task->data[2] = 0;
            }
            else
            {
                switch (task->data[1])
                {
                    case 0:
                        sub_81206F0();
                        task->data[1]++;
                        break;
                    case 1:
                        task->data[1]++;
                        break;
                    case 2:
                        task->tCharIndex++;
                        task->data[1] = 0;
                        task->data[2] = gBardSong.length;
                        task->tState = 4;
                        break;
                }
            }
            break;
        case 4:
            task->data[2]--;
            if (task->data[2] == 0)
                task->tState = 3;
            break;
    }
    RunTextPrintersAndIsPrinter0Active();
}

void ScrSpecial_SetMauvilleOldManEventObjGfx(void)
{
    VarSet(VAR_OBJ_GFX_ID_0, EVENT_OBJ_GFX_BARD);
}

// Language fixers?

void sub_8120B70(union OldMan * oldMan)
{
    s32 i;
    u8 sp00[8];

    switch (oldMan->common.id)
    {
        case MAUVILLE_MAN_TRADER:
        {
            struct MauvilleOldManTrader * trader = &oldMan->trader;
            for (i = 0; i < 4; i++)
            {
                if (trader->language[i] == LANGUAGE_JAPANESE)
                {
                    ConvertInternationalString(trader->playerNames[i], LANGUAGE_JAPANESE);
                }
            }
        }
            break;
        case MAUVILLE_MAN_STORYTELLER:
        {
            struct MauvilleManStoryteller * storyteller = &oldMan->storyteller;
            for (i = 0; i < 4; i++)
            {
                if (storyteller->gameStatIDs[i] != 0)
                {
                    memcpy(sp00, storyteller->trainerNames[i], 7);
                    sp00[7] = EOS;
                    if (IsStringJapanese(sp00))
                    {
                        memset(sp00, CHAR_SPACE, 8);
                        StringCopy(sp00, gText_Friend);
                        memcpy(storyteller->trainerNames[i], sp00, 7);
                        storyteller->language[i] = GAME_LANGUAGE;
                    }
                }
            }
        }
            break;
    }
}

void sub_8120C0C(union OldMan * oldMan, u32 r8, u32 r7, u32 r3)
{
    s32 i;

    switch (oldMan->common.id)
    {
        case MAUVILLE_MAN_TRADER:
        {
            struct MauvilleOldManTrader * trader = &oldMan->trader;

            for (i = 0; i < 4; i++)
            {
                if (IsStringJapanese(trader->playerNames[i]))
                {
                    trader->language[i] = r8;
                }
                else
                {
                    trader->language[i] = r7;
                }
            }
        }
            break;
        case MAUVILLE_MAN_STORYTELLER:
        {
            struct MauvilleManStoryteller * storyteller = &oldMan->storyteller;

            for (i = 0; i < 4; i++)
            {
                if (IsStringJapanese(storyteller->trainerNames[i]))
                {
                    storyteller->language[i] = r8;
                }
                else
                {
                    storyteller->language[i] = r7;
                }
            }
        }
            break;
        case MAUVILLE_MAN_BARD:
        {
            struct MauvilleManBard * bard = &oldMan->bard;

            if (r3 == LANGUAGE_JAPANESE)
                bard->language = r8;
            else
                bard->language = r7;
        }
            break;
        case MAUVILLE_MAN_HIPSTER:
        {
            struct MauvilleManHipster * hipster = &oldMan->hipster;

            if (r3 == LANGUAGE_JAPANESE)
                hipster->language = r8;
            else
                hipster->language = r7;
        }
            break;
        case MAUVILLE_MAN_GIDDY:
        {
            struct MauvilleManGiddy * giddy = &oldMan->giddy;

            if (r3 == LANGUAGE_JAPANESE)
                giddy->language = r8;
            else
                giddy->language = r7;
        }
            break;
    }
}

void SanitizeReceivedEmeraldOldMan(union OldMan * oldMan, u32 version, u32 language)
{
    u8 sp00[8];
    s32 i;
    if (oldMan->common.id == MAUVILLE_MAN_STORYTELLER && language == LANGUAGE_JAPANESE)
    {
        struct MauvilleManStoryteller * storyteller = &oldMan->storyteller;

        for (i = 0; i < 4; i++)
        {
            if (storyteller->gameStatIDs[i] != 0)
            {
                memcpy(sp00, storyteller->trainerNames[i], 7);
                sp00[7] = EOS;
                if (IsStringJapanese(sp00))
                    storyteller->language[i] = LANGUAGE_JAPANESE;
                else
                    storyteller->language[i] = GAME_LANGUAGE;
            }
        }
    }
}

void SanitizeReceivedRubyOldMan(union OldMan * oldMan, u32 version, u32 language)
{
    bool32 isRuby = (version == VERSION_SAPPHIRE || version == VERSION_RUBY);

    switch (oldMan->common.id)
    {
        case MAUVILLE_MAN_TRADER:
        {
            struct MauvilleOldManTrader * trader = &oldMan->trader;
            s32 i;

            if (isRuby)
            {
                for (i = 0; i < 4; i++)
                {
                    u8 * str = trader->playerNames[i];
                    if (str[0] == EXT_CTRL_CODE_BEGIN && str[1] == EXT_CTRL_CODE_JPN)
                    {
                        StripExtCtrlCodes(str);
                        trader->language[i] = LANGUAGE_JAPANESE;
                    }
                    else
                        trader->language[i] = language;
                }
            }
            else
            {
                for (i = 0; i < 4; i++)
                {
                    if (trader->language[i] == LANGUAGE_JAPANESE)
                    {
                        StripExtCtrlCodes(trader->playerNames[i]);
                    }
                }
            }
        }
            break;
        case MAUVILLE_MAN_STORYTELLER:
        {

            struct MauvilleManStoryteller * storyteller = &oldMan->storyteller;
            s32 i;

            if (isRuby)
            {
                for (i = 0; i < 4; i++)
                {
                    if (storyteller->gameStatIDs[i] != 0)
                        storyteller->language[i] = language;
                }
            }
        }
            break;
        case MAUVILLE_MAN_BARD:
        {
            struct MauvilleManBard * bard = &oldMan->bard;

            if (isRuby)
            {
                bard->language = language;
            }
        }
            break;
        case MAUVILLE_MAN_HIPSTER:
        {
            struct MauvilleManHipster * hipster = &oldMan->hipster;

            if (isRuby)
            {
                hipster->language = language;
            }
        }
            break;
        case MAUVILLE_MAN_GIDDY:
        {
            struct MauvilleManGiddy * giddy = &oldMan->giddy;

            if (isRuby)
            {
                giddy->language = language;
            }
        }
            break;
    }
}

struct Story
{
    u8 stat;
    u8 minVal;
    const u8 *title;
    const u8 *action;
    const u8 *fullText;
};

static const struct Story sStorytellerStories[] = {
    {GAME_STAT_NUM_UNION_ROOM_BATTLES, 1, MauvilleCity_PokemonCenter_1F_Text_28E930, MauvilleCity_PokemonCenter_1F_Text_28E947, MauvilleCity_PokemonCenter_1F_Text_28E956},
    {GAME_STAT_STARTED_TRENDS, 1, MauvilleCity_PokemonCenter_1F_Text_28E9D7, MauvilleCity_PokemonCenter_1F_Text_28E9EF, MauvilleCity_PokemonCenter_1F_Text_28E9FE},
    {GAME_STAT_PLANTED_BERRIES, 1, MauvilleCity_PokemonCenter_1F_Text_28EA7D, MauvilleCity_PokemonCenter_1F_Text_28EA98, MauvilleCity_PokemonCenter_1F_Text_28EAA8},
    {GAME_STAT_TRADED_BIKES, 1, MauvilleCity_PokemonCenter_1F_Text_28EB19, MauvilleCity_PokemonCenter_1F_Text_28EB31, MauvilleCity_PokemonCenter_1F_Text_28EB3E},
    {GAME_STAT_GOT_INTERVIEWED, 1, MauvilleCity_PokemonCenter_1F_Text_28EBB5, MauvilleCity_PokemonCenter_1F_Text_28EBCD, MauvilleCity_PokemonCenter_1F_Text_28EBDD},
    {GAME_STAT_TRAINER_BATTLES, 1, MauvilleCity_PokemonCenter_1F_Text_28EC60, MauvilleCity_PokemonCenter_1F_Text_28EC79, MauvilleCity_PokemonCenter_1F_Text_28EC81},
    {GAME_STAT_POKEMON_CAPTURES, 1, MauvilleCity_PokemonCenter_1F_Text_28ED04, MauvilleCity_PokemonCenter_1F_Text_28ED21, MauvilleCity_PokemonCenter_1F_Text_28ED30},
    {GAME_STAT_FISHING_CAPTURES, 1, MauvilleCity_PokemonCenter_1F_Text_28EDA1, MauvilleCity_PokemonCenter_1F_Text_28EDB5, MauvilleCity_PokemonCenter_1F_Text_28EDCF},
    {GAME_STAT_HATCHED_EGGS, 1, MauvilleCity_PokemonCenter_1F_Text_28EE45, MauvilleCity_PokemonCenter_1F_Text_28EE5D, MauvilleCity_PokemonCenter_1F_Text_28EE6A},
    {GAME_STAT_EVOLVED_POKEMON, 1, MauvilleCity_PokemonCenter_1F_Text_28EEDD, MauvilleCity_PokemonCenter_1F_Text_28EEF1, MauvilleCity_PokemonCenter_1F_Text_28EF01},
    {GAME_STAT_USED_POKECENTER, 1, MauvilleCity_PokemonCenter_1F_Text_28EF73, MauvilleCity_PokemonCenter_1F_Text_28EF95, MauvilleCity_PokemonCenter_1F_Text_28EFAA},
    {GAME_STAT_RESTED_AT_HOME, 1, MauvilleCity_PokemonCenter_1F_Text_28F045, MauvilleCity_PokemonCenter_1F_Text_28F05A, MauvilleCity_PokemonCenter_1F_Text_28F071},
    {GAME_STAT_ENTERED_SAFARI_ZONE, 1, MauvilleCity_PokemonCenter_1F_Text_28F0F3, MauvilleCity_PokemonCenter_1F_Text_28F10D, MauvilleCity_PokemonCenter_1F_Text_28F125},
    {GAME_STAT_USED_CUT, 1, MauvilleCity_PokemonCenter_1F_Text_28F1BE, MauvilleCity_PokemonCenter_1F_Text_28F1D5, MauvilleCity_PokemonCenter_1F_Text_28F1DE},
    {GAME_STAT_USED_ROCK_SMASH, 1, MauvilleCity_PokemonCenter_1F_Text_28F24F, MauvilleCity_PokemonCenter_1F_Text_28F269, MauvilleCity_PokemonCenter_1F_Text_28F277},
    {GAME_STAT_MOVED_SECRET_BASE, 1, MauvilleCity_PokemonCenter_1F_Text_28F2FC, MauvilleCity_PokemonCenter_1F_Text_28F314, MauvilleCity_PokemonCenter_1F_Text_28F32A},
    {GAME_STAT_USED_SPLASH, 1, MauvilleCity_PokemonCenter_1F_Text_28F3AD, MauvilleCity_PokemonCenter_1F_Text_28F3C6, MauvilleCity_PokemonCenter_1F_Text_28F3D2},
    {GAME_STAT_USED_STRUGGLE, 1, MauvilleCity_PokemonCenter_1F_Text_28F44B, MauvilleCity_PokemonCenter_1F_Text_28F461, MauvilleCity_PokemonCenter_1F_Text_28F47C},
    {GAME_STAT_SLOT_JACKPOTS, 1, MauvilleCity_PokemonCenter_1F_Text_28F50C, MauvilleCity_PokemonCenter_1F_Text_28F51B, MauvilleCity_PokemonCenter_1F_Text_28F538},
    {GAME_STAT_CONSECUTIVE_ROULETTE_WINS, 2, MauvilleCity_PokemonCenter_1F_Text_28F5BE, MauvilleCity_PokemonCenter_1F_Text_28F5D1, MauvilleCity_PokemonCenter_1F_Text_28F5F2},
    {GAME_STAT_ENTERED_BATTLE_TOWER, 1, MauvilleCity_PokemonCenter_1F_Text_28F678, MauvilleCity_PokemonCenter_1F_Text_28F694, MauvilleCity_PokemonCenter_1F_Text_28F6B4},
    {GAME_STAT_POKEBLOCKS, 1, MauvilleCity_PokemonCenter_1F_Text_28F751, MauvilleCity_PokemonCenter_1F_Text_28F76A, MauvilleCity_PokemonCenter_1F_Text_28F776},
    {GAME_STAT_ENTERED_CONTEST, 1, MauvilleCity_PokemonCenter_1F_Text_28F7F6, MauvilleCity_PokemonCenter_1F_Text_28F811, MauvilleCity_PokemonCenter_1F_Text_28F822},
    {GAME_STAT_WON_CONTEST, 1, MauvilleCity_PokemonCenter_1F_Text_28F89C, MauvilleCity_PokemonCenter_1F_Text_28F8AF, MauvilleCity_PokemonCenter_1F_Text_28F8BC},
    {GAME_STAT_SHOPPED, 1, MauvilleCity_PokemonCenter_1F_Text_28F92F, MauvilleCity_PokemonCenter_1F_Text_28F941, MauvilleCity_PokemonCenter_1F_Text_28F949},
    {GAME_STAT_USED_ITEMFINDER, 1, MauvilleCity_PokemonCenter_1F_Text_28F9D1, MauvilleCity_PokemonCenter_1F_Text_28F9EA, MauvilleCity_PokemonCenter_1F_Text_28F9FD},
    {GAME_STAT_GOT_RAINED_ON, 1, MauvilleCity_PokemonCenter_1F_Text_28FA81, MauvilleCity_PokemonCenter_1F_Text_28FA99, MauvilleCity_PokemonCenter_1F_Text_28FAA7},
    {GAME_STAT_CHECKED_POKEDEX, 1, MauvilleCity_PokemonCenter_1F_Text_28FB1D, MauvilleCity_PokemonCenter_1F_Text_28FB35, MauvilleCity_PokemonCenter_1F_Text_28FB47},
    {GAME_STAT_RECEIVED_RIBBONS, 1, MauvilleCity_PokemonCenter_1F_Text_28FBC4, MauvilleCity_PokemonCenter_1F_Text_28FBD9, MauvilleCity_PokemonCenter_1F_Text_28FBEA},
    {GAME_STAT_JUMPED_DOWN_LEDGES, 1, MauvilleCity_PokemonCenter_1F_Text_28FC6B, MauvilleCity_PokemonCenter_1F_Text_28FC85, MauvilleCity_PokemonCenter_1F_Text_28FC98},
    {GAME_STAT_WATCHED_TV, 1, MauvilleCity_PokemonCenter_1F_Text_28FD1D, MauvilleCity_PokemonCenter_1F_Text_28FD35, MauvilleCity_PokemonCenter_1F_Text_28FD40},
    {GAME_STAT_CHECKED_CLOCK, 1, MauvilleCity_PokemonCenter_1F_Text_28FDA2, MauvilleCity_PokemonCenter_1F_Text_28FDBD, MauvilleCity_PokemonCenter_1F_Text_28FDCE},
    {GAME_STAT_WON_POKEMON_LOTTERY, 1, MauvilleCity_PokemonCenter_1F_Text_28FE57, MauvilleCity_PokemonCenter_1F_Text_28FE72, MauvilleCity_PokemonCenter_1F_Text_28FE88},
    {GAME_STAT_USED_DAYCARE, 1, MauvilleCity_PokemonCenter_1F_Text_28FF0C, MauvilleCity_PokemonCenter_1F_Text_28FF27, MauvilleCity_PokemonCenter_1F_Text_28FF44},
    {GAME_STAT_RODE_CABLE_CAR, 1, MauvilleCity_PokemonCenter_1F_Text_28FFDD, MauvilleCity_PokemonCenter_1F_Text_28FFFA, MauvilleCity_PokemonCenter_1F_Text_29000D},
    {GAME_STAT_ENTERED_HOT_SPRINGS, 1, MauvilleCity_PokemonCenter_1F_Text_290097, MauvilleCity_PokemonCenter_1F_Text_2900B5, MauvilleCity_PokemonCenter_1F_Text_2900CB}
};

static void StorytellerSetup(void)
{
    s32 i;
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;

    sStorytellerPtr->id = MAUVILLE_MAN_STORYTELLER;
    sStorytellerPtr->alreadyRecorded = FALSE;
    for (i = 0; i < 4; i++)
    {
        sStorytellerPtr->gameStatIDs[i] = 0;
        sStorytellerPtr->trainerNames[0][i] = EOS;  // Maybe they meant storyteller->trainerNames[i][0] instead?
    }
}

static void Storyteller_ResetFlag(void)
{
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;

    sStorytellerPtr->id = MAUVILLE_MAN_STORYTELLER;
    sStorytellerPtr->alreadyRecorded = FALSE;
}

static u32 StorytellerGetGameStat(u8 stat)
{
    if (stat == 50)
        stat = 0;
    return GetGameStat(stat);
}

static const struct Story *GetStoryByStat(u32 stat)
{
    s32 i;

    for (i = 0; i < 36; i++)
    {
        if (sStorytellerStories[i].stat == stat)
            return &sStorytellerStories[i];
    }
    return &sStorytellerStories[35];
}

static const u8 *GetStoryTitleByStat(u32 stat)
{
    return GetStoryByStat(stat)->title;
}

static const u8 *GetStoryTextByStat(u32 stat)
{
    return GetStoryByStat(stat)->fullText;
}

static const u8 *GetStoryActionByStat(u32 stat)
{
    return GetStoryByStat(stat)->action;
}

static u8 GetFreeStorySlot(void)
{
    u8 i;

    for (i = 0; i < 4; i++)
    {
        if (sStorytellerPtr->gameStatIDs[i] == 0)
            break;
    }
    return i;
}

static u32 StorytellerGetRecordedTrainerStat(u32 trainer)
{
    u8 *ptr = sStorytellerPtr->statValues[trainer];

    return ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24);
}

static void StorytellerSetRecordedTrainerStat(u32 trainer, u32 val)
{
    u8 *ptr = sStorytellerPtr->statValues[trainer];

    ptr[0] = val;
    ptr[1] = val >> 8;
    ptr[2] = val >> 16;
    ptr[3] = val >> 24;
}

static bool32 HasTrainerStatIncreased(u32 trainer)
{
    if (StorytellerGetGameStat(sStorytellerPtr->gameStatIDs[trainer]) > StorytellerGetRecordedTrainerStat(trainer))
        return TRUE;
    else
        return FALSE;
}

static void GetStoryByStattellerPlayerName(u32 player, void *dst)
{
    u8 *name = sStorytellerPtr->trainerNames[player];

    memset(dst, EOS, 8);
    memcpy(dst, name, 7);
}

static void StorytellerSetPlayerName(u32 player, const u8 * src)
{
    u8 * name = sStorytellerPtr->trainerNames[player];
    memset(name, EOS, 7);
    memcpy(name, src, 7);
}


static void StorytellerRecordNewStat(u32 player, u32 stat)
{
    sStorytellerPtr->gameStatIDs[player] = stat;
    StorytellerSetPlayerName(player, gSaveBlock2Ptr->playerName);
    StorytellerSetRecordedTrainerStat(player, StorytellerGetGameStat(stat));
    ConvertIntToDecimalStringN(gStringVar1, StorytellerGetGameStat(stat), STR_CONV_MODE_LEFT_ALIGN, 10);
    StringCopy(gStringVar2, GetStoryActionByStat(stat));
    sStorytellerPtr->language[player] = gGameLanguage;
}

static void ScrambleStatList(u8 * arr, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++)
        arr[i] = i;
    for (i = 0; i < count; i++)
    {
        u32 a = Random() % count;
        u32 b = Random() % count;
        u8 temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
}

struct UnknownStruct_0859F288
{
    s32 length;
    u32 unused2;
};

static const struct UnknownStruct_0859F288 sStorytellerStuff = {
    ARRAY_COUNT(sStorytellerStories),
    sizeof(sStorytellerStuff)
};

static bool8 StorytellerInitializeRandomStat(void)
{
    u8 arr[sStorytellerStuff.length];
    s32 i;
    s32 j;

    ScrambleStatList(arr, ARRAY_COUNT(sStorytellerStories));
    for (i = 0; i < (s32)ARRAY_COUNT(sStorytellerStories); i++)
    {
        u8 stat = sStorytellerStories[arr[i]].stat;
        u8 minVal = sStorytellerStories[arr[i]].minVal;

        for (j = 0; j < 4; j++)
        {
            if (sStorytellerPtr->gameStatIDs[j] == stat)
                break;
        }
        if (j == 4 && StorytellerGetGameStat(stat) >= minVal)
        {
            sStorytellerPtr->alreadyRecorded = TRUE;
            if (GetFreeStorySlot() == 4)
                StorytellerRecordNewStat(sSelectedStory, stat);
            else
                StorytellerRecordNewStat(GetFreeStorySlot(), stat);
            return TRUE;
        }
    }
    return FALSE;
}

static void StorytellerDisplayStory(u32 player)
{
    u8 stat = sStorytellerPtr->gameStatIDs[player];

    ConvertIntToDecimalStringN(gStringVar1, StorytellerGetRecordedTrainerStat(player), STR_CONV_MODE_LEFT_ALIGN, 10);
    StringCopy(gStringVar2, GetStoryActionByStat(stat));
    GetStoryByStattellerPlayerName(player, gStringVar3);
    ConvertInternationalString(gStringVar3, sStorytellerPtr->language[player]);
    ShowFieldMessage(GetStoryTextByStat(stat));
}

static void PrintStoryList(void)
{
    s32 i;
    s32 width = GetStringWidth(1, gText_Exit, 0);
    u8 tileWidth;
    for (i = 0; i < 4; i++)
    {
        s32 curWidth;
        u16 gameStatID = sStorytellerPtr->gameStatIDs[i];

        if (gameStatID == 0)
            break;
        curWidth = GetStringWidth(1, GetStoryTitleByStat(gameStatID), 0);
        if (curWidth > width)
            width = curWidth;
    }
    sStorytellerWindowId = CreateWindowFromRect(0, 0, ConvertPixelWidthToTileWidth(width), GetFreeStorySlot() * 2 + 2);
    SetStandardWindowBorderStyle(sStorytellerWindowId, 0);
    for (i = 0; i < 4; i++)
    {
        u16 gameStatID = sStorytellerPtr->gameStatIDs[i];
        if (gameStatID == 0)
            break;
        AddTextPrinterParameterized(sStorytellerWindowId, 1, GetStoryTitleByStat(gameStatID), 8, 16 * i + 1, 0xFF, NULL);
    }
    AddTextPrinterParameterized(sStorytellerWindowId, 1, gText_Exit, 8, 16 * i + 1, 0xFF, NULL);
    InitMenuInUpperLeftCornerPlaySoundWhenAPressed(sStorytellerWindowId, GetFreeStorySlot() + 1, 0);
    CopyWindowToVram(sStorytellerWindowId, 3);
}

static void Task_StoryListMenu(u8 taskId) // Task_StoryListMenu
{
    struct Task *task = &gTasks[taskId];
    s32 selection;

    switch (task->data[0])
    {
        case 0:
            PrintStoryList();
            task->data[0]++;
            break;
        case 1:
            selection = Menu_ProcessInput();
            if (selection == MENU_NOTHING_CHOSEN)
                break;
            if (selection == MENU_B_PRESSED || selection == GetFreeStorySlot())
            {
                gSpecialVar_Result = 0;
            }
            else
            {
                gSpecialVar_Result = 1;
                sSelectedStory = selection;
            }
            ClearToTransparentAndRemoveWindow(sStorytellerWindowId);
            DestroyTask(taskId);
            EnableBothScriptContexts();
            break;
    }
}

// Sets gSpecialVar_Result to TRUE if player selected a story
void ScrSpecial_StorytellerStoryListMenu(void)
{
    CreateTask(Task_StoryListMenu, 80);
}

void ScrSpecial_StorytellerDisplayStory(void)
{
    StorytellerDisplayStory(sSelectedStory);
}

u8 ScrSpecial_StorytellerGetFreeStorySlot(void)
{
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;
    return GetFreeStorySlot();
}

// Returns TRUE if stat has increased
bool8 ScrSpecial_StorytellerUpdateStat(void)
{
    u8 r4;
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;
    r4 = sStorytellerPtr->gameStatIDs[sSelectedStory];

    if (HasTrainerStatIncreased(sSelectedStory) == TRUE)
    {
        StorytellerRecordNewStat(sSelectedStory, r4);
        return TRUE;
    }
    return FALSE;
}

bool8 ScrSpecial_HasStorytellerAlreadyRecorded(void)
{
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;

    if (sStorytellerPtr->alreadyRecorded == FALSE)
        return FALSE;
    else
        return TRUE;
}

bool8 ScrSpecial_StorytellerInitializeRandomStat(void)
{
    sStorytellerPtr = &gSaveBlock1Ptr->oldMan.storyteller;
    return StorytellerInitializeRandomStat();
}

