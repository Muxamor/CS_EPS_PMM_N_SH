#ifndef _FLASH_H_
#define _FLASH_H_


#define SECT_IN_RAM_FLASH_PROC_SIZE       0x00000320     /*�������� �� ���������������� map-�����  */


#define FLASH_CMD_ERASE_SECTORS_2       0x02
#define FLASH_CMD_CHECK_CRC_IMAGE_N     0x04     /* ����� ����� � 0-� ���� */
#define FLASH_CMD_CHECK_VALID_IMAGE_N   0x06     /* ... */
#define FLASH_CMD_FIX_VALID_IMAGE_N     0x08
#define FLASH_CMD_SET_PREF_BLOCK_N      0x0A
#define FLASH_CMD_DO_COPY_AND_GO        0x0C
#define FLASH_CMD_RESTART               0x0E

#define FLASH_STAT_LOAD_OK              0x10
#define FLASH_STAT_MASK_NIX_ERR         0xC0
#define FLASH_STAT_MASK_ERR             0x80


#define PREF_REC_ADDR_START       0x80FFF80   /* ������ - 1024 */
#define PREF_REC_ADDR_END         0x8100000


#define M8(adr)  (*((volatile uint8_t  *) (adr)))
#define M16(adr) (*((volatile uint16_t *) (adr)))
#define M32(adr) (*((volatile uint32_t *) (adr)))
#define M64(adr) (*((volatile uint64_t *) (adr)))
// Flash Keys
#define FLASH_KEY1        0x45670123
#define FLASH_KEY2        0xCDEF89AB
#define FLASH_SNB_POS     ((uint32_t)0x00000003)
#define FLASH_SNB_MSK     ((uint32_t)0x00000FF8)
#define FLASH_PGERR       (FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR  | FLASH_SR_PGAERR | \
                           FLASH_SR_SIZERR | FLASH_SR_PGSERR  | FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR  | \
                           FLASH_SR_OPTVERR )



typedef union {
  uint8_t   bb[8];
  struct {
    uint32_t  size : 24;
    uint32_t  cmd :   8;
    uint32_t  crc;
    } Ctrl;
  struct {
    int8_t Status;
    uint8_t CurrentBlock;
    uint8_t PrefBlock;
    uint8_t ResetSrc;
    } Info;
}typeUniVarCANFlashFragment;


void CallbackCAN_Flash(CAN_TypeDef *can_ref, typeIdxMask id, uint16_t leng, int state);
void Flash_Init(void);
void Flash_UnInit(void);
int8_t Flash_EraseSector(uint16_t sect_num);
int8_t Flash_Write(uint32_t adr, uint8_t *buf, uint32_t size);
void Flash_CopyFlashProcAndRun(uint32_t size);
int8_t Flash_InRAM_EraseSector(uint16_t sect_num)
  __attribute__((section ("sect_FlashProcInRamPiece")));
void Flash_InRAM_CopyCodeAndRestart(uint32_t size)
  __attribute__((section ("sect_FlashProcInRam")));
int8_t CheckBlock(register uint32_t *addr);
uint8_t HavePrefFlashBlockNum(void);
uint8_t HaveRunFlashBlockNum(void);



#endif

