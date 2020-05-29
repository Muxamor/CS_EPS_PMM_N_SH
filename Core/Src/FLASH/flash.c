#include <stm32l496xx.h>
#include <string.h>
#include "canv.h"
#include "flash.h"


#define __dsb(v)  asm(" dsb")
#define __isb(v)  asm(" isb")

typeUniVarCANFlashFragment VarCAN_FlashFragment;
uint8_t InRAM_FlashProcMem[SECT_IN_RAM_FLASH_PROC_SIZE];
int8_t Flash_Status;



int8_t Flash_EraseSector(uint16_t sect_num) {
  int8_t err;
  uint32_t n, k, m, saved_acr;
  err = 0;
  n = sect_num;  // Get Sector Number
  /*�������� �� �������*/
  if(n < 512) {
    m = 0x8000000 | (n << 11);
    k = 0x200;
    }
  else err = -1;
  if(err == 0) {
    for( ; k>0; k--) {
      if((M32(m) + 1) != 0) break;
      m = m + 4;
      }
    if(k != 0) {
      /* Flash init */
      __disable_irq();
      FLASH->KEYR = FLASH_KEY1;                             // Unlock Flash
      FLASH->KEYR = FLASH_KEY2;
      saved_acr = FLASH->ACR;
      FLASH->ACR  = 4;                  // no Cache, no Prefetch
#ifndef _SYM_
      __dsb(0); __isb(0);
#endif
      /* Reset Error Flags */
      FLASH->SR  |= FLASH_PGERR;
      /* ������� */
      memcpy(InRAM_FlashProcMem, (void*)((uint32_t)&Flash_InRAM_EraseSector & (uint32_t)-2), SECT_IN_RAM_FLASH_PROC_SIZE);
      err = ((int8_t(*)(uint16_t))(InRAM_FlashProcMem+1))(n);    // RUN !
      FLASH->CR |= FLASH_CR_LOCK;                           // Lock Flash
      FLASH->ACR = saved_acr;
      __enable_irq();
      }
    }
  return err;
}


int8_t Flash_Write(uint32_t addr, uint8_t *buf, uint32_t size) {
  int8_t err;
  uint32_t saved_acr;

  /* Flash init */
  __disable_irq();
  // reset CR
  FLASH->KEYR = FLASH_KEY1;                             // Unlock Flash
  FLASH->KEYR = FLASH_KEY2;
  saved_acr = FLASH->ACR;
  FLASH->ACR  = 4;                  // no Cache, no Prefetch
#ifndef _SYM_
  __dsb(0); __isb(0);
#endif
  /**/

  err = 0;
  size = (size + 7) & ~7;                               // Adjust size for Words
  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
  FLASH->CR  =  0;
  while(size) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
    if(M64(addr) != *((uint64_t *)buf)) {
      __disable_irq();
      FLASH->CR = FLASH_CR_PG;              // Programming Enabled, Programming Enabled (Word)
      M32(addr) = *((uint32_t *)buf);                        // Program Double Word
      M32(addr+4) = *((uint32_t *)buf+1);
      while (FLASH->SR & FLASH_SR_BSY);
      FLASH->CR = 0;                        // Programming Disabled
      if(FLASH->SR & FLASH_PGERR) {                     // Check for Error
        FLASH->SR |= FLASH_PGERR;                       // Reset Error Flags
        err = -1;                                       // Failed
        break;
        }
      __enable_irq();
      }
    addr += 8;                                          // Go to next Word
    buf += 8;
    size  -= 8;
    }
  FLASH->CR |= FLASH_CR_LOCK;                           // Lock Flash
  FLASH->ACR = saved_acr;
  __enable_irq();
  return err;
}


void Flash_CopyFlashProcAndRun(uint32_t size) {
  uint32_t saved_acr;
  /* Flash init */
  __disable_irq();
  FLASH->KEYR = FLASH_KEY1;                             // Unlock Flash
  FLASH->KEYR = FLASH_KEY2;
  saved_acr = FLASH->ACR;
  FLASH->ACR  = 4;                  // no Cache, no Prefetch
#ifndef _SYM_
  __dsb(0); __isb(0);
#endif
  /* Reset Error Flags */
  FLASH->SR  |= FLASH_PGERR;
  /**/
  memcpy(InRAM_FlashProcMem, (void*)((uint32_t)&Flash_InRAM_CopyCodeAndRestart & (uint32_t)-2), SECT_IN_RAM_FLASH_PROC_SIZE);
  ((void(*)(uint32_t))(InRAM_FlashProcMem+1))(size);    //Copy and RUN !
  FLASH->CR |= FLASH_CR_LOCK;                           // Lock Flash
  FLASH->ACR = saved_acr;
  __enable_irq();
}


