#include "proto.h"

#define LOADER_BASE_ADDR 0x33000000 // loader.bin 加载到内存中的地址

void print_nand_id();

BYTE* __main()
{

  int i;
  BYTE* boot_ptr;
  BYTE* str;

  boot_ptr = (BYTE*)(0x33000000);
  str = (BYTE*)(0x33100000);


  GPIO_init();

  Uart_init(115200);
  Uart_SendString("setup boot to nand...\n",22);

  NF_init();

  // 要写之前必须先擦除
  if( NF_EraseBlock(0) == 0 )
    Uart_SendString("Erase Fail!\n",12);

  for(i=0;i<2;i++)
    {
      if( NF_WritePage(0, i, boot_ptr) == 0 )
	Uart_SendString("Write Fail!\n",12);
      
      boot_ptr += 2048;
    }

  for(i=0;i<2;i++)
    {
      if( NF_ReadPage(0, i, str) )
	Uart_SendString(str,2048);	
      else
      	Uart_SendString("Read Fail!\n",11);

      str += 2048;
    }


  while(1){}

  return (BYTE*)LOADER_BASE_ADDR;
}

// gcc 的静态库要求链接到的函数，为空就好
void raise()
{}

void print_nand_id()
{
  HWORD id;
  BYTE maker, device;

  /*
  device = (BYTE)id;
  maker = (BYTE)(id >> 8);
  Uart_SendByte(maker);
  Uart_SendByte(device);
  Uart_SendByte('\n');
  */

  id = NF_CheckId();
  device = (BYTE)id;
  maker = (BYTE)(id >> 8);

  Uart_SendByte(maker);
  Uart_SendByte(device);
  Uart_SendByte('\n');
}