void CallbackCAN_Flash(CAN_TypeDef *can_ref, typeIdxMask id, uint16_t leng, int state) {
  int i;
  uint32_t addr;
  uint16_t n;
  uint8_t crc_ok, do_write_flg, pref2, cmd;
  int8_t err;
  uint32_t size, size4, crc, crc1;
  uint64_t  tmp64;
  if((state == 0)&&(id.uf.RTR == 1)) {
    if(id.uf.Offset == 0x80000) {  // ������ ����� ����������
      VarCAN_FlashFragment.Info.Status = Flash_Status;
      VarCAN_FlashFragment.Info.CurrentBlock = HaveRunFlashBlockNum();
      VarCAN_FlashFragment.Info.PrefBlock = HavePrefFlashBlockNum();
      VarCAN_FlashFragment.Info.ResetSrc = (uint8_t)(RCC->CSR >> 24);
      }
    else if(id.uf.Offset < 0x80000) {  //������ ��������� Flash
      if(HaveRunFlashBlockNum() == 0)
        memcpy(VarCAN_FlashFragment.bb, (uint8_t*)(0x8080000 + id.uf.Offset), leng);
      else
        memcpy(VarCAN_FlashFragment.bb, (uint8_t*)(0x8000000 + id.uf.Offset), leng);
      }
    }
  else if((state == 1)&&(id.uf.RTR == 0)) {
    if(id.uf.Offset == 0x80000) {            //������ � ���� ����������
      Flash_Status = 0;
      cmd = VarCAN_FlashFragment.Ctrl.cmd;
      switch(cmd & 0xFE) {
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_ERASE_SECTORS_2:
          Flash_Status = cmd;
          if((VarCAN_FlashFragment.Ctrl.size == 0)&&(VarCAN_FlashFragment.Ctrl.crc == 0)) {
            err = 0;
            if(HaveRunFlashBlockNum() == 0) {
              /*�������� �������� 256..511 */
              for(n=256; n <= 511; n++)
                err = (err) ? err : Flash_EraseSector(n);
              }
            else {
              /*�������� �������� 0..255 */
              for(n=0; n <= 255; n++)
                err = (err) ? err : Flash_EraseSector(n);
              }
            if(err) Flash_Status |= FLASH_STAT_MASK_ERR;
            }
          break;
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_CHECK_CRC_IMAGE_N:
        case FLASH_CMD_DO_COPY_AND_GO:
          Flash_Status = cmd;
          do_write_flg = (cmd == FLASH_CMD_DO_COPY_AND_GO) ? 1 : 0;
          if(do_write_flg && HaveRunFlashBlockNum()) {
            /* ���������� - ���� ����������� ��������� �� 2-� ����� */
            Flash_Status |= FLASH_STAT_MASK_NIX_ERR;
            break;            
            }
          /*�������� ����������� �����*/
          size = VarCAN_FlashFragment.Ctrl.size & 0x7FFFF;  //������� �� 512k
          crc = VarCAN_FlashFragment.Ctrl.crc;
          RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
          if(do_write_flg == 0)
            addr = (cmd & 1) ? 0x8080000 : 0x8000000;
          else
            addr = 0x8080000;
          size4 = size / 4;
          CRC->CR = 1;
          for(i=0; i<size4; i++) {
            CRC->DR = M32(addr) + 1;
            addr = addr + 4;
            }
          crc1 = CRC->DR;
          RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
          crc_ok = (crc == crc1) ? 1 : 0;
          if(crc_ok == 0) Flash_Status |= FLASH_STAT_MASK_ERR;
          if((do_write_flg == 1) && (crc_ok == 1)) {
            if((*((uint32_t*)0x8080004) & 0x80000 ) == 0) {  //reset vector ������ ���� ��� 1-�� �����!
              /*���������� ���� ! */
              Flash_CopyFlashProcAndRun(size);
              /*���� ��� ����� �� ��������� - ������*/
              Flash_Status |= FLASH_STAT_MASK_ERR;
              }
            else {
              Flash_Status |= FLASH_STAT_MASK_NIX_ERR;
              }
            }
          break;
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_CHECK_VALID_IMAGE_N:
          Flash_Status = cmd;
          addr = (cmd & 1) ? 0x8080000 : 0x8000000;
          if(CheckBlock((uint32_t*)addr) != 0) Flash_Status |= FLASH_STAT_MASK_ERR;
          break;
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_FIX_VALID_IMAGE_N:
          Flash_Status = cmd;
          RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
          addr = (cmd & 1) ? 0x8080000 : 0x8000000;
          size4 = (0x80000-0x100)/4 - 2;
          CRC->CR = 1;
          for(i=0; i<size4; i++) {
            CRC->DR = M32(addr);
            addr = addr + 4;
            }
          crc1 = CRC->DR;
          RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
          *((uint32_t*)InRAM_FlashProcMem) = crc1;
          *((uint32_t*)InRAM_FlashProcMem+1) = i + 1;
          if(Flash_Write(addr, (uint8_t*)InRAM_FlashProcMem, 8) != 0)
            Flash_Status |= FLASH_STAT_MASK_ERR;
          break;
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_SET_PREF_BLOCK_N:
		  Flash_EraseSector(511);
          Flash_Status = cmd;
          pref2 = (cmd & 1) ? 1 : 0;
          if((HavePrefFlashBlockNum() ^ pref2) & 1) {
            /*���� ��������� ���*/
            for(addr=PREF_REC_ADDR_START; addr<PREF_REC_ADDR_END; addr += 8) {
              tmp64 = 1;
              if(M64(addr) != 0) {
                for(n=0; n<64; n++) {
                  if(M64(addr) & tmp64) break;
                  tmp64 = tmp64 << 1;
                  }
                break;
                }
              }
            if(addr < PREF_REC_ADDR_END) {
              /*�������� ��������� ���*/
              tmp64 = M64(addr) & ~((uint64_t)1<<n);
              if(Flash_Write(addr, (uint8_t*)&tmp64, 8))
                Flash_Status |= FLASH_STAT_MASK_ERR;
              }
            else {
              /*������ ��������*/
              Flash_Status |= FLASH_STAT_MASK_ERR;
              }
            }
          break;
        /*--------------------------------------------------------------------*/
        case FLASH_CMD_RESTART:
          if((VarCAN_FlashFragment.Ctrl.size == 0) && (VarCAN_FlashFragment.Ctrl.crc == 0)) {
            SCB->AIRCR = 0x05FA0004;
            for(;;);
            }
          break;
        /*--------------------------------------------------------------------*/
        default:
          break;
        }
      }/*if(id.uf.Offset == 0x100000)*/
    else if(id.uf.Offset < 0x80000) {  //----- ������ �� Flash --------------
      Flash_Status = FLASH_STAT_LOAD_OK;
      if(HaveRunFlashBlockNum() == 0) {
        err = Flash_Write(0x8080000 + id.uf.Offset, VarCAN_FlashFragment.bb, leng);
        }
      else {
        err = Flash_Write(0x8000000 + id.uf.Offset, VarCAN_FlashFragment.bb, leng);
        }
      if(err) Flash_Status |= FLASH_STAT_MASK_ERR;
      }
    }
}



/*========================================================================================*/


int8_t Flash_InRAM_EraseSector(uint16_t sect_num) {
  int8_t err;
  err = 0;
  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
  FLASH->CR =  FLASH_CR_PER | ((sect_num << FLASH_SNB_POS) & FLASH_SNB_MSK);                            // Sector Erase Enabled
//  FLASH->CR |=  ((sect_num << FLASH_SNB_POS) & FLASH_SNB_MSK); // Sector Number
  FLASH->CR |= FLASH_CR_STRT;  // Start Erase
  __dsb(0);
  while (FLASH->SR & FLASH_SR_BSY) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
    }
  FLASH->CR =  0;                           // Page Erase Disabled 
  if(FLASH->SR & FLASH_PGERR) {                         // Check for Error
    FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
    err = -1;                                           // Failed
    }
  return err;
}


void Flash_InRAM_CopyCodeAndRestart(uint32_t size) {
  uint32_t n, k, m;
  uint32_t adr_src, adr_dst;
  /*-------------------------- sectors erase ---------------------------*/
  for(n=0; n <= 255; n++)  {   //n - sector number
    m = 0x8000000 | (n << 11);  k = 0x200;
    for( ; k>0; k--) {
      if((M32(m) + 1) != 0) break;
      m = m + 4;
      }
    if(k != 0) {
      /* ������� */
      FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
      FLASH->CR =  FLASH_CR_PER;                            // Sector Erase Enabled 
      FLASH->CR |=  ((n << FLASH_SNB_POS) & FLASH_SNB_MSK); // Sector Number
      FLASH->CR |= FLASH_CR_STRT;                           // Start Erase
      __dsb(0);
      while (FLASH->SR & FLASH_SR_BSY) {
        IWDG->KR = 0xAAAA;                                  // Reload IWDG
        }
      FLASH->CR = 0;                           // Page Erase Disabled 
      if(FLASH->SR & FLASH_PGERR) {                         // Check for Error
        FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
        goto Lret;                                          // Failed
        }
      }
    }
  /*-------------------------- copy - program ---------------------------*/
  size = (size + 7) & ~7;                               // Adjust size for Words
  adr_src = 0x8080000;          // up 512k
  adr_dst = 0x8000000;
  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
  FLASH->CR  =  0;                                      // reset CR 
  while(size) {
    IWDG->KR = 0xAAAA;                                  // Reload IWDG
    if(M64(adr_dst) != M64(adr_src)) {
      FLASH->CR = FLASH_CR_PG;                // Programming Enabled, Programming Enabled (Word)
      M32(adr_dst) = M32(adr_src);                        // Program Double Word
      M32(adr_dst+4) = M32(adr_src+4);
      while (FLASH->SR & FLASH_SR_BSY);
      FLASH->CR = 0;                          // Programming Disabled
      if(FLASH->SR & FLASH_PGERR) {                       // Check for Error
        FLASH->SR |= FLASH_PGERR;                         // Reset Error Flags
        goto Lret;                                        // Failed
        }
      }
    adr_dst += 8;                                       // Go to next Word
    adr_src += 8;
    size  -= 8;
    }
  /*-------------------------- restart ---------------------------------*/
  SCB->AIRCR = 0x05FA0004;
  for(;;);
Lret:
  return;
}


